# Tooling demonstrations

These examples deliberately contain invalid memory accesses, a memory leak,
and signed integer overflow. They are excluded from the library tests and the
root analysis scripts. A diagnostic or nonzero exit is expected for unsafe cases.

Run the following commands from the repository root. The examples do not depend
on Unity. Use a Clang installation that supports the requested sanitizers.

## Runtime sanitizers

```sh
clang -std=c11 -Wall -Wextra -Wpedantic -O0 -g -fno-omit-frame-pointer -fsanitize=address,undefined -fno-sanitize-recover=all examples/tooling/sanitizers.c -o tooling_sanitizers.exe
./tooling_sanitizers.exe asan
./tooling_sanitizers.exe ubsan
```

Run each invocation separately: ASan should report a heap use-after-free;
UBSan should report signed integer overflow. `-O0` helps preserve these small
teaching examples. On Windows PowerShell use `.\tooling_sanitizers.exe` and
make sure the sanitizer runtime DLL directory is on PATH. Find the installed
Clang resource directory with `clang -print-resource-dir`; the Windows runtimes
are normally under its `lib/windows` subdirectory.

Preserved functions: `test_asan_use_after_free`, `test_ubsan_signed_overflow`.
The former uses a direct allocation check instead of a Unity assertion.

## Static analysis

```sh
clang --analyze --analyzer-output text -std=c11 -Wall -Wextra -Wpedantic examples/tooling/static_analysis.c
cppcheck --std=c11 --language=c --enable=warning,style,performance,portability --check-level=normal examples/tooling/static_analysis.c
```

Preserved functions:

- `analyzer_use_after_free`: unconditional access after freeing memory.
- `analyzer_use_after_free2`: conditional free followed by an invalid access.
- `analyzer_leak_demo1`: intentionally omits `array_free`.
- `analyzer_no_leak_demo2`: matching allocation/free as a comparison.

Diagnostics vary with analyzer and version, especially for allocations hidden
behind helper functions. The entry point exposes these examples to analysis;
do not run this file as an ordinary test executable.
