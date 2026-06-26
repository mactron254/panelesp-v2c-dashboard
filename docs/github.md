# Preparar GitHub

## Antes de publicar

Verificar:

```powershell
idf.py build
cd versions\v0.1.0
idf.py build
```

Comprobar:

- `README.md` explica build y flash.
- `docs/configuration.md` explica pantallas y servicios.
- `AGENTS.md` explica como una IA debe configurar el proyecto.
- `VERSION` y `STABLE_VERSION` son correctos.
- No hay tokens reales en `sdkconfig`, logs o docs.

## Estructura esperada

```text
.
|-- AGENTS.md
|-- README.md
|-- VERSION
|-- STABLE_VERSION
|-- main/
|-- components/
|   |-- ui/
|   |-- services/
|   `-- esp_lcd_jd9165/
|-- docs/
|-- tools/
`-- versions/
    `-- v0.1.0/
```

## Crear nueva version

```powershell
.\tools\new-version.ps1 -Version 0.1.1
```

Compilar:

```powershell
cd versions\v0.1.1
idf.py set-target esp32p4
idf.py build
```

Marcar estable solo si compila y se prueba:

```powershell
..\..\tools\set-stable-version.ps1 -Version 0.1.1
```

## Directrices para usuarios con IA

Pueden dar 3 instrucciones tipo:

```text
1. Quiero solo pantallas V2C y Solar.
2. Mi Home Assistant es http://192.168.1.50:8123 y parking es cover.garaje.
3. Compila firmware para ESP32-P4 JC1060P470C_I_W.
```

La IA debe aplicar config, compilar y reportar binario generado.
