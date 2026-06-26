# LVGL Pro - PanelESP V2C Dashboard

Proyecto XML para LVGL Pro Editor gratuito / Online Share.

Estado v0.1.5:

- Fuente visual principal para pulir UI antes de portar a firmware C.
- Estilo Claro Lumina: fondo calido, tarjetas claras, texto alto contraste.
- Target 1024x600 horizontal.
- Header comun: Lumina Energy, Parking, WiFi, version, SET.
- Nav inferior: solo Cargador, EVCC, Solar, Vehiculo.
- Ajustes se abre por SET, no por nav inferior.
- microSD opcional para iconos, logs y backup; la UI debe arrancar sin SD.

Abrir online:

1. Abrir directo: https://viewer.lvgl.io/?repo=https://github.com/mactron254/panelesp-v2c-dashboard/tree/main/lvgl_pro
2. Alternativa manual: abrir https://viewer.lvgl.io y pegar https://github.com/mactron254/panelesp-v2c-dashboard/tree/main/lvgl_pro en Open Project
3. Para ver cambios nuevos en Viewer, primero subir cambios a GitHub main o rama equivalente.

Estructura:

- project.xml: LVGL 9.5.0 y target 1024x600.
- globals.xml: tokens Claro Lumina y subjects demo.
- screens/: charger, evcc, solar, vehicle, settings.
- components/: piezas reutilizables para evolucion futura.
- sdcard/: plantilla opcional para assets, logs y backup.

microSD:

- Formato recomendado: FAT32.
- Tabla de particiones: MBR.
- Particiones: una sola particion primaria.
- Tamano cluster: 32 KB recomendado para 4 GB; 16 KB tambien valido.
- Etiqueta sugerida: PANELESP.
- Evitar exFAT/NTFS para maxima compatibilidad ESP-IDF/FatFS.

Regla:

- No sustituir firmware funcional hasta validar export C y build ESP-IDF.

