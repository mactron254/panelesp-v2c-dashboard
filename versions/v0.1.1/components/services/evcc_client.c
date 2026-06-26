#include "evcc_client.h"

#include "app_config.h"
#include "cJSON.h"
#include "esp_http_client.h"
#include "esp_log.h"

#include <stdio.h>
#include <string.h>

static const char *TAG = "evcc";

typedef struct {
    char *buffer;
    int length;
    int capacity;
} http_capture_t;

static esp_err_t capture_event_handler(esp_http_client_event_t *evt)
{
    if (evt->event_id != HTTP_EVENT_ON_DATA || !evt->user_data) {
        return ESP_OK;
    }
    http_capture_t *capture = (http_capture_t *)evt->user_data;
    int copy_len = evt->data_len;
    if (capture->length + copy_len >= capture->capacity) {
        copy_len = capture->capacity - capture->length - 1;
    }
    if (copy_len > 0) {
        memcpy(capture->buffer + capture->length, evt->data, copy_len);
        capture->length += copy_len;
        capture->buffer[capture->length] = '\0';
    }
    return ESP_OK;
}

static cJSON *json_find(cJSON *root, const char *name)
{
    cJSON *item = cJSON_GetObjectItem(root, name);
    if (item) {
        return item;
    }
    cJSON_ArrayForEach(item, root) {
        if (cJSON_IsObject(item)) {
            cJSON *nested = json_find(item, name);
            if (nested) {
                return nested;
            }
        }
    }
    return NULL;
}

esp_err_t evcc_client_poll(dashboard_state_t *state)
{
    const app_config_t *cfg = app_config_get();
    if (cfg->evcc_base_url[0] == '\0') {
        state->evcc_online = false;
        return ESP_ERR_INVALID_STATE;
    }

    char url[192];
    snprintf(url, sizeof(url), "%s/api/state", cfg->evcc_base_url);

    char response[2048] = {0};
    http_capture_t capture = {
        .buffer = response,
        .capacity = sizeof(response),
    };
    esp_http_client_config_t client_config = {
        .url = url,
        .event_handler = capture_event_handler,
        .user_data = &capture,
        .timeout_ms = 2500,
    };
    esp_http_client_handle_t client = esp_http_client_init(&client_config);
    esp_err_t err = esp_http_client_perform(client);
    int status = esp_http_client_get_status_code(client);
    esp_http_client_cleanup(client);

    if (err != ESP_OK || status < 200 || status >= 300) {
        state->evcc_online = false;
        ESP_LOGW(TAG, "EVCC poll failed err=%s status=%d", esp_err_to_name(err), status);
        return err == ESP_OK ? ESP_FAIL : err;
    }

    cJSON *root = cJSON_Parse(response);
    if (!root) {
        state->evcc_online = false;
        return ESP_ERR_INVALID_RESPONSE;
    }

    cJSON *soc = json_find(root, "vehicleSoc");
    cJSON *charge_power = json_find(root, "chargePower");
    cJSON *session_energy = json_find(root, "chargedEnergy");
    cJSON *price = json_find(root, "effectivePrice");

    if (cJSON_IsNumber(soc)) {
        state->evcc.soc_percent = (int)soc->valuedouble;
    }
    if (cJSON_IsNumber(charge_power)) {
        state->evcc.power_kw = (float)(charge_power->valuedouble / 1000.0);
    }
    if (cJSON_IsNumber(session_energy)) {
        state->evcc.session_kwh = (float)session_energy->valuedouble;
    }
    if (cJSON_IsNumber(price)) {
        state->evcc.price_eur_kwh = (float)price->valuedouble;
    }
    state->evcc_online = true;
    cJSON_Delete(root);
    return ESP_OK;
}
