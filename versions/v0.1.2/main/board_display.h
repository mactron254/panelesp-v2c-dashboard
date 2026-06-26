#pragma once

#include "esp_err.h"
#include "lvgl.h"

#define PANEL_H_RES 1024
#define PANEL_V_RES 600

esp_err_t board_display_init(lv_display_t **display_out);
esp_err_t board_touch_init(lv_display_t *display);
void board_display_lock(void);
void board_display_unlock(void);
