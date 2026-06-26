# Configuracion

Este firmware esta pensado para que cada usuario active solo lo que necesita.

## Ruta rapida

```powershell
cd "C:\Users\marco\Documents\panel cargdaor 2.0\versions\v0.1.0"
idf.py menuconfig
idf.py build
```

En `menuconfig`, entrar en:

`PanelESP Dashboard`

## Elegir pantallas

Ruta:

`PanelESP Dashboard > Screens`

Opciones:

- `Enable Cargador V2C screen`
- `Enable EVCC screen`
- `Enable Solar Home Assistant screen`
- `Enable Passat GTE screen`

Se puede dejar 1, 2, 3 o 4 pantallas. La barra inferior se centra sola con las pantallas activas.

Recomendacion:

- Solo V2C: activar `Cargador V2C`.
- V2C + excedentes: activar `Cargador V2C`, `EVCC`, `Solar`.
- Vista completa casa/coche: activar las 4.

## Home Assistant

Ruta:

`PanelESP Dashboard > Home Assistant`

Campos:

- `Home Assistant base URL`: ejemplo `http://192.168.50.120:8123`
- `Home Assistant long-lived token`: token local, no subir a Git
- `Parking cover entity`: por defecto `cover.puerta_parking_inteligente`

Servicios usados para parking:

- `cover.open_cover`
- `cover.close_cover`
- `cover.stop_cover`

## EVCC

Ruta:

`PanelESP Dashboard > EVCC`

Campo:

- `EVCC base URL`: ejemplo `http://192.168.50.120:7070`

## V2C Trydan

Ruta:

`PanelESP Dashboard > V2C Trydan`

Campo:

- `V2C Trydan host or IP`: IP local del cargador, sin `http://`

Si esta vacio, UI usa demo y muestra estado local.

## OTA

Ruta:

`PanelESP Dashboard > OTA`

Campos:

- `HTTPS OTA firmware URL`
- `HTTPS OTA server certificate PEM`

Si URL/certificado no son validos, OTA falla sin reiniciar.

## Hardware

Ruta:

`PanelESP Dashboard > Board pins`

Valores por defecto para JC1060P470C_I_W:

- LCD reset: GPIO 27
- Backlight: GPIO 23
- GT911 SDA: GPIO 7
- GT911 SCL: GPIO 8
- GT911 reset: GPIO 22
- GT911 interrupt: GPIO 21
- MIPI DSI PHY LDO: canal 3, 2500 mV

## WiFi

ESP32-P4 no tiene WiFi nativo. Esta placa debe usar WiFi remoto mediante coprocesador externo.

- Componente objetivo estable: `espressif/esp_wifi_remote^1.6.1`.
- Pendiente definir transporte real de la placa: `esp-hosted`, `eppp` o `AT`.
- Hasta entonces la app arranca en modo demo y muestra `WiFi remoto pendiente`.

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

## No subir secretos

No subir `sdkconfig` si contiene:

- WiFi real.
- Token Home Assistant.
- URL OTA privada.
- Certificados privados.

Subir solo `sdkconfig.defaults` con valores demo.
