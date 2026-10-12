# Crust

Crust is a small C foundation library: platform detection, fixed-width types, atomics, game math and a set of allocator-aware data structures, behind a single header.

It is written in C99, has no third-party dependencies and never allocates behind your back: containers either work on memory you hand them, or take an explicit `Crust_Allocator` on every call that allocates.

> **Status:** in development (`1.0.0-dev`). The API may still change.

## Features

| Area | What's inside |
|---|---|
| Platform | Compiler, toolchain and architecture detection (`CRUST_COMPILER_*`, `CRUST_TOOLCHAIN_*`, `CRUST_ARCH_*`), `CRUST_INLINE`, `CRUST_ALIGNAS`, `CRUST_ASSERT`, ... |
| Types | `i8`–`i64`, `u8`–`u64`, `isize`/`usize`, `f32`/`f64`, limits and constants |
| Scalar helpers | `min`/`max`/`clamp`/`abs`, power-of-two and alignment helpers for every integer type, 32-bit `lzcnt`/`tzcnt`/`popcnt` |
| Atomics | 32-bit, 64-bit and `usize` swap, compare-and-swap, load, store, increment and decrement with explicit memory orders |
| Math | `Crust_Vec2`, `Crust_Vec3`, `Crust_Vec4`, `Crust_Quat` (including `slerp`/`nlerp`), `Crust_Transform` |
| Synchronization | `Crust_SpinLock`, `Crust_RwLock` |
| Buffers | `Crust_RingBuffer`, `Crust_BipBuffer` and their lock-free single-producer/single-consumer variants `Crust_SpscRingBuffer`, `Crust_SpscBipBuffer`; `Crust_Stack` |
| Allocation | `Crust_Allocator` interface with system and arena implementations, `Crust_Arena` |
| Containers | `Crust_HashIndexMap` (open addressing, user-defined key operations), `Crust_GenerationalHandlePool` |
| Offset heap | `Crust_Heap`: a TLSF-style allocator that hands out offsets instead of pointers, meant for sub-allocating GPU memory; many heaps can share one node pool |

The sources are split into `src/freestanding` (everything that doesn't call the C standard library directly) and `src/libc` (thin wrappers for memory operations, trigonometry and the system allocator).

## Supported targets

- **Compilers:** MSVC, GCC, Clang
- **Architectures:** x86, x64, ARM, ARM64, WebAssembly (wasm32/wasm64)
- **Toolchains:** MSVC, MinGW, Android, Emscripten, WASI, Apple

## Building

crust is built with CMake (3.10 or newer). The repository ships presets for Visual Studio 2022/2026, Emscripten and macOS (presets need CMake 3.28+):

```bash
cmake --preset msvc2026-x64
cmake --build --preset build-msvc2026-x64-release
```

The build presets also install the library into `build/`: headers into `build/include/crust`, libraries into `build/lib` and the CMake package into `build/lib/cmake/crust`.

Without presets:

```bash
cmake -S . -B build
cmake --build build
```

Options:

| Option | Default | Description |
|---|---|---|
| `BUILD_SHARED_LIBS` | `OFF` | Build crust as a shared library |
| `CRUST_BUILD_TESTS` | `OFF` | Build the tests (see [Tests](#tests)) |

## Using crust in your project

As an installed package:

```cmake
find_package(crust CONFIG REQUIRED)
target_link_libraries(app PRIVATE crust::crust)
```

Point `CMAKE_PREFIX_PATH` at the install directory, e.g. `-DCMAKE_PREFIX_PATH=path/to/crust/build`.

As a subdirectory:

```cmake
add_subdirectory(crust)
target_link_libraries(app PRIVATE crust::crust)
```

Then include the single header:

```c
#include <crust.h>
```

## Examples

### Arena allocator

```c
static u8 memory[64 * 1024];

Crust_Arena arena = crustArenaAttach(memory, sizeof(memory));
Crust_Allocator allocator = crustArenaAllocatorInit(&arena);

u32 *indices = crustAllocArray(allocator, u32, 1024);
Crust_Vec3 *positions = crustAllocArray(allocator, Crust_Vec3, 256);

// ... use the memory ...

crustArenaReset(&arena);
```

`crustAllocArray` returns `CRUST_NULL` when the arena is full.

### Sub-allocating GPU memory

```c
Crust_Allocator allocator = crustSystemAllocatorInit();

// One node pool bounds the total number of allocations across all heaps
Crust_HeapNodePool pool;
crustHeapNodePoolAlloc(allocator, &pool, 4096);

// One heap per device memory block (VkDeviceMemory, ID3D12Heap, MTLHeap, ...)
Crust_Heap heap = crustHeapInit(&pool, 256 * 1024 * 1024);

Crust_HeapAllocation allocation = crustHeapAllocAligned(&heap, 64 * 1024, 256);
if (allocation.index != CRUST_U32_MAX)
{
	// bind the resource at allocation.offset inside the block
	crustHeapFree(&heap, allocation);
}

crustHeapNodePoolFree(allocator, &pool);
```

### Lock-free producer/consumer queue

```c
static u8 storage[4096]; // capacity must be a power of two
static Crust_SpscRingBuffer queue;

queue = crustSpscRingBufferAttach(storage, sizeof(storage));

// producer thread: returns how many bytes fit
usize written = crustSpscRingBufferWrite(&queue, data, size);

// consumer thread: returns how many bytes were available
usize read = crustSpscRingBufferRead(&queue, buffer, sizeof(buffer));
```

### Generational handles

```c
Crust_GenerationalHandlePool handles;
crustGenerationalHandlePoolAlloc(allocator, &handles, 64);

Crust_GenerationalHandle handle = crustGenerationalHandlePoolAdd(&handles, 1024);
crustGenerationalHandlePoolRemove(&handles, handle);

// the slot can be reused, but the old handle stays invalid
crustGenerationalHandlePoolCheck(&handles, handle); // 0

crustGenerationalHandlePoolFree(allocator, &handles);
```

## Conventions

- **Naming:** types are `Crust_Name`, functions `crustName`, macros and constants `CRUST_NAME`, function pointer types `PFN_crustName`.
- **Memory:** `*Attach` functions wrap memory owned by the caller; `*Alloc`/`*Free`/`*Grow` take the `Crust_Allocator` to use. Nothing is allocated implicitly.
- **Contracts:** API misuse (out-of-range sizes, invalid handles, double frees, ...) is caught by `CRUST_ASSERT` in debug builds. Release builds don't check, so keep debug builds in your workflow.
- **Limitations:** `Crust_Transform` assumes uniform scale. `Crust_RwLock` prefers writers, so read locks are not reentrant.

## Tests

The tests use CTest and are disabled by default:

```bash
cmake --preset msvc2026-x64 -DCRUST_BUILD_TESTS=ON
cmake --build --preset build-msvc2026-x64-debug
ctest --test-dir build/msvc2026-x64 -C Debug --output-on-failure
```

Multithreaded test cases are skipped on Emscripten.

## License

[MIT](LICENSE)
