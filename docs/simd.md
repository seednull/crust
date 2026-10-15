# SIMD

Crust has one portable SIMD type, `Crust_Simd128`: 128 bits with no lane type attached. The operation decides how the bits are read, so the same value can be added as four `f32`, compared as sixteen `u8` and masked as one 128-bit block:

```c
Crust_Simd128 v = crustSimd128LoadF32(values);
Crust_Simd128 positive = crustSimd128GreaterF32(v, crustSimd128AllZeros());
Crust_Simd128 clamped = crustSimd128Select(positive, v, crustSimd128AllZeros());
u32 any_negative = crustSimd128BitmaskU32(crustSimd128LessF32(v, crustSimd128AllZeros())) != 0;
```

Header: `<crust/simd.h>`. Every function is `static CRUST_INLINE` and works on values, so there is nothing to link and nothing to initialize.

- [Enabling SIMD](#enabling-simd)
- [The type](#the-type)
- [Naming](#naming)
- [API](#api)
- [Semantics and contracts](#semantics-and-contracts)
- [Performance notes](#performance-notes)
- [Examples](#examples)

## Enabling SIMD

SIMD is opt-in. Without it, `Crust_Simd128` is a plain 16-byte struct and every operation is a loop over its lanes: the **scalar backend**. It gives the same results bit for bit, so code written against the API works everywhere and can be switched to hardware SIMD later.

Turn on the hardware backends with the CMake option, which adds the define and the compiler flags:

```bash
cmake -S . -B build -DCRUST_USE_SIMD=ON
```

or, without CMake, define `CRUST_USE_SIMD=1` and pass the flags yourself:

| Architecture | Backend | Flags for GCC and Clang | Requirement |
|---|---|---|---|
| x86, x64 | SSE (`CRUST_SIMD_SSE`) | `-msse4.2 -mpopcnt` | x86-64-v2 CPU: SSE4.2 and POPCNT. GCC 7 or Clang 9 |
| ARM64 | NEON (`CRUST_SIMD_NEON`) | none | none |
| WebAssembly | WASM (`CRUST_SIMD_WASM`) | `-msimd128` | A runtime with WebAssembly SIMD, as in every current browser and Node.js |
| Anything else | Scalar (`CRUST_SIMD_SCALAR`) | none | none |

- **Missing flags are an error.** With `CRUST_USE_SIMD=1`, the header stops with `#error "CRUST_USE_SIMD requires SSE4.2 and POPCNT on x86: compile with -msse4.2 -mpopcnt"` (or the WebAssembly equivalent) instead of silently falling back.
- **MSVC needs no flags** and checks nothing: the program must run on an x86-64-v2 CPU. There is no runtime CPU detection.
- **Keep the setting consistent.** `Crust_Simd128` is a different C type with and without SIMD. Every translation unit that passes these values to another must use the same `CRUST_USE_SIMD`. The CMake target propagates the define to everything that links it.

## The type

| Backend | `Crust_Simd128` is |
|---|---|
| SSE | `__m128i` |
| NEON | `uint8x16_t` |
| WASM | `v128_t` |
| Scalar | `struct { CRUST_ALIGNAS(16) u8 bytes[16]; }` |

- **16 bytes, 16-byte aligned** on every backend.
- **Treat it as opaque.** It is the native register type so the compiler can keep it in registers, but code that reaches into it is not portable. Mixing Crust calls with raw intrinsics on the same value works and is fine for platform-specific fast paths.
- **Lane 0 is the lowest address.** `crustSimd128StoreU32(p, v)` writes lane 0 to `p[0]`, lane 1 to `p[1]` and so on, on every backend.

## Naming

Functions are named `crustSimd128` + operation + suffix:

| Suffix | Meaning |
|---|---|
| `U8`, `I8`, `U16`, `I16`, `U32`, `I32`, `U64`, `I64` | Integer lanes of that width and signedness: 16, 8, 4 or 2 lanes |
| `F32`, `F64` | Floating-point lanes: 4 or 2 lanes |
| none | The operation ignores lanes: `And`, `Or`, `Select`, `Any`, ... |

- **Where signedness doesn't matter,** `I` versions still exist for readability and do exactly what the `U` version does: `crustSimd128AddI32` is `crustSimd128AddU32`.
- **Words are spelled out:** `Equal`, `LessEqual`, `Negate`, `Truncate`, `Saturate`.
- **Halves are `Low` and `High`:** lanes `0 … n/2−1` and `n/2 … n−1`.
- **Conversions are `Convert<To>From<From>`,** for example `crustSimd128ConvertF32FromI32`.
- **Swizzles name lanes `X Y Z W`,** see [Swizzles](#swizzles).

## API

### Constants

| Function | Result |
|---|---|
| `crustSimd128AllZeros()` | All 128 bits clear |
| `crustSimd128AllOnes()` | All 128 bits set: every lane is `-1`, `U*_MAX`, or a NaN for float lanes |

### Memory

| Function | Notes |
|---|---|
| `crustSimd128LoadU8(const u8 *ptr)` … `LoadF64` | Reads 16 bytes, any alignment |
| `crustSimd128LoadAlignedU8(ptr)` … `LoadAlignedF64` | Reads 16 bytes; `ptr` must be 16-byte aligned (asserted) |
| `crustSimd128StoreU8(u8 *ptr, v)` … `StoreF64` | Writes 16 bytes, any alignment |
| `crustSimd128StoreAlignedU8(ptr, v)` … `StoreAlignedF64` | Writes 16 bytes; `ptr` must be 16-byte aligned (asserted) |

Available for all ten lane types. The pointer type matches the lane type, so `crustSimd128LoadF32` takes `const f32 *`: no casts, no aliasing problems. Because the vector itself is untyped, data can be loaded with one type's function and processed with another's: a byte buffer that holds floats can be loaded with `crustSimd128LoadU8` and then used with `F32` operations.

There are no partial loads and stores. Process the tail of an array with a scalar loop, or build the last vector with `Make`/`SetLane`.

### Constructors

| Function | Result |
|---|---|
| `crustSimd128SplatU8(v)` … `SplatF64(v)` | `v` in every lane, all ten types |
| `crustSimd128MakeU32(v0, v1, v2, v3)`, `MakeI32`, `MakeF32` | Four lanes, `v0` in lane 0 |
| `crustSimd128MakeU64(v0, v1)`, `MakeI64`, `MakeF64` | Two lanes, `v0` in lane 0 |

There is no `Make` for 8- and 16-bit lanes. Use `Load` from an array.

### Lanes

| Function | Result |
|---|---|
| `crustSimd128GetLaneU8(v, lane)` … `GetLaneF64` | The value of one lane |
| `crustSimd128SetLaneU8(v, lane, value)` … `SetLaneF64` | `v` with one lane replaced |

`lane` is a run-time `u32`; out-of-range lanes are asserted. Both are implemented through a small stack array. GCC and Clang turn that into single lane instructions when the index is a constant; MSVC often keeps the round trip through memory. Prefer whole-vector operations in hot loops.

### Interleaving

| Function | Result |
|---|---|
| `crustSimd128ZipLowU32(a, b)` | `a0 b0 a1 b1`: lanes from the low halves, alternating |
| `crustSimd128ZipHighU32(a, b)` | `a2 b2 a3 b3`: lanes from the high halves, alternating |

Available for all ten lane types. Each is one instruction on every hardware backend. Zipping a vector with itself duplicates lanes: `ZipLowU32(v, v)` is `x x y y`, `ZipLowU64(v, v)` is `x y x y`.

### Swizzles

A swizzle rearranges the lanes of one vector, like `v.yzxw` in a shader. The lane width comes right after `Swizzle`, followed by one letter per result lane, starting from lane 0:

```c
Crust_Simd128 r = crustSimd128Swizzle32YZXW(v); // r = (v.y, v.z, v.x, v.w)
Crust_Simd128 h = crustSimd128Swizzle64YX(d);   // swaps the two 64-bit halves
```

| Family | Patterns |
|---|---|
| `crustSimd128Swizzle32…` | The 23 permutations of `XYZW` other than `XYZW` itself, and the broadcasts `XXXX`, `YYYY`, `ZZZZ`, `WWWW` |
| `crustSimd128Swizzle64…` | `YX`, `XX`, `YY` |

Swizzles move bits and don't care about the lane type: use `Swizzle32` for `u32`, `i32` and `f32` lanes alike. Patterns with repeated lanes other than broadcasts (`XXYY`, `XYXY`, ...) are not provided; `ZipLow`/`ZipHigh` of a vector with itself covers the common ones.

Cost: on SSE every swizzle is one `pshufd`. On WASM it is one constant `i8x16.shuffle`. On NEON, `Swizzle32YXWZ` is `rev64`; `YZWX`, `ZWXY`, `WXYZ` and `Swizzle64YX` are `ext`; broadcasts are `dup`; the remaining patterns are a `tbl` lookup with a constant table, which compilers load once outside loops.

### Bitwise operations

| Function | Result |
|---|---|
| `crustSimd128And(a, b)` | `a & b` |
| `crustSimd128Or(a, b)` | `a \| b` |
| `crustSimd128Xor(a, b)` | `a ^ b` |
| `crustSimd128AndNot(a, b)` | `a & ~b`. Note the order: SSE's `_mm_andnot_si128` computes `~a & b` |
| `crustSimd128Not(v)` | `~v` |
| `crustSimd128Select(mask, a, b)` | `(mask & a) \| (~mask & b)`: bits of `a` where `mask` is set, bits of `b` elsewhere |

### Masks

| Function | Result |
|---|---|
| `crustSimd128Any(v)` | `1` if any bit of `v` is set |
| `crustSimd128All(v)` | `1` if all 128 bits are set |
| `crustSimd128BitmaskU8(v)` | Bit `i` is the top bit of lane `i`: a 16-bit result |
| `crustSimd128BitmaskU16(v)` | Same for 8 lanes |
| `crustSimd128BitmaskU32(v)` | Same for 4 lanes |
| `crustSimd128BitmaskU64(v)` | Same for 2 lanes |

`Any` and `All` answer "did any lane / every lane pass" for comparison results. `Bitmask` tells which lanes passed; combine it with `crustTzcntU32` and `crustPopcntU32`.

### Integer arithmetic

| Function | Types | Result |
|---|---|---|
| `Add`, `Sub` | all 8 integer types | Wraps around on overflow |
| `AddSaturate`, `SubSaturate` | `U8`, `I8`, `U16`, `I16` | Clamps to the type's range |
| `Mul` | `U16`, `I16`, `U32`, `I32` | Low half of the product, wraps around |
| `Min`, `Max` | all 8 integer types | Signed or unsigned by suffix |
| `ShiftLeft(v, count)` | all 8 integer types | Shifts every lane by the same run-time count |
| `ShiftRight(v, count)` | all 8 integer types | Logical for `U`, arithmetic for `I` |

Shift counts must be smaller than the lane width; this is asserted. There are no per-lane shift counts, no 8-bit or 64-bit multiplies and no saturating 32-bit arithmetic.

### Integer comparisons

`Equal`, `NotEqual`, `Less`, `LessEqual`, `Greater`, `GreaterEqual`, for all 8 integer types. Each lane of the result is all ones when the comparison holds and all zeros otherwise. Signedness comes from the suffix: `LessU8` treats `0xFF` as 255, `LessI8` as −1.

### Floating-point operations

For `F32` and `F64`:

| Function | Result |
|---|---|
| `Add`, `Sub`, `Mul`, `Div` | IEEE arithmetic, correctly rounded |
| `Sqrt` | Square root, correctly rounded |
| `Abs`, `Negate` | Clear or flip the sign bit |
| `Min`, `Max` | `(a < b) ? a : b` and `(a < b) ? b : a`, see [semantics](#floating-point) |
| `Floor`, `Ceil`, `RoundEven`, `Truncate` | Round to an integral value: toward −∞, toward +∞, to nearest with ties to even, toward zero |
| `Equal`, `NotEqual`, `Less`, `LessEqual`, `Greater`, `GreaterEqual` | All-ones or all-zeros lanes |

There is no fused multiply-add, reciprocal estimate or horizontal sum; a [dot product example](#dot-product) shows how to reduce lanes.

### Conversions

| Function | Result |
|---|---|
| `ConvertF32FromI32(v)` | Four `i32` to `f32`, rounded to nearest |
| `ConvertI32FromF32(v)` | Four `f32` to `i32`, truncated toward zero. Precondition: every lane satisfies `\|x\| < 2³¹`, which excludes NaN (asserted) |
| `ConvertI32FromF32Saturate(v)` | Same, but out-of-range lanes saturate to `CRUST_I32_MIN`/`CRUST_I32_MAX` and NaN becomes 0 |
| `ConvertF64FromF32Low(v)`, `…High(v)` | Lanes 0–1 or 2–3 widened to `f64`, exact |
| `ConvertF32FromF64(low, high)` | `low` fills lanes 0–1, `high` lanes 2–3, rounded to nearest |
| `ConvertU16FromU8Low/High`, `ConvertI16FromI8Low/High` | Half of the lanes, zero- or sign-extended |
| `ConvertU32FromU16Low/High`, `ConvertI32FromI16Low/High` | same |
| `ConvertU64FromU32Low/High`, `ConvertI64FromI32Low/High` | same |
| `ConvertI8FromI16Saturate(low, high)` | Sixteen `i16` narrowed to `i8` with saturation; `low` fills lanes 0–7, `high` lanes 8–15 |
| `ConvertU8FromI16Saturate(low, high)` | Signed `i16` clamped to `0 … 255` |
| `ConvertI16FromI32Saturate(low, high)` | Eight `i32` narrowed to `i16` with saturation |
| `ConvertU16FromI32Saturate(low, high)` | Signed `i32` clamped to `0 … 65535` |

Narrowing takes two vectors so that no lanes are wasted: two vectors of eight `i16` become one vector of sixteen `u8`.

## Semantics and contracts

### Results are identical on every backend

Every function is specified to produce the same bits on SSE, NEON, WASM and the scalar backend, including NaN propagation by `Min`/`Max`, signed zeros, saturation and wrap-around. The one exception is a NaN that arithmetic creates, such as `0 · ∞`: it is a NaN on every backend, but its sign and payload come from the hardware. The tests check whichever backend is built against scalar reference implementations, bit for bit. Inputs that break a precondition are not covered: out-of-range shift counts, `ConvertI32FromF32` of out-of-range values, misaligned `Aligned` pointers, out-of-range lanes. These are asserted in debug builds. In release builds their results differ between backends, and an out-of-range lane index is undefined behavior. GCC needs FP contraction off for this, and SSE math on 32-bit x86, see [Platform notes](platform.md#platform-notes).

### Masks

Comparisons produce lanes that are all ones or all zeros. `Select`, `And` and the other bitwise operations treat masks as plain bits, so a mask from a 32-bit comparison works in `Select` on `f32` data. Mixing widths also works bitwise: a `U8` mask applied to `f32` lanes selects individual bytes.

### Integers

- **Overflow** wraps around for `Add`, `Sub` and `Mul`, signed or not. There is no undefined behavior: signed lanes are two's complement.
- **Saturating** functions clamp instead of wrapping.

### Floating point

- **IEEE semantics** for arithmetic, `Sqrt` and comparisons: comparisons with NaN are false, except `NotEqual`, which is true.
- **`Min` and `Max`** are defined as `(a < b) ? a : b` and `(a < b) ? b : a`, exactly like `crustMinF32` and `crustMaxF32`. So `Min` returns `b` when either input is NaN or when they compare equal, which makes `Min(+0, −0)` return `−0`. This is not IEEE `minNum`, and it is not what `vminq_f32` or `wasm_f32x4_min` do; Crust picks the instructions that match its own definition.
- **No fusing.** `crustSimd128AddF32(crustSimd128MulF32(a, b), c)` is a multiply followed by an add, never an FMA, as long as the compiler doesn't contract floating-point expressions across statements. GCC does in its default GNU dialects, see [Platform notes](platform.md#platform-notes).
- **Default floating-point environment.** Crust does not change rounding modes or flush denormals. The SSE backend uses explicit rounding immediates, so `RoundEven` rounds to nearest even regardless of `MXCSR`. Denormal handling follows the current FTZ/DAZ settings, which Crust leaves alone.

## Performance notes

- **Debug builds.** MSVC doesn't inline at `/Od`, so every operation becomes a call that passes vectors through memory. SIMD code in debug builds can be slower than plain scalar code.
- **Aligned vs. unaligned.** Unaligned loads and stores cost the same as aligned ones on current CPUs when the data happens to be aligned. On SSE without AVX, only aligned loads can be folded into arithmetic instructions as memory operands.
- **Lanes.** `GetLane`/`SetLane` take a run-time index and may go through a stack copy, especially on MSVC. Keep values in vectors and reduce once at the end.
- **Emulated operations.** Some operations have no single instruction:

  | Operation | SSE | NEON | WASM |
  |---|---|---|---|
  | `ShiftLeftU8`, `ShiftRightU8`, `ShiftRightI8` | 16-bit shift and mask | native | native |
  | `ShiftRightI64` | xor trick | native | native |
  | Unsigned `Less`/`Greater` on 8–32 bits | sign flip + signed compare | native | native |
  | `LessU64` | sign flip + signed compare | native | sign flip + signed compare |
  | `Min`/`Max` for 64-bit lanes | compare + select | compare + select | compare + select |
  | `ConvertI32FromF32Saturate` | 3–4 extra instructions | native | native |

## Examples

The examples handle the tail of an array with a scalar loop. All of them work unchanged on every backend, including the scalar one.

### Finding a byte

```c
usize findByte(const u8 *data, usize size, u8 value)
{
	Crust_Simd128 needle = crustSimd128SplatU8(value);
	usize i = 0;

	for (; i + 16 <= size; i += 16)
	{
		Crust_Simd128 chunk = crustSimd128LoadU8(data + i);
		u32 mask = crustSimd128BitmaskU8(crustSimd128EqualU8(chunk, needle));

		if (mask != 0)
			return i + crustTzcntU32(mask);
	}

	for (; i < size; ++i)
	{
		if (data[i] == value)
			return i;
	}

	return CRUST_USIZE_MAX;
}
```

### Dot product

Reducing four lanes takes two swizzles and two additions. The sum ends up in every lane, in the order `(m0 + m1) + (m2 + m3)` on every backend:

```c
f32 dot4(const f32 *a, const f32 *b)
{
	Crust_Simd128 m = crustSimd128MulF32(crustSimd128LoadF32(a), crustSimd128LoadF32(b));
	Crust_Simd128 s = crustSimd128AddF32(m, crustSimd128Swizzle32YXWZ(m));
	s = crustSimd128AddF32(s, crustSimd128Swizzle32ZWXY(s));

	return crustSimd128GetLaneF32(s, 0);
}
```

### Matrix times vector

With a column-major 4×4 matrix, each column is scaled by one component of the vector:

```c
Crust_Simd128 transform(const Crust_Simd128 columns[4], Crust_Simd128 v)
{
	Crust_Simd128 r = crustSimd128MulF32(columns[0], crustSimd128Swizzle32XXXX(v));
	r = crustSimd128AddF32(r, crustSimd128MulF32(columns[1], crustSimd128Swizzle32YYYY(v)));
	r = crustSimd128AddF32(r, crustSimd128MulF32(columns[2], crustSimd128Swizzle32ZZZZ(v)));

	return crustSimd128AddF32(r, crustSimd128MulF32(columns[3], crustSimd128Swizzle32WWWW(v)));
}
```
