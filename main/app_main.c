#include "app_config.h"
#include "board_display.h"
#include "dashboard_state.h"
#include "evcc_client.h"
#include "home_assistant.h"
#include "nvs_flash.h"
#include "ui.h"
#include "v2c_trydan.h"

#include "esp_event.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "soc/soc_caps.h"
#if SOC_WIFI_SUPPORTED
#include "esp_wifi.h"
#endif
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <stdio.h>
#include <string.h>

static const char *TAG = "panelesp";
static dashboard_state_t s_state;

#if SOC_WIFI_SUPPORTED
static void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    (void)arg;
    (void)event_data;

    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        s_state.wifi_connected = false;
        snprintf(s_state.status_message, sizeof(s_state.status_message), "WiFi sin conexion");
        esp_wifi_connect();
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        s_state.wifi_connected = true;
        snprintf(s_state.status_message, sizeof(s_state.status_message), "WiFi conectado");
    }
}

static esp_err_t wifi_init(void)
{
    const app_config_t *cfg = app_config_get();
    if (cfg->wifi_ssid[0] == '\0') {
        ESP_LOGW(TAG, "WiFi SSID empty; demo mode only");
        snprintf(s_state.status_message, sizeof(s_state.status_message), "WiFi no configurado");
        return ESP_ERR_INVALID_STATE;
    }

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&wifi_init_config));
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, wifi_event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, wifi_event_handler, NULL));

    wifi_config_t wifi_config = {0};
    snprintf((char *)wifi_config.sta.ssid, sizeof(wifi_config.sta.ssid), "%s", cfg->wifi_ssid);
    snprintf((char *)wifi_config.sta.password, sizeof(wifi_config.sta.password), "%s", cfg->wifi_password);
    wifi_config.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;
    wifi_config.sta.sae_pwe_h2e = WPA3_SAE_PWE_BOTH;

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
    return ESP_OK;
}
#else
static esp_err_t wifi_init(void)
{
    ESP_LOGW(TAG, "WiFi remoto pendiente de configurar; modo demo/sin conexion");
    snprintf(s_state.status_message, sizeof(s_state.status_message), "WiFi remoto pendiente");
    return ESP_ERR_NOT_SUPPORTED;
}
#endif

static void lvgl_task(void *arg)
{
    (void)arg;
    while (true) {
        board_display_lock();
        lv_timer_handler();
        board_display_unlock();
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}

static void data_task(void *arg)
{
    (void)arg;
    TickType_t last_wake = xTaskGetTickCount();

    while (true) {
        dashboard_state_tick_demo(&s_state);

        if (s_state.wifi_connected) {
            home_assistant_poll(&s_state);
            evcc_client_poll(&s_state);
            v2c_trydan_poll(&s_state);
        }

        board_display_lock();
        dashboard_ui_set_state(&s_state);
        board_display_unlock();

        vTaskDelayUntil(&last_wake, pdMS_TO_TICKS(2000));
    }
}

void app_main(void)
{
    esp_err_t nvs_ret = nvs_flash_init();
    if (nvs_ret == ESP_ERR_NVS_NO_FREE_PAGES || nvs_ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        nvs_ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(nvs_ret);

    dashboard_state_init(&s_state);
    s_state.demo_mode = app_config_get()->demo_mode;

    lv_display_t *display = NULL;
    ESP_ERROR_CHECK(board_display_init(&display));
    esp_err_t touch_ret = board_touch_init(display);
    if (touch_ret != ESP_OK) {
        ESP_LOGW(TAG, "Touch init failed: %s. Continuing without touch.", esp_err_to_name(touch_ret));
    }

    board_display_lock();
    dashboard_ui_init(&s_state);
    board_display_unlock();

    wifi_init();

    xTaskCreatePinnedToCore(lvgl_task, "lvgl_task", 8192, NULL, 6, NULL, 1);
    xTaskCreate(data_task, "data_task", 8192, NULL, 4, NULL);
}
