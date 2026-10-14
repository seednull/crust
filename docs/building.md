# Building and integration

## Requirements

- A C99 compiler: MSVC, GCC or Clang. The public header also compiles as C++.
- CMake 3.10 or newer. The presets in `CMakePresets.json` need CMake 3.28 or newer.

## Repository layout

| Path | Contents |
|---|---|
| `include/crust.h` | Includes every module header, for when you want the whole library |
| `include/crust/*.h` | One header per module, see [Headers](#headers) |
| `include/crust/platform/*.inl` | System intrinsic includes and the per-backend definitions of the inline CPU and SIMD functions. The module headers include them, never include them yourself |
| `src/freestanding` | Containers, buffers, allocators and locks. Nothing here calls the C standard library directly |
| `src/libc` | The only code that talks to the C standard library: `crustMemcpy`/`crustMemset`, trigonometry and the system allocator |
| `tests` | CTest-based tests and a minimal test framework |

## Options

| Option | Default | Description |
|---|---|---|
| `BUILD_SHARED_LIBS` | `OFF` | Build Crust as a shared library |
| `CRUST_BUILD_TESTS` | `OFF` | Build the tests |
| `CRUST_USE_SIMD` | `OFF` | Use SSE, NEON or WebAssembly SIMD for `Crust_Simd128` instead of the scalar fallback, see [SIMD](simd.md#enabling-simd) |

`CRUST_USE_SIMD=ON` adds a public `CRUST_USE_SIMD=1` definition to the `crust` target and the compiler flags the backend needs: `-msse4.2 -mpopcnt` on x86 and `-msimd128` on WebAssembly. Before adding them, CMake compiles a small test with the flags and checks that they really enable the instruction set for the target being built (`__SSE4_2__` and `__POPCNT__`, or `__wasm_simd128__`), so other targets such as ARM64 and MSVC get no flags. Both the definition and the flags are `PUBLIC`, so every target that links `crust` sees the same configuration.

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
| `build/include` | `crust.h` and the `crust` directory with the module headers and `platform/*.inl` |
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

Then include the whole library or only the modules you use:

```c
#include <crust.h>
#include <crust/simd.h>
```

## Headers

`<crust.h>` includes every module. Each module header can also be included on its own and pulls in what it depends on:

| Header | Contents | Includes | Docs |
|---|---|---|---|
| `<crust/core.h>` | Version, platform detection, portability macros, assertions, base types, limits, min/max/clamp/abs, powers of two and alignment | — | [Platform](platform.md), [Types](types.md) |
| `<crust/scalar.h>` | Floating-point functions, bit operations, trigonometry | `core.h` | [Types](types.md) |
| `<crust/atomic.h>` | Memory orders, atomic operations, `crustCpuRelax` | `core.h` | [Atomics and locks](concurrency.md) |
| `<crust/lock.h>` | `Crust_SpinLock`, `Crust_RwLock` | `core.h` | [Atomics and locks](concurrency.md) |
| `<crust/simd.h>` | `Crust_Simd128` | `scalar.h` | [SIMD](simd.md) |
| `<crust/vec.h>` | `Crust_Vec2`, `Crust_Vec3`, `Crust_Vec4` | `scalar.h` | [Math](math.md) |
| `<crust/quat.h>` | `Crust_Quat` | `vec.h` | [Math](math.md) |
| `<crust/transform.h>` | `Crust_Transform` | `quat.h` | [Math](math.md) |
| `<crust/memory.h>` | `crustMemcpy`, `crustMemset` | `core.h` | [Memory](memory.md) |
| `<crust/allocator.h>` | `Crust_Allocator`, the `crustAlloc*` macros, the system allocator | `core.h` | [Memory](memory.md) |
| `<crust/arena.h>` | `Crust_Arena` and the arena allocator | `allocator.h` | [Memory](memory.md) |
| `<crust/ring.h>` | `Crust_RingBuffer`, `Crust_SpscRingBuffer` | `core.h` | [Buffers](buffers.md) |
| `<crust/bip.h>` | `Crust_BipBuffer`, `Crust_SpscBipBuffer` | `core.h` | [Buffers](buffers.md) |
| `<crust/stack.h>` | `Crust_Stack` | `core.h` | [Buffers](buffers.md) |
| `<crust/hash.h>` | `Crust_HashIndexMap` | `allocator.h` | [Containers](containers.md) |
| `<crust/pool.h>` | `Crust_GenerationalHandlePool` | `allocator.h` | [Containers](containers.md) |
| `<crust/heap.h>` | `Crust_Heap`, `Crust_HeapNodePool` | `allocator.h` | [Offset heap](heap.md) |

- **Includes inside Crust are relative.** The headers include each other with quoted paths relative to their own location, so they work without any include path setup, for example after copying `include` into your project, and they never pick up a same-named header from another library.
- **Include modules with the `crust/` prefix:** `<crust/ring.h>`. Never put `include/crust` itself on the include path.
- **Only three modules include system headers:** `scalar.h`, `atomic.h` and `simd.h`, see [System headers](platform.md#system-headers). Everything that depends only on `core.h` or `allocator.h` includes no system header at all.
- **Self-contained.** The tests compile every header on its own, so a module never relies on something that only another header happens to include.

### Without CMake

Compile every file in `src/freestanding` and `src/libc` with `include` on the include path and link `libm` on Unix-like systems. Defines that change the build:

| Define | Where | Effect |
|---|---|---|
| `CRUST_USE_SIMD=1` | Everywhere a Crust header is included | Enables the SIMD backend. Add `-msse4.2 -mpopcnt` (x86) or `-msimd128` (WebAssembly) for GCC and Clang |
| `CRUST_SHARED_LIBRARY` | Everywhere a Crust header is included | Marks the API as imported from a shared library |
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
- The `crust_test_headers` target compiles every public header on its own, so a header that misses an include fails the build.
- Test the SIMD backend by configuring with `-DCRUST_USE_SIMD=ON`. Without it the tests exercise the scalar backend.
- With tests enabled, executables and shared libraries go to `bin/` in the build directory, so the tests find `crust.dll` in shared builds on Windows.
- The Emscripten presets run the tests under Node.js. Builds without threads, such as Emscripten, compile the multithreaded test cases of the atomic, lock, ring and bip buffer tests out.

## Continuous integration

GitHub Actions builds and tests every push to `main` and every pull request, see `.github/workflows/ci.yml`. Every job treats compiler warnings as errors, and the GCC, Clang and Emscripten jobs compile in strict C99 (`-std=c99`, no extensions).

| Job | Runner | Configurations |
|---|---|---|
| MSVC | Windows x64, Windows ARM64 | x64, Win32 and ARM64; SIMD off and on; Debug and Release |
| Linux GCC, Linux Clang | Ubuntu x64 | SIMD off and on; Debug and Release |
| Linux ARM64 GCC | Ubuntu ARM64 | NEON and scalar; Debug and Release |
| macOS ARM64 Clang | macOS on Apple silicon | NEON and scalar; Debug and Release |
| Emscripten | Ubuntu x64 | WebAssembly SIMD and scalar, tests under Node.js |
| Sanitizers | Ubuntu x64, Clang | AddressSanitizer and UndefinedBehaviorSanitizer, SIMD off and on |
| Shared library | Windows (MSVC), Ubuntu (GCC) | `BUILD_SHARED_LIBS=ON` |
| Installed package | Ubuntu x64 | Installs Crust and builds `tests/package`, a minimal project that uses it through `find_package` |
