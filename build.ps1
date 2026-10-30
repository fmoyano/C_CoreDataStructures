$ErrorActionPreference = "Stop"
Push-Location $PSScriptRoot
try {
    clang -std=c11 -Wall -Wextra -Wpedantic -g -fno-omit-frame-pointer -fsanitize=address,undefined -Ithird_party/unity/src test_dyn_array_macro.c third_party/unity/src/unity.c -o test_dyn_array.exe
    if ($LASTEXITCODE -ne 0) { throw "Compilation failed (exit $LASTEXITCODE)." }
}
finally { Pop-Location }
