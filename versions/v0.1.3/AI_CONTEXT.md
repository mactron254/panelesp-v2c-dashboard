# Contexto IA - PanelESP V2C Dashboard

Version snapshot: 0.1.3.

Objetivo inmediato:

1. Hacer funcionar app completa normal con 4 pantallas.
2. Hardware real validado: LCD, tactil y WiFi C6 por ESP-Hosted/SDIO obtienen IP.
3. Crear variantes por pantalla solo despues.

Hardware extra:

- Ranura microSD disponible.
- Referencia BSP incluida: SDMMC slot 0, 4-bit.
- Pines referencia: D0 GPIO39, D1 GPIO40, D2 GPIO41, D3 GPIO42, CLK GPIO43, CMD GPIO44.
- Alimentacion referencia: LDO canal 4.
- Uso futuro: assets LVGL, logs, config export/import, apoyo OTA.
- No activar por defecto hasta validar en placa real.

Reglas:

- Responder en espanol.
- Mantener 4 pantallas activas por defecto.
- Registrar errores en `docs/errors.md`.
- No hardcodear tokens.
- No borrar versiones antiguas.
- Usar ESP-IDF 6.0.1+ estable.

WiFi C6:

- Backend por defecto en ESP32-P4: `PANEL_WIFI_BACKEND_ESP_HOSTED_C6_SDIO`.
- Dependencias: `espressif/esp_wifi_remote` 1.6.1 y `espressif/esp_hosted` 2.12.9.
- Necesario: `CONFIG_ESP_HOSTED_MEMPOOL_PREFER_SPIRAM=y` y `CONFIG_ESP_HOSTED_DFLT_TASK_FROM_SPIRAM=y`.
- Credenciales solo en `sdkconfig` local.

Build:

```powershell
cd "C:\Users\marco\Documents\panel cargdaor 2.0\versions\v0.1.2"
idf.py set-target esp32p4
idf.py build
```

OTA:

- HTTP OTA LAN permitido solo para pruebas locales con CONFIG_ESP_HTTPS_OTA_ALLOW_HTTP=y.
- Produccion debe volver a HTTPS con certificado.

Notas v0.1.3:

- Barra OTA superior con progreso.
- Ajustes desde boton superior con engranaje.
- Nav inferior solo 4 pantallas principales.
- Version visible en header y ajustes.
