# Crust

[![CI](https://github.com/seednull/crust/actions/workflows/ci.yml/badge.svg)](https://github.com/seednull/crust/actions/workflows/ci.yml)
[![Version](https://img.shields.io/github/v/tag/seednull/crust?sort=semver)](CHANGELOG.md)
[![C99](https://img.shields.io/badge/C-99-blue)](docs/building.md)
[![License](https://img.shields.io/github/license/seednull/crust)](LICENSE)

Crust is a small C foundation library: platform detection, fixed-width types, atomics, SIMD, game math and a set of allocator-aware data structures, behind a single header or one header per module.

It is written in C99, has no third-party dependencies and never allocates behind your back: containers either work on memory you hand them, or take an explicit `Crust_Allocator` on every call that allocates.

Versions follow semantic versioning: the API changes incompatibly only with a new major version. See [CHANGELOG.md](CHANGELOG.md) for what changed.

## Features

| Area | What's inside | Headers |
|---|---|---|
| Platform | Compiler, toolchain, architecture and SIMD backend detection, portability macros, assertions | `core.h` |
| Types and scalars | `i8`–`i64`, `u8`–`u64`, `isize`/`usize`, `f32`/`f64` (identical to the `<stdint.h>` types), limits, min/max/clamp/abs, alignment helpers, `lzcnt`/`tzcnt`/`popcnt`, rounding and trigonometry | `core.h`, `scalar.h` |
| Atomics and locks | 32-bit, 64-bit and `usize` atomics with explicit memory orders, `Crust_SpinLock`, `Crust_RwLock` | `atomic.h`, `lock.h` |
| SIMD | `Crust_Simd128` on SSE4.2, NEON and WebAssembly SIMD with a bit-exact scalar fallback: integer and float arithmetic, comparisons, masks, conversions, swizzles | `simd.h` |
| Math | `Crust_Vec2`, `Crust_Vec3`, `Crust_Vec4`, `Crust_Quat` (with `slerp`/`nlerp`), `Crust_Transform` | `vec.h`, `quat.h`, `transform.h` |
| Memory | `Crust_Allocator` interface, system and arena allocators, typed allocation macros | `allocator.h`, `arena.h`, `memory.h` |
| Buffers | `Crust_RingBuffer`, `Crust_BipBuffer`, `Crust_Stack` and the lock-free single-producer/single-consumer `Crust_SpscRingBuffer`, `Crust_SpscBipBuffer` | `ring.h`, `bip.h`, `stack.h` |
| Containers | `Crust_HashIndexMap` (open addressing, user-defined key operations), `Crust_GenerationalHandlePool` | `hash.h`, `pool.h` |
| Offset heap | `Crust_Heap`: a TLSF-style allocator that hands out offsets instead of pointers, meant for sub-allocating GPU memory | `heap.h` |

## Supported targets

- **Compilers:** MSVC, GCC, Clang; the header also compiles as C++
- **Architectures:** x86, x64, ARM, ARM64, WebAssembly (wasm32/wasm64)
- **Toolchains:** MSVC, MinGW, Android, Emscripten, WASI, Apple

## Quick start

Build with CMake 3.12 or newer, or use one of the presets (CMake 3.28+):

```bash
cmake -S . -B build -DCRUST_USE_SIMD=ON
cmake --build build
```

Link it from your project:

```cmake
add_subdirectory(crust)
target_link_libraries(app PRIVATE crust::crust)
```

Then include it:

```c
#include <crust.h>
```

Other ways to link Crust, options, presets and building without CMake are covered in [building.md](docs/building.md).

## A taste

```c
#include <crust.h>

int main(void)
{
	static u8 memory[64 * 1024];

	Crust_Arena arena = crustArenaAttach(memory, sizeof(memory));
	Crust_Allocator allocator = crustArenaAllocatorInit(&arena);

	f32 *values = crustAllocArray(allocator, f32, 1024);

	for (usize i = 0; i < 1024; ++i)
		values[i] = (f32)i - 512.0f;

	// four lanes at a time, identical results with and without CRUST_USE_SIMD
	for (usize i = 0; i < 1024; i += 4)
	{
		Crust_Simd128 v = crustSimd128LoadF32(values + i);
		crustSimd128StoreF32(values + i, crustSimd128MaxF32(v, crustSimd128AllZeros()));
	}

	crustArenaReset(&arena);
	return 0;
}
```

## Conventions

- **Naming:** types are `Crust_Name`, functions `crustName`, macros and constants `CRUST_NAME`, function pointer types `PFN_crustName`.
- **Memory:** `*Attach` functions wrap memory owned by the caller; `*Alloc`/`*Free`/`*Grow` take the `Crust_Allocator` to use. Nothing is allocated implicitly.
- **Contracts:** API misuse (out-of-range sizes, invalid handles, double frees, ...) is caught by `CRUST_ASSERT` in debug builds. Release builds don't check, so keep debug builds in your workflow.
- **Failures:** functions that can fail return `0`, `CRUST_NULL`, `CRUST_USIZE_MAX` or `CRUST_U32_MAX`.
- **Fields:** struct fields that the docs mention are part of the API, and container fields are for reading only. The others are internal and can change in a minor release, so rebuild everything that includes Crust headers when you update.

## Documentation

1. [Building and integration](docs/building.md)
2. [Platform](docs/platform.md)
3. [Types and scalar functions](docs/types.md)
4. [Atomics and locks](docs/concurrency.md)
5. [SIMD](docs/simd.md)
6. [Math](docs/math.md)
7. [Memory and allocators](docs/memory.md)
8. [Buffers](docs/buffers.md)
9. [Containers](docs/containers.md)
10. [Offset heap](docs/heap.md)
