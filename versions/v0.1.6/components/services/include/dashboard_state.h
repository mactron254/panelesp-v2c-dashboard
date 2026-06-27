#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    DASHBOARD_SCREEN_CHARGER = 0,
    DASHBOARD_SCREEN_EVCC,
    DASHBOARD_SCREEN_SOLAR,
    DASHBOARD_SCREEN_VEHICLE,
    DASHBOARD_SCREEN_SETTINGS,
    DASHBOARD_SCREEN_COUNT,
} dashboard_screen_t;

typedef enum {
    PARKING_ACTION_OPEN = 0,
    PARKING_ACTION_CLOSE,
    PARKING_ACTION_STOP,
} parking_action_t;

typedef struct {
    bool connected;
    bool charging;
    bool dynamic_mode;
    int power_w;
    int current_limit_a;
    float session_kwh;
    int session_minutes;
    float estimated_cost_eur;
} charger_state_t;

typedef struct {
    int soc_percent;
    float power_kw;
    int current_limit_a;
    float session_kwh;
    float price_eur_kwh;
    char mode[24];
} evcc_state_t;

typedef struct {
    float solar_kw;
    float grid_kw;
    float house_kw;
    float vehicle_kw;
    float loads_kw;
    float battery_kw;
    char battery_mode[24];
} solar_state_t;

typedef struct {
    int battery_percent;
    int fuel_percent;
    int electric_range_km;
    int fuel_range_km;
    int total_range_km;
    float climate_temp_c;
    bool climate_available;
    bool cable_connected;
    bool doors_locked;
    char location[48];
} vehicle_state_t;

typedef struct {
    bool wifi_connected;
    bool ha_online;
    bool evcc_online;
    bool v2c_online;
    bool demo_mode;
    bool parking_busy;
    bool sd_mounted;
    char status_message[96];
    char last_log[96];
    char wifi_ip[16];
    charger_state_t charger;
    evcc_state_t evcc;
    solar_state_t solar;
    vehicle_state_t vehicle;
} dashboard_state_t;

void dashboard_state_init(dashboard_state_t *state);
void dashboard_state_tick_demo(dashboard_state_t *state);
