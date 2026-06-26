# Nuevo Chat - PanelESP V2C Dashboard

Lee esto primero y continua sin reiniciar proyecto.

## Proyecto

Ruta local:

```text
C:\Users\marco\Documents\panel cargdaor 2.0
```

Firmware ESP-IDF para placa ESP32-P4 `JC1060P470C_I_W`, pantalla 1024x600 MIPI-DSI `JD9165`, tactil `GT911`, WiFi con ESP32-C6 por ESP-Hosted/SDIO, LVGL 9.5.

## Versiones

- Version actual de trabajo: `versions/v0.1.4`.
- Version estable marcada: `STABLE_VERSION = 0.1.0`.
- No borrar versiones antiguas.
- No commitear `sdkconfig`, `build`, `managed_components`, `dependencies.lock`.

## Estado validado

- LCD funciona.
- Tactil funciona.
- WiFi C6 funciona y obtiene IP.
- OTA HTTP LAN ya llego a `reiniciar para aplicar`.
- En `v0.1.3` progreso OTA visible, pero durante OTA hay parpadeo de pantalla.

## Cambio actual v0.1.4

- OTA pasa al menu Ajustes.
- Header muestra `WiFi OK/--`, version `v0.1.4`, boton `SET` en vez de emoji engranaje.
- Boton OTA superior eliminado.
- Barra OTA sin animacion (`LV_ANIM_OFF`).
- Mensaje OTA se actualiza cada 2% y task espera 30 ms para reducir parpadeo.

## Proxima prueba

```powershell
cd "C:\Users\marco\Documents\panel cargdaor 2.0\versions\v0.1.4"
idf.py set-target esp32p4
idf.py build
idf.py -p COM6 flash monitor
```

Luego abrir Ajustes (`SET`) y pulsar `OTA`.

## Si sigue parpadeando

Reducir mas carga durante OTA:

1. No llamar `dashboard_ui_set_state()` completo mientras `OTA_SERVICE_RUNNING`.
2. Crear timer dedicado OTA cada 500 ms.
3. Mantener solo barra/label OTA activos hasta terminar.
4. Si hace falta, pausar polling HA/EVCC/V2C durante OTA.

## MicroSD

Se va a probar microSD 2 GB o 4 GB. Velocidad suficiente: clase 10/U1. Usos futuros: logs, assets, config export/import, backup OTA. No activar como dependencia de arranque todavia.