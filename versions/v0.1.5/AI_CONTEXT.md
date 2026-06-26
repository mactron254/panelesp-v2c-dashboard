# Contexto IA - PanelESP V2C Dashboard

## Estado actual

Proyecto ESP-IDF para ESP32-P4 JC1060P470C_I_W con LVGL 9.5.

Version actual: 0.1.5.
Version estable marcada: 0.1.0 hasta validar hardware/OTA completo.

## LVGL Pro Viewer

URL configurada:

https://github.com/mactron254/panelesp-v2c-dashboard/tree/main/lvgl_pro

Abrir directo: https://viewer.lvgl.io/?repo=https://github.com/mactron254/panelesp-v2c-dashboard/tree/main/lvgl_pro. Para ver cambios nuevos, subir lvgl_pro a GitHub.

## Comandos base

Desde C:\Users\marco\Documents\panel cargdaor 2.0\versions\v0.1.5:

idf.py set-target esp32p4
idf.py build
idf.py -p COM6 flash monitor

## Ultimos cambios

- v0.1.4: OTA movida a Ajustes, header con WiFi + version + SET, barra OTA sin animacion.
- v0.1.5: pulido LVGL Pro estilo Claro Lumina, header/nav comun, nueva pantalla settings.xml, microSD opcional para iconos/logs/backup.

## Pendiente inmediato

- Validar XML local y en LVGL Pro Viewer.
- Compilar versions/v0.1.5.
- Tras validar XML, decidir portado de diseno a firmware C.

## microSD

Ranura microSD disponible. Para 4 GB basta clase 10/U1. Formato recomendado: FAT32, MBR, una particion primaria, cluster 32 KB, etiqueta PANELESP. Evitar exFAT/NTFS salvo soporte firmware especifico. Uso inicial: logs, iconos, backup/export config. No depender de SD para UI principal hasta validar montaje SDMMC.

## Reglas

- Responder en espanol, breve y tecnico.
- No hardcodear tokens ni secretos.
- Cada version en versions/vX.Y.Z.
- Registrar errores en docs/errors.md.
- No mover STABLE_VERSION hasta validar version real.

