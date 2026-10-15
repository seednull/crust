# Building and integration

## Requirements

- A C99 compiler: MSVC, GCC or Clang. The headers also compile as C++.
- CMake 3.12 or newer. The presets in `CMakePresets.json` need CMake 3.28 or newer.

## Options

| Option | Default | Description |
|---|---|---|
| `CRUST_BUILD_TESTS` | `OFF` | Build the tests |
| `CRUST_USE_SIMD` | `OFF` | Use SSE, NEON or WebAssembly SIMD for `Crust_Simd128` instead of the scalar fallback, see [SIMD](simd.md#enabling-simd) |

`CRUST_USE_SIMD=ON` adds a public `CRUST_USE_SIMD=1` definition and, where the target needs them, the backend's compiler flags: `-msse4.2 -mpopcnt` on x86, `-msimd128` on WebAssembly.

Crust is always a static library. It is compiled as position-independent code, and its functions have hidden visibility, so it can go into your own shared libraries without exporting anything or clashing with another copy of Crust.

## Building

```bash
cmake -S . -B build -DCRUST_BUILD_TESTS=ON -DCRUST_USE_SIMD=ON
cmake --build build
```

Or with a preset:

```bash
cmake --preset msvc2026-x64
cmake --build --preset build-msvc2026-x64-release
```

| Configure preset | Generator | Notes |
|---|---|---|
| `msvc2022-x64`, `msvc2026-x64` | Visual Studio 2022 / 2026 | Multi-config: pick `Debug` or `Release` at build time |
| `emscripten-ninja`, `emscripten-ninja-debug` | Ninja | Needs the Emscripten SDK; Release and Debug |
| `osx-clang-release`, `osx-clang-debug` | Unix Makefiles | Uses `/usr/bin/clang` and builds for the host architecture, arm64 or x64 |
| `linux-gcc-release`, `linux-gcc-debug`, `linux-clang-release`, `linux-clang-debug` | Unix Makefiles | Uses `gcc`/`g++` or `clang`/`clang++` from `PATH` |

Every configure preset writes into `build/<preset>`, and every build preset also installs Crust into `build/`: the headers into `build/include`, the library into `build/lib` with a `d` suffix in Debug builds, the CMake package into `build/lib/cmake/crust`.

## Using Crust in your project

As an installed package, with `CMAKE_PREFIX_PATH` pointing at the install directory. The package accepts any requested version with the same major number.

```cmake
find_package(crust CONFIG REQUIRED)
target_link_libraries(app PRIVATE crust::crust)
```

As a subdirectory:

```cmake
add_subdirectory(crust)
target_link_libraries(app PRIVATE crust::crust)
```

To turn an option on from the parent project, set the cache variable before `add_subdirectory`, for example `set(CRUST_USE_SIMD ON CACHE BOOL "")`.

Include `<crust.h>` for the whole library or a single module with the `crust/` prefix, such as `<crust/ring.h>`; never put `include/crust` itself on the include path. Every module header includes what it depends on through paths relative to itself, so a copy of `include` works without any setup.

### Without CMake

Compile every file in `src/freestanding` and `src/libc` with `include` on the include path, add `-fPIC` if the result goes into a shared library, and link `libm` on Unix-like systems. Defines that change the build:

| Define | Where | Effect |
|---|---|---|
| `CRUST_USE_SIMD=1` | Everywhere a Crust header is included, with the same value | Enables the SIMD backend, see [SIMD](simd.md#enabling-simd) for the compiler flags |
| `NDEBUG` | Anywhere | Disables `CRUST_ASSERT`, see [Platform](platform.md#assertions) |

### Running without the C standard library

Only `src/libc` uses the C standard library. To run without one, build `src/freestanding` and provide your own `crustMemcpy`, `crustMemset`, `crustCosF32`, `crustAcosF32`, `crustSinF32`, `crustAsinF32`, `crustTanF32`, `crustAtan2F32` and their `F64` versions. `crustSystemAllocatorInit` is needed only if you call it. On GCC and Clang some inline functions, such as `crustFloorF32` on x86 without SSE4.1, may still compile into calls to the C math library.

## Tests

```bash
cmake --preset msvc2026-x64 -DCRUST_BUILD_TESTS=ON
cmake --build --preset build-msvc2026-x64-debug
ctest --test-dir build/msvc2026-x64 -C Debug --output-on-failure
```

Configure with `-DCRUST_USE_SIMD=ON` to test the SIMD backend; without it the tests exercise the scalar backend. When cross-compiling, set `CMAKE_CROSSCOMPILING_EMULATOR`, for example to `qemu-arm` or `wasmtime`, to run them.

## Continuous integration

GitHub Actions builds and tests every push to `main` and every pull request, see `.github/workflows/ci.yml`. Every job treats compiler warnings as errors, and the GCC, Clang, MinGW and Emscripten jobs compile in strict C99 (`-std=c99`, no extensions).

| Job | Runner | Configurations |
|---|---|---|
| MSVC | Windows x64, Windows ARM64 | x64, Win32 and ARM64; SIMD off and on; Debug and Release |
| Clang-cl | Windows x64 | The `ClangCL` toolset of Visual Studio; x64 and Win32; SIMD off and on; Debug and Release |
| MinGW | Windows x64 | MSYS2 UCRT64 GCC; SIMD off and on; Debug and Release |
| Linux GCC, Linux Clang | Ubuntu x64 | SIMD off and on; Debug and Release |
| Linux ARM64 GCC | Ubuntu ARM64 | NEON and scalar; Debug and Release |
| Linux x86 GCC | Ubuntu x64, `-m32` | x87 math with the scalar backend; SSE math (`-msse2 -mfpmath=sse`) with SIMD off and on; Debug and Release |
| Linux ARM32 GCC | Ubuntu x64, cross-compiled | `arm-linux-gnueabihf`, tests under `qemu-arm`; scalar backend with SIMD off and on; Debug and Release |
| Android | Ubuntu x64, NDK | Build only: `arm64-v8a` with SIMD off and on, `armeabi-v7a`, `x86_64` with SSE; Debug and Release |
| WASI | Ubuntu x64, WASI SDK | Tests under Wasmtime; WebAssembly SIMD and scalar; Debug and Release |
| Emscripten wasm64 | Ubuntu x64 | Build only, with `-sMEMORY64=1`; WebAssembly SIMD and scalar; Debug and Release |
| macOS ARM64 Clang | macOS on Apple silicon | NEON and scalar; Debug and Release |
| Emscripten | Ubuntu x64 | WebAssembly SIMD and scalar, tests under Node.js |
| Sanitizers | Ubuntu x64, Clang | AddressSanitizer with UndefinedBehaviorSanitizer, and ThreadSanitizer; SIMD off and on |
| Installed package | Ubuntu x64 | Installs Crust and builds `tests/package`, a minimal project that uses it through `find_package` |
| Presets | Windows, Ubuntu, macOS | Configures and builds with `msvc2022-x64`, `linux-gcc-release`, `linux-clang-release` and `osx-clang-release`, then runs the tests |
