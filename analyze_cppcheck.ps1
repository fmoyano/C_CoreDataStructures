$ErrorActionPreference = "Stop"
Push-Location $PSScriptRoot
try {
   cppcheck --std=c11 --language=c "--enable=warning,style,performance,portability" --check-level=normal --max-configs=1 --error-exitcode=1 -Ithird_party/unity/src test_dyn_array_macro.c
    $analysisExitCode = $LASTEXITCODE
}
finally { Pop-Location }
exit $analysisExitCode
