#pragma once

#include "dashboard_state.h"
#include "esp_err.h"

esp_err_t evcc_client_poll(dashboard_state_t *state);
