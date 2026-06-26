#pragma once

#include "dashboard_state.h"
#include "esp_err.h"

esp_err_t v2c_trydan_poll(dashboard_state_t *state);
esp_err_t v2c_trydan_set_charging(bool enabled);
esp_err_t v2c_trydan_set_current_limit(int amps);
