# Contexto IA - PanelESP V2C Dashboard

## Estado actual

Proyecto ESP-IDF para ESP32-P4 JC1060P470C_I_W con LVGL 9.5.

Version actual:

- `VERSION`: 0.1.0
- `STABLE_VERSION`: 0.1.0

Objetivo inmediato:

1. Hacer funcionar aplicacion completa normal con 4 pantallas.
2. Validar en placa real: pantalla, tactil, navegacion, servicios, parking, OTA.
3. Solo despues crear variantes/versiones por pantallas.

## Hardware objetivo

- ESP32-P4
- LCD 1024x600 horizontal
- MIPI-DSI JD9165
- Tactil GT911 por I2C
- PSRAM 32 MB
- Flash 16 MB
- Ranura microSD disponible

Pinout actual:

- LCD reset: GPIO 27
- Backlight: GPIO 23
- GT911 SDA: GPIO 7
- GT911 SCL: GPIO 8
- GT911 reset: GPIO 22
- GT911 interrupt: GPIO 21
- MIPI DSI PHY LDO: canal 3, 2500 mV
- microSD: referencia BSP incluida usa SDMMC slot 0, 4-bit, D0 GPIO39, D1 GPIO40, D2 GPIO41, D3 GPIO42, CLK GPIO43, CMD GPIO44, LDO canal 4. Validar en placa antes de activar por defecto.
- WiFi: ESP32-P4 no tiene WiFi nativo. Usar `esp_wifi_remote` con coprocesador externo; ultima estable verificada en registry Espressif: `1.6.1`. Pendiente transporte real.

Uso previsto microSD:

- Assets LVGL pesados: imagenes, iconos, temas.
- Logs de errores/servicios para diagnostico.
- Config export/import sin recompilar.
- Paquetes OTA descargados/verificados antes de aplicar, si memoria lo pide.

## Pantallas

App normal debe mantener las 4 pantallas activas por defecto:

- Cargador V2C
- EVCC
- Solar HA
- Passat GTE

La seleccion individual existe en `menuconfig`, pero las variantes por pantalla se haran mas tarde, cuando la aplicacion base funcione.

Ruta:

`PanelESP Dashboard > Screens`

## Servicios

Home Assistant:

- URL demo: `http://192.168.50.120:8123`
- Parking: `cover.puerta_parking_inteligente`
- Token nunca hardcodeado.

EVCC:

- URL demo: `http://192.168.50.120:7070`

V2C Trydan:

- IP configurable.
- Si no hay IP, modo demo.

OTA:

- URL/certificado configurables.
- Si falla descarga/particion, no reiniciar.

## Reglas para nuevo chat IA

1. Responder siempre en espanol.
2. Usar respuestas compactas.
3. No crear variantes por pantalla hasta que la app completa funcione en hardware.
4. Mantener 4 pantallas activas por defecto.
5. Registrar todo error nuevo en `docs/errors.md`.
6. Si se corrige un error, anadir causa, solucion y fecha.
7. No subir secretos reales a Git.
8. No borrar versiones antiguas.
9. Antes de cambiar codigo, leer `README.md`, `AGENTS.md`, `AI_CONTEXT.md`, `docs/errors.md`.
10. Para build usar ESP-IDF 6.0.1+ estable.

## Comandos base

```powershell
cd "C:\Users\marco\Documents\panel cargdaor 2.0"
idf.py set-target esp32p4
idf.py build
```

Version actual:

```powershell
cd "C:\Users\marco\Documents\panel cargdaor 2.0\versions\v0.1.0"
idf.py set-target esp32p4
idf.py build
```
