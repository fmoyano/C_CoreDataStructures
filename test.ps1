$ErrorActionPreference = "Stop"
& "$PSScriptRoot/build.ps1"

# Discover the sanitizer runtime directory from the installed Clang version.
$resourceDir = & clang -print-resource-dir
if ($LASTEXITCODE -ne 0) { throw "Cannot locate the Clang resource directory." }
$runtimeDir = Join-Path $resourceDir.Trim() "lib/windows"
$originalPath = $env:PATH
try {
    if (Test-Path $runtimeDir) { $env:PATH = "$runtimeDir;$env:PATH" }
    & "$PSScriptRoot/test_dyn_array.exe"
    $testExitCode = $LASTEXITCODE
}
finally { $env:PATH = $originalPath }
exit $testExitCode
