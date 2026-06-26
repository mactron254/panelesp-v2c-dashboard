#include "app_config.h"

#include "sdkconfig.h"

static const app_config_t s_config = {
    .wifi_ssid = CONFIG_PANEL_WIFI_SSID,
    .wifi_password = CONFIG_PANEL_WIFI_PASSWORD,
    .ha_base_url = CONFIG_PANEL_HA_BASE_URL,
    .ha_token = CONFIG_PANEL_HA_TOKEN,
    .ha_parking_entity = CONFIG_PANEL_HA_PARKING_ENTITY,
    .evcc_base_url = CONFIG_PANEL_EVCC_BASE_URL,
    .v2c_host = CONFIG_PANEL_V2C_HOST,
    .ota_url = CONFIG_PANEL_OTA_URL,
    .ota_cert_pem = CONFIG_PANEL_OTA_CERT_PEM,
    .demo_mode = (CONFIG_PANEL_HA_TOKEN[0] == '\0'),
};

const app_config_t *app_config_get(void)
{
    return &s_config;
}
