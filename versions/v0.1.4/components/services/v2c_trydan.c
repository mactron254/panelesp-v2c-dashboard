#include "v2c_trydan.h"

#include "app_config.h"
#include "esp_http_client.h"
#include "esp_log.h"

#include <stdio.h>

static const char *TAG = "v2c_trydan";

static bool v2c_ready(void)
{
    return app_config_get()->v2c_host[0] != '\0';
}

esp_err_t v2c_trydan_poll(dashboard_state_t *state)
{
    if (!v2c_ready()) {
        state->v2c_online = false;
        return ESP_ERR_INVALID_STATE;
    }

    char url[160];
    snprintf(url, sizeof(url), "http://%s/RealTimeData", app_config_get()->v2c_host);
    esp_http_client_config_t client_config = {
        .url = url,
        .timeout_ms = 2500,
    };
    esp_http_client_handle_t client = esp_http_client_init(&client_config);
    esp_err_t err = esp_http_client_perform(client);
    int status = esp_http_client_get_status_code(client);
    esp_http_client_cleanup(client);

    state->v2c_online = (err == ESP_OK && status >= 200 && status < 300);
    if (!state->v2c_online) {
        ESP_LOGW(TAG, "poll failed err=%s status=%d", esp_err_to_name(err), status);
        return err == ESP_OK ? ESP_FAIL : err;
    }
    return ESP_OK;
}

esp_err_t v2c_trydan_set_charging(bool enabled)
{
    if (!v2c_ready()) {
        return ESP_ERR_INVALID_STATE;
    }
    char url[192];
    snprintf(url, sizeof(url), "http://%s/SetCharge?value=%d", app_config_get()->v2c_host, enabled ? 1 : 0);
    esp_http_client_config_t client_config = {
        .url = url,
        .timeout_ms = 2500,
    };
    esp_http_client_handle_t client = esp_http_client_init(&client_config);
    esp_err_t err = esp_http_client_perform(client);
    int status = esp_http_client_get_status_code(client);
    esp_http_client_cleanup(client);
    return (err == ESP_OK && status >= 200 && status < 300) ? ESP_OK : ESP_FAIL;
}

esp_err_t v2c_trydan_set_current_limit(int amps)
{
    if (!v2c_ready()) {
        return ESP_ERR_INVALID_STATE;
    }
    char url[192];
    snprintf(url, sizeof(url), "http://%s/SetCurrent?value=%d", app_config_get()->v2c_host, amps);
    esp_http_client_config_t client_config = {
        .url = url,
        .timeout_ms = 2500,
    };
    esp_http_client_handle_t client = esp_http_client_init(&client_config);
    esp_err_t err = esp_http_client_perform(client);
    int status = esp_http_client_get_status_code(client);
    esp_http_client_cleanup(client);
    return (err == ESP_OK && status >= 200 && status < 300) ? ESP_OK : ESP_FAIL;
}
