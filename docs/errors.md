# Errores y soluciones

Registro vivo de fallos encontrados durante desarrollo. Cada nuevo error debe anadirse aqui.

## 2026-06-18 - Componente `json` no encontrado

Error:

```text
Failed to resolve component 'json' required by component 'services': unknown name.
```

Causa:

ESP-IDF 6 usa componente `cjson` desde component manager, no `json`.

Solucion:

- Cambiado `REQUIRES json` a `REQUIRES cjson`.
- Anadido `espressif/cjson` en `components/services/idf_component.yml`.

Estado:

Resuelto.

## 2026-06-18 - `esp_driver_ldo` no encontrado

Error:

```text
Failed to resolve component 'esp_driver_ldo' required by component 'main': unknown name.
```

Causa:

En ESP-IDF 6, LDO usado por MIPI DSI PHY esta bajo `esp_hw_support`.

Solucion:

- `main/CMakeLists.txt` usa `esp_hw_support`.

Estado:

Resuelto.

## 2026-06-18 - `esp_lcd_jd9165` incompatible con ESP-IDF 6

Errores:

```text
panel_dev_config->color_space
esp_lcd_dpi_panel_config_t has no member named 'pixel_format'
struct extra_dpi_panel_flags has no member named 'use_dma2d'
```

Causa:

Driver `espressif/esp_lcd_jd9165` 1.0.4 usa campos antiguos para ESP-IDF anterior.

Solucion:

- Driver JD9165 vendorizado en `components/esp_lcd_jd9165`.
- `color_space` cambiado a `rgb_ele_order`.
- `pixel_format` cambiado a `in_color_format`.
- Eliminado `.flags.use_dma2d`.

Estado:

Resuelto para build ESP-IDF 6.0.1.

## 2026-06-18 - GT911 config no existe

Error:

```text
unknown type name 'esp_lcd_touch_io_gt911_config_t'
```

Causa:

`esp_lcd_touch_gt911` 1.0.0 no expone esa estructura.

Solucion:

- Eliminado `esp_lcd_touch_io_gt911_config_t`.
- `driver_data = NULL`.

Estado:

Resuelto.

## 2026-06-18 - WiFi en ESP32-P4

Error:

```text
CONFIG_ESP_WIFI_STATIC_RX_BUFFER_NUM undeclared
CONFIG_ESP_WIFI_DYNAMIC_RX_BUFFER_NUM undeclared
CONFIG_ESP_WIFI_TX_BUFFER_TYPE undeclared
```

Causa:

ESP32-P4 no expone WiFi clasico directo en esta configuracion. La placa tiene subsistema adicional, pero el firmware base no debe depender de WiFi nativo P4 para compilar.

Solucion:

- `app_main.c` protege WiFi con `#if SOC_WIFI_SUPPORTED`.
- Si no hay WiFi soportado, app arranca en modo demo/sin conexion.

Estado:

Resuelto para build. Pendiente definir conectividad real de placa si se usa ESP32-C6/bridge.

## 2026-06-19 - Flags de pantallas no regenerados

Error:

```text
CONFIG_PANEL_SCREEN_CHARGER undeclared
CONFIG_PANEL_SCREEN_EVCC undeclared
```

Causa:

Se anadio Kconfig nuevo, pero `sdkconfig` viejo no tenia las claves.

Solucion:

- Ejecutado `idf.py set-target esp32p4` para regenerar `sdkconfig`.

Estado:

Resuelto.

## 2026-06-25 - COM4 desaparece al flashear

Error:

```text
A fatal error occurred: Could not open COM4, the port is busy or doesn't exist.
```

Causa probable:

Windows detecto `USB JTAG/serial debug unit` como `COM4`, pero el puerto desaparecio al iniciar `esptool`. Puede ser cable USB inestable/solo carga, puerto USB, placa reiniciando fuera de modo descarga, o dispositivo ocupado.

Solucion propuesta:

1. Usar cable USB de datos.
2. Conectar directo al PC, sin hub.
3. Mantener `BOOT` pulsado.
4. Pulsar y soltar `RESET`.
5. Soltar `BOOT` cuando empiece el flasheo.
6. Reintentar `idf.py -p COMx flash`.

Estado:

Pendiente: esperar que Windows vuelva a mostrar puerto COM estable.

Actualizacion 2026-06-25:

- Driver CH340/CH341 del vendedor extraido en `tools/drivers/CH340`.
- Driver `ch341ser.inf` aparece instalado en Windows.
- Windows no detecta ningun `USB-SERIAL CH340 (COMx)`.
- Solo aparece `VID_303A&PID_1001` como `USB JTAG/serial debug unit`/`COM4`.
- `idf.py -p COM4 -b 115200 flash` falla igual: `COM4` desaparece al abrir.

Siguiente paso:

Conectar puerto fisico correcto `UART/USB1` con cable de datos y alimentacion suficiente. Debe aparecer `VID_1A86` o `USB-SERIAL CH340`.

## 2026-06-25 - Firmware exige ESP32-P4 rev 3.1, placa es rev 1.0

Error:

```text
A fatal error occurred: 'bootloader/bootloader.bin' requires chip revision in range [v3.1 - v3.99] (this chip is revision v1.0).
```

Causa:

ESP-IDF 6.0.1 genera por defecto binario para ESP32-P4 rev 3.1+. La placa detectada por esptool es ESP32-P4 rev v1.0.

Solucion:

- Anadido a `sdkconfig.defaults`:
  - `CONFIG_ESP32P4_SELECTS_REV_LESS_V3=y`
  - `CONFIG_ESP32P4_REV_MIN_100=y`
- Regenerado `sdkconfig`/CMake con `idf.py reconfigure`.
- Flasheado correcto en COM6.

Estado:

Resuelto.

## 2026-06-25 - Reinicio antes de `app_main` por CPU 400 MHz en ESP32-P4 rev 1.0

Error:

```text
assert failed: esp_clk_init clk.c:104 (res)
```

Causa:

La placa detectada es ESP32-P4 rev v1.0. Con `CONFIG_ESP32P4_SELECTS_REV_LESS_V3=y`, ESP-IDF avisa que 400 MHz no esta garantizado en revisiones anteriores a v3.

Solucion:

- Anadido a `sdkconfig.defaults`:
  - `CONFIG_ESP_DEFAULT_CPU_FREQ_MHZ_360=y`
- Actualizado `sdkconfig` generado.
- `idf.py reconfigure` ya no muestra aviso de 400 MHz.
- Firmware flasheado y arranca hasta `app_main`.

Estado:

Resuelto.

## 2026-06-25 - Pantalla negra con JD9165

Error:

La placa arrancaba, pero LCD quedaba negro.

Causa:

El driver `components/esp_lcd_jd9165` tenia init minimo (`0x11`, `0x29`). El panel JC1060P470C_I_W necesita la secuencia completa del vendedor para JD9165BA y timings del `.dtsi`.

Solucion:

- Timings aplicados:
  - `dpi_clock_freq_mhz = 51.2`
  - `hsync_back_porch = 136`
  - `hsync_pulse_width = 24`
  - `hsync_front_porch = 160`
  - `vsync_back_porch = 21`
  - `vsync_pulse_width = 2`
  - `vsync_front_porch = 12`
- `LCD_RST` confirmado en `GPIO27`.
- Init completo JD9165 del vendedor copiado al driver vendorizado.
- Build y flash correctos en COM6.
- Usuario confirma que ve el menu.

Estado:

Resuelto.

## 2026-06-25 - GT911 lee ID pero falla polling tactil

Error:

```text
GT911: TouchPad_ID:0x31,0x31,0x31
GT911: esp_lcd_touch_gt911_read_data(124): I2C read error!
GT911 read failed repeatedly; touch disabled
```

Causa:

El driver registry `espressif/esp_lcd_touch_gt911` 1.0.0 leia parcialmente el ID, pero fallaba al leer registro XY. Ademas habia mezcla de pines: el BSP comun marcaba INT como NC, pero esta placa necesita direccion fijada por INT GPIO21.

Solucion:

- Vendorizado `esp_lcd_touch_gt911` 1.1.3 desde zip del vendedor.
- Eliminada dependencia registry 1.0.0.
- Configuracion final:
  - SDA GPIO7
  - SCL GPIO8
  - RST GPIO22
  - INT GPIO21
  - I2C 100 kHz
  - direccion GT911 `0x5D`
- Eliminada prelectura manual del ID antes de que el driver configure direccion/reset.
- Log correcto:

```text
GT911: TouchPad_ID:0x39,0x31,0x31
GT911: TouchPad_Config_Version:99
```

Estado:

Resuelto. Usuario confirma tactil correcto y navegacion entre pantallas funcional.

## 2026-06-26 - Migracion de trabajo a LVGL Pro XML

Motivo:

La UI ya funciona en hardware con LVGL C. Para iterar mas rapido y facilitar trabajo con IA se adopta LVGL Pro Editor free / XML como capa de diseno.

Solucion:

- Creado proyecto `lvgl_pro/`.
- `project.xml` fija LVGL `9.5.0` y target `1024x600`.
- `globals.xml` contiene tokens visuales, estilos y subjects demo.
- `components/` contiene barra superior, navegacion inferior, cards y botones.
- `screens/` contiene las 4 pantallas base.
- Firmware C funcional no se sustituye todavia.

Estado:

Preparado. Pendiente validar XML en LVGL Pro Editor/Viewer y export C.

## 2026-06-26 - WiFi remoto ESP32-C6 pendiente

Error:

UI muestra WiFi no soportado / WiFi remoto pendiente.

Causa:

ESP32-P4 no tiene WiFi nativo. La placa usa ESP32-C6 como coprocesador WiFi. El firmware actual aun no integra `esp_wifi_remote`/`esp_hosted`.

Datos encontrados:

- Demo vendedor `esp_brookesia_phone` usa `espressif/esp_wifi_remote` y `espressif/esp_hosted`.
- Ejemplo `slave` indica transporte `SDIO`.
- Vendedor incluye binario `JC1060P470_C6.bin`.

Estado:

Pendiente integrar WiFi remoto sin guardar credenciales reales en Git.

## 2026-06-26 - LVGL Pro preview solo carga componentes simples

Error:

LVGL Pro Viewer muestra aviso `No runtime in the project, using the bundled runtimes for the preview`. Las pantallas no previsualizan, pero `action_button.xml` si.

Causa:

El aviso de runtime es informativo. El fallo real viene de pantallas con componentes custom, bindings y constantes; el Viewer web es mas sensible que el parser XML basico.

Solucion:

- `lvgl_pro/screens/*.xml` reescritas inline.
- Sin componentes custom.
- Sin bindings.
- Sin referencias a constantes globales.
- Solo widgets LVGL built-in y colores directos.

Estado:

Pendiente validar en `viewer.lvgl.io`.

Actualizacion:

- Screens ya cargan tras usar sintaxis basica.
- Components fallaban salvo `action_button`.
- Components simplificados a sintaxis basica sin flex, bindings, constantes ni props color.

## 2026-06-26 - Redisenyo LVGL Pro estable

Motivo:

El usuario aporto referencias visuales: EVCC oscuro, V2C app 3.0, Power Flow Card Plus y estilo VW moderno para Passat GTE.

Solucion:

- Redisenyo de `lvgl_pro/screens/*.xml` con widgets built-in seguros.
- Aniadidos componentes `mode_chip`, `power_bar`, `flow_node`, `gauge_arc` para evolucion controlada.
- Las pantallas siguen inline para evitar que un componente rompa todo el preview.

Estado:

Pendiente validar en `viewer.lvgl.io` y enriquecer gradualmente.

## 2026-06-26 - Ajuste V2C y flujo Solar

- Problema: V2C se alejo demasiado del diseno Lumina Energy visto en hardware.
- Resolucion: `lvgl_pro/screens/charger.xml` vuelve a composicion clara con titulo Lumina Energy, chip parking, gauge central, metricas laterales y botones iniciar/detener.
- Problema: Solar no comunicaba direccion de energia como Power Flow Card Plus.
- Resolucion: `lvgl_pro/screens/solar.xml` ahora usa nodos circulares, lineas con direccion, puntos de flujo y timeline `flow_demo` activable con boton.
- Nota: si LVGL Viewer rechaza `play_timeline_event` en screen, mantener puntos/direccion visibles y mover la animacion a componente aislado.

## 2026-06-26 - EVCC falta bloque inferior completo

- Problema: `lvgl_pro/screens/evcc.xml` mantenia la composicion principal pero faltaban chips de modo `Apagado`, `Solar`, `Min+Sol`, `Rapido` y bloque demo/Passat GTE.
- Resolucion: se repuso panel inferior con potencia, cargado, duracion, Passat GTE, estado desconectado, nivel, plan y objetivo 100%.
- Validacion: XML bien formado localmente. Pendiente validar en LVGL Viewer.

## 2026-06-26 - Charger arc, switches, solar dropdown y parking

- Problema: `charger.xml` simulaba gauge con botones; no era `lv_arc` real y podia confundir el comportamiento futuro.
- Resolucion: se cambia a `lv_arc` con `value="69"`, estilos `main/indicator/knob` y `disabled="true"` para que la UI no sea control manual. Firmware actualizara valor segun potencia real.
- Problema: controles activables eran botones; debian ser interruptores.
- Resolucion: se anaden `lv_switch` para `Control dinamico` y `Carga activa`.
- Problema: Solar necesitaba selector de modo bateria SAJ AS1.
- Resolucion: se anade `lv_dropdown` con `Autoconsumo`, `TOU`, `Reserva` y etiqueta de modo actual.
- Problema: Parking debe ser accion HA `cover.puerta_parking_inteligente` con abrir/cerrar segun estado.
- Resolucion visual: boton queda como `Parking Abrir/Cerrar`. Logica real pendiente en firmware/HA: `cover.open_cover` si cerrado, `cover.close_cover` si abierto.
- Validacion: XML bien formado localmente. Pendiente validar `lv_arc disabled`, `lv_dropdown options` y `play_timeline_event` en LVGL Viewer.
## 2026-06-26 - ESP-Hosted C6 SDIO falla por mempool sin RAM interna

Error:

```text
E HS_MP: mempool create failed: no mem
assert failed: sdio_mempool_create sdio_drv.c:255 (buf_mp_g)
```

Causa:

`esp_hosted` reserva buffers de transporte SDIO en RAM interna por defecto. En ESP32-P4 con UI LVGL 1024x600 y PSRAM, la RAM interna disponible no basta para ese pool.

Solucion:

- Activado `CONFIG_ESP_HOSTED_MEMPOOL_PREFER_SPIRAM=y`.
- Activado `CONFIG_ESP_HOSTED_DFLT_TASK_FROM_SPIRAM=y`.
- Mantener `esp_wifi_remote` 1.6.1 y `esp_hosted` 2.12.9.
- Credenciales WiFi solo en `sdkconfig` local, no en Git.

Validacion:

- Build `versions/v0.1.1` correcto.
- Flash por `COM6` correcto.
- Arranque sin assert durante 45 s.
- IP recibida por WiFi C6: `192.168.50.106`.

Estado:

Resuelto en `versions/v0.1.1`.
## 2026-06-26 - OTA HTTP LAN devuelve ESP_ERR_INVALID_ARG

Error:

```text
OTA fallo: ESP_ERR_INVALID_ARG
```

Causa:

`esp_https_ota` rechaza URLs `http://` si `CONFIG_ESP_HTTPS_OTA_ALLOW_HTTP` esta desactivado. La prueba OTA LAN usa HTTP temporalmente.

Solucion:

- Creada version `versions/v0.1.2` desde `v0.1.1`.
- Activado `CONFIG_ESP_HTTPS_OTA_ALLOW_HTTP=y` en `sdkconfig.defaults`.
- Mantener HTTP solo para pruebas LAN. Para version estable usar HTTPS con certificado.

Estado:

Pendiente flashear `v0.1.2` por USB y repetir OTA.
## 2026-06-26 - UI OTA y ajustes v0.1.3

Cambio solicitado:

- Mostrar progreso OTA en pantalla.
- Quitar texto inferior de WiFi/IP.
- Mostrar version firmware.
- Mover Ajustes fuera del nav inferior y abrirlo desde boton superior con engranaje.

Solucion:

- Creada version `versions/v0.1.3` desde `v0.1.2`.
- `ota_service` usa flujo `esp_https_ota_begin/perform/finish` para exponer porcentaje.
- UI anade barra OTA superior, version `v0.1.3`, boton superior de ajustes y nav inferior solo con 4 pantallas principales.
- El texto inferior permanente se elimina.

Validacion:

- Build `versions/v0.1.3` correcto.
- Flash por `COM6` correcto.
- Servidor OTA LAN sirve binario `v0.1.3` con tamano `1696608` bytes.

Estado:

Pendiente validacion visual por usuario en placa y prueba OTA viendo barra.
