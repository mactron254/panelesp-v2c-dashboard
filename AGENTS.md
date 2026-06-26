# AGENTS.md

## Idioma y estilo

- Responder siempre en espanol.
- Ser breve y tecnico.
- Si se usa `pnpm`, usar siempre version 11 o superior.

## Objetivo del proyecto

Firmware ESP-IDF para ESP32-P4 JC1060P470C_I_W con LVGL 9.5. Dashboard configurable para:

- Cargador V2C Trydan.
- EVCC.
- Solar/Home Assistant.
- Passat GTE.

## Directrices para IA

Cuando alguien pida configurar este proyecto, seguir este orden:

1. Leer `README.md`, `docs/configuration.md`, `VERSION`, `STABLE_VERSION`.
2. Leer `AI_CONTEXT.md` y `docs/errors.md` si existen.
3. Preguntar solo estos datos si faltan: pantallas deseadas, IP/URL de servicios, entidades Home Assistant.
4. Cambiar configuracion por `idf.py menuconfig` o `sdkconfig.defaults`. No hardcodear tokens en codigo.
5. Compilar desde version activa:
   `cd versions/vX.Y.Z && idf.py set-target esp32p4 && idf.py build`.
6. Si build pasa, solo entonces marcar version estable con `tools/set-stable-version.ps1`.

Prioridad actual: primero hacer funcionar app normal completa con 4 pantallas en hardware. Las variantes por pantalla se crean despues.

## Reglas de versionado

- Cada version vive en `versions/vX.Y.Z`.
- No borrar versiones antiguas.
- Cambios grandes crean nueva carpeta con `tools/new-version.ps1`.
- `STABLE_VERSION` solo apunta a una version compilada y validada.
- Variantes por pantalla se crearan solo cuando la aplicacion base funcione completa.

## Configuracion facil

Pantallas opcionales en:

`idf.py menuconfig > PanelESP Dashboard > Screens`

Servicios en:

- `PanelESP Dashboard > Home Assistant`
- `PanelESP Dashboard > EVCC`
- `PanelESP Dashboard > V2C Trydan`
- `PanelESP Dashboard > OTA`

## Seguridad

- Nunca guardar tokens reales en Git.
- Preferir variables/NVS/menuconfig local.
- No subir `sdkconfig` con secretos reales.
- `sdkconfig.defaults` debe contener valores demo o seguros.

## Validacion minima

- `idf.py build` debe pasar en raiz y en `versions/vX.Y.Z`.
- Probar navegacion segun pantallas activas.
- Parking global visible siempre.
- Sin token HA: modo demo usable.
- OTA invalida: error visible, sin reinicio.
- Todo error nuevo queda registrado en `docs/errors.md`.
