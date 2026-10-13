# Platform

`crust.h` starts by detecting the compiler, toolchain, architecture and SIMD backend, then defines a handful of portability macros. Everything here is available as soon as the header is included.

## Version

| Macro | Value |
|---|---|
| `CRUST_VERSION_MAJOR` | `1` |
| `CRUST_VERSION_MINOR` | `0` |
| `CRUST_VERSION_PATCH` | `0` |
| `CRUST_VERSION` | `"1.0.0-dev"` |

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
| `CRUST_TOOLCHAIN_APPLE` | `__apple_build_version__`, that is Apple Clang. Upstream Clang on macOS does not set it |

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

- `CRUST_INLINE` is used as `static CRUST_INLINE` for every header function. MSVC does not inline anything at `/Od`, even `__forceinline`, so debug builds pay for every call.
- `CRUST_ALIGNAS` goes in front of the declaration, which works for both compilers: `CRUST_ALIGNAS(16) u8 bytes[16];`.
- `CRUST_ALIGNOF` works on types, which is what `crustAlloc` relies on.

Other macros:

| Macro | Meaning |
|---|---|
| `CRUST_NULL` | `((void *)0)` in C, `0` in C++ |
| `CRUST_UNUSED(x)` | Silences unused-variable warnings |
| `CRUST_APIENTRY` | Marks exported functions: empty for static builds, `__declspec(dllexport/dllimport)` or `visibility("default")` for shared builds, see [Building](building.md#without-cmake) |

## Assertions

`CRUST_ASSERT(x)` checks API contracts: valid pointers, sizes, alignments, indices, handles and preconditions such as `crustLzcntU32(0)`.

```c
#if defined(NDEBUG)
	#define CRUST_ASSERT(x) ((void)0)
#else
	#define CRUST_ASSERT(x) /* calls crustAssertFailure(#x, __FILE__, __LINE__) when x is false */
#endif
```

- **On failure** Crust stops in the debugger: `__debugbreak()` on MSVC, `__builtin_debugtrap()` on Clang, `__builtin_trap()` on GCC. Nothing is printed; the expression, file and line are arguments of `crustAssertFailure` and visible in the debugger.
- **With `NDEBUG`** the checks disappear completely and the expression is not evaluated. Violating a contract in a release build is undefined behavior: out-of-bounds access, corrupted containers, wrong results.
- **Two configurations.** Assertions in header functions follow the `NDEBUG` setting of *your* translation unit, while assertions inside the compiled library follow the configuration Crust itself was built with.

Keep debug builds in your workflow: they are what catches misuse.

## C and C++

`crust.h` compiles as C99 and as C++. The prototypes are wrapped in `extern "C"`. The system intrinsic headers (`<intrin.h>`, `<nmmintrin.h>`, `<arm_neon.h>`, `<wasm_simd128.h>`) are included before that block, because in C++ some of them pull in declarations that cannot live inside `extern "C"`.
