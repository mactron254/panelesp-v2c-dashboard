# Errores y soluciones

Registro vivo de fallos encontrados durante desarrollo.

## 2026-06-18 - Componente `json` no encontrado

Causa: ESP-IDF 6 usa `cjson`.

Solucion: `REQUIRES cjson` y dependencia `espressif/cjson`.

Estado: resuelto.

## 2026-06-18 - `esp_driver_ldo` no encontrado

Causa: LDO esta bajo `esp_hw_support` en ESP-IDF 6.

Solucion: `main/CMakeLists.txt` usa `esp_hw_support`.

Estado: resuelto.

## 2026-06-18 - JD9165 incompatible con ESP-IDF 6

Causa: driver 1.0.4 usa campos antiguos.

Solucion: driver vendorizado y adaptado: `rgb_ele_order`, `in_color_format`, sin `use_dma2d`.

Estado: resuelto.

## 2026-06-18 - GT911 config no existe

Causa: `esp_lcd_touch_gt911` 1.0.0 no expone `esp_lcd_touch_io_gt911_config_t`.

Solucion: `driver_data = NULL`.

Estado: resuelto.

## 2026-06-18 - WiFi en ESP32-P4

Causa: ESP32-P4 no expone WiFi clasico directo en esta configuracion.

Solucion: WiFi protegido con `#if SOC_WIFI_SUPPORTED`; si no hay WiFi, demo/sin conexion.

Estado: build resuelto; conectividad real pendiente.

## 2026-06-19 - Flags pantallas no regenerados

Causa: `sdkconfig` viejo sin nuevas claves Kconfig.

Solucion: `idf.py set-target esp32p4`.

Estado: resuelto.

## 2026-06-25 - COM4 desaparece al flashear

Error:

```text
A fatal error occurred: Could not open COM4, the port is busy or doesn't exist.
```

Causa probable: puerto USB serial desaparece al reset/flasheo.

Solucion propuesta: cable de datos, USB directo al PC, mantener `BOOT`, pulsar `RESET`, soltar `BOOT` cuando empiece flasheo, reintentar `idf.py -p COMx flash`.

Estado: pendiente.

Actualizacion 2026-06-25: driver CH340 instalado, pero Windows no detecta `USB-SERIAL CH340`; solo aparece `VID_303A` USB-JTAG/COM4 y desaparece al flashear. Falta puerto fisico UART/USB1 correcto, cable de datos o alimentacion estable.

## 2026-06-25 - Firmware exige ESP32-P4 rev 3.1, placa es rev 1.0

Error: bootloader exige rev `[v3.1 - v3.99]`, chip detectado rev `v1.0`.

Causa: ESP-IDF 6.0.1 usa por defecto ESP32-P4 rev 3.1+.

Solucion: `CONFIG_ESP32P4_SELECTS_REV_LESS_V3=y` y `CONFIG_ESP32P4_REV_MIN_100=y` en `sdkconfig.defaults`; `idf.py reconfigure`; flasheo correcto en COM6.

Estado: resuelto.

## 2026-06-25 - Reinicio antes de `app_main` por CPU 400 MHz

Error: `assert failed: esp_clk_init clk.c:104 (res)`.

Causa: placa ESP32-P4 rev v1.0; 400 MHz no esta garantizado en revisiones anteriores a v3.

Solucion: `CONFIG_ESP_DEFAULT_CPU_FREQ_MHZ_360=y` en `sdkconfig.defaults`; `sdkconfig` actualizado; reconfigure, build y flash.

Estado: resuelto; firmware arranca hasta `app_main`.

## 2026-06-25 - Pantalla negra con JD9165

Causa: driver JD9165 tenia init minimo; el panel necesita secuencia completa del vendedor y timings del `.dtsi`.

Solucion: timings `51.2 MHz`, `HBP 136`, `HPW 24`, `HFP 160`, `VBP 21`, `VPW 2`, `VFP 12`; `LCD_RST GPIO27`; init completo JD9165 copiado al driver vendorizado.

Estado: resuelto; usuario confirma que ve el menu.

## 2026-06-25 - GT911 lee ID pero falla polling tactil

Error: `TouchPad_ID:0x31,0x31,0x31` seguido de `I2C read error`.

Causa: driver registry 1.0.0 no va bien en esta placa; faltaba seleccionar direccion por INT GPIO21.

Solucion: driver GT911 1.1.3 vendorizado desde zip vendedor; `RST GPIO22`, `INT GPIO21`, I2C `100 kHz`, direccion `0x5D`; eliminada prelectura manual antes del reset del driver.

Estado: resuelto. Usuario confirma tactil correcto y navegacion entre pantallas funcional.

## 2026-06-26 - LVGL Pro XML preparado

Causa: UI ya funciona en hardware; se adopta LVGL Pro Editor free/XML para iterar diseno con IA.

Solucion: creado `lvgl_pro/` con `project.xml`, `globals.xml`, componentes base y pantallas. Firmware C LVGL sigue intacto hasta validar export C.

Estado: pendiente validar XML en LVGL Pro Editor/Viewer.

## 2026-06-26 - WiFi remoto ESP32-C6 pendiente

Causa: ESP32-P4 no tiene WiFi nativo; placa usa ESP32-C6. Vendedor usa `esp_wifi_remote` + `esp_hosted`, transporte probable SDIO y binario `JC1060P470_C6.bin`.

Solucion pendiente: integrar WiFi remoto sin guardar credenciales reales en Git.

Estado: init resuelto; log correcto `TouchPad_ID:0x39,0x31,0x31`, config 99. Pendiente validar orientacion tactil.
