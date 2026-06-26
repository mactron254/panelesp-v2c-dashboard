#pragma once

#include <stdbool.h>

typedef struct {
    const char *wifi_ssid;
    const char *wifi_password;
    const char *ha_base_url;
    const char *ha_token;
    const char *ha_parking_entity;
    const char *evcc_base_url;
    const char *v2c_host;
    const char *ota_url;
    const char *ota_cert_pem;
    bool demo_mode;
} app_config_t;

const app_config_t *app_config_get(void);
