//
typedef struct Crust_Simd128_t
{
	CRUST_ALIGNAS(16) u8 bytes[16];
} Crust_Simd128;

//
static CRUST_INLINE Crust_Simd128 crustSimd128Zero(void)
{
	Crust_Simd128 result;
	for (u32 i = 0; i < 16; ++i)
		result.bytes[i] = 0;

	return result;
}

static CRUST_INLINE Crust_Simd128 crustSimd128LoadU(const void *ptr)
{
	const u8 *src = (const u8 *)ptr;

	Crust_Simd128 result;
	for (u32 i = 0; i < 16; ++i)
		result.bytes[i] = src[i];

	return result;
}

static CRUST_INLINE void crustSimd128StoreU(void *ptr, Crust_Simd128 v)
{
	u8 *dst = (u8 *)ptr;

	for (u32 i = 0; i < 16; ++i)
		dst[i] = v.bytes[i];
}

//
static CRUST_INLINE Crust_Simd128 crustSimd128SetU32(u32 v0, u32 v1, u32 v2, u32 v3)
{
	u32 lanes[4] = { v0, v1, v2, v3 };
	return crustSimd128LoadU(lanes);
}

static CRUST_INLINE Crust_Simd128 crustSimd128SetF32(f32 v0, f32 v1, f32 v2, f32 v3)
{
	f32 lanes[4] = { v0, v1, v2, v3 };
	return crustSimd128LoadU(lanes);
}

static CRUST_INLINE Crust_Simd128 crustSimd128SplatU32(u32 v)
{
	return crustSimd128SetU32(v, v, v, v);
}

static CRUST_INLINE Crust_Simd128 crustSimd128SplatF32(f32 v)
{
	return crustSimd128SetF32(v, v, v, v);
}

//
static CRUST_INLINE Crust_Simd128 crustSimd128Select(Crust_Simd128 mask, Crust_Simd128 a, Crust_Simd128 b)
{
	Crust_Simd128 result;
	for (u32 i = 0; i < 16; ++i)
		result.bytes[i] = (u8)((mask.bytes[i] & a.bytes[i]) | (~mask.bytes[i] & b.bytes[i]));

	return result;
}

//
static CRUST_INLINE Crust_Simd128 crustSimd128AddU32(Crust_Simd128 a, Crust_Simd128 b)
{
	u32 x[4];
	u32 y[4];
	crustSimd128StoreU(x, a);
	crustSimd128StoreU(y, b);

	for (u32 i = 0; i < 4; ++i)
		x[i] += y[i];

	return crustSimd128LoadU(x);
}

//
static CRUST_INLINE Crust_Simd128 crustSimd128AddF32(Crust_Simd128 a, Crust_Simd128 b)
{
	f32 x[4];
	f32 y[4];
	crustSimd128StoreU(x, a);
	crustSimd128StoreU(y, b);

	for (u32 i = 0; i < 4; ++i)
		x[i] += y[i];

	return crustSimd128LoadU(x);
}

static CRUST_INLINE Crust_Simd128 crustSimd128MulF32(Crust_Simd128 a, Crust_Simd128 b)
{
	f32 x[4];
	f32 y[4];
	crustSimd128StoreU(x, a);
	crustSimd128StoreU(y, b);

	for (u32 i = 0; i < 4; ++i)
		x[i] *= y[i];

	return crustSimd128LoadU(x);
}

static CRUST_INLINE Crust_Simd128 crustSimd128LtF32(Crust_Simd128 a, Crust_Simd128 b)
{
	f32 x[4];
	f32 y[4];
	crustSimd128StoreU(x, a);
	crustSimd128StoreU(y, b);

	u32 result[4];
	for (u32 i = 0; i < 4; ++i)
		result[i] = (x[i] < y[i]) ? CRUST_U32_MAX : 0;

	return crustSimd128LoadU(result);
}

static CRUST_INLINE Crust_Simd128 crustSimd128MinF32(Crust_Simd128 a, Crust_Simd128 b)
{
	f32 x[4];
	f32 y[4];
	crustSimd128StoreU(x, a);
	crustSimd128StoreU(y, b);

	for (u32 i = 0; i < 4; ++i)
		x[i] = crustMinF32(x[i], y[i]);

	return crustSimd128LoadU(x);
}

static CRUST_INLINE Crust_Simd128 crustSimd128MaxF32(Crust_Simd128 a, Crust_Simd128 b)
{
	f32 x[4];
	f32 y[4];
	crustSimd128StoreU(x, a);
	crustSimd128StoreU(y, b);

	for (u32 i = 0; i < 4; ++i)
		x[i] = crustMaxF32(x[i], y[i]);

	return crustSimd128LoadU(x);
}
