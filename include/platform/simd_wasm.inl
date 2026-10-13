//
typedef v128_t Crust_Simd128;

//
static CRUST_INLINE Crust_Simd128 crustSimd128Zero(void)
{
	return wasm_i32x4_splat(0);
}

static CRUST_INLINE Crust_Simd128 crustSimd128LoadU(const void *ptr)
{
	return wasm_v128_load(ptr);
}

static CRUST_INLINE void crustSimd128StoreU(void *ptr, Crust_Simd128 v)
{
	wasm_v128_store(ptr, v);
}

//
static CRUST_INLINE Crust_Simd128 crustSimd128SplatU32(u32 v)
{
	return wasm_i32x4_splat((i32)v);
}

static CRUST_INLINE Crust_Simd128 crustSimd128SplatF32(f32 v)
{
	return wasm_f32x4_splat(v);
}

static CRUST_INLINE Crust_Simd128 crustSimd128SetU32(u32 v0, u32 v1, u32 v2, u32 v3)
{
	return wasm_i32x4_make((i32)v0, (i32)v1, (i32)v2, (i32)v3);
}

static CRUST_INLINE Crust_Simd128 crustSimd128SetF32(f32 v0, f32 v1, f32 v2, f32 v3)
{
	return wasm_f32x4_make(v0, v1, v2, v3);
}

//
static CRUST_INLINE Crust_Simd128 crustSimd128Select(Crust_Simd128 mask, Crust_Simd128 a, Crust_Simd128 b)
{
	return wasm_v128_bitselect(a, b, mask);
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

static CRUST_INLINE Crust_Simd128 crustSimd128MulF32(Crust_Simd128 a, Crust_Simd128 b)
{
	return wasm_f32x4_mul(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128LtF32(Crust_Simd128 a, Crust_Simd128 b)
{
	return wasm_f32x4_lt(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128MinF32(Crust_Simd128 a, Crust_Simd128 b)
{
	return wasm_f32x4_pmin(b, a);
}

static CRUST_INLINE Crust_Simd128 crustSimd128MaxF32(Crust_Simd128 a, Crust_Simd128 b)
{
	return wasm_f32x4_pmax(a, b);
}
