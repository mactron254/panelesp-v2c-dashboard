# Contexto IA - PanelESP V2C Dashboard

## Estado actual

Proyecto ESP-IDF para ESP32-P4 JC1060P470C_I_W con LVGL 9.5.

Version actual: `0.1.4`.
Version estable marcada: `0.1.0` hasta validar hardware/OTA completo.

## Comandos base

```powershell
cd "C:\Users\marco\Documents\panel cargdaor 2.0\versions\v0.1.4"
idf.py set-target esp32p4
idf.py build
idf.py -p COM6 flash monitor
```

## Ultimos cambios

- v0.1.2: OTA HTTP LAN habilitada para pruebas.
- v0.1.3: progreso OTA en pantalla, version visible, ajustes arriba, sin WiFi/IP abajo.
- v0.1.4: OTA movida a Ajustes, header con `WiFi` + version + `SET`, eliminado emoji engranaje, barra OTA sin animacion y progreso menos agresivo para reducir parpadeo.

## Pendiente inmediato

- Compilar/flashear `v0.1.4`.
- Probar OTA desde Ajustes.
- Verificar si baja parpadeo durante descarga.
- Si sigue parpadeando, siguiente paso: pausar polling/redibujos de UI durante `OTA_SERVICE_RUNNING` y dejar solo barra cada 500 ms.

## MicroSD

Ranura microSD disponible. Para 2-4 GB basta clase 10/U1. No depender de SD para UI principal hasta validar montaje SDMMC.

## Reglas

- Responder en espanol, breve y tecnico.
- No hardcodear tokens ni secretos.
- Cada version en `versions/vX.Y.Z`.
- Registrar errores en `docs/errors.md`.
- No mover `STABLE_VERSION` hasta validar version real.