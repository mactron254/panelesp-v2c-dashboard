#pragma once

#include "dashboard_state.h"
#include "esp_err.h"

esp_err_t home_assistant_poll(dashboard_state_t *state);
esp_err_t home_assistant_parking_action(parking_action_t action);
