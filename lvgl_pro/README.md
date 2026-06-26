# LVGL Pro - PanelESP V2C Dashboard

Proyecto XML para LVGL Pro Editor gratuito / Online Share.

Estado:

- Firmware actual funcional sigue en LVGL C dentro de ESP-IDF.
- Esta carpeta es capa declarativa para iterar UI sin tocar driver LCD/tactil.
- Integracion final sera exportar C desde LVGL Pro y meterlo en `components/ui_pro_generated`.

Abrir online:

1. Subir repo publico a GitHub.
2. Abrir <https://viewer.lvgl.io>.
3. En `Open Project`, pegar URL del repo o ruta:
   `https://github.com/ORG/REPO/tree/main/lvgl_pro`

Estructura:

- `project.xml`: LVGL 9.5.0 y target 1024x600.
- `globals.xml`: colores, estilos, subjects demo.
- `components/`: piezas reutilizables.
- `screens/`: 4 pantallas actuales.

Regla:

- No sustituir firmware funcional hasta validar export C y build ESP-IDF.
