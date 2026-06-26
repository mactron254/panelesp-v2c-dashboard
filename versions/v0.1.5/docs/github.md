# Preparar GitHub

## Checklist

- `idf.py build` pasa.
- `README.md` explica build/flash.
- `docs/configuration.md` explica pantallas y servicios.
- `AGENTS.md` explica como una IA configura el proyecto.
- `VERSION` y `STABLE_VERSION` correctos.
- Sin tokens reales.

## Instrucciones IA ejemplo

```text
1. Quiero solo pantallas V2C y Solar.
2. Home Assistant es http://192.168.1.50:8123 y parking es cover.garaje.
3. Compila firmware para ESP32-P4 JC1060P470C_I_W.
```

IA debe configurar, compilar y reportar binario.
