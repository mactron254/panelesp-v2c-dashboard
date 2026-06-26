#include "ui.h"

#include "app_config.h"

#include "home_assistant.h"
#include "ota_service.h"
#include "ui_theme.h"
#include "v2c_trydan.h"

#include "sdkconfig.h"

#include <stdio.h>
#include <string.h>

#define SCREEN_W 1024
#define SCREEN_H 600
#define HEADER_H 60
#define NAV_H 74
#define CONTENT_Y HEADER_H
#define CONTENT_H (SCREEN_H - HEADER_H - NAV_H)
#define PANEL_UI_VERSION "v0.1.3"

typedef struct {
    ui_theme_t theme;
    dashboard_state_t *state;
    dashboard_screen_t active_screen;
    lv_obj_t *root;
    lv_obj_t *content;
    lv_obj_t *pages[DASHBOARD_SCREEN_COUNT];
    lv_obj_t *nav_buttons[DASHBOARD_SCREEN_COUNT];
    lv_obj_t *status_label;
    lv_obj_t *parking_modal;
    lv_obj_t *ota_panel;
    lv_obj_t *ota_bar;
    lv_obj_t *ota_label;
    lv_obj_t *version_label;
    lv_obj_t *settings_version;

    lv_obj_t *charger_power;
    lv_obj_t *charger_energy;
    lv_obj_t *charger_time;
    lv_obj_t *charger_cost;
    lv_obj_t *charger_current;
    lv_obj_t *charger_arc;

    lv_obj_t *evcc_soc;
    lv_obj_t *evcc_car;
    lv_obj_t *evcc_power;
    lv_obj_t *evcc_energy;
    lv_obj_t *evcc_price;
    lv_obj_t *evcc_current;
    lv_obj_t *evcc_arc;

    lv_obj_t *solar_solar;
    lv_obj_t *solar_grid;
    lv_obj_t *solar_house;
    lv_obj_t *solar_vehicle;
    lv_obj_t *solar_loads;
    lv_obj_t *solar_battery;

    lv_obj_t *vehicle_battery;
    lv_obj_t *vehicle_fuel;
    lv_obj_t *vehicle_range;
    lv_obj_t *vehicle_split;
    lv_obj_t *vehicle_climate;
    lv_obj_t *vehicle_location;

    lv_obj_t *settings_wifi;
    lv_obj_t *settings_wifi_ip;
    lv_obj_t *settings_ha;
    lv_obj_t *settings_evcc;
    lv_obj_t *settings_v2c;
    lv_obj_t *settings_ota;
    lv_obj_t *settings_ha_url;
    lv_obj_t *settings_evcc_url;
    lv_obj_t *settings_v2c_host;
} ui_ctx_t;

static ui_ctx_t s_ui;

static bool screen_enabled(dashboard_screen_t screen)
{
    switch (screen) {
    case DASHBOARD_SCREEN_CHARGER:
        return CONFIG_PANEL_SCREEN_CHARGER;
    case DASHBOARD_SCREEN_EVCC:
        return CONFIG_PANEL_SCREEN_EVCC;
    case DASHBOARD_SCREEN_SOLAR:
        return CONFIG_PANEL_SCREEN_SOLAR;
    case DASHBOARD_SCREEN_VEHICLE:
        return CONFIG_PANEL_SCREEN_VEHICLE;
    case DASHBOARD_SCREEN_SETTINGS:
        return CONFIG_PANEL_SCREEN_SETTINGS;
    default:
        return false;
    }
}

static dashboard_screen_t first_enabled_screen(void)
{
    for (int i = 0; i < DASHBOARD_SCREEN_COUNT; ++i) {
        if (screen_enabled((dashboard_screen_t)i)) {
            return (dashboard_screen_t)i;
        }
    }
    return DASHBOARD_SCREEN_CHARGER;
}

static lv_obj_t *label(lv_obj_t *parent, const char *text, const lv_font_t *font, uint32_t color)
{
    lv_obj_t *obj = lv_label_create(parent);
    lv_label_set_text(obj, text);
    lv_obj_set_style_text_font(obj, font, 0);
    lv_obj_set_style_text_color(obj, lv_color_hex(color), 0);
    return obj;
}

static lv_obj_t *card(lv_obj_t *parent, int x, int y, int w, int h)
{
    lv_obj_t *obj = lv_obj_create(parent);
    lv_obj_remove_style_all(obj);
    lv_obj_add_style(obj, &s_ui.theme.card, 0);
    lv_obj_set_pos(obj, x, y);
    lv_obj_set_size(obj, w, h);
    return obj;
}

static lv_obj_t *button(lv_obj_t *parent, int x, int y, int w, int h, const char *text, bool primary)
{
    lv_obj_t *btn = lv_button_create(parent);
    lv_obj_remove_style_all(btn);
    lv_obj_add_style(btn, primary ? &s_ui.theme.button_primary : &s_ui.theme.button_secondary, 0);
    lv_obj_set_pos(btn, x, y);
    lv_obj_set_size(btn, w, h);

    lv_obj_t *txt = label(btn, text, &lv_font_montserrat_18, primary ? 0xFFFFFF : UI_COLOR_TEXT);
    lv_obj_center(txt);
    return btn;
}

static lv_obj_t *metric_card(lv_obj_t *parent, int x, int y, int w, int h, const char *title, lv_obj_t **value)
{
    lv_obj_t *c = card(parent, x, y, w, h);
    lv_obj_t *t = label(c, title, &lv_font_montserrat_14, UI_COLOR_MUTED);
    lv_obj_set_pos(t, 0, 0);
    lv_obj_t *v = label(c, "--", &lv_font_montserrat_36, UI_COLOR_TEXT);
    lv_obj_set_pos(v, 0, 48);
    *value = v;
    return c;
}

static lv_obj_t *arc_metric(lv_obj_t *parent, int x, int y, int size, int min, int max, int value)
{
    lv_obj_t *arc = lv_arc_create(parent);
    lv_obj_set_pos(arc, x, y);
    lv_obj_set_size(arc, size, size);
    lv_arc_set_range(arc, min, max);
    lv_arc_set_value(arc, value);
    lv_arc_set_bg_angles(arc, 20, 340);
    lv_obj_remove_style(arc, NULL, LV_PART_KNOB);
    lv_obj_set_style_arc_width(arc, 9, LV_PART_MAIN);
    lv_obj_set_style_arc_width(arc, 9, LV_PART_INDICATOR);
    lv_obj_set_style_arc_color(arc, lv_color_hex(UI_COLOR_LINE), LV_PART_MAIN);
    lv_obj_set_style_arc_color(arc, lv_color_hex(UI_COLOR_PRIMARY), LV_PART_INDICATOR);
    lv_obj_clear_flag(arc, LV_OBJ_FLAG_CLICKABLE);
    return arc;
}

static void set_active_screen(dashboard_screen_t screen)
{
    s_ui.active_screen = screen;
    for (int i = 0; i < DASHBOARD_SCREEN_COUNT; ++i) {
        if (s_ui.pages[i]) {
            if (i == screen) {
                lv_obj_remove_flag(s_ui.pages[i], LV_OBJ_FLAG_HIDDEN);
                if (s_ui.nav_buttons[i]) {
                    lv_obj_add_style(s_ui.nav_buttons[i], &s_ui.theme.nav_button_active, 0);
                }
            } else {
                lv_obj_add_flag(s_ui.pages[i], LV_OBJ_FLAG_HIDDEN);
                if (s_ui.nav_buttons[i]) {
                    lv_obj_remove_style(s_ui.nav_buttons[i], &s_ui.theme.nav_button_active, 0);
                }
            }
        }
    }
}

static void nav_event(lv_event_t *event)
{
    dashboard_screen_t screen = (dashboard_screen_t)(uintptr_t)lv_event_get_user_data(event);
    set_active_screen(screen);
}

static void settings_show_event(lv_event_t *event)
{
    (void)event;
    set_active_screen(DASHBOARD_SCREEN_SETTINGS);
}

static void parking_modal_close(lv_event_t *event)
{
    (void)event;
    lv_obj_add_flag(s_ui.parking_modal, LV_OBJ_FLAG_HIDDEN);
}

static void parking_action_event(lv_event_t *event)
{
    parking_action_t action = (parking_action_t)(uintptr_t)lv_event_get_user_data(event);
    esp_err_t err = home_assistant_parking_action(action);
    dashboard_ui_show_status(err == ESP_OK ? "Parking: orden enviada" : "Parking: falta token/HA o fallo red");
    lv_obj_add_flag(s_ui.parking_modal, LV_OBJ_FLAG_HIDDEN);
}

static void parking_show_event(lv_event_t *event)
{
    (void)event;
    lv_obj_remove_flag(s_ui.parking_modal, LV_OBJ_FLAG_HIDDEN);
}

static void charge_start_event(lv_event_t *event)
{
    (void)event;
    if (v2c_trydan_set_charging(true) == ESP_OK) {
        s_ui.state->charger.charging = true;
        dashboard_ui_show_status("V2C: carga iniciada");
    } else {
        s_ui.state->charger.charging = true;
        dashboard_ui_show_status("Demo: carga iniciada");
    }
    dashboard_ui_set_state(s_ui.state);
}

static void charge_stop_event(lv_event_t *event)
{
    (void)event;
    if (v2c_trydan_set_charging(false) == ESP_OK) {
        s_ui.state->charger.charging = false;
        dashboard_ui_show_status("V2C: carga detenida");
    } else {
        s_ui.state->charger.charging = false;
        dashboard_ui_show_status("Demo: carga detenida");
    }
    dashboard_ui_set_state(s_ui.state);
}

static void amps_delta_event(lv_event_t *event)
{
    int delta = (int)(intptr_t)lv_event_get_user_data(event);
    int next = s_ui.state->charger.current_limit_a + delta;
    if (next < 6) {
        next = 6;
    } else if (next > 32) {
        next = 32;
    }
    s_ui.state->charger.current_limit_a = next;
    v2c_trydan_set_current_limit(next);
    dashboard_ui_set_state(s_ui.state);
}

static void ota_event(lv_event_t *event)
{
    (void)event;
    esp_err_t err = ota_service_start();
    dashboard_ui_show_status(err == ESP_OK ? "OTA: descarga iniciada" : ota_service_get_message());
}

static void create_header(lv_obj_t *root)
{
    lv_obj_t *brand = label(root, "Lumina Energy", &lv_font_montserrat_28, UI_COLOR_TEXT);
    lv_obj_set_pos(brand, 42, 16);

    lv_obj_t *bolt = label(root, "L", &lv_font_montserrat_28, UI_COLOR_PRIMARY);
    lv_obj_set_pos(bolt, 18, 14);

    lv_obj_t *parking = button(root, 642, 9, 160, 40, "Puerta Parking", true);
    lv_obj_add_event_cb(parking, parking_show_event, LV_EVENT_CLICKED, NULL);

    s_ui.version_label = label(root, PANEL_UI_VERSION, &lv_font_montserrat_16, UI_COLOR_MUTED);
    lv_obj_set_pos(s_ui.version_label, 812, 20);

    lv_obj_t *settings = button(root, 870, 9, 50, 40, "âš™", false);
    lv_obj_add_event_cb(settings, settings_show_event, LV_EVENT_CLICKED, NULL);

    lv_obj_t *ota = button(root, 940, 9, 64, 40, "OTA", false);
    lv_obj_add_event_cb(ota, ota_event, LV_EVENT_CLICKED, NULL);

    s_ui.ota_panel = lv_obj_create(root);
    lv_obj_remove_style_all(s_ui.ota_panel);
    lv_obj_set_pos(s_ui.ota_panel, 650, 50);
    lv_obj_set_size(s_ui.ota_panel, 354, 28);
    lv_obj_add_flag(s_ui.ota_panel, LV_OBJ_FLAG_HIDDEN);
    s_ui.ota_label = label(s_ui.ota_panel, "OTA", &lv_font_montserrat_14, UI_COLOR_TEXT);
    lv_obj_set_pos(s_ui.ota_label, 0, 0);
    s_ui.ota_bar = lv_bar_create(s_ui.ota_panel);
    lv_obj_set_pos(s_ui.ota_bar, 96, 4);
    lv_obj_set_size(s_ui.ota_bar, 250, 10);
    lv_bar_set_range(s_ui.ota_bar, 0, 100);
    lv_bar_set_value(s_ui.ota_bar, 0, LV_ANIM_OFF);
    lv_obj_set_style_bg_color(s_ui.ota_bar, lv_color_hex(UI_COLOR_LINE), LV_PART_MAIN);
    lv_obj_set_style_bg_color(s_ui.ota_bar, lv_color_hex(UI_COLOR_PRIMARY), LV_PART_INDICATOR);
}

static void create_nav(lv_obj_t *root)
{
    lv_obj_t *nav = lv_obj_create(root);
    lv_obj_remove_style_all(nav);
    lv_obj_add_style(nav, &s_ui.theme.nav_bar, 0);
    lv_obj_set_pos(nav, 0, SCREEN_H - NAV_H);
    lv_obj_set_size(nav, SCREEN_W, NAV_H);

    const char *names[] = {"Cargador", "EVCC", "Solar", "Vehiculo", "Ajustes"};
    int enabled_count = 0;
    for (int i = 0; i < DASHBOARD_SCREEN_COUNT; ++i) {
        enabled_count += (i != DASHBOARD_SCREEN_SETTINGS && screen_enabled((dashboard_screen_t)i)) ? 1 : 0;
    }
    if (enabled_count == 0) {
        enabled_count = 1;
    }
    int slot = 0;
    for (int i = 0; i < DASHBOARD_SCREEN_COUNT; ++i) {
        if (i == DASHBOARD_SCREEN_SETTINGS || !screen_enabled((dashboard_screen_t)i)) {
            continue;
        }
        lv_obj_t *btn = lv_button_create(nav);
        lv_obj_remove_style_all(btn);
        lv_obj_add_style(btn, &s_ui.theme.nav_button, 0);
        lv_obj_set_size(btn, 116, 50);
        lv_obj_set_pos(btn, ((SCREEN_W - (enabled_count * 116)) / (enabled_count + 1)) * (slot + 1) + (116 * slot), 12);
        lv_obj_add_event_cb(btn, nav_event, LV_EVENT_CLICKED, (void *)(uintptr_t)i);
        lv_obj_t *txt = label(btn, names[i], &lv_font_montserrat_14, UI_COLOR_MUTED);
        lv_obj_center(txt);
        s_ui.nav_buttons[i] = btn;
        slot++;
    }
}

static void create_parking_modal(lv_obj_t *root)
{
    s_ui.parking_modal = lv_obj_create(root);
    lv_obj_remove_style_all(s_ui.parking_modal);
    lv_obj_set_style_bg_color(s_ui.parking_modal, lv_color_hex(0x000000), 0);
    lv_obj_set_style_bg_opa(s_ui.parking_modal, LV_OPA_50, 0);
    lv_obj_set_size(s_ui.parking_modal, SCREEN_W, SCREEN_H);
    lv_obj_add_flag(s_ui.parking_modal, LV_OBJ_FLAG_HIDDEN);

    lv_obj_t *panel = card(s_ui.parking_modal, 312, 164, 400, 260);
    lv_obj_t *title = label(panel, "Puerta Parking", &lv_font_montserrat_24, UI_COLOR_TEXT);
    lv_obj_set_pos(title, 0, 0);
    lv_obj_t *hint = label(panel, "Enviar orden a Home Assistant", &lv_font_montserrat_16, UI_COLOR_MUTED);
    lv_obj_set_pos(hint, 0, 40);

    lv_obj_t *open = button(panel, 0, 88, 170, 54, "Abrir", true);
    lv_obj_add_event_cb(open, parking_action_event, LV_EVENT_CLICKED, (void *)(uintptr_t)PARKING_ACTION_OPEN);

    lv_obj_t *close = button(panel, 190, 88, 170, 54, "Cerrar", false);
    lv_obj_add_event_cb(close, parking_action_event, LV_EVENT_CLICKED, (void *)(uintptr_t)PARKING_ACTION_CLOSE);

    lv_obj_t *stop = button(panel, 0, 160, 170, 54, "Parar", false);
    lv_obj_add_event_cb(stop, parking_action_event, LV_EVENT_CLICKED, (void *)(uintptr_t)PARKING_ACTION_STOP);

    lv_obj_t *cancel = button(panel, 190, 160, 170, 54, "Cancelar", false);
    lv_obj_add_event_cb(cancel, parking_modal_close, LV_EVENT_CLICKED, NULL);
}

static void create_charger_page(lv_obj_t *parent)
{
    lv_obj_t *p = lv_obj_create(parent);
    lv_obj_remove_style_all(p);
    lv_obj_set_size(p, SCREEN_W, CONTENT_H);
    s_ui.pages[DASHBOARD_SCREEN_CHARGER] = p;

    lv_obj_t *mode = card(p, 32, 24, 220, 145);
    label(mode, "MODO DE CARGA", &lv_font_montserrat_14, UI_COLOR_MUTED);
    lv_obj_t *dynamic = label(mode, "Dinamico", &lv_font_montserrat_24, UI_COLOR_TEXT);
    lv_obj_set_pos(dynamic, 0, 35);
    lv_obj_t *help = label(mode, "Ajusta la potencia segun\nconsumo de vivienda.", &lv_font_montserrat_16, UI_COLOR_TEXT);
    lv_obj_set_pos(help, 0, 78);

    lv_obj_t *lock = card(p, 32, 190, 220, 80);
    lv_obj_t *lock_txt = label(lock, "Desbloqueado >", &lv_font_montserrat_18, UI_COLOR_TEXT);
    lv_obj_set_pos(lock_txt, 0, 8);

    lv_obj_t *limit = card(p, 32, 292, 220, 128);
    label(limit, "LIMITE MAXIMO", &lv_font_montserrat_14, UI_COLOR_MUTED);
    lv_obj_t *minus = button(limit, 0, 48, 44, 44, "-", false);
    lv_obj_add_event_cb(minus, amps_delta_event, LV_EVENT_CLICKED, (void *)(intptr_t)-1);
    s_ui.charger_current = label(limit, "--", &lv_font_montserrat_36, UI_COLOR_TEXT);
    lv_obj_set_pos(s_ui.charger_current, 62, 47);
    lv_obj_t *plus = button(limit, 140, 48, 44, 44, "+", false);
    lv_obj_add_event_cb(plus, amps_delta_event, LV_EVENT_CLICKED, (void *)(intptr_t)1);

    s_ui.charger_arc = arc_metric(p, 350, 68, 260, 0, 11000, 7400);
    lv_obj_t *power_title = label(p, "POTENCIA", &lv_font_montserrat_14, UI_COLOR_MUTED);
    lv_obj_set_pos(power_title, 442, 160);
    s_ui.charger_power = label(p, "--", &lv_font_montserrat_48, UI_COLOR_PRIMARY);
    lv_obj_set_pos(s_ui.charger_power, 408, 198);
    lv_obj_t *unit = label(p, "W", &lv_font_montserrat_24, UI_COLOR_TEXT);
    lv_obj_set_pos(unit, 500, 260);

    lv_obj_t *start = button(p, 314, 372, 190, 58, "Iniciar Carga", true);
    lv_obj_add_event_cb(start, charge_start_event, LV_EVENT_CLICKED, NULL);
    lv_obj_t *stop = button(p, 536, 372, 160, 58, "Detener", false);
    lv_obj_add_event_cb(stop, charge_stop_event, LV_EVENT_CLICKED, NULL);

    metric_card(p, 760, 24, 218, 116, "ENERGIA", &s_ui.charger_energy);
    metric_card(p, 760, 166, 218, 116, "TIEMPO", &s_ui.charger_time);
    metric_card(p, 760, 306, 218, 116, "COSTE EST.", &s_ui.charger_cost);
}

static void create_evcc_page(lv_obj_t *parent)
{
    lv_obj_t *p = lv_obj_create(parent);
    lv_obj_remove_style_all(p);
    lv_obj_set_size(p, SCREEN_W, CONTENT_H);
    s_ui.pages[DASHBOARD_SCREEN_EVCC] = p;

    lv_obj_t *left = card(p, 24, 0, 390, 438);
    s_ui.evcc_arc = arc_metric(left, 66, 22, 180, 0, 100, 78);
    s_ui.evcc_soc = label(left, "--", &lv_font_montserrat_42, UI_COLOR_PRIMARY);
    lv_obj_set_pos(s_ui.evcc_soc, 124, 82);
    lv_obj_t *soc_label = label(left, "ESTADO DE CARGA", &lv_font_montserrat_14, UI_COLOR_MUTED);
    lv_obj_set_pos(soc_label, 86, 142);
    s_ui.evcc_car = label(left, "Passat GTE", &lv_font_montserrat_24, UI_COLOR_TEXT);
    lv_obj_set_pos(s_ui.evcc_car, 86, 220);
    s_ui.evcc_power = label(left, "--", &lv_font_montserrat_18, UI_COLOR_TEXT);
    lv_obj_set_pos(s_ui.evcc_power, 86, 260);

    lv_obj_t *mode = card(p, 438, 0, 562, 122);
    label(mode, "MODO DE CARGA", &lv_font_montserrat_14, UI_COLOR_MUTED);
    const char *modes[] = {"OFF", "SOLAR", "MIN + SOLAR", "RAPIDO"};
    for (int i = 0; i < 4; ++i) {
        lv_obj_t *b = button(mode, 0 + i * 136, 40, 124, 58, modes[i], i == 2);
        (void)b;
    }

    lv_obj_t *limit = card(p, 438, 146, 270, 128);
    label(limit, "LIMITE AMPERAJE", &lv_font_montserrat_14, UI_COLOR_MUTED);
    s_ui.evcc_current = label(limit, "--", &lv_font_montserrat_36, UI_COLOR_TEXT);
    lv_obj_set_pos(s_ui.evcc_current, 96, 52);

    metric_card(p, 724, 146, 276, 128, "ENERGIA SESION", &s_ui.evcc_energy);
    lv_obj_t *evcc_phases = NULL;
    metric_card(p, 438, 296, 270, 128, "FASES ACTIVAS", &evcc_phases);
    lv_label_set_text(evcc_phases, "L1  L2  L3");
    metric_card(p, 724, 296, 276, 128, "PRECIO ENERGIA", &s_ui.evcc_price);
}

static void create_solar_page(lv_obj_t *parent)
{
    lv_obj_t *p = lv_obj_create(parent);
    lv_obj_remove_style_all(p);
    lv_obj_set_size(p, SCREEN_W, CONTENT_H);
    s_ui.pages[DASHBOARD_SCREEN_SOLAR] = p;

    lv_obj_t *modes = card(p, 24, 26, 260, 270);
    label(modes, "MODO DE BATERIA", &lv_font_montserrat_14, UI_COLOR_MUTED);
    button(modes, 0, 50, 214, 48, "Autoconsumo", false);
    button(modes, 0, 112, 214, 48, "TOU (Horario)", false);
    button(modes, 0, 174, 214, 48, "Reserva", false);

    lv_obj_t *flow = card(p, 314, 26, 686, 400);
    lv_obj_t *house = lv_obj_create(flow);
    lv_obj_remove_style_all(house);
    lv_obj_set_style_bg_color(house, lv_color_hex(UI_COLOR_PRIMARY), 0);
    lv_obj_set_style_bg_opa(house, LV_OPA_COVER, 0);
    lv_obj_set_style_radius(house, 999, 0);
    lv_obj_set_size(house, 90, 90);
    lv_obj_set_pos(house, 310, 150);
    label(house, "CASA", &lv_font_montserrat_16, 0xFFFFFF);
    lv_obj_center(lv_obj_get_child(house, 0));

    static lv_point_precise_t pts1[] = {{355, 150}, {355, 90}};
    static lv_point_precise_t pts2[] = {{310, 195}, {170, 195}};
    static lv_point_precise_t pts3[] = {{400, 195}, {520, 145}};
    static lv_point_precise_t pts4[] = {{400, 195}, {520, 250}};
    lv_obj_t *line1 = lv_line_create(flow);
    lv_line_set_points(line1, pts1, 2);
    lv_obj_set_style_line_width(line1, 3, 0);
    lv_obj_set_style_line_color(line1, lv_color_hex(UI_COLOR_ACCENT), 0);
    lv_obj_t *line2 = lv_line_create(flow);
    lv_line_set_points(line2, pts2, 2);
    lv_obj_set_style_line_width(line2, 3, 0);
    lv_obj_set_style_line_dash_width(line2, 8, 0);
    lv_obj_set_style_line_dash_gap(line2, 10, 0);
    lv_obj_set_style_line_color(line2, lv_color_hex(0xBFC5C7), 0);
    lv_obj_t *line3 = lv_line_create(flow);
    lv_line_set_points(line3, pts3, 2);
    lv_obj_set_style_line_width(line3, 3, 0);
    lv_obj_set_style_line_dash_width(line3, 8, 0);
    lv_obj_set_style_line_dash_gap(line3, 10, 0);
    lv_obj_set_style_line_color(line3, lv_color_hex(UI_COLOR_PRIMARY), 0);
    lv_obj_t *line4 = lv_line_create(flow);
    lv_line_set_points(line4, pts4, 2);
    lv_obj_set_style_line_width(line4, 3, 0);
    lv_obj_set_style_line_dash_width(line4, 8, 0);
    lv_obj_set_style_line_dash_gap(line4, 10, 0);
    lv_obj_set_style_line_color(line4, lv_color_hex(UI_COLOR_PRIMARY), 0);

    label(flow, "SOLAR", &lv_font_montserrat_18, UI_COLOR_MUTED);
    lv_obj_t *solar_l = lv_obj_get_child(flow, lv_obj_get_child_count(flow) - 1);
    lv_obj_set_pos(solar_l, 315, 78);
    s_ui.solar_solar = label(flow, "--", &lv_font_montserrat_18, UI_COLOR_TERRACOTTA);
    lv_obj_set_pos(s_ui.solar_solar, 312, 106);

    label(flow, "RED", &lv_font_montserrat_18, UI_COLOR_MUTED);
    lv_obj_t *grid_l = lv_obj_get_child(flow, lv_obj_get_child_count(flow) - 1);
    lv_obj_set_pos(grid_l, 104, 210);
    s_ui.solar_grid = label(flow, "--", &lv_font_montserrat_18, UI_COLOR_MUTED);
    lv_obj_set_pos(s_ui.solar_grid, 102, 238);

    s_ui.solar_house = label(flow, "--", &lv_font_montserrat_20, UI_COLOR_TEXT);
    lv_obj_set_pos(s_ui.solar_house, 326, 245);

    label(flow, "VEHICULO", &lv_font_montserrat_14, UI_COLOR_MUTED);
    lv_obj_t *veh_l = lv_obj_get_child(flow, lv_obj_get_child_count(flow) - 1);
    lv_obj_set_pos(veh_l, 520, 142);
    s_ui.solar_vehicle = label(flow, "--", &lv_font_montserrat_18, UI_COLOR_TEXT);
    lv_obj_set_pos(s_ui.solar_vehicle, 526, 166);

    label(flow, "CARGAS", &lv_font_montserrat_14, UI_COLOR_MUTED);
    lv_obj_t *load_l = lv_obj_get_child(flow, lv_obj_get_child_count(flow) - 1);
    lv_obj_set_pos(load_l, 528, 272);
    s_ui.solar_loads = label(flow, "--", &lv_font_montserrat_18, UI_COLOR_TEXT);
    lv_obj_set_pos(s_ui.solar_loads, 526, 296);

    label(flow, "BATERIA", &lv_font_montserrat_18, UI_COLOR_MUTED);
    lv_obj_t *bat_l = lv_obj_get_child(flow, lv_obj_get_child_count(flow) - 1);
    lv_obj_set_pos(bat_l, 314, 318);
    s_ui.solar_battery = label(flow, "--", &lv_font_montserrat_18, UI_COLOR_TEXT);
    lv_obj_set_pos(s_ui.solar_battery, 320, 344);
}

static void create_vehicle_page(lv_obj_t *parent)
{
    lv_obj_t *p = lv_obj_create(parent);
    lv_obj_remove_style_all(p);
    lv_obj_set_size(p, SCREEN_W, CONTENT_H);
    s_ui.pages[DASHBOARD_SCREEN_VEHICLE] = p;

    lv_obj_t *hero = card(p, 24, 18, 550, 220);
    lv_obj_set_style_bg_color(hero, lv_color_hex(0xDDE8EA), 0);
    label(hero, "Passat GTE", &lv_font_montserrat_28, UI_COLOR_PRIMARY);
    s_ui.vehicle_location = label(hero, "--", &lv_font_montserrat_18, UI_COLOR_MUTED);
    lv_obj_set_pos(s_ui.vehicle_location, 0, 42);
    lv_obj_t *car = label(hero, "====================", &lv_font_montserrat_28, UI_COLOR_TEXT);
    lv_obj_set_pos(car, 65, 126);
    lv_obj_t *ready = label(hero, "SISTEMA LISTO", &lv_font_montserrat_14, UI_COLOR_TEXT);
    lv_obj_set_pos(ready, 360, 160);

    metric_card(p, 610, 18, 180, 150, "BATERIA", &s_ui.vehicle_battery);
    metric_card(p, 812, 18, 180, 150, "DEPOSITO", &s_ui.vehicle_fuel);

    lv_obj_t *range = card(p, 24, 264, 265, 136);
    label(range, "AUTONOMIA TOTAL", &lv_font_montserrat_14, UI_COLOR_MUTED);
    s_ui.vehicle_range = label(range, "--", &lv_font_montserrat_42, UI_COLOR_PRIMARY);
    lv_obj_set_pos(s_ui.vehicle_range, 0, 42);

    lv_obj_t *split = card(p, 312, 264, 262, 136);
    s_ui.vehicle_split = label(split, "--", &lv_font_montserrat_20, UI_COLOR_TEXT);
    lv_obj_set_pos(s_ui.vehicle_split, 0, 20);

    lv_obj_t *climate = card(p, 610, 194, 382, 232);
    label(climate, "CLIMATIZACION", &lv_font_montserrat_14, UI_COLOR_MUTED);
    s_ui.vehicle_climate = label(climate, "--", &lv_font_montserrat_48, UI_COLOR_ACCENT);
    lv_obj_set_pos(s_ui.vehicle_climate, 120, 72);
    button(climate, 22, 162, 64, 50, "-", false);
    button(climate, 276, 162, 64, 50, "+", false);
}

static void create_settings_page(lv_obj_t *parent)
{
    const app_config_t *cfg = app_config_get();
    lv_obj_t *p = lv_obj_create(parent);
    lv_obj_remove_style_all(p);
    lv_obj_set_size(p, SCREEN_W, CONTENT_H);
    s_ui.pages[DASHBOARD_SCREEN_SETTINGS] = p;

    lv_obj_t *title = label(p, "Ajustes", &lv_font_montserrat_28, UI_COLOR_TEXT);
    lv_obj_set_pos(title, 32, 18);
    s_ui.settings_version = label(p, PANEL_UI_VERSION, &lv_font_montserrat_20, UI_COLOR_PRIMARY);
    lv_obj_set_pos(s_ui.settings_version, 884, 22);

    lv_obj_t *net = card(p, 32, 72, 448, 168);
    label(net, "RED", &lv_font_montserrat_14, UI_COLOR_MUTED);
    s_ui.settings_wifi = label(net, "WiFi: --", &lv_font_montserrat_20, UI_COLOR_TEXT);
    lv_obj_set_pos(s_ui.settings_wifi, 0, 42);
    s_ui.settings_wifi_ip = label(net, "IP: --", &lv_font_montserrat_20, UI_COLOR_TEXT);
    lv_obj_set_pos(s_ui.settings_wifi_ip, 0, 82);
    lv_obj_t *ssid = label(net, cfg->wifi_ssid[0] ? cfg->wifi_ssid : "SSID no configurado", &lv_font_montserrat_16, UI_COLOR_MUTED);
    lv_obj_set_pos(ssid, 0, 124);

    lv_obj_t *svc = card(p, 512, 72, 480, 230);
    label(svc, "SERVICIOS", &lv_font_montserrat_14, UI_COLOR_MUTED);
    s_ui.settings_ha = label(svc, "HA: --", &lv_font_montserrat_18, UI_COLOR_TEXT);
    lv_obj_set_pos(s_ui.settings_ha, 0, 38);
    s_ui.settings_evcc = label(svc, "EVCC: --", &lv_font_montserrat_18, UI_COLOR_TEXT);
    lv_obj_set_pos(s_ui.settings_evcc, 0, 76);
    s_ui.settings_v2c = label(svc, "V2C: --", &lv_font_montserrat_18, UI_COLOR_TEXT);
    lv_obj_set_pos(s_ui.settings_v2c, 0, 114);
    s_ui.settings_ota = label(svc, "OTA: --", &lv_font_montserrat_18, UI_COLOR_TEXT);
    lv_obj_set_pos(s_ui.settings_ota, 0, 152);

    lv_obj_t *urls = card(p, 32, 274, 960, 156);
    label(urls, "CONFIG ACTIVA", &lv_font_montserrat_14, UI_COLOR_MUTED);
    s_ui.settings_ha_url = label(urls, cfg->ha_base_url, &lv_font_montserrat_16, UI_COLOR_TEXT);
    lv_obj_set_pos(s_ui.settings_ha_url, 0, 38);
    s_ui.settings_evcc_url = label(urls, cfg->evcc_base_url, &lv_font_montserrat_16, UI_COLOR_TEXT);
    lv_obj_set_pos(s_ui.settings_evcc_url, 0, 72);
    s_ui.settings_v2c_host = label(urls, cfg->v2c_host[0] ? cfg->v2c_host : "V2C sin IP configurada", &lv_font_montserrat_16, UI_COLOR_TEXT);
    lv_obj_set_pos(s_ui.settings_v2c_host, 0, 106);
}
void dashboard_ui_init(dashboard_state_t *state)
{
    memset(&s_ui, 0, sizeof(s_ui));
    s_ui.state = state;
    ui_theme_init(&s_ui.theme);

    s_ui.root = lv_obj_create(NULL);
    lv_obj_remove_style_all(s_ui.root);
    lv_obj_add_style(s_ui.root, &s_ui.theme.screen, 0);
    lv_obj_set_size(s_ui.root, SCREEN_W, SCREEN_H);

    create_header(s_ui.root);

    s_ui.content = lv_obj_create(s_ui.root);
    lv_obj_remove_style_all(s_ui.content);
    lv_obj_set_pos(s_ui.content, 0, CONTENT_Y);
    lv_obj_set_size(s_ui.content, SCREEN_W, CONTENT_H);

    if (CONFIG_PANEL_SCREEN_CHARGER) {
        create_charger_page(s_ui.content);
    }
    if (CONFIG_PANEL_SCREEN_EVCC) {
        create_evcc_page(s_ui.content);
    }
    if (CONFIG_PANEL_SCREEN_SOLAR) {
        create_solar_page(s_ui.content);
    }
    if (CONFIG_PANEL_SCREEN_VEHICLE) {
        create_vehicle_page(s_ui.content);
    }
    if (CONFIG_PANEL_SCREEN_SETTINGS) {
        create_settings_page(s_ui.content);
    }
    create_nav(s_ui.root);

    create_parking_modal(s_ui.root);
    lv_screen_load(s_ui.root);
    set_active_screen(first_enabled_screen());
    dashboard_ui_set_state(state);
}

void dashboard_ui_show_status(const char *message)
{
    if (s_ui.ota_label && message && strstr(message, "OTA") == message) {
        lv_obj_remove_flag(s_ui.ota_panel, LV_OBJ_FLAG_HIDDEN);
        lv_label_set_text(s_ui.ota_label, message);
    }
}

void dashboard_ui_set_state(const dashboard_state_t *state)
{
    char text[96];

    if (s_ui.charger_power) {
        snprintf(text, sizeof(text), "%d", state->charger.power_w);
        lv_label_set_text(s_ui.charger_power, text);
        lv_arc_set_value(s_ui.charger_arc, state->charger.power_w);
        snprintf(text, sizeof(text), "%.1f kWh", state->charger.session_kwh);
        lv_label_set_text(s_ui.charger_energy, text);
        snprintf(text, sizeof(text), "%02d:%02d h:m", state->charger.session_minutes / 60, state->charger.session_minutes % 60);
        lv_label_set_text(s_ui.charger_time, text);
        snprintf(text, sizeof(text), "%.2f EUR", state->charger.estimated_cost_eur);
        lv_label_set_text(s_ui.charger_cost, text);
        snprintf(text, sizeof(text), "%d A", state->charger.current_limit_a);
        lv_label_set_text(s_ui.charger_current, text);
    }

    if (s_ui.evcc_soc) {
        snprintf(text, sizeof(text), "%d%%", state->evcc.soc_percent);
        lv_label_set_text(s_ui.evcc_soc, text);
        lv_arc_set_value(s_ui.evcc_arc, state->evcc.soc_percent);
        snprintf(text, sizeof(text), "Cargando a %.1f kW", state->evcc.power_kw);
        lv_label_set_text(s_ui.evcc_power, text);
        snprintf(text, sizeof(text), "%.1f kWh", state->evcc.session_kwh);
        lv_label_set_text(s_ui.evcc_energy, text);
        snprintf(text, sizeof(text), "%.3f EUR/kWh", state->evcc.price_eur_kwh);
        lv_label_set_text(s_ui.evcc_price, text);
        snprintf(text, sizeof(text), "%d A", state->evcc.current_limit_a);
        lv_label_set_text(s_ui.evcc_current, text);
    }

    if (s_ui.solar_solar) {
        snprintf(text, sizeof(text), "%.1f kW", state->solar.solar_kw);
        lv_label_set_text(s_ui.solar_solar, text);
        snprintf(text, sizeof(text), "%.1f kW", state->solar.grid_kw);
        lv_label_set_text(s_ui.solar_grid, text);
        snprintf(text, sizeof(text), "%.1f kW", state->solar.house_kw);
        lv_label_set_text(s_ui.solar_house, text);
        snprintf(text, sizeof(text), "%.1f kW", state->solar.vehicle_kw);
        lv_label_set_text(s_ui.solar_vehicle, text);
        snprintf(text, sizeof(text), "%.1f kW", state->solar.loads_kw);
        lv_label_set_text(s_ui.solar_loads, text);
        snprintf(text, sizeof(text), "%.1f kW", state->solar.battery_kw);
        lv_label_set_text(s_ui.solar_battery, text);
    }

    if (s_ui.vehicle_battery) {
        snprintf(text, sizeof(text), "%d%%", state->vehicle.battery_percent);
        lv_label_set_text(s_ui.vehicle_battery, text);
        snprintf(text, sizeof(text), "%d%%", state->vehicle.fuel_percent);
        lv_label_set_text(s_ui.vehicle_fuel, text);
        snprintf(text, sizeof(text), "%d km", state->vehicle.total_range_km);
        lv_label_set_text(s_ui.vehicle_range, text);
        snprintf(text, sizeof(text), "Electrico   %d km\n\nCombustible %d km", state->vehicle.electric_range_km, state->vehicle.fuel_range_km);
        lv_label_set_text(s_ui.vehicle_split, text);
        snprintf(text, sizeof(text), "%.1f C", state->vehicle.climate_temp_c);
        lv_label_set_text(s_ui.vehicle_climate, text);
        lv_label_set_text(s_ui.vehicle_location, state->vehicle.location);
    }

    if (s_ui.ota_panel) {
        ota_service_state_t ota_state = ota_service_get_state();
        int ota_progress = ota_service_get_progress_percent();
        if (ota_state == OTA_SERVICE_IDLE) {
            lv_obj_add_flag(s_ui.ota_panel, LV_OBJ_FLAG_HIDDEN);
        } else {
            lv_obj_remove_flag(s_ui.ota_panel, LV_OBJ_FLAG_HIDDEN);
            lv_label_set_text(s_ui.ota_label, ota_service_get_message());
            lv_bar_set_value(s_ui.ota_bar, ota_progress, LV_ANIM_ON);
        }
    }

    if (s_ui.settings_wifi) {
        snprintf(text, sizeof(text), "WiFi: %s", state->wifi_connected ? "conectado" : "sin conexion");
        lv_label_set_text(s_ui.settings_wifi, text);
        snprintf(text, sizeof(text), "IP: %s", state->wifi_ip);
        lv_label_set_text(s_ui.settings_wifi_ip, text);
        snprintf(text, sizeof(text), "HA: %s", state->ha_online ? "online" : "demo/offline");
        lv_label_set_text(s_ui.settings_ha, text);
        snprintf(text, sizeof(text), "EVCC: %s", state->evcc_online ? "online" : "demo/offline");
        lv_label_set_text(s_ui.settings_evcc, text);
        snprintf(text, sizeof(text), "V2C: %s", state->v2c_online ? "online" : "demo/offline");
        lv_label_set_text(s_ui.settings_v2c, text);
        snprintf(text, sizeof(text), "OTA: %s", ota_service_get_message());
        lv_label_set_text(s_ui.settings_ota, text);
    }
}




