# LVGL Pro XML - notas de referencia

Fuente revisada:

- https://lvgl.io/docs/open/details/xml
- https://lvgl.io/docs/pro
- https://lvgl.io/docs/pro/online-viewer

## Puntos clave

- La documentacion XML se movio desde Open Docs a LVGL Pro Docs.
- LVGL Pro usa XML declarativo para UIs embebidas: pantallas, componentes, widgets, estilos, assets, subjects, animaciones y tests.
- Online Share / Viewer carga proyectos desde GitHub publicos.
- La URL puede apuntar a repo, rama o carpeta. Para este proyecto:
  https://viewer.lvgl.io/?repo=https://github.com/mactron254/panelesp-v2c-dashboard/tree/main/lvgl_pro
- La carpeta cargada por Viewer debe contener project.xml y globals.xml.
- Viewer permite ver/editar en navegador, pero no guarda cambios en GitHub. Hay que commitear y subir desde repo local.
- CLI lved-cli.js puede validar XML, generar C, compilar preview y correr tests. Requiere Node 18+; Podman si no es Windows.
- globals.xml puede declarar constantes, estilos, imagenes, fuentes y subjects globales.
- iew define la estructura visual dentro de screen, component, widget o 	est.
- screen no usa extends; component puede extender widgets/componentes; widget solo widgets.
- En atributos XML usar entidades como &#10; para saltos de linea en maps/dropdowns; evitar \n literal.
- Para widgets interactivos usar eventos oficiales como clicked, alue_changed, screen_loaded, etc.

## Reglas para este proyecto

- Mantener lvgl_pro/ autocontenido y cargable desde Viewer.
- Mantener XML simple mientras Viewer sea sensible: widgets built-in, estilos locales/globales, sin dependencias externas obligatorias.
- No depender de microSD para cargar pantalla inicial.
- Subir cambios a GitHub antes de pedir validacion visual en Viewer.
