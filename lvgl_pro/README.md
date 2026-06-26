# LVGL Pro - PanelESP V2C Dashboard

Proyecto XML para LVGL Pro Editor gratuito / Online Share.

Estado:

- Firmware actual funcional sigue en LVGL C dentro de ESP-IDF.
- Esta carpeta es capa declarativa para iterar UI sin tocar driver LCD/tactil.
- Integracion final sera exportar C desde LVGL Pro y meterlo en `components/ui_pro_generated`.

Abrir online:

1. Abrir <https://viewer.lvgl.io>.
2. En `Open Project`, pegar exactamente:
   `https://github.com/ORG/REPO/tree/main/lvgl_pro`

No abrir un XML suelto. LVGL Pro necesita abrir la carpeta de proyecto que contiene `project.xml` y `globals.xml`.

Nota: si Online Share muestra `Project path is not available` al exportar codigo, usar LVGL Pro Editor local. El visor web sirve para previsualizar/editar; la exportacion puede depender de ruta local.

Estructura:

- `project.xml`: LVGL 9.5.0 y target 1024x600.
- `globals.xml`: colores, estilos, subjects demo.
- `components/`: piezas reutilizables.
- `screens/`: 4 pantallas actuales.

Regla:

- No sustituir firmware funcional hasta validar export C y build ESP-IDF.
