# Building and integration

## Requirements

- A C99 compiler: MSVC, GCC or Clang. The public header also compiles as C++.
- CMake 3.10 or newer. The presets in `CMakePresets.json` need CMake 3.28 or newer.

## Repository layout

| Path | Contents |
|---|---|
| `include/crust.h` | The single public header: platform detection, types, every prototype and all inline functions |
| `include/platform/*.inl` | Definitions of the inline CPU and SIMD functions, one file per backend. `crust.h` includes them, never include them yourself |
| `src/freestanding` | Containers, buffers, allocators and locks. Nothing here calls the C standard library directly |
| `src/libc` | The only code that talks to the C standard library: `crustMemcpy`/`crustMemset`, trigonometry and the system allocator |
| `tests` | CTest-based tests and a minimal test framework |

## Options

| Option | Default | Description |
|---|---|---|
| `BUILD_SHARED_LIBS` | `OFF` | Build Crust as a shared library |
| `CRUST_BUILD_TESTS` | `OFF` | Build the tests |
| `CRUST_USE_SIMD` | `OFF` | Use SSE, NEON or WebAssembly SIMD for `Crust_Simd128` instead of the scalar fallback, see [SIMD](simd.md#enabling-simd) |

`CRUST_USE_SIMD=ON` adds a public `CRUST_USE_SIMD=1` definition to the `crust` target and the compiler flags the backend needs: `-msse4.2 -mpopcnt` on x86 and `-msimd128` on WebAssembly, when the compiler accepts them. MSVC needs no flags. Both the definition and the flags are `PUBLIC`, so every target that links `crust` sees the same configuration.

## Building with presets

```bash
cmake --preset msvc2026-x64
cmake --build --preset build-msvc2026-x64-release
```

| Configure preset | Generator | Notes |
|---|---|---|
| `msvc2022-x64`, `msvc2026-x64` | Visual Studio 2022 / 2026 | Multi-config: pick `Debug` or `Release` at build time |
| `emscripten-ninja`, `emscripten-ninja-debug` | Ninja | Needs the Emscripten SDK; Release and Debug |
| `osx-clang-x64-release`, `osx-clang-x64-debug` | Unix Makefiles | Uses `/usr/bin/clang` and builds for the host architecture |

Every configure preset writes into `build/<preset>`. The build presets (`build-msvc2026-x64-debug`, `build-emscripten-release`, `build-osx-release`, ...) also run the `install` target, which copies the library into `build/`:

| Directory | Contents |
|---|---|
| `build/include/crust` | `crust.h` and `platform/*.inl` |
| `build/lib` | The library. Debug builds get a `d` suffix (`crustd.lib`, `libcrustd.a`), Windows builds have no `lib` prefix |
| `build/lib/cmake/crust` | The CMake package |

## Building without presets

```bash
cmake -S . -B build -DCRUST_BUILD_TESTS=ON -DCRUST_USE_SIMD=ON
cmake --build build
```

## Using Crust in your project

As an installed package:

```cmake
find_package(crust CONFIG REQUIRED)
target_link_libraries(app PRIVATE crust::crust)
```

Point `CMAKE_PREFIX_PATH` at the install directory, for example `-DCMAKE_PREFIX_PATH=path/to/crust/build`. The package is versioned with `SameMajorVersion` compatibility.

As a subdirectory:

```cmake
add_subdirectory(crust)
target_link_libraries(app PRIVATE crust::crust)
```

To turn an option on from the parent project, set the cache variable before `add_subdirectory`, for example `set(CRUST_USE_SIMD ON CACHE BOOL "")`, or pass `-DCRUST_USE_SIMD=ON` on the command line.

Then include the header:

```c
#include <crust.h>
```

### Without CMake

Compile every file in `src/freestanding` and `src/libc` with `include` on the include path and link `libm` on Unix-like systems. Defines that change the build:

| Define | Where | Effect |
|---|---|---|
| `CRUST_USE_SIMD=1` | Everywhere `crust.h` is included | Enables the SIMD backend. Add `-msse4.2 -mpopcnt` (x86) or `-msimd128` (WebAssembly) for GCC and Clang |
| `CRUST_SHARED_LIBRARY` | Everywhere `crust.h` is included | Marks the API as imported from a shared library |
| `CRUST_IMPLEMENTATION` | Only when compiling Crust itself | Together with `CRUST_SHARED_LIBRARY`, exports the API instead of importing it |
| `NDEBUG` | Anywhere | Disables `CRUST_ASSERT`, see [Platform](platform.md#assertions) |

Keep `CRUST_USE_SIMD` identical in every translation unit that exchanges `Crust_Simd128` values: the type is a different C type with and without SIMD.

### Running without the C standard library

Only `src/libc` uses the C standard library. To run on a target without one, build `src/freestanding` and provide your own definitions of the functions that `src/libc` implements:

- `crustMemcpy`, `crustMemset`;
- `crustCosF32`, `crustAcosF32`, `crustSinF32`, `crustAsinF32`, `crustTanF32`, `crustAtan2F32` and their `F64` versions;
- `crustSystemAllocatorInit`, or simply never call it.

The inline functions in the header use compiler builtins. On GCC and Clang some of them, such as `crustFloorF32` on x86 without SSE4.1 or `crustSqrtF32` when the compiler keeps `errno` semantics, may still compile into calls to the C math library.

## Platform requirements

| Target | Requirement |
|---|---|
| MSVC, x86 and x64 | `crustFloor*`, `crustCeil*`, `crustRoundEven*` and `crustTruncate*` use SSE4.1 instructions and `crustPopcntU32` uses `POPCNT`, unconditionally. Any CPU from roughly 2011 on has both |
| x86 with `CRUST_USE_SIMD` | An x86-64-v2 CPU (SSE4.2 and POPCNT). There is no runtime check. GCC 7 or Clang 9 is the minimum for the SSE backend |
| ARM64 with `CRUST_USE_SIMD` | Nothing: NEON is part of the baseline |
| WebAssembly with `CRUST_USE_SIMD` | `-msimd128` and a runtime with WebAssembly SIMD, which every current browser and Node.js has |
| Other architectures (32-bit ARM, ...) | `CRUST_USE_SIMD` silently falls back to the scalar backend |

## Tests

```bash
cmake --preset msvc2026-x64 -DCRUST_BUILD_TESTS=ON
cmake --build --preset build-msvc2026-x64-debug
ctest --test-dir build/msvc2026-x64 -C Debug --output-on-failure
```

- Every test is a separate executable that uses the small framework in `tests/framework`.
- Test the SIMD backend by configuring with `-DCRUST_USE_SIMD=ON`. Without it the tests exercise the scalar backend.
- The Emscripten presets run the tests under Node.js. The multithreaded `lock` test is skipped there.
