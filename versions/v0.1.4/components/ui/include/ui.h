#pragma once

#include "dashboard_state.h"

void dashboard_ui_init(dashboard_state_t *state);
void dashboard_ui_set_state(const dashboard_state_t *state);
void dashboard_ui_show_status(const char *message);
