//
static CRUST_INLINE Crust_Simd128 crustSimd128AllZeros(void)
{
	return wasm_i32x4_splat(0);
}

static CRUST_INLINE Crust_Simd128 crustSimd128AllOnes(void)
{
	return wasm_i32x4_splat(-1);
}

//
static CRUST_INLINE Crust_Simd128 crustSimd128LoadU8(const u8 *ptr)
{
	return wasm_v128_load(ptr);
}

static CRUST_INLINE Crust_Simd128 crustSimd128LoadU16(const u16 *ptr)
{
	return wasm_v128_load(ptr);
}

static CRUST_INLINE Crust_Simd128 crustSimd128LoadU32(const u32 *ptr)
{
	return wasm_v128_load(ptr);
}

static CRUST_INLINE Crust_Simd128 crustSimd128LoadU64(const u64 *ptr)
{
	return wasm_v128_load(ptr);
}

static CRUST_INLINE Crust_Simd128 crustSimd128LoadF32(const f32 *ptr)
{
	return wasm_v128_load(ptr);
}

static CRUST_INLINE Crust_Simd128 crustSimd128LoadF64(const f64 *ptr)
{
	return wasm_v128_load(ptr);
}

static CRUST_INLINE Crust_Simd128 crustSimd128LoadAlignedU8(const u8 *ptr)
{
	CRUST_ASSERT(crustIsAlignedUSize((usize)ptr, 16));
	return wasm_v128_load(ptr);
}

static CRUST_INLINE Crust_Simd128 crustSimd128LoadAlignedU16(const u16 *ptr)
{
	CRUST_ASSERT(crustIsAlignedUSize((usize)ptr, 16));
	return wasm_v128_load(ptr);
}

static CRUST_INLINE Crust_Simd128 crustSimd128LoadAlignedU32(const u32 *ptr)
{
	CRUST_ASSERT(crustIsAlignedUSize((usize)ptr, 16));
	return wasm_v128_load(ptr);
}

static CRUST_INLINE Crust_Simd128 crustSimd128LoadAlignedU64(const u64 *ptr)
{
	CRUST_ASSERT(crustIsAlignedUSize((usize)ptr, 16));
	return wasm_v128_load(ptr);
}

static CRUST_INLINE Crust_Simd128 crustSimd128LoadAlignedF32(const f32 *ptr)
{
	CRUST_ASSERT(crustIsAlignedUSize((usize)ptr, 16));
	return wasm_v128_load(ptr);
}

static CRUST_INLINE Crust_Simd128 crustSimd128LoadAlignedF64(const f64 *ptr)
{
	CRUST_ASSERT(crustIsAlignedUSize((usize)ptr, 16));
	return wasm_v128_load(ptr);
}

static CRUST_INLINE void crustSimd128StoreU8(u8 *ptr, Crust_Simd128 v)
{
	wasm_v128_store(ptr, v);
}

static CRUST_INLINE void crustSimd128StoreU16(u16 *ptr, Crust_Simd128 v)
{
	wasm_v128_store(ptr, v);
}

static CRUST_INLINE void crustSimd128StoreU32(u32 *ptr, Crust_Simd128 v)
{
	wasm_v128_store(ptr, v);
}

static CRUST_INLINE void crustSimd128StoreU64(u64 *ptr, Crust_Simd128 v)
{
	wasm_v128_store(ptr, v);
}

static CRUST_INLINE void crustSimd128StoreF32(f32 *ptr, Crust_Simd128 v)
{
	wasm_v128_store(ptr, v);
}

static CRUST_INLINE void crustSimd128StoreF64(f64 *ptr, Crust_Simd128 v)
{
	wasm_v128_store(ptr, v);
}

static CRUST_INLINE void crustSimd128StoreAlignedU8(u8 *ptr, Crust_Simd128 v)
{
	CRUST_ASSERT(crustIsAlignedUSize((usize)ptr, 16));
	wasm_v128_store(ptr, v);
}

static CRUST_INLINE void crustSimd128StoreAlignedU16(u16 *ptr, Crust_Simd128 v)
{
	CRUST_ASSERT(crustIsAlignedUSize((usize)ptr, 16));
	wasm_v128_store(ptr, v);
}

static CRUST_INLINE void crustSimd128StoreAlignedU32(u32 *ptr, Crust_Simd128 v)
{
	CRUST_ASSERT(crustIsAlignedUSize((usize)ptr, 16));
	wasm_v128_store(ptr, v);
}

static CRUST_INLINE void crustSimd128StoreAlignedU64(u64 *ptr, Crust_Simd128 v)
{
	CRUST_ASSERT(crustIsAlignedUSize((usize)ptr, 16));
	wasm_v128_store(ptr, v);
}

static CRUST_INLINE void crustSimd128StoreAlignedF32(f32 *ptr, Crust_Simd128 v)
{
	CRUST_ASSERT(crustIsAlignedUSize((usize)ptr, 16));
	wasm_v128_store(ptr, v);
}

static CRUST_INLINE void crustSimd128StoreAlignedF64(f64 *ptr, Crust_Simd128 v)
{
	CRUST_ASSERT(crustIsAlignedUSize((usize)ptr, 16));
	wasm_v128_store(ptr, v);
}

//
static CRUST_INLINE Crust_Simd128 crustSimd128SplatU8(u8 v)
{
	return wasm_i8x16_splat((i8)v);
}

static CRUST_INLINE Crust_Simd128 crustSimd128SplatU16(u16 v)
{
	return wasm_i16x8_splat((i16)v);
}

static CRUST_INLINE Crust_Simd128 crustSimd128SplatU32(u32 v)
{
	return wasm_i32x4_splat((i32)v);
}

static CRUST_INLINE Crust_Simd128 crustSimd128SplatU64(u64 v)
{
	return wasm_i64x2_splat((i64)v);
}

static CRUST_INLINE Crust_Simd128 crustSimd128SplatF32(f32 v)
{
	return wasm_f32x4_splat(v);
}

static CRUST_INLINE Crust_Simd128 crustSimd128SplatF64(f64 v)
{
	return wasm_f64x2_splat(v);
}

static CRUST_INLINE Crust_Simd128 crustSimd128MakeU32(u32 v0, u32 v1, u32 v2, u32 v3)
{
	return wasm_i32x4_make((i32)v0, (i32)v1, (i32)v2, (i32)v3);
}

static CRUST_INLINE Crust_Simd128 crustSimd128MakeF32(f32 v0, f32 v1, f32 v2, f32 v3)
{
	return wasm_f32x4_make(v0, v1, v2, v3);
}

static CRUST_INLINE Crust_Simd128 crustSimd128MakeU64(u64 v0, u64 v1)
{
	return wasm_i64x2_make((i64)v0, (i64)v1);
}

static CRUST_INLINE Crust_Simd128 crustSimd128MakeF64(f64 v0, f64 v1)
{
	return wasm_f64x2_make(v0, v1);
}

//
static CRUST_INLINE Crust_Simd128 crustSimd128And(Crust_Simd128 a, Crust_Simd128 b)
{
	return wasm_v128_and(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128Or(Crust_Simd128 a, Crust_Simd128 b)
{
	return wasm_v128_or(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128Xor(Crust_Simd128 a, Crust_Simd128 b)
{
	return wasm_v128_xor(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128AndNot(Crust_Simd128 a, Crust_Simd128 b)
{
	return wasm_v128_andnot(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128Not(Crust_Simd128 v)
{
	return wasm_v128_not(v);
}

static CRUST_INLINE Crust_Simd128 crustSimd128Select(Crust_Simd128 mask, Crust_Simd128 a, Crust_Simd128 b)
{
	return wasm_v128_bitselect(a, b, mask);
}

//
static CRUST_INLINE u32 crustSimd128Any(Crust_Simd128 v)
{
	return (u32)wasm_v128_any_true(v);
}

static CRUST_INLINE u32 crustSimd128All(Crust_Simd128 v)
{
	return (u32)!wasm_v128_any_true(wasm_v128_not(v));
}

static CRUST_INLINE u32 crustSimd128BitmaskU8(Crust_Simd128 v)
{
	return (u32)wasm_i8x16_bitmask(v);
}

static CRUST_INLINE u32 crustSimd128BitmaskU16(Crust_Simd128 v)
{
	return (u32)wasm_i16x8_bitmask(v);
}

static CRUST_INLINE u32 crustSimd128BitmaskU32(Crust_Simd128 v)
{
	return (u32)wasm_i32x4_bitmask(v);
}

static CRUST_INLINE u32 crustSimd128BitmaskU64(Crust_Simd128 v)
{
	return (u32)wasm_i64x2_bitmask(v);
}

//
static CRUST_INLINE Crust_Simd128 crustSimd128AddU32(Crust_Simd128 a, Crust_Simd128 b)
{
	return wasm_i32x4_add(a, b);
}

//
static CRUST_INLINE Crust_Simd128 crustSimd128AddF32(Crust_Simd128 a, Crust_Simd128 b)
{
	return wasm_f32x4_add(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128SubF32(Crust_Simd128 a, Crust_Simd128 b)
{
	return wasm_f32x4_sub(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128MulF32(Crust_Simd128 a, Crust_Simd128 b)
{
	return wasm_f32x4_mul(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128DivF32(Crust_Simd128 a, Crust_Simd128 b)
{
	return wasm_f32x4_div(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128SqrtF32(Crust_Simd128 v)
{
	return wasm_f32x4_sqrt(v);
}

static CRUST_INLINE Crust_Simd128 crustSimd128AbsF32(Crust_Simd128 v)
{
	return wasm_f32x4_abs(v);
}

static CRUST_INLINE Crust_Simd128 crustSimd128NegateF32(Crust_Simd128 v)
{
	return wasm_f32x4_neg(v);
}

static CRUST_INLINE Crust_Simd128 crustSimd128MinF32(Crust_Simd128 a, Crust_Simd128 b)
{
	return wasm_f32x4_pmin(b, a);
}

static CRUST_INLINE Crust_Simd128 crustSimd128MaxF32(Crust_Simd128 a, Crust_Simd128 b)
{
	return wasm_f32x4_pmax(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128EqualF32(Crust_Simd128 a, Crust_Simd128 b)
{
	return wasm_f32x4_eq(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128NotEqualF32(Crust_Simd128 a, Crust_Simd128 b)
{
	return wasm_f32x4_ne(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128LessF32(Crust_Simd128 a, Crust_Simd128 b)
{
	return wasm_f32x4_lt(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128LessEqualF32(Crust_Simd128 a, Crust_Simd128 b)
{
	return wasm_f32x4_le(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128FloorF32(Crust_Simd128 v)
{
	return wasm_f32x4_floor(v);
}

static CRUST_INLINE Crust_Simd128 crustSimd128CeilF32(Crust_Simd128 v)
{
	return wasm_f32x4_ceil(v);
}

static CRUST_INLINE Crust_Simd128 crustSimd128RoundEvenF32(Crust_Simd128 v)
{
	return wasm_f32x4_nearest(v);
}

static CRUST_INLINE Crust_Simd128 crustSimd128TruncateF32(Crust_Simd128 v)
{
	return wasm_f32x4_trunc(v);
}

//
static CRUST_INLINE Crust_Simd128 crustSimd128AddF64(Crust_Simd128 a, Crust_Simd128 b)
{
	return wasm_f64x2_add(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128SubF64(Crust_Simd128 a, Crust_Simd128 b)
{
	return wasm_f64x2_sub(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128MulF64(Crust_Simd128 a, Crust_Simd128 b)
{
	return wasm_f64x2_mul(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128DivF64(Crust_Simd128 a, Crust_Simd128 b)
{
	return wasm_f64x2_div(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128SqrtF64(Crust_Simd128 v)
{
	return wasm_f64x2_sqrt(v);
}

static CRUST_INLINE Crust_Simd128 crustSimd128AbsF64(Crust_Simd128 v)
{
	return wasm_f64x2_abs(v);
}

static CRUST_INLINE Crust_Simd128 crustSimd128NegateF64(Crust_Simd128 v)
{
	return wasm_f64x2_neg(v);
}

static CRUST_INLINE Crust_Simd128 crustSimd128MinF64(Crust_Simd128 a, Crust_Simd128 b)
{
	return wasm_f64x2_pmin(b, a);
}

static CRUST_INLINE Crust_Simd128 crustSimd128MaxF64(Crust_Simd128 a, Crust_Simd128 b)
{
	return wasm_f64x2_pmax(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128EqualF64(Crust_Simd128 a, Crust_Simd128 b)
{
	return wasm_f64x2_eq(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128NotEqualF64(Crust_Simd128 a, Crust_Simd128 b)
{
	return wasm_f64x2_ne(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128LessF64(Crust_Simd128 a, Crust_Simd128 b)
{
	return wasm_f64x2_lt(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128LessEqualF64(Crust_Simd128 a, Crust_Simd128 b)
{
	return wasm_f64x2_le(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128FloorF64(Crust_Simd128 v)
{
	return wasm_f64x2_floor(v);
}

static CRUST_INLINE Crust_Simd128 crustSimd128CeilF64(Crust_Simd128 v)
{
	return wasm_f64x2_ceil(v);
}

static CRUST_INLINE Crust_Simd128 crustSimd128RoundEvenF64(Crust_Simd128 v)
{
	return wasm_f64x2_nearest(v);
}

static CRUST_INLINE Crust_Simd128 crustSimd128TruncateF64(Crust_Simd128 v)
{
	return wasm_f64x2_trunc(v);
}

//
static CRUST_INLINE Crust_Simd128 crustSimd128ConvertF32FromI32(Crust_Simd128 v)
{
	return wasm_f32x4_convert_i32x4(v);
}

static CRUST_INLINE Crust_Simd128 crustSimd128ConvertI32FromF32(Crust_Simd128 v)
{
	CRUST_ASSERT(crustSimd128All(crustSimd128LessF32(crustSimd128AbsF32(v), crustSimd128SplatF32(2147483648.0f))));
	return wasm_i32x4_trunc_sat_f32x4(v);
}

static CRUST_INLINE Crust_Simd128 crustSimd128ConvertI32FromF32Saturate(Crust_Simd128 v)
{
	return wasm_i32x4_trunc_sat_f32x4(v);
}

static CRUST_INLINE Crust_Simd128 crustSimd128ConvertF64FromF32Low(Crust_Simd128 v)
{
	return wasm_f64x2_promote_low_f32x4(v);
}

static CRUST_INLINE Crust_Simd128 crustSimd128ConvertF64FromF32High(Crust_Simd128 v)
{
	return wasm_f64x2_promote_low_f32x4(wasm_i32x4_shuffle(v, v, 2, 3, 2, 3));
}

static CRUST_INLINE Crust_Simd128 crustSimd128ConvertF32FromF64(Crust_Simd128 low, Crust_Simd128 high)
{
	return wasm_i32x4_shuffle(wasm_f32x4_demote_f64x2_zero(low), wasm_f32x4_demote_f64x2_zero(high), 0, 1, 4, 5);
}
