$runtimeDir = "C:\Program Files\LLVM\lib\clang\21\lib\windows"
$env:PATH = "$runtimeDir;$env:PATH"

clang -std=c11 -Wall -Wextra -Wpedantic -g -fno-omit-frame-pointer -fsanitize=address -fsanitize=undefined -Ithird_party/unity/src test_dyn_array_macro.c third_party/unity/src/unity.c -o test_dyn_array.exe

if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE
}

.\test_dyn_array.exe
exit $LASTEXITCODE