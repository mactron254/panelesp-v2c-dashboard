# WiFi remoto ESP32-C6

La placa JC1060P470C_I_W usa ESP32-P4 como host y ESP32-C6 como coprocesador WiFi.

## Estado

- ESP32-P4 no tiene WiFi nativo.
- Firmware actual arranca en demo con mensaje `WiFi remoto pendiente`.
- Vendedor incluye firmware C6: `JC1060P470_C6.bin`.
- Demo `esp_brookesia_phone` usa:
  - `espressif/esp_wifi_remote`
  - `espressif/esp_hosted`
- Ejemplo `slave` indica transporte principal `SDIO`.

## Config local

SSID/password deben ir solo en `sdkconfig` local, NVS o `menuconfig`.

No guardar claves reales en:

- `sdkconfig.defaults`
- codigo C
- XML LVGL Pro
- docs versionadas

Ruta:

```text
idf.py menuconfig > PanelESP Dashboard > WiFi
```

## Plan tecnico

1. Confirmar pines SDIO P4<->C6 desde esquematico `5_ESP32-C6.png`.
2. Anadir dependencias estables:
   - `espressif/esp_wifi_remote`
   - `espressif/esp_hosted`
3. Configurar host P4 para SDIO.
4. Flashear/validar firmware C6 vendedor si el modulo no responde.
5. Conectar `esp_wifi_init()` a backend remoto.
6. Probar conexion WPA2-Personal.

## Riesgos

- Version del binario C6 puede no coincidir con version host.
- Transporte SDIO necesita pines y timing exactos.
- Cambiar WiFi puede romper build si dependencias no soportan IDF 6.0.1.

## Regla

Mantener modo demo usable si WiFi remoto falla.
