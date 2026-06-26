# Configuracion

```powershell
cd "C:\Users\marco\Documents\panel cargdaor 2.0\versions\v0.1.0"
idf.py menuconfig
idf.py build
```

## Pantallas

Ruta:

`PanelESP Dashboard > Screens`

Opciones:

- `Enable Cargador V2C screen`
- `Enable EVCC screen`
- `Enable Solar Home Assistant screen`
- `Enable Passat GTE screen`

Se puede activar 1, 2, 3 o 4. La barra inferior se centra sola.

## Servicios

Home Assistant:

- Base URL: `http://192.168.50.120:8123`
- Token largo: local, no subir a Git.
- Parking: `cover.puerta_parking_inteligente`

EVCC:

- Base URL: `http://192.168.50.120:7070`

V2C:

- IP local del Trydan, sin `http://`

OTA:

- URL HTTPS del binario.
- Certificado PEM del servidor.

## Hardware JC1060P470C_I_W

- LCD reset: GPIO 27
- Backlight: GPIO 23
- GT911 SDA: GPIO 7
- GT911 SCL: GPIO 8
- GT911 reset: GPIO 22
- GT911 interrupt: GPIO 21
- MIPI DSI PHY LDO: canal 3, 2500 mV

## WiFi

ESP32-P4 no tiene WiFi nativo. Esta placa debe usar WiFi remoto con coprocesador externo.

- Componente objetivo estable: `espressif/esp_wifi_remote^1.6.1`.
- Pendiente definir transporte real: `esp-hosted`, `eppp` o `AT`.
- Hasta entonces: modo demo, `WiFi remoto pendiente`.

## microSD

La placa incluye ranura microSD. Uso previsto, cuando pantalla/tactil base ya funcionen:

- Guardar assets LVGL grandes.
- Guardar logs de diagnostico.
- Importar/exportar configuracion.
- Apoyar OTA con descarga temporal si hace falta.

Estado actual:

- No activar aun en firmware.
- Referencia encontrada en BSP incluido: SDMMC slot 0, 4-bit.
- Pines referencia: D0 GPIO39, D1 GPIO40, D2 GPIO41, D3 GPIO42, CLK GPIO43, CMD GPIO44.
- Alimentacion referencia: LDO canal 4.
- Validar en placa antes de activar por defecto.
- No debe bloquear arranque si no hay tarjeta.

## Secretos

No subir `sdkconfig` si contiene WiFi real, token HA, OTA privada o certificados.
