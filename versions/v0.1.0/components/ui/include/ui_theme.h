#pragma once

#include "lvgl.h"

#define UI_COLOR_BG          0xF9F9FC
#define UI_COLOR_CARD        0xFFFFFF
#define UI_COLOR_CARD_DIM    0xEEEFF2
#define UI_COLOR_TEXT        0x1A1C1E
#define UI_COLOR_MUTED       0x657072
#define UI_COLOR_PRIMARY     0x113639
#define UI_COLOR_PRIMARY_2   0x2A4D50
#define UI_COLOR_ACCENT      0xFD9A78
#define UI_COLOR_TERRACOTTA  0x96482D
#define UI_COLOR_LINE        0xE2E2E5
#define UI_COLOR_OK          0xA8CDD0
#define UI_COLOR_WARN        0xFFB59D

typedef struct {
    lv_style_t screen;
    lv_style_t header_title;
    lv_style_t nav_bar;
    lv_style_t nav_button;
    lv_style_t nav_button_active;
    lv_style_t card;
    lv_style_t metric_label;
    lv_style_t metric_value;
    lv_style_t button_primary;
    lv_style_t button_secondary;
    lv_style_t chip;
} ui_theme_t;

void ui_theme_init(ui_theme_t *theme);
