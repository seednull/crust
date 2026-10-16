# Changelog

## Unreleased

### Added

- `crustAllocatorSizeAdd(a, b)`, the addition counterpart of `crustAllocatorSizeMul`: it asserts on overflow and returns `CRUST_USIZE_MAX` in release builds.

### Changed

- `CRUST_DEBUG_BREAK()` is `__builtin_debugtrap()` with Clang, a breakpoint that a debugger can continue past, like `__debugbreak()` with MSVC. Before, it was `__builtin_trap()`, which ends the program. Failed assertions call `CRUST_DEBUG_BREAK()` and behave as before.
- `crustHeapFree` asserts that `allocation.offset` is the offset the block was allocated at.
- `CRUST_BUILD_TESTS` requires a C++ compiler. Before, the C++ header test was silently skipped when CMake didn't find one.
- Documented that unoptimized MinGW builds for `msvcrt.dll` can compute `crustSqrtF64` one bit off.
- Documented that `-Wfloat-equal` reports the intended float comparisons of the scalar SIMD backend.

### Fixed

- With MSVC, `SEQ_CST` atomic loads were compare-and-swap operations: about 8 times slower than other loads, writing to the cache line, so that concurrent readers contended, and faulting on read-only memory. They are now plain loads on x86 and x64 and `ldar` on ARM64, as with GCC and Clang.
- The GCC and Clang atomics cast away `volatile` before calling the `__atomic` builtins, which take `volatile` pointers themselves, so code including `<crust/atomic.h>` got `-Wcast-qual` warnings.
- `crustHeapNodePoolFootprint`, `crustHashIndexMapFootprint` and `crustGenerationalHandlePoolFootprint` wrapped around when the bound didn't fit into `usize`, as happens for large capacities on 32-bit targets, and returned far too small sizes. They now assert, and return `CRUST_USIZE_MAX` in release builds.

## 1.6.0 - 2026-10-16

### Added

- `CRUST_CACHE_LINE_SIZE` (`128`) is the distance that keeps data written by different threads off the same cache line. `Crust_SpscRingBuffer` and `Crust_SpscBipBuffer` use it for their alignment and padding.

### Changed

- `Crust_SpscRingBuffer` and `Crust_SpscBipBuffer` fill the space up to their 128-byte boundaries with explicit internal padding fields instead of padding that the compiler inserts, so their headers no longer disable MSVC warning C4324 and stay clean with `/Wall` and `-Wpadded`. Their size, alignment and the offsets of the other fields are unchanged.

## 1.5.0 - 2026-10-15

### Added

- `crustGenerationalHandlePoolFootprint`, `crustHashIndexMapFootprint` and `crustHeapNodePoolFootprint` return an upper bound of the bytes that the matching `Alloc` takes from an arena for a given capacity, wherever the arena's free space starts, so a memory block for the containers can be sized before they are created.
- `crustGenerationalHandlePoolInit` and `crustHeapNodePoolInit` return an empty pool without allocating, the same state that `Free` leaves, and `Grow` on it works like `Alloc`. A zero-initialized pool is not empty, because its lists end with `CRUST_USIZE_MAX` or `CRUST_U32_MAX`.

### Changed

- `crustGenerationalHandlePoolCheck` returns `0` for a handle whose index is out of range instead of asserting, so handles from untrusted input can be checked directly.
- `crustGenerationalHandlePoolHead`, `crustGenerationalHandlePoolTail` and `crustGenerationalHandlePoolAdd` no longer assert on a pool without storage: `Head` and `Tail` return the end of the list and `Add` returns a zeroed handle, as for a full pool.
- `CRUST_APIENTRY` gives Crust's functions hidden visibility with GCC and Clang outside Windows, so they stay out of the exports of a shared library that links Crust, whatever builds Crust. The CMake build no longer sets `C_VISIBILITY_PRESET`.
- Documented `-ffp-contract=off` as the way to keep GCC from fusing multiplies and adds: an ISO mode does this only for C, not for C++. Also documented that the sign and payload of any NaN that arithmetic returns are unspecified, including NaNs passed on from the inputs, and that code including Crust headers must be rebuilt when Crust is updated.
- CMake 3.12 or newer is required. The `crust` target declares `c_std_99` as a public compile feature, so code that links it is compiled as C99 or newer even when the parent project asks for an older standard.

## 1.4.0 - 2026-10-15

### Changed

- Crust is always built as a static library: `BUILD_SHARED_LIBS` no longer applies to it, and `CRUST_SHARED_LIBRARY` and `CRUST_IMPLEMENTATION` have no effect. It is compiled as position-independent code with hidden symbols, so it can be linked into shared libraries without exporting its symbols or clashing with another copy of Crust.
- Documented that GCC needs ISO mode or `-ffp-contract=off` for floating-point results to match the other targets bit for bit: in its default GNU dialects it fuses multiplies and adds into FMA instructions, even across inlined SIMD calls. Also documented that a NaN created by arithmetic has a hardware-dependent sign and payload.
- The Linux presets build in strict C99, like CI.
- Documented that struct fields the docs don't mention are internal and can change in a minor release.

### Fixed

- `crustHeapStageAlloc`, `crustHeapStageAllocAligned`, `crustHeapAlloc` and `crustHeapAllocAligned` asserted in debug builds for sizes above `0xF0000000`, including padded aligned sizes, because their size class doesn't fit into 32 bits. They now fail, as in release builds.
- `crustArenaAlloc` subtracted pointers beyond the attached memory when the alignment padding didn't fit, which is undefined behavior in C. It now computes the padding in integers and forms the pointer only after the capacity check.

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
