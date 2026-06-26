# Prompt para abrir chat nuevo

Copia este texto en un chat nuevo con la IA:

```text
Estamos trabajando en el proyecto:

C:\Users\marco\Documents\panel cargdaor 2.0

Responde siempre en espanol, breve y tecnico. Usa estilo compacto. Si usas pnpm, debe ser pnpm 11 o superior.

Primero lee estos archivos:

1. README.md
2. AGENTS.md
3. AI_CONTEXT.md
4. docs/errors.md
5. docs/configuration.md
6. VERSION
7. STABLE_VERSION

Objetivo actual:

Primero hacer funcionar la aplicacion normal completa con las 4 pantallas:

- Cargador V2C
- EVCC
- Solar Home Assistant
- Passat GTE

No crear todavia variantes por pantalla. Las variantes/versiones por pantalla se haran despues, cuando la app base funcione bien en la placa real.

Hardware:

- ESP32-P4 JC1060P470C_I_W
- LCD 1024x600 horizontal
- MIPI-DSI JD9165
- Tactil GT911 por I2C
- PSRAM 32 MB
- Flash 16 MB

Pinout actual:

- LCD reset: GPIO 27
- Backlight: GPIO 23
- GT911 SDA: GPIO 7
- GT911 SCL: GPIO 8
- GT911 reset: GPIO 22
- GT911 interrupt: GPIO 21
- MIPI DSI PHY LDO: canal 3, 2500 mV

Reglas importantes:

- No hardcodear tokens.
- No subir sdkconfig con secretos.
- Mantener sdkconfig.defaults con valores demo/seguros.
- Registrar cada error nuevo en docs/errors.md.
- Si corriges un error, anade causa, solucion, fecha y estado.
- No borrar versiones antiguas.
- Version actual: 0.1.0.
- Ultima estable: 0.1.0.
- Mantener la app normal con 4 pantallas activas por defecto.

Estructura del proyecto:

- main/: app_main, display, touch, Kconfig.
- components/ui/: interfaz LVGL.
- components/services/: Home Assistant, EVCC, V2C, OTA, estado/config.
- components/esp_lcd_jd9165/: driver local parcheado para ESP-IDF 6.
- docs/: configuracion, errores, GitHub, validacion, hardware.
- tools/: scripts de versionado.
- versions/v0.1.0/: snapshot versionado.

Build base:

cd "C:\Users\marco\Documents\panel cargdaor 2.0"
idf.py set-target esp32p4
idf.py build

Build version:

cd "C:\Users\marco\Documents\panel cargdaor 2.0\versions\v0.1.0"
idf.py set-target esp32p4
idf.py build

Ultimo estado conocido:

- El firmware compila en raiz y en versions/v0.1.0.
- LVGL 9.5 activo.
- JD9165 vendorizado y parcheado.
- GT911 compila con driver 1.0.0.
- ESP32-P4 no usa WiFi nativo directo; si no hay WiFi soportado, arranca en demo/sin conexion.
- Falta probar en hardware real: pantalla, tactil, servicios, parking y OTA.

Siguiente trabajo recomendado:

1. Probar en placa real.
2. Si falla arranque/pantalla/tactil, leer monitor serie.
3. Registrar error en docs/errors.md.
4. Corregir codigo.
5. Compilar raiz y versions/v0.1.0.
6. Solo cuando app base funcione, crear variantes por pantallas.
```

## Nota para el humano

Antes de abrir chat nuevo, si hay logs nuevos de `idf.py monitor`, pegarlos tambien.
