# Nuevo Chat - PanelESP V2C Dashboard

Lee esto primero y continua sin reiniciar proyecto.

## Proyecto

Ruta local: C:\Users\marco\Documents\panel cargdaor 2.0

Firmware ESP-IDF para ESP32-P4 JC1060P470C_I_W, pantalla 1024x600 MIPI-DSI JD9165, tactil GT911, WiFi con ESP32-C6 por ESP-Hosted/SDIO, LVGL 9.5.

## Versiones

- Version actual de trabajo: versions/v0.1.5.
- Version estable marcada: STABLE_VERSION = 0.1.0.
- No borrar versiones antiguas.
- No commitear sdkconfig, build, managed_components, dependencies.lock.

## LVGL Pro Viewer

URL configurada para cargar en LVGL Viewer:

https://github.com/mactron254/panelesp-v2c-dashboard/tree/main/lvgl_pro

Abrir directo: https://viewer.lvgl.io/?repo=https://github.com/mactron254/panelesp-v2c-dashboard/tree/main/lvgl_pro. Para ver cambios nuevos, primero subir lvgl_pro a GitHub.

## Estado validado

- LCD funciona.
- Tactil funciona.
- WiFi C6 funciona y obtiene IP.
- OTA HTTP LAN llego a reiniciar para aplicar.
- En v0.1.4 OTA esta en Ajustes y barra sin animacion para reducir parpadeo.

## Cambio actual v0.1.5

Objetivo: pulido UI LVGL Pro antes de portar a firmware C.

- lvgl_pro/ redisenado en estilo Claro Lumina.
- Header comun: Lumina Energy, Parking, WiFi, v0.1.5, SET.
- Nav inferior solo: Cargador, EVCC, Solar, Vehiculo.
- Nueva pantalla LVGL Pro screens/settings.xml para SET.
- charger.xml: gauge potencia, control dinamico, limite A, energia/tiempo/coste, iniciar/detener.
- evcc.xml: SOC, modos EVCC, sesion, potencia, precio, plan Passat.
- solar.xml: flujo solar/red/bateria/casa/GTE y modo bateria SAJ AS1.
- vehicle.xml: estado Passat GTE, bateria, combustible, autonomia, clima, cable/puertas.
- microSD 4GB opcional: lvgl_pro/sdcard/assets/icons, logs, backup.
- Misma carpeta lvgl_pro sincronizada dentro de versions/v0.1.5/lvgl_pro.

## Proxima prueba

Validar XML local y LVGL Pro Viewer. Luego compilar desde C:\Users\marco\Documents\panel cargdaor 2.0\versions\v0.1.5:

idf.py set-target esp32p4
idf.py build
idf.py -p COM6 flash monitor

## microSD

Tarjeta 4GB clase 10/U1 suficiente. Formato recomendado: FAT32, MBR, una particion primaria, cluster 32 KB, etiqueta PANELESP. Evitar exFAT/NTFS. Uso inicial: iconos, logs, backup/export config. No hacer dependencia de arranque hasta validar montaje SDMMC.

