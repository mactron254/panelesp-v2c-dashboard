#pragma once

#include "esp_err.h"

typedef enum {
    OTA_SERVICE_IDLE = 0,
    OTA_SERVICE_RUNNING,
    OTA_SERVICE_UPDATED,
    OTA_SERVICE_FAILED,
} ota_service_state_t;

esp_err_t ota_service_start(void);
ota_service_state_t ota_service_get_state(void);
const char *ota_service_get_message(void);
