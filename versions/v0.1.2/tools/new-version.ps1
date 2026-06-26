param(
    [Parameter(Mandatory = $true)]
    [ValidatePattern('^\d+\.\d+\.\d+$')]
    [string] $Version,

    [switch] $Stable
)

$ErrorActionPreference = 'Stop'

$root = Split-Path -Parent $PSScriptRoot
$target = Join-Path $root ("versions\v{0}" -f $Version)

if (Test-Path -LiteralPath $target) {
    throw "Version folder already exists: $target"
}

New-Item -ItemType Directory -Path $target | Out-Null

$excludeDirs = @('.git', '.codex', 'build', 'managed_components', 'versions')
$excludeFiles = @('sdkconfig', 'sdkconfig.old', 'dependencies.lock')

Get-ChildItem -LiteralPath $root -Force | ForEach-Object {
    if ($_.PSIsContainer) {
        if ($excludeDirs -contains $_.Name) {
            return
        }
        Copy-Item -LiteralPath $_.FullName -Destination (Join-Path $target $_.Name) -Recurse -Force
    } else {
        if ($excludeFiles -contains $_.Name) {
            return
        }
        Copy-Item -LiteralPath $_.FullName -Destination (Join-Path $target $_.Name) -Force
    }
}

Set-Content -LiteralPath (Join-Path $target 'VERSION') -Value $Version -NoNewline
Set-Content -LiteralPath (Join-Path $root 'VERSION') -Value $Version -NoNewline

if ($Stable) {
    Set-Content -LiteralPath (Join-Path $root 'STABLE_VERSION') -Value $Version -NoNewline
}

Write-Host "Created $target"
if ($Stable) {
    Write-Host "Stable version set to $Version"
}
