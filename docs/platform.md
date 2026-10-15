# Platform

Header: `<crust/core.h>`, which every other Crust header includes.

`core.h` detects the compiler, toolchain, architecture and SIMD backend, then defines a handful of portability macros. Everything here is available as soon as any Crust header is included.

## Version

| Macro | Value |
|---|---|
| `CRUST_VERSION_MAJOR` | `1` |
| `CRUST_VERSION_MINOR` | `3` |
| `CRUST_VERSION_PATCH` | `0` |
| `CRUST_VERSION` | `"1.3.0"` |

## Detection macros

Every detection macro is **always defined**, to `0` or `1`. Test them with `#if`, not `#ifdef`:

```c
#if CRUST_ARCH_ARM64 && CRUST_COMPILER_MSVC
	// Windows on ARM
#endif
```

### Compiler

| Macro | Set when |
|---|---|
| `CRUST_COMPILER_CLANG` | `__clang__` is defined. Checked first, so `clang-cl` counts as Clang |
| `CRUST_COMPILER_MSVC` | `_MSC_VER` is defined and the compiler is not Clang |
| `CRUST_COMPILER_GCC` | `__GNUC__` is defined and the compiler is neither of the above |

Any other compiler stops the build with `#error Unsupported compiler`.

### Toolchain

The toolchain describes the target environment rather than the compiler. At most one macro is `1`; they are checked in this order:

| Macro | Set when |
|---|---|
| `CRUST_TOOLCHAIN_EMSCRIPTEN` | `__EMSCRIPTEN__` |
| `CRUST_TOOLCHAIN_WASI` | `__wasi__` or `__wasip1__`/`__wasip2__`/`__wasip3__` |
| `CRUST_TOOLCHAIN_ANDROID` | `__ANDROID__` |
| `CRUST_TOOLCHAIN_MINGW` | `__MINGW32__` or `__MINGW64__` |
| `CRUST_TOOLCHAIN_MSVC` | `_MSC_VER`, which includes `clang-cl` |
| `CRUST_TOOLCHAIN_APPLE` | `__APPLE__` |

A plain GCC or Clang build for Linux or another Unix leaves all toolchain macros at `0`.

### Architecture

| Macro | Set when | Width |
|---|---|---|
| `CRUST_ARCH_WASM64` | `__wasm64__` | 64-bit |
| `CRUST_ARCH_WASM32` | `__wasm32__`, or `__wasm__` without `__wasm64__` | 32-bit |
| `CRUST_ARCH_ARM64` | `_M_ARM64`, `_M_ARM64EC` or `__aarch64__` | 64-bit |
| `CRUST_ARCH_ARM` | `__arm__` | 32-bit |
| `CRUST_ARCH_X64` | `_M_X64`, `_M_AMD64` or `__x86_64__` | 64-bit |
| `CRUST_ARCH_X86` | `_M_IX86` or `__i386__` | 32-bit |

`CRUST_ARCH_32BIT` or `CRUST_ARCH_64BIT` is set to match. Any other architecture stops the build with `#error`.

### SIMD backend

Exactly one of `CRUST_SIMD_SSE`, `CRUST_SIMD_NEON`, `CRUST_SIMD_WASM` and `CRUST_SIMD_SCALAR` is `1`. The choice depends on `CRUST_USE_SIMD` and the architecture, see [SIMD](simd.md#enabling-simd).

## Portability macros

| Macro | MSVC | GCC and Clang |
|---|---|---|
| `CRUST_INLINE` | `__forceinline` | `inline __attribute__((always_inline))` |
| `CRUST_RESTRICT` | `__restrict` | `__restrict__` |
| `CRUST_ALIGNOF(type)` | `__alignof(type)` | `__alignof__(type)` |
| `CRUST_ALIGNAS(n)` | `__declspec(align(n))` | `__attribute__((aligned(n)))` |
| `CRUST_DEBUG_BREAK()` | `__debugbreak()` | `__builtin_trap()` |

`CRUST_ALIGNAS` goes in front of the declaration, which works for both compilers: `CRUST_ALIGNAS(16) u8 bytes[16];`.

Other macros:

| Macro | Meaning |
|---|---|
| `CRUST_NULL` | `((void *)0)` in C, `0` in C++ |
| `CRUST_UNUSED(x)` | Silences unused-variable warnings |
| `CRUST_APIENTRY` | Marks the functions implemented in Crust's sources. Empty |

## Assertions

`CRUST_ASSERT(x)` checks API contracts: valid pointers, sizes, alignments, indices, handles and preconditions such as `crustLzcntU32(0)`.

Running out of something the caller can't cheaply predict, such as memory or heap nodes, is reported through the function's result instead. Limits the caller controls and can read from public fields, such as the free space of a ring buffer, are preconditions.

```c
#if defined(NDEBUG)
	#define CRUST_ASSERT(x) ((void)0)
#else
	#define CRUST_ASSERT(x) /* calls crustAssertFailure(#x, __FILE__, __LINE__) when x is false */
#endif
```

- **On failure** Crust stops in the debugger without printing anything; the expression, file and line are arguments of `crustAssertFailure`.
- **With `NDEBUG`** the checks disappear completely and the expression is not evaluated. Violating a contract in a release build is undefined behavior: out-of-bounds access, corrupted containers, wrong results.
- **Two configurations.** Assertions in header functions follow the `NDEBUG` setting of *your* translation unit, while assertions inside the compiled library follow the configuration Crust itself was built with.

Keep debug builds in your workflow: they are what catches misuse.

## C and C++

Every Crust header compiles as C99 and as C++, with the declarations wrapped in `extern "C"`.

## System headers

Crust headers include only compiler intrinsic headers, never a C library header such as `<stdlib.h>` or `<string.h>`. Only `scalar.h`, `atomic.h` and `simd.h` include any, and the other modules include none. The SSE headers normally pull in `<mm_malloc.h>` (GCC, Clang) or `<malloc.h>` (MSVC), which Crust skips: if you use `_mm_malloc` after including Crust, include that header yourself.

## Platform notes

| Target | Note |
|---|---|
| MSVC, x86 and x64 | `crustFloor*`, `crustCeil*`, `crustRoundEven*` and `crustTruncate*` use SSE4.1 and `crustPopcntU32` uses `POPCNT`, unconditionally. Any CPU from about 2011 on has both |
| GCC | Compile in ISO mode (`-std=c99`, or `CMAKE_C_EXTENSIONS OFF`) or with `-ffp-contract=off`. In its default GNU dialects GCC fuses multiplies and adds into FMA instructions wherever the target has them, even across inlined Crust calls: always on ARM64, on x86 with `-mfma` or `-march=haswell` and later. Results then differ in the last bit |
| 32-bit x86 with GCC | Compile with `-msse2 -mfpmath=sse`. GCC's default x87 math rounds twice, so results can differ from the other targets in the last bit. Crust doesn't add these flags itself, because they change the arithmetic of all your code |
| 32-bit x86 with MSVC | Floats returned by value pass through the x87 stack, which can turn a signaling NaN into a quiet one |
| ARM64 with MSVC | With `/O2`, a loop that applies `crustMinF32` or `crustMaxF32` to an array can be vectorized into `fmin`/`fmax`, which treat NaN and signed zeros differently. Use the SIMD `Min`/`Max` or put `#pragma loop(no_vector)` before the loop |
