# microSD opcional

Uso previsto para tarjeta 4GB clase 10/U1.

Formato recomendado:

- FAT32.
- MBR, no GPT.
- Una particion primaria.
- Cluster 32 KB recomendado para 4 GB; 16 KB tambien valido.
- Etiqueta: PANELESP.
- Evitar exFAT y NTFS salvo que el firmware active soporte especifico.

Carpetas previstas:

- assets/icons: iconos LVGL futuros.
- logs: eventos de UI, OTA, WiFi y servicios.
- backup: export/import de configuracion.

Regla: firmware debe arrancar aunque la microSD no exista o no monte.
