# LVGL Pro workflow

Objetivo: usar LVGL Pro Editor free para disenar UI en XML y exportar C cuando este estable.

## Estado

- `lvgl_pro/` creado como proyecto XML LVGL 9.5.0.
- Firmware actual LVGL C no se toca: pantalla, tactil y navegacion ya funcionan.
- Proximo paso tecnico: validar XML en LVGL Viewer/Editor y despues exportar C.

## Flujo

1. Editar XML en `lvgl_pro/`.
2. Previsualizar en LVGL Pro Editor o <https://viewer.lvgl.io>.
3. Mantener 1024x600 en `project.xml`.
4. Exportar C a carpeta temporal.
5. Integrar salida en `components/ui_pro_generated`.
6. Cambiar `components/ui` solo cuando build ESP-IDF pase.

## Online Share

`viewer.lvgl.io` puede abrir repos publicos gratis si la carpeta contiene:

- `project.xml`
- `globals.xml`

Cuando el repo este en GitHub:

```text
https://github.com/ORG/REPO/tree/main/lvgl_pro
```

## Seguridad

- `project_local.xml` queda ignorado por Git.
- No guardar tokens Figma, HA ni WiFi en XML versionado.

## Pendiente

- Validar XML contra editor real.
- Definir si imagenes/iconos van compilados o desde microSD.
- Crear tests XML con capturas cuando UI final este cerrada.
