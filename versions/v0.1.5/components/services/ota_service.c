#include "ota_service.h"

#include "app_config.h"
#include "esp_https_ota.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <stdio.h>

static const char *TAG = "ota_service";
static ota_service_state_t s_state = OTA_SERVICE_IDLE;
static char s_message[96] = "OTA lista";
static int s_progress = 0;

static void ota_task(void *arg)
{
    (void)arg;
    const app_config_t *cfg = app_config_get();
    s_state = OTA_SERVICE_RUNNING;
    s_progress = 1;
    snprintf(s_message, sizeof(s_message), "OTA: conectando");

    esp_http_client_config_t http_config = {
        .url = cfg->ota_url,
        .cert_pem = cfg->ota_cert_pem[0] ? cfg->ota_cert_pem : NULL,
        .timeout_ms = 10000,
        .keep_alive_enable = true,
    };
    esp_https_ota_config_t ota_config = {
        .http_config = &http_config,
    };

    esp_https_ota_handle_t handle = NULL;
    esp_err_t err = esp_https_ota_begin(&ota_config, &handle);
    if (err != ESP_OK) {
        s_state = OTA_SERVICE_FAILED;
        s_progress = 0;
        snprintf(s_message, sizeof(s_message), "OTA fallo inicio: %s", esp_err_to_name(err));
        ESP_LOGE(TAG, "OTA begin failed: %s", esp_err_to_name(err));
        vTaskDelete(NULL);
        return;
    }

    int total = esp_https_ota_get_image_size(handle);
    int last_reported_pct = 0;
    snprintf(s_message, sizeof(s_message), "OTA: descargando");

    while (true) {
        err = esp_https_ota_perform(handle);
        int read = esp_https_ota_get_image_len_read(handle);
        if (total > 0 && read >= 0) {
            int pct = (read * 100) / total;
            if (pct < 1) {
                pct = 1;
            } else if (pct > 99) {
                pct = 99;
            }
            s_progress = pct;
            if (pct >= last_reported_pct + 2 || pct >= 99) {
                last_reported_pct = pct;
                snprintf(s_message, sizeof(s_message), "OTA: %d%%", pct);
            }
        }
        if (err != ESP_ERR_HTTPS_OTA_IN_PROGRESS) {
            break;
        }
        vTaskDelay(pdMS_TO_TICKS(30));
    }

    if (err == ESP_OK && esp_https_ota_is_complete_data_received(handle)) {
        err = esp_https_ota_finish(handle);
        if (err == ESP_OK) {
            s_state = OTA_SERVICE_UPDATED;
            s_progress = 100;
            snprintf(s_message, sizeof(s_message), "OTA OK. Reinicia para aplicar.");
        } else {
            s_state = OTA_SERVICE_FAILED;
            snprintf(s_message, sizeof(s_message), "OTA fallo final: %s", esp_err_to_name(err));
            ESP_LOGE(TAG, "OTA finish failed: %s", esp_err_to_name(err));
        }
    } else {
        esp_https_ota_abort(handle);
        s_state = OTA_SERVICE_FAILED;
        s_progress = 0;
        snprintf(s_message, sizeof(s_message), "OTA fallo: %s", esp_err_to_name(err));
        ESP_LOGE(TAG, "OTA failed: %s", esp_err_to_name(err));
    }
    vTaskDelete(NULL);
}

esp_err_t ota_service_start(void)
{
    const app_config_t *cfg = app_config_get();
    if (cfg->ota_url[0] == '\0') {
        s_state = OTA_SERVICE_FAILED;
        s_progress = 0;
        snprintf(s_message, sizeof(s_message), "URL OTA vacia");
        return ESP_ERR_INVALID_ARG;
    }
    if (s_state == OTA_SERVICE_RUNNING) {
        return ESP_ERR_INVALID_STATE;
    }
    BaseType_t ok = xTaskCreate(ota_task, "ota_task", 8192, NULL, 5, NULL);
    return ok == pdPASS ? ESP_OK : ESP_ERR_NO_MEM;
}

ota_service_state_t ota_service_get_state(void)
{
    return s_state;
}

const char *ota_service_get_message(void)
{
    return s_message;
}

int ota_service_get_progress_percent(void)
{
    return s_progress;
}