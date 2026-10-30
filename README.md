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
.\test.ps1    # Build and run the library tests
.\analyze.ps1
.\analyze_cppcheck.ps1
```

`test.ps1` discovers the runtime directory from the installed Clang version.
Clang must support the requested sanitizers on your platform. Cppcheck is
required only for `analyze_cppcheck.ps1`. Both `clang` and `cppcheck` must be
available on PATH when running their respective scripts.

In PowerShell, pass sanitizer options separately (`-fsanitize=address
-fsanitize=undefined`) or quote a combined option. Quote comma-separated options
such as `"--enable=warning,style,performance,portability"` as well.

For a manual build in a POSIX shell with C11 and strict warnings:

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

For Windows, use `build.ps1` and `test.ps1` as shown above.

## Development tools

The project currently uses:

* Clang
* Unity
* AddressSanitizer
* UndefinedBehaviorSanitizer

Static analysis scripts use Clang Static Analyzer and Cppcheck. They target
only the library test translation unit, not the deliberately unsafe examples.

Clang Static Analyzer may finish without console output when it reports no
findings. Cppcheck 2.21.0 has reported `nullPointerRedundantCheck` around a Unity
assertion and `redundantInitialization` around header initialization in this
project. These diagnostics remain under review; the project does not currently
claim a warning-free Cppcheck run. Limiting configuration exploration with
`--max-configs=1` avoids the additional configurations seen during investigation,
but also reduces configuration coverage.

## Repository layout

- `dyn_array.h`: current generic dynamic array implementation.
- `test_dyn_array_macro.c`: Unity tests for the library.
- `build.ps1`, `test.ps1`: Windows build and test entry points.
- `analyze.ps1`, `analyze_cppcheck.ps1`: library static analysis.
- `examples/tooling/`: isolated sanitizer and static-analysis demonstrations;
  see its [README](examples/tooling/README.md) for commands and expected failures.
- `third_party/unity/`: Unity test framework (Git submodule).

The previous integer-only array and its tests have been removed. They remain
available in Git history.

## API contracts and limitations

This is an educational work in progress. Passing the current tests does not
establish memory safety for every input.

### Insertion

`array_insert(array, index, value)` evaluates `index` exactly once. Insertion
accepts indices from zero through the current length, inclusive. An index larger
than the length causes `abort()`.

```c
size_t index = 1;
array_insert(numbers, index++, 99);  // index is incremented once.
```

`value` is evaluated after capacity growth and element shifting. If it depends
on the contents of the same array or a pointer into it, copy the value to a local
variable before insertion:

```c
/* Assuming numbers contains at least three elements. */
int saved_value = numbers[2];
array_insert(numbers, 0, saved_value);

/* Avoid: the element may change before value is evaluated. */
/* array_insert(numbers, 0, numbers[2]); */
```

The macro uses `do { ... } while (0)` and is used as a statement, not as an
expression returning a value.

### Removing the last element

`array_pop(array)` returns the last element and decreases the length by one.
It leaves the capacity unchanged. Passing a NULL or empty array causes `abort()`
before accessing an element.

```c
int *numbers = NULL;
array_push(numbers, 42);
int last = array_pop(numbers);  // last == 42; length == 0.
array_free(numbers);
```

Calls expected to abort must run separately from the ordinary Unity suite;
otherwise they terminate the entire test process. The regular tests cover
popping the last remaining element, and a regression test checks that insertion
evaluates its index once.

### General macro and pointer rules

- Pass a simple, modifiable pointer variable as the `array` argument. It may be
  evaluated more than once; avoid expressions with side effects.
- The single-evaluation guarantee for insertion's `index` does not extend to all
  arguments of every macro.
- Growth may relocate the allocation and invalidate pointers to its elements.
  Insertion and removal also shift elements.
- Do not use internal temporary names such as `array_insert_index_` in macro
  arguments; C macros do not provide hygienic local names.
- Concurrent access requires synchronization by the caller.

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
