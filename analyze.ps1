$ErrorActionPreference = "Stop"
Push-Location $PSScriptRoot
try {
    clang --analyze --analyzer-output text -std=c11 -Wall -Wextra -Wpedantic -Ithird_party/unity/src test_dyn_array_macro.c
    $analysisExitCode = $LASTEXITCODE
}
finally { Pop-Location }
exit $analysisExitCode
