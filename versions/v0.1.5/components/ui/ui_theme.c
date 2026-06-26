#include "ui_theme.h"

static void init_text_style(lv_style_t *style, uint32_t color, const lv_font_t *font)
{
    lv_style_init(style);
    lv_style_set_text_color(style, lv_color_hex(color));
    lv_style_set_text_font(style, font);
}

void ui_theme_init(ui_theme_t *theme)
{
    lv_style_init(&theme->screen);
    lv_style_set_bg_color(&theme->screen, lv_color_hex(UI_COLOR_BG));
    lv_style_set_bg_opa(&theme->screen, LV_OPA_COVER);
    lv_style_set_text_color(&theme->screen, lv_color_hex(UI_COLOR_TEXT));
    lv_style_set_text_font(&theme->screen, &lv_font_montserrat_18);

    init_text_style(&theme->header_title, UI_COLOR_TEXT, &lv_font_montserrat_24);

    lv_style_init(&theme->nav_bar);
    lv_style_set_bg_color(&theme->nav_bar, lv_color_hex(UI_COLOR_CARD));
    lv_style_set_bg_opa(&theme->nav_bar, LV_OPA_COVER);
    lv_style_set_radius(&theme->nav_bar, 12);
    lv_style_set_border_width(&theme->nav_bar, 0);
    lv_style_set_shadow_width(&theme->nav_bar, 16);
    lv_style_set_shadow_opa(&theme->nav_bar, LV_OPA_10);

    lv_style_init(&theme->nav_button);
    lv_style_set_bg_opa(&theme->nav_button, LV_OPA_TRANSP);
    lv_style_set_border_width(&theme->nav_button, 0);
    lv_style_set_radius(&theme->nav_button, 8);
    lv_style_set_text_color(&theme->nav_button, lv_color_hex(UI_COLOR_MUTED));
    lv_style_set_text_font(&theme->nav_button, &lv_font_montserrat_14);

    lv_style_init(&theme->nav_button_active);
    lv_style_set_bg_color(&theme->nav_button_active, lv_color_hex(UI_COLOR_ACCENT));
    lv_style_set_bg_opa(&theme->nav_button_active, LV_OPA_COVER);
    lv_style_set_text_color(&theme->nav_button_active, lv_color_hex(UI_COLOR_TEXT));

    lv_style_init(&theme->card);
    lv_style_set_bg_color(&theme->card, lv_color_hex(UI_COLOR_CARD));
    lv_style_set_bg_opa(&theme->card, LV_OPA_COVER);
    lv_style_set_radius(&theme->card, 16);
    lv_style_set_border_width(&theme->card, 0);
    lv_style_set_shadow_width(&theme->card, 14);
    lv_style_set_shadow_opa(&theme->card, LV_OPA_10);
    lv_style_set_pad_all(&theme->card, 22);

    init_text_style(&theme->metric_label, UI_COLOR_MUTED, &lv_font_montserrat_14);
    lv_style_set_text_letter_space(&theme->metric_label, 2);

    init_text_style(&theme->metric_value, UI_COLOR_TEXT, &lv_font_montserrat_36);

    lv_style_init(&theme->button_primary);
    lv_style_set_bg_color(&theme->button_primary, lv_color_hex(UI_COLOR_PRIMARY));
    lv_style_set_bg_opa(&theme->button_primary, LV_OPA_COVER);
    lv_style_set_text_color(&theme->button_primary, lv_color_hex(0xFFFFFF));
    lv_style_set_text_font(&theme->button_primary, &lv_font_montserrat_18);
    lv_style_set_radius(&theme->button_primary, 10);
    lv_style_set_border_width(&theme->button_primary, 0);
    lv_style_set_shadow_width(&theme->button_primary, 10);
    lv_style_set_shadow_opa(&theme->button_primary, LV_OPA_20);

    lv_style_init(&theme->button_secondary);
    lv_style_set_bg_color(&theme->button_secondary, lv_color_hex(UI_COLOR_CARD));
    lv_style_set_bg_opa(&theme->button_secondary, LV_OPA_COVER);
    lv_style_set_text_color(&theme->button_secondary, lv_color_hex(UI_COLOR_TEXT));
    lv_style_set_text_font(&theme->button_secondary, &lv_font_montserrat_18);
    lv_style_set_radius(&theme->button_secondary, 10);
    lv_style_set_border_color(&theme->button_secondary, lv_color_hex(0xC1C8C8));
    lv_style_set_border_width(&theme->button_secondary, 1);

    lv_style_init(&theme->chip);
    lv_style_set_bg_color(&theme->chip, lv_color_hex(UI_COLOR_CARD_DIM));
    lv_style_set_bg_opa(&theme->chip, LV_OPA_COVER);
    lv_style_set_radius(&theme->chip, 999);
    lv_style_set_pad_hor(&theme->chip, 12);
    lv_style_set_pad_ver(&theme->chip, 6);
    lv_style_set_text_font(&theme->chip, &lv_font_montserrat_14);
    lv_style_set_text_color(&theme->chip, lv_color_hex(UI_COLOR_TEXT));
}
