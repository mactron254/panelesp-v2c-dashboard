#include "dashboard_state.h"

#include <stdio.h>
#include <string.h>

void dashboard_state_init(dashboard_state_t *state)
{
    memset(state, 0, sizeof(*state));
    state->demo_mode = true;
    snprintf(state->status_message, sizeof(state->status_message), "Modo demo");
    snprintf(state->last_log, sizeof(state->last_log), "Arranque demo");
    snprintf(state->wifi_ip, sizeof(state->wifi_ip), "--");

    state->charger = (charger_state_t) {
        .connected = true,
        .charging = false,
        .dynamic_mode = true,
        .power_w = 7400,
        .current_limit_a = 32,
        .session_kwh = 42.5f,
        .session_minutes = 165,
        .estimated_cost_eur = 6.38f,
    };
    state->evcc = (evcc_state_t) {
        .soc_percent = 78,
        .power_kw = 11.2f,
        .current_limit_a = 16,
        .session_kwh = 42.8f,
        .price_eur_kwh = 0.142f,
    };
    snprintf(state->evcc.mode, sizeof(state->evcc.mode), "MIN + SOLAR");

    state->solar = (solar_state_t) {
        .solar_kw = 3.4f,
        .grid_kw = 0.0f,
        .house_kw = 2.1f,
        .vehicle_kw = 1.2f,
        .loads_kw = 0.9f,
        .battery_kw = -0.8f,
    };
    snprintf(state->solar.battery_mode, sizeof(state->solar.battery_mode), "Autoconsumo");

    state->vehicle = (vehicle_state_t) {
        .battery_percent = 80,
        .fuel_percent = 92,
        .electric_range_km = 42,
        .fuel_range_km = 800,
        .total_range_km = 842,
        .climate_temp_c = 21.5f,
        .climate_available = false,
        .cable_connected = false,
        .doors_locked = true,
    };
    snprintf(state->vehicle.location, sizeof(state->vehicle.location), "Estacionado - Madrid, ES");
}

void dashboard_state_tick_demo(dashboard_state_t *state)
{
    static int phase;
    phase = (phase + 1) % 20;

    state->charger.power_w = 6800 + (phase * 45);
    state->evcc.soc_percent = 76 + (phase % 5);
    state->solar.solar_kw = 3.0f + (float)(phase % 8) * 0.12f;
    state->solar.house_kw = 1.8f + (float)(phase % 6) * 0.08f;
    state->solar.vehicle_kw = state->charger.charging ? 1.2f : 0.0f;
}
