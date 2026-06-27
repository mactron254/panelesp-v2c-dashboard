# PanelESP V2C Dashboard

Firmware ESP-IDF para ESP32-P4 con dashboard LVGL 9.5 para cargador V2C Trydan, EVCC, Home Assistant solar y Volkswagen Passat GTE.

## Hardware objetivo

- ESP32-P4 con 32 MB PSRAM y 16 MB flash.
- Pantalla 7" 1024x600 horizontal.
- LCD MIPI-DSI con controlador JD9165.
- Tactil capacitivo GT911 por I2C.

## Pinout confirmado desde `JC1060P470C_I_W.zip`

- LCD reset: GPIO 27.
- LCD backlight: GPIO 23, PWM LEDC.
- GT911 I2C: SDA GPIO 7, SCL GPIO 8.
- GT911 reset: GPIO 22.
- GT911 interrupt: GPIO 21.
- MIPI-DSI: 2 lanes.
- MIPI DSI PHY LDO: canal 3, 2500 mV.

## Build

```powershell
cd "C:\Users\marco\Documents\panel cargdaor 2.0\versions\v0.1.0"
idf.py set-target esp32p4
idf.py build
```

Usa ESP-IDF 6.0.1 o superior estable con soporte ESP32-P4/MIPI-DSI.

## Flash

```powershell
idf.py -p COMx flash monitor
```

## Configuracion

Usa `idf.py menuconfig`:

- `PanelESP Dashboard > WiFi`
- `PanelESP Dashboard > Home Assistant`
- `PanelESP Dashboard > EVCC`
- `PanelESP Dashboard > V2C Trydan`
- `PanelESP Dashboard > Screens`
- `PanelESP Dashboard > OTA`
- `PanelESP Dashboard > Board pins`

El token de Home Assistant se configura en NVS/menuconfig y no se guarda en codigo fuente.

Ver guia completa: [`docs/configuration.md`](docs/configuration.md).

## Pantallas opcionales

En `PanelESP Dashboard > Screens` cada usuario puede activar 1, 2, 3 o 4 pantallas:

- Cargador V2C.
- EVCC.
- Solar Home Assistant.
- Passat GTE.

La navegacion inferior se adapta sola.

## Continuidad con IA

- Directrices: [`AGENTS.md`](AGENTS.md).
- Contexto para chat nuevo: [`AI_CONTEXT.md`](AI_CONTEXT.md).
- Errores y soluciones: [`docs/errors.md`](docs/errors.md).

## Estado v1

- UI LVGL completa con 4 pantallas y datos simulados.
- Clientes HTTP reales preparados para HA, EVCC, V2C Trydan y OTA.
- Parking global por `cover.puerta_parking_inteligente`.
- OTA HTTPS por URL configurable.

Los pines de MIPI/I2C/reset/backlight deben ajustarse cuando se confirme el pinout exacto del P4JC1060P470C.
