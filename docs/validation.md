# Validacion

## Build

```powershell
idf.py set-target esp32p4
idf.py build
```

Requisitos:

- ESP-IDF 5.3+.
- Dependencias descargadas por Component Manager.
- Pines reales configurados en `idf.py menuconfig`.
- Defaults ya cargados desde `JC1060P470C_I_W.zip`: LCD reset 27, backlight 23, GT911 SDA 7/SCL 8/RST 22/INT 21, MIPI PHY LDO canal 3 a 2500 mV.

## UI

- Arranca en pantalla `Cargador`.
- Bottom nav cambia entre `Cargador`, `EVCC`, `Solar`, `Vehiculo`.
- Boton `Puerta Parking` abre modal con `Abrir`, `Cerrar`, `Parar`, `Cancelar`.
- Sin WiFi/token, la UI sigue en modo demo.
- No debe haber solapes en 1024x600 horizontal.

## Servicios

- Home Assistant responde en `http://192.168.50.120:8123/api/`.
- Parking envia `POST /api/services/cover/open_cover|close_cover|stop_cover`.
- EVCC lee `GET http://192.168.50.120:7070/api/state`.
- V2C Trydan queda desactivado hasta configurar IP local.
- OTA falla con mensaje claro si `PANEL_OTA_URL` esta vacia o no responde.
