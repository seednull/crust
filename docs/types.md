# Types and scalar functions

## Base types

| Crust | Same type as | Notes |
|---|---|---|
| `i8`, `i16`, `i32`, `i64` | `int8_t` … `int64_t` | |
| `u8`, `u16`, `u32`, `u64` | `uint8_t` … `uint64_t` | |
| `isize` | `ptrdiff_t` | |
| `usize` | `size_t` | |
| `f32`, `f64` | `float`, `double` | |

These are not just types of the same size: they are the **same C types** as the `<stdint.h>` and `<stddef.h>` ones, without including those headers. GCC and Clang take them from the predefined `__INT8_TYPE__` … `__SIZE_TYPE__` macros that `<stdint.h>` itself uses; MSVC uses the same explicit typedefs as its own headers. So `u32 *` and `uint32_t *` are interchangeable, and redeclaring `extern uint64_t x;` as `extern u64 x;` compiles. `tests/types.c` checks this on every target.

Things that follow from it:

- **`usize` and `u64` can be different types of the same size.** On macOS `size_t` is `unsigned long` and `uint64_t` is `unsigned long long`; on 32-bit targets `usize` is 32-bit. Don't pass a `usize *` where a `u64 *` is expected.
- **`u64` is `unsigned long` on 64-bit Linux** and `unsigned long long` on Windows and macOS. For `printf`, cast to `unsigned long long` and use `%llu`, or use `%zu` for `usize`.

## Limits and constants

| Constant | Value |
|---|---|
| `CRUST_I8_MIN`, `CRUST_I8_MAX` … `CRUST_I64_MIN`, `CRUST_I64_MAX` | Signed limits |
| `CRUST_U8_MAX` … `CRUST_U64_MAX` | Unsigned limits |
| `CRUST_ISIZE_MIN`, `CRUST_ISIZE_MAX`, `CRUST_USIZE_MAX` | Limits for the pointer width |
| `CRUST_F32_EPSILON`, `CRUST_F64_EPSILON` | Distance from 1.0 to the next representable value |
| `CRUST_F32_MAX`, `CRUST_F64_MAX` | Largest finite value |
| `CRUST_F32_MIN`, `CRUST_F64_MIN` | **Most negative** finite value, `-CRUST_F32_MAX`. Not the C `FLT_MIN` |
| `CRUST_F32_POSITIVE_MIN`, `CRUST_F64_POSITIVE_MIN` | Smallest positive normal value, the C `FLT_MIN`/`DBL_MIN` |
| `CRUST_PI`, `CRUST_TAU` | π and 2π as `double` literals |

`CRUST_USIZE_MAX` and `CRUST_U32_MAX` double as "not found" or "invalid" markers across the API.

## Min, max, clamp and abs

Available for `U8`, `U16`, `U32`, `U64`, `USize`, `I8`, `I16`, `I32`, `I64`, `ISize`, `F32` and `F64`:

```c
u32 a = crustMinU32(x, y);
f32 b = crustClampF32(t, 0.0f, 1.0f);
u64 c = crustAbsI64(delta);
```

| Function | Definition |
|---|---|
| `crustMin*(a, b)` | `(a < b) ? a : b` |
| `crustMax*(a, b)` | `(a < b) ? b : a` |
| `crustClamp*(v, lo, hi)` | `crustMax(lo, crustMin(v, hi))`. If `lo > hi` the result is `lo` |
| `crustAbsI*(v)` | Returns the **unsigned** type of the same width, so `crustAbsI32(CRUST_I32_MIN)` is `2147483648u` without overflow |

For floats the definitions are taken literally, which fixes NaN and signed-zero behavior:

- `crustMinF32(a, b)` returns `b` when either argument is NaN or when they compare equal, so `crustMinF32(+0.0f, -0.0f)` is `-0.0f`.
- `crustMaxF32(a, b)` returns `a` in those cases.

The SIMD `Min`/`Max` functions follow the same definitions bit for bit.

## Powers of two and alignment

Available for `U8`, `U16`, `U32`, `U64` and `USize`:

| Function | Result | Preconditions |
|---|---|---|
| `crustIsPow2*(v)` | `1` if `v` is a power of two | `v != 0` |
| `crustIsAligned*(v, alignment)` | `1` if `v` is a multiple of `alignment` | `alignment` is a power of two |
| `crustAlignDown*(v, alignment)` | `v` rounded down to a multiple of `alignment` | same |
| `crustAlignUp*(v, alignment)` | `v` rounded up to a multiple of `alignment`. Wraps around if the result does not fit | same |

The preconditions are asserted. For pointers, go through `usize`:

```c
u8 *aligned = (u8 *)crustAlignUpUSize((usize)ptr, 64);
```

## Bit operations

| Function | Result | Preconditions |
|---|---|---|
| `crustLzcntU32(v)` | Number of leading zero bits | `v != 0` |
| `crustTzcntU32(v)` | Number of trailing zero bits | `v != 0` |
| `crustPopcntU32(v)` | Number of set bits | none |

Zero is excluded for `Lzcnt` and `Tzcnt` because the underlying `bsr`/`bsf` and `__builtin_clz`/`__builtin_ctz` leave the result undefined for it; check before calling. On MSVC x86 and x64, `crustPopcntU32` executes the `POPCNT` instruction.

## Floating-point functions

Inline, available for `F32` and `F64`:

| Function | Result |
|---|---|
| `crustAbsF32(v)` | Absolute value; clears the sign bit, so `-0.0f` becomes `+0.0f` and NaN payloads are kept |
| `crustSqrtF32(v)` | Square root |
| `crustRsqrtF32(v)` | `1.0f / crustSqrtF32(v)`: an exact division, not a hardware approximation |
| `crustFloorF32(v)` | Rounds toward −∞ |
| `crustCeilF32(v)` | Rounds toward +∞ |
| `crustRoundEvenF32(v)` | Rounds to nearest, **ties to even**: `0.5 → 0`, `1.5 → 2`, `2.5 → 2` |
| `crustTruncateF32(v)` | Rounds toward zero |

`crustRoundEven` is not C's `round`, which rounds ties away from zero. It matches C23 `roundeven`, the default IEEE rounding and the SIMD rounding instructions.

How they are implemented:

| Compiler | Implementation |
|---|---|
| GCC, Clang | `__builtin_fabsf`, `__builtin_sqrtf`, `__builtin_floorf`, `__builtin_ceilf`, `__builtin_rintf`, `__builtin_truncf`. These usually become single instructions, but can become libm calls when the target has no matching instruction (x86 without SSE4.1, 32-bit ARM). `rint` follows the current rounding mode, so `crustRoundEven*` assumes the default round-to-nearest mode |
| MSVC x86/x64 | SSE intrinsics: `sqrtss`, and `roundss`/`roundsd` from SSE4.1 for the rounding functions |
| MSVC ARM64 | NEON intrinsics: `vrndm`, `vrndp`, `vrndn`, `vrnd` |

On MSVC x86 (32-bit), floating-point values returned by value travel through the x87 stack. That doesn't change results of these functions, but it can turn a signaling NaN into a quiet one.

### Trigonometry

`crustCosF32`, `crustAcosF32`, `crustSinF32`, `crustAsinF32`, `crustTanF32`, `crustAtan2F32(y, x)` and their `F64` versions are exported functions in `src/libc` that forward to the C math library. They are not inline.

## Memory functions

| Function | Meaning |
|---|---|
| `crustMemcpy(dst, src, size)` | `memcpy` |
| `crustMemset(dst, value, size)` | `memset` |

Both are exported functions in `src/libc`. The rest of Crust uses them instead of calling the C library directly, see [running without the C standard library](building.md#running-without-the-c-standard-library).
