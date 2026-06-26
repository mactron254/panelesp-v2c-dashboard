# AGENTS.md

## Idioma y estilo

- Responder siempre en espanol.
- Ser breve y tecnico.
- Si se usa `pnpm`, usar siempre version 11 o superior.

## Objetivo

Firmware ESP-IDF para ESP32-P4 JC1060P470C_I_W con LVGL 9.5. Dashboard configurable: V2C, EVCC, Solar/Home Assistant y Passat GTE.

## Directrices IA

1. Leer `README.md`, `docs/configuration.md`, `VERSION`, `STABLE_VERSION`.
2. Leer `AI_CONTEXT.md` y `docs/errors.md`.
3. Pedir solo datos faltantes: pantallas deseadas, IP/URL servicios, entidades HA.
4. Configurar por `idf.py menuconfig` o `sdkconfig.defaults`; nunca hardcodear tokens.
5. Compilar con `idf.py set-target esp32p4 && idf.py build`.
6. Marcar estable solo si build pasa y hardware/UI se valida.

Prioridad actual: primero app normal completa con 4 pantallas. Variantes por pantalla despues.

## Pantallas

Ruta:

`idf.py menuconfig > PanelESP Dashboard > Screens`

Activar 1, 2, 3 o 4:

- Cargador V2C.
- EVCC.
- Solar Home Assistant.
- Passat GTE.

## Seguridad

- No guardar tokens reales en Git.
- No subir `sdkconfig` con secretos.
- `sdkconfig.defaults` solo con valores demo/seguros.
