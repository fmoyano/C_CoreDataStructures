# C Core Data Structures

A small collection of basic data structures implemented in **C11**, with a focus on generic, reusable APIs built with macros.

The goal of this project is both practical and educational: to better understand how common data structures work internally while building implementations that can later be reused in other C projects.

## Goals

* Implement common data structures from scratch
* Keep the code compatible with C11
* Provide generic APIs through macros where appropriate
* Keep memory management explicit
* Maintain simple and predictable APIs
* Use unit tests to verify behavior and edge cases
* Use compiler warnings, sanitizers, and static analysis to catch defects

## Current status

Currently implemented / in progress:

* Generic dynamic array

  * Allocation
  * Length and capacity tracking
  * Reserve / growth
  * Push
  * Insert
  * Remove
  * Pop
  * Clear
  * Free

More data structures will be added incrementally.

## Generic arrays

The dynamic array API is designed around typed pointers while storing metadata internally.

Example:

```c
int *numbers = array_allocate(4, sizeof(*numbers));

array_push(numbers, 10);
array_push(numbers, 20);

printf("%zu\n", array_length(numbers));

array_free(numbers);
```

The macros infer the element size from the pointer type where possible, allowing the same API to be used with different element types.

## Testing

Unit tests use the [Unity](https://github.com/ThrowTheSwitch/Unity) test framework.

Initialize Unity after cloning (GitHub ZIP downloads do not include submodule contents):

```bash
git submodule update --init --recursive
```

On Windows with Clang available on PATH:

```powershell
.\build.ps1   # Compile with AddressSanitizer and UndefinedBehaviorSanitizer
.\test.ps1    # Build and run the eight library tests
.\analyze.ps1
.\analyze_cppcheck.ps1
```

`test.ps1` discovers the runtime directory from the installed Clang version.
Clang must support the requested sanitizers on your platform. Cppcheck is
required only for `analyze_cppcheck.ps1`.

For a manual build with C11 and strict warnings:

Example:

```bash
clang -std=c11 -Wall -Wextra -Wpedantic \
    -Ithird_party/unity/src \
    test_dyn_array_macro.c \
    third_party/unity/src/unity.c \
    -o test_dyn_array_macro
```

Run the test executable:

```bash
./test_dyn_array_macro
```

On Windows:

```powershell
.\test_dyn_array_macro.exe
```

## Development tools

The project currently uses:

* Clang
* Unity
* AddressSanitizer
* UndefinedBehaviorSanitizer

Static analysis scripts use Clang Static Analyzer and Cppcheck. They target
only the library test translation unit, not the deliberately unsafe examples.

## Repository layout

- `dyn_array.h`: current generic dynamic array implementation.
- `test_dyn_array_macro.c`: eight Unity tests for the library.
- `build.ps1`, `test.ps1`: Windows build and test entry points.
- `analyze.ps1`, `analyze_cppcheck.ps1`: library static analysis.
- `examples/tooling/`: isolated sanitizer and static-analysis demonstrations;
  see its [README](examples/tooling/README.md) for commands and expected failures.
- `third_party/unity/`: Unity test framework (Git submodule).

The previous integer-only array and its tests have been removed. They remain
available in Git history.

## Current limitations

This is an educational work in progress. Passing the current tests does not
establish memory safety for every input. In particular, `array_pop` requires a
non-empty array; macro arguments must not have side effects; and inserting a
value taken from the same array can be affected by element shifting. These
API issues are separate from the repository cleanup.

## Project philosophy

The project intentionally avoids hiding implementation details behind large frameworks or libraries.

The aim is to understand and explore concepts such as:

* Memory layout
* Allocation and reallocation
* Pointer arithmetic
* Alignment
* Ownership and lifetime
* Amortized growth
* Undefined behavior
* Macro-based generic programming

The implementations may evolve as new edge cases, design trade-offs, and safer API patterns are explored.

## License

This project is licensed under the **MIT License**. See the [LICENSE](LICENSE) file for details.
