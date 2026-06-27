param(
    [Parameter(Mandatory = $true)]
    [ValidatePattern('^\d+\.\d+\.\d+$')]
    [string] $Version
)

$ErrorActionPreference = 'Stop'

$root = Split-Path -Parent $PSScriptRoot
$target = Join-Path $root ("versions\v{0}" -f $Version)

if (-not (Test-Path -LiteralPath $target)) {
    throw "Version folder does not exist: $target"
}

Set-Content -LiteralPath (Join-Path $root 'STABLE_VERSION') -Value $Version -NoNewline
Write-Host "Stable version set to $Version"
