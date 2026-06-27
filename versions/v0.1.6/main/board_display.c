#include "board_display.h"

#include "bsp/display.h"
#include "bsp/esp-bsp.h"
#include "esp_check.h"
#include "esp_log.h"
#include "esp_lvgl_port.h"
#include "sdkconfig.h"

#include <stdbool.h>

static const char *TAG = "board_display";
static lv_display_t *s_display;
static bool s_started;

esp_err_t board_display_init(lv_display_t **display_out)
{
    ESP_RETURN_ON_FALSE(display_out, ESP_ERR_INVALID_ARG, TAG, "display_out missing");

    if (!s_started) {
        ESP_LOGI(TAG, "init BSP ESP32-P4 Function EV Board display 1024x600 LVGL9");

        bsp_display_cfg_t cfg = {
            .lvgl_port_cfg = ESP_LVGL_PORT_INIT_CONFIG(),
            .buffer_size = BSP_LCD_H_RES * BSP_LCD_V_RES,
            .double_buffer = BSP_LCD_DRAW_BUFF_DOUBLE,
            .flags = {
                .buff_dma = false,
                .buff_spiram = true,
                .sw_rotate = false,
            },
        };

        s_display = bsp_display_start_with_config(&cfg);
        ESP_RETURN_ON_FALSE(s_display, ESP_FAIL, TAG, "bsp display start failed");

        ESP_RETURN_ON_ERROR(bsp_display_backlight_on(), TAG, "backlight on");
        ESP_RETURN_ON_ERROR(bsp_display_brightness_set(CONFIG_PANEL_LCD_BK_LIGHT_BRIGHTNESS), TAG, "brightness set");
        s_started = true;
    }

    *display_out = s_display;
    return ESP_OK;
}

esp_err_t board_touch_init(lv_display_t *display)
{
    ESP_RETURN_ON_FALSE(display, ESP_ERR_INVALID_ARG, TAG, "display missing");
    ESP_LOGI(TAG, "touch handled by BSP display init");
    return ESP_OK;
}

void board_display_lock(void)
{
    if (s_started) {
        bsp_display_lock(0);
    }
}

void board_display_unlock(void)
{
    if (s_started) {
        bsp_display_unlock();
    }
}
