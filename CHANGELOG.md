# Changelog

## Unreleased

### Fixed

- `crustHeapStageAlloc`, `crustHeapStageAllocAligned`, `crustHeapAlloc` and `crustHeapAllocAligned` asserted in debug builds for sizes above `0xF0000000`, including padded aligned sizes, because their size class doesn't fit into 32 bits. They now fail, as in release builds.

## 1.3.0 - 2026-10-15

### Added

- `CRUST_F32_PI`, `CRUST_F64_PI`, `CRUST_F32_TAU` and `CRUST_F64_TAU`.
- Committing `0` bytes cancels a staged bip buffer read or write.

### Changed

- Ring buffer reads and writes no longer divide by the capacity, which makes them about 45% faster.
- 64-bit atomics assert that their address is 8-byte aligned.
- Shared libraries on Linux and macOS export only the API.
- `CRUST_TOOLCHAIN_APPLE` is set for every compiler that targets Apple platforms, not only Apple Clang.

## 1.2.1 - 2026-10-15

### Changed

- The macOS presets `osx-clang-x64-debug` and `osx-clang-x64-release` are renamed to `osx-clang-debug` and `osx-clang-release`. They build for the host architecture, which is arm64 on Apple silicon.
- Documented that 32-bit x86 builds with GCC need `-msse2 -mfpmath=sse` for floating-point results to match the other targets bit for bit. GCC's default x87 math rounds twice.

### Added

- Linux presets: `linux-gcc-debug`, `linux-gcc-release`, `linux-clang-debug` and `linux-clang-release`, with matching `build-linux-*` build presets. They use `gcc`/`g++` or `clang`/`clang++` from `PATH`.

### Fixed

- With MSVC on ARM64, `atomic.h` and `scalar.h` didn't compile as C++: some of the intrinsics they use are declared only in `intrin.h`, which Crust doesn't include. Crust now declares them itself, with the signatures from `intrin.h`.
- With Clang and the MSVC toolchain on 32-bit x86, such as clang-cl for Win32, code calling `crustSqrtF32`, `crustFloorF32` or `crustCeilF32` could fail to link: the 32-bit UCRT doesn't export `sqrtf`, `floorf` and `ceilf`. These functions now go through the `f64` versions there, with identical results.

## 1.2.0 - 2026-10-14

Running out of a resource is now reported through function results and stays well-defined in release builds, see [Assertions](docs/platform.md#assertions). Contract violations are still asserted.

### Changed

- `crustHashIndexMapAlloc`, `crustHashIndexMapGrow`, `crustGenerationalHandlePoolAlloc`, `crustGenerationalHandlePoolGrow` and `crustHeapNodePoolAlloc` return `u8`: `1` on success, `0` when the allocator fails. A failed `Alloc` leaves the container empty, a failed `Grow` leaves it untouched. Code that ignores the result compiles as before.
- `crustHeapStageAlloc` and `crustHeapStageAllocAligned`, and with them `crustHeapAlloc` and `crustHeapAllocAligned`, fail when the node pool doesn't have the nodes the allocation needs. Before, running out of nodes corrupted memory in release builds.
- `crustGenerationalHandlePoolAdd` returns a zeroed handle when the pool is full. Before, it asserted and corrupted the pool in release builds.
- `crustAllocatorSizeMul`, used by the typed allocation macros, returns `CRUST_USIZE_MAX` on overflow in release builds, so the allocation fails instead of returning a block that is too small. Debug builds still assert.
- The hash map no longer divides on every probe step.
- With `CRUST_BUILD_TESTS`, executables and shared libraries go to `bin/` in the build directory.

### Added

- `crustHeapNodePoolGrow`.
- `Crust_HeapNodePool.count`, the number of nodes in use. **ABI change:** the struct is larger, so code compiled against older headers can't be linked against a newer shared library. Rebuild it.

### Fixed

- `crustHeapStageAllocAligned` asserted in debug builds when the padding for the alignment didn't fit into the heap, although the request was valid, and the padded size could overflow. The allocation now just fails.
- `crustGenerationalHandlePoolGrow` lost its storage and wrote through a null pointer when the allocator failed.
- Scalar `crustSimd128MinF32`, `MaxF32`, `MinF64` and `MaxF64` returned wrong results for NaN and signed zeros with optimized MSVC builds on ARM64, where the compiler vectorized them into `fmin`/`fmax`.

## 1.1.0 - 2026-10-14

### Changed

- `crust.h` is split into 17 module headers in `include/crust`, such as `<crust/simd.h>` and `<crust/ring.h>`, see [Using Crust in your project](docs/building.md#using-crust-in-your-project). `<crust.h>` includes all of them, so existing code compiles unchanged.
- The headers are installed as `include/crust.h` and `include/crust/*.h`, and the installed include directory is `include`. Before, `crust.h` was installed into `include/crust` and that directory was on the include path.
- Only `scalar.h`, `atomic.h` and `simd.h` include system headers, and only the compiler's intrinsic headers. Modules that depend on nothing but `core.h` or `allocator.h` include none, see [System headers](docs/platform.md#system-headers).

## 1.0.0 - 2026-10-14

First release.

- Platform detection, portability macros and assertions.
- Fixed-width types identical to the `<stdint.h>` types, limits, min/max/clamp/abs, alignment helpers, bit operations, rounding and trigonometry.
- 32-bit, 64-bit and `usize` atomics with explicit memory orders, `Crust_SpinLock` and `Crust_RwLock`.
- `Crust_Simd128` on SSE4.2, NEON and WebAssembly SIMD with a bit-exact scalar fallback.
- `Crust_Vec2`, `Crust_Vec3`, `Crust_Vec4`, `Crust_Quat` and `Crust_Transform`.
- The `Crust_Allocator` interface with system and arena allocators.
- `Crust_RingBuffer`, `Crust_BipBuffer`, `Crust_Stack`, `Crust_SpscRingBuffer` and `Crust_SpscBipBuffer`.
- `Crust_HashIndexMap` and `Crust_GenerationalHandlePool`.
- `Crust_Heap`, a TLSF-style offset allocator.
