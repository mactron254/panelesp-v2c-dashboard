#include "board_display.h"

#include "driver/gpio.h"
#include "driver/i2c_master.h"
#include "driver/ledc.h"
#include "esp_check.h"
#include "esp_heap_caps.h"
#include "esp_idf_version.h"
#include "esp_ldo_regulator.h"
#include "esp_lcd_jd9165.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_touch_gt911.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"

#define LCD_BIT_PER_PIXEL 16
#define LCD_DRAW_BUFFER_LINES 60
#define LCD_BACKLIGHT_LEDC_CHANNEL LEDC_CHANNEL_0
#define LCD_BACKLIGHT_LEDC_TIMER LEDC_TIMER_0

#ifndef CONFIG_PANEL_TOUCH_I2C_SCL_SPEED_HZ
#define CONFIG_PANEL_TOUCH_I2C_SCL_SPEED_HZ 400000
#endif

static const char *TAG = "board_display";
static SemaphoreHandle_t s_lvgl_mutex;
static esp_lcd_panel_handle_t s_panel;
static esp_lcd_touch_handle_t s_touch;
static esp_ldo_channel_handle_t s_mipi_phy_ldo;

static uint32_t lvgl_tick_cb(void)
{
    return (uint32_t)(esp_timer_get_time() / 1000ULL);
}

static void lvgl_flush_cb(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map)
{
    const int x1 = area->x1;
    const int y1 = area->y1;
    const int x2 = area->x2 + 1;
    const int y2 = area->y2 + 1;

    esp_lcd_panel_draw_bitmap(s_panel, x1, y1, x2, y2, px_map);
    lv_display_flush_ready(disp);
}

static void lvgl_touch_cb(lv_indev_t *indev, lv_indev_data_t *data)
{
    (void)indev;
    static uint8_t touch_failures;

    if (s_touch == NULL) {
        data->state = LV_INDEV_STATE_RELEASED;
        return;
    }

    esp_err_t read_ret = esp_lcd_touch_read_data(s_touch);
    if (read_ret != ESP_OK) {
        touch_failures++;
        if (touch_failures >= 5) {
            ESP_LOGW(TAG, "GT911 read failed repeatedly; touch disabled");
            s_touch = NULL;
        }
        data->state = LV_INDEV_STATE_RELEASED;
        return;
    }
    touch_failures = 0;

    esp_lcd_touch_point_data_t points[1] = {0};
    uint8_t count = 0;
    esp_lcd_touch_get_data(s_touch, points, &count, 1);

    if (count > 0) {
        data->point.x = points[0].x;
        data->point.y = points[0].y;
        data->state = LV_INDEV_STATE_PRESSED;
    } else {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}

static esp_err_t board_enable_mipi_dsi_phy_power(void)
{
    if (CONFIG_PANEL_MIPI_DSI_PHY_LDO_CHAN <= 0) {
        return ESP_OK;
    }

    esp_ldo_channel_config_t ldo_config = {
        .chan_id = CONFIG_PANEL_MIPI_DSI_PHY_LDO_CHAN,
        .voltage_mv = CONFIG_PANEL_MIPI_DSI_PHY_LDO_MV,
    };
    ESP_RETURN_ON_ERROR(esp_ldo_acquire_channel(&ldo_config, &s_mipi_phy_ldo), TAG, "MIPI DSI PHY LDO");
    ESP_LOGI(TAG, "MIPI DSI PHY LDO channel=%d voltage=%dmV",
             CONFIG_PANEL_MIPI_DSI_PHY_LDO_CHAN, CONFIG_PANEL_MIPI_DSI_PHY_LDO_MV);
    return ESP_OK;
}

static esp_err_t board_backlight_init(void)
{
    if (CONFIG_PANEL_LCD_BK_LIGHT_GPIO < 0) {
        return ESP_OK;
    }

#if CONFIG_PANEL_LCD_BK_LIGHT_PWM
    ledc_timer_config_t timer_config = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_10_BIT,
        .timer_num = LCD_BACKLIGHT_LEDC_TIMER,
        .freq_hz = 5000,
        .clk_cfg = LEDC_AUTO_CLK,
    };
    ESP_RETURN_ON_ERROR(ledc_timer_config(&timer_config), TAG, "backlight ledc timer");

    ledc_channel_config_t channel_config = {
        .gpio_num = CONFIG_PANEL_LCD_BK_LIGHT_GPIO,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LCD_BACKLIGHT_LEDC_CHANNEL,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = LCD_BACKLIGHT_LEDC_TIMER,
        .duty = (1023 * CONFIG_PANEL_LCD_BK_LIGHT_BRIGHTNESS) / 100,
        .hpoint = 0,
    };
    ESP_RETURN_ON_ERROR(ledc_channel_config(&channel_config), TAG, "backlight ledc channel");
    ESP_RETURN_ON_ERROR(ledc_set_duty(LEDC_LOW_SPEED_MODE, LCD_BACKLIGHT_LEDC_CHANNEL,
                                      (1023 * CONFIG_PANEL_LCD_BK_LIGHT_BRIGHTNESS) / 100),
                        TAG, "backlight ledc duty");
    ESP_RETURN_ON_ERROR(ledc_update_duty(LEDC_LOW_SPEED_MODE, LCD_BACKLIGHT_LEDC_CHANNEL),
                        TAG, "backlight ledc update");
    ESP_LOGI(TAG, "LCD backlight=%d%% GPIO=%d", CONFIG_PANEL_LCD_BK_LIGHT_BRIGHTNESS,
             CONFIG_PANEL_LCD_BK_LIGHT_GPIO);
#else
    gpio_config_t bk_gpio_config = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = 1ULL << CONFIG_PANEL_LCD_BK_LIGHT_GPIO,
    };
    ESP_RETURN_ON_ERROR(gpio_config(&bk_gpio_config), TAG, "backlight gpio");
    gpio_set_level(CONFIG_PANEL_LCD_BK_LIGHT_GPIO, CONFIG_PANEL_LCD_BK_LIGHT_BRIGHTNESS > 0);
#endif

    return ESP_OK;
}

void board_display_lock(void)
{
    if (s_lvgl_mutex) {
        xSemaphoreTakeRecursive(s_lvgl_mutex, portMAX_DELAY);
    }
}

void board_display_unlock(void)
{
    if (s_lvgl_mutex) {
        xSemaphoreGiveRecursive(s_lvgl_mutex);
    }
}

esp_err_t board_display_init(lv_display_t **display_out)
{
    ESP_RETURN_ON_FALSE(display_out, ESP_ERR_INVALID_ARG, TAG, "display_out missing");

    s_lvgl_mutex = xSemaphoreCreateRecursiveMutex();
    ESP_RETURN_ON_FALSE(s_lvgl_mutex, ESP_ERR_NO_MEM, TAG, "LVGL mutex alloc failed");

    ESP_RETURN_ON_ERROR(board_enable_mipi_dsi_phy_power(), TAG, "MIPI DSI PHY power");
    ESP_RETURN_ON_ERROR(board_backlight_init(), TAG, "backlight init");

    ESP_LOGI(TAG, "init MIPI-DSI JD9165 1024x600");
    esp_lcd_dsi_bus_handle_t dsi_bus = NULL;
    esp_lcd_dsi_bus_config_t bus_config = JD9165_PANEL_BUS_DSI_2CH_CONFIG();
    bus_config.lane_bit_rate_mbps = 900;
    ESP_RETURN_ON_ERROR(esp_lcd_new_dsi_bus(&bus_config, &dsi_bus), TAG, "new dsi bus");

    esp_lcd_panel_io_handle_t dbi_io = NULL;
    esp_lcd_dbi_io_config_t dbi_config = JD9165_PANEL_IO_DBI_CONFIG();
    ESP_RETURN_ON_ERROR(esp_lcd_new_panel_io_dbi(dsi_bus, &dbi_config, &dbi_io), TAG, "new dbi io");

    const esp_lcd_dpi_panel_config_t dpi_config = JD9165_1024_600_PANEL_60HZ_DPI_CONFIG(LCD_COLOR_FMT_RGB565);
    jd9165_vendor_config_t vendor_config = {
        .mipi_config = {
            .dsi_bus = dsi_bus,
            .dpi_config = &dpi_config,
        },
    };
    const esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = CONFIG_PANEL_LCD_RST_GPIO,
        .rgb_ele_order = LCD_RGB_ELEMENT_ORDER_RGB,
        .bits_per_pixel = LCD_BIT_PER_PIXEL,
        .vendor_config = &vendor_config,
    };
    ESP_RETURN_ON_ERROR(esp_lcd_new_panel_jd9165(dbi_io, &panel_config, &s_panel), TAG, "new jd9165");
    ESP_RETURN_ON_ERROR(esp_lcd_panel_reset(s_panel), TAG, "panel reset");
    ESP_RETURN_ON_ERROR(esp_lcd_panel_init(s_panel), TAG, "panel init");
    ESP_RETURN_ON_ERROR(esp_lcd_panel_disp_on_off(s_panel, true), TAG, "panel on");
    ESP_LOGI(TAG, "MIPI pattern test");
    esp_lcd_dpi_panel_set_pattern(s_panel, MIPI_DSI_PATTERN_BAR_VERTICAL);
    vTaskDelay(pdMS_TO_TICKS(1000));
    esp_lcd_dpi_panel_set_pattern(s_panel, MIPI_DSI_PATTERN_NONE);

    lv_init();
    lv_tick_set_cb(lvgl_tick_cb);

    const size_t draw_buf_size = PANEL_H_RES * LCD_DRAW_BUFFER_LINES * (LCD_BIT_PER_PIXEL / 8);
    void *draw_buf_1 = heap_caps_malloc(draw_buf_size, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    void *draw_buf_2 = heap_caps_malloc(draw_buf_size, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    ESP_RETURN_ON_FALSE(draw_buf_1 && draw_buf_2, ESP_ERR_NO_MEM, TAG, "LVGL draw buffers");

    lv_display_t *display = lv_display_create(PANEL_H_RES, PANEL_V_RES);
    lv_display_set_color_format(display, LV_COLOR_FORMAT_RGB565);
    lv_display_set_flush_cb(display, lvgl_flush_cb);
    lv_display_set_buffers(display, draw_buf_1, draw_buf_2, draw_buf_size, LV_DISPLAY_RENDER_MODE_PARTIAL);

    *display_out = display;
    return ESP_OK;
}

esp_err_t board_touch_init(lv_display_t *display)
{
    ESP_RETURN_ON_FALSE(display, ESP_ERR_INVALID_ARG, TAG, "display missing");

    i2c_master_bus_handle_t i2c_bus = NULL;
    i2c_master_bus_config_t i2c_bus_config = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = CONFIG_PANEL_TOUCH_I2C_PORT,
        .sda_io_num = CONFIG_PANEL_TOUCH_I2C_SDA_GPIO,
        .scl_io_num = CONFIG_PANEL_TOUCH_I2C_SCL_GPIO,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };
    ESP_RETURN_ON_ERROR(i2c_new_master_bus(&i2c_bus_config, &i2c_bus), TAG, "new i2c bus");

    esp_lcd_panel_io_handle_t touch_io = NULL;
    esp_lcd_panel_io_i2c_config_t io_config = ESP_LCD_TOUCH_IO_I2C_GT911_CONFIG();
    io_config.scl_speed_hz = CONFIG_PANEL_TOUCH_I2C_SCL_SPEED_HZ;
    ESP_RETURN_ON_ERROR(esp_lcd_new_panel_io_i2c(i2c_bus, &io_config, &touch_io), TAG, "touch io");

    esp_lcd_touch_io_gt911_config_t gt911_config = {
        .dev_addr = ESP_LCD_TOUCH_IO_I2C_GT911_ADDRESS,
    };
    esp_lcd_touch_config_t touch_config = {
        .x_max = PANEL_H_RES,
        .y_max = PANEL_V_RES,
        .rst_gpio_num = CONFIG_PANEL_TOUCH_RST_GPIO,
        .int_gpio_num = CONFIG_PANEL_TOUCH_INT_GPIO,
        .levels = {
            .reset = 0,
            .interrupt = 0,
        },
        .flags = {
            .swap_xy = 0,
            .mirror_x = 0,
            .mirror_y = 0,
        },
        .driver_data = &gt911_config,
    };
    ESP_RETURN_ON_ERROR(esp_lcd_touch_new_i2c_gt911(touch_io, &touch_config, &s_touch), TAG, "new gt911");

    lv_indev_t *indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_display(indev, display);
    lv_indev_set_read_cb(indev, lvgl_touch_cb);

    return ESP_OK;
}
