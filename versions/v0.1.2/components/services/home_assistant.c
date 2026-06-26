#include "home_assistant.h"

#include "app_config.h"
#include "cJSON.h"
#include "esp_http_client.h"
#include "esp_log.h"

#include <stdio.h>
#include <string.h>

static const char *TAG = "home_assistant";

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

static bool ha_ready(void)
{
    const app_config_t *cfg = app_config_get();
    return cfg->ha_base_url[0] != '\0' && cfg->ha_token[0] != '\0';
}

esp_err_t home_assistant_poll(dashboard_state_t *state)
{
    if (!ha_ready()) {
        state->ha_online = false;
        return ESP_ERR_INVALID_STATE;
    }

    const app_config_t *cfg = app_config_get();
    char url[192];
    snprintf(url, sizeof(url), "%s/api/", cfg->ha_base_url);

    char response[256] = {0};
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
    char auth[384];
    snprintf(auth, sizeof(auth), "Bearer %s", cfg->ha_token);
    esp_http_client_set_header(client, "Authorization", auth);
    esp_http_client_set_header(client, "Content-Type", "application/json");
    esp_err_t err = esp_http_client_perform(client);
    int status = esp_http_client_get_status_code(client);
    esp_http_client_cleanup(client);

    state->ha_online = (err == ESP_OK && status >= 200 && status < 300);
    if (!state->ha_online) {
        ESP_LOGW(TAG, "HA poll failed err=%s status=%d", esp_err_to_name(err), status);
        return err == ESP_OK ? ESP_FAIL : err;
    }
    return ESP_OK;
}

esp_err_t home_assistant_parking_action(parking_action_t action)
{
    if (!ha_ready()) {
        return ESP_ERR_INVALID_STATE;
    }

    const char *service = "open_cover";
    if (action == PARKING_ACTION_CLOSE) {
        service = "close_cover";
    } else if (action == PARKING_ACTION_STOP) {
        service = "stop_cover";
    }

    const app_config_t *cfg = app_config_get();
    char url[256];
    snprintf(url, sizeof(url), "%s/api/services/cover/%s", cfg->ha_base_url, service);

    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "entity_id", cfg->ha_parking_entity);
    char *body = cJSON_PrintUnformatted(root);

    esp_http_client_config_t client_config = {
        .url = url,
        .timeout_ms = 3500,
    };
    esp_http_client_handle_t client = esp_http_client_init(&client_config);
    char auth[384];
    snprintf(auth, sizeof(auth), "Bearer %s", cfg->ha_token);
    esp_http_client_set_method(client, HTTP_METHOD_POST);
    esp_http_client_set_header(client, "Authorization", auth);
    esp_http_client_set_header(client, "Content-Type", "application/json");
    esp_http_client_set_post_field(client, body, strlen(body));

    esp_err_t err = esp_http_client_perform(client);
    int status = esp_http_client_get_status_code(client);
    esp_http_client_cleanup(client);
    cJSON_free(body);
    cJSON_Delete(root);

    if (err != ESP_OK || status < 200 || status >= 300) {
        ESP_LOGW(TAG, "parking action failed err=%s status=%d", esp_err_to_name(err), status);
        return err == ESP_OK ? ESP_FAIL : err;
    }
    return ESP_OK;
}
