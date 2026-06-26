# Contexto IA - PanelESP V2C Dashboard

Version snapshot: 0.1.0.

Objetivo inmediato:

1. Hacer funcionar app completa normal con 4 pantallas.
2. Validar hardware real.
3. Crear variantes por pantalla solo despues.

Hardware extra:

- Ranura microSD disponible.
- Referencia BSP incluida: SDMMC slot 0, 4-bit.
- Pines referencia: D0 GPIO39, D1 GPIO40, D2 GPIO41, D3 GPIO42, CLK GPIO43, CMD GPIO44.
- Alimentacion referencia: LDO canal 4.
- Uso futuro: assets LVGL, logs, config export/import, apoyo OTA.
- No activar por defecto hasta validar en placa real.

Reglas:

- Responder en espanol.
- Mantener 4 pantallas activas por defecto.
- Registrar errores en `docs/errors.md`.
- No hardcodear tokens.
- No borrar versiones antiguas.
- Usar ESP-IDF 6.0.1+ estable.

Build:

```powershell
cd "C:\Users\marco\Documents\panel cargdaor 2.0\versions\v0.1.0"
idf.py set-target esp32p4
idf.py build
```
