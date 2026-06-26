# Versionado por carpetas

Cada version estable del firmware debe vivir en su propia carpeta:

```text
versions/
  v0.1.0/
  v0.2.0/
  v1.0.0/
```

Reglas:

- La raiz del repo es zona de trabajo actual.
- `VERSION` indica version actual en desarrollo.
- `STABLE_VERSION` indica ultima version estable conocida.
- Antes de cambios grandes, crear nueva carpeta `versions/vX.Y.Z`.
- Cada entrega que funcione debe marcarse como estable.
- No modificar versiones antiguas salvo correccion documental clara.
- Si una version nueva falla, copiar la version antigua desde su carpeta o comparar contra ella.
- No versionar `build/`, `managed_components/`, `.git/`, `.codex/`, `sdkconfig`, `dependencies.lock`.

Crear snapshot:

```powershell
.\tools\new-version.ps1 -Version 0.2.0
```

Crear snapshot y marcarlo como ultima estable:

```powershell
.\tools\new-version.ps1 -Version 0.2.0 -Stable
```

Marcar una version existente como estable:

```powershell
.\tools\set-stable-version.ps1 -Version 0.2.0
```
