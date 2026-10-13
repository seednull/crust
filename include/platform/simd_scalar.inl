//
static CRUST_INLINE Crust_Simd128 crustSimd128AllZeros(void)
{
	Crust_Simd128 result;
	for (u32 i = 0; i < 16; ++i)
		result.bytes[i] = 0;

	return result;
}

static CRUST_INLINE Crust_Simd128 crustSimd128AllOnes(void)
{
	Crust_Simd128 result;
	for (u32 i = 0; i < 16; ++i)
		result.bytes[i] = CRUST_U8_MAX;

	return result;
}

//
static CRUST_INLINE Crust_Simd128 crustSimd128LoadU8(const u8 *ptr)
{
	Crust_Simd128 result;
	for (u32 i = 0; i < 16; ++i)
		result.bytes[i] = ptr[i];

	return result;
}

static CRUST_INLINE Crust_Simd128 crustSimd128LoadU16(const u16 *ptr)
{
	return crustSimd128LoadU8((const u8 *)ptr);
}

static CRUST_INLINE Crust_Simd128 crustSimd128LoadU32(const u32 *ptr)
{
	return crustSimd128LoadU8((const u8 *)ptr);
}

static CRUST_INLINE Crust_Simd128 crustSimd128LoadU64(const u64 *ptr)
{
	return crustSimd128LoadU8((const u8 *)ptr);
}

static CRUST_INLINE Crust_Simd128 crustSimd128LoadF32(const f32 *ptr)
{
	return crustSimd128LoadU8((const u8 *)ptr);
}

static CRUST_INLINE Crust_Simd128 crustSimd128LoadF64(const f64 *ptr)
{
	return crustSimd128LoadU8((const u8 *)ptr);
}

static CRUST_INLINE Crust_Simd128 crustSimd128LoadAlignedU8(const u8 *ptr)
{
	CRUST_ASSERT(crustIsAlignedUSize((usize)ptr, 16));
	return crustSimd128LoadU8(ptr);
}

static CRUST_INLINE Crust_Simd128 crustSimd128LoadAlignedU16(const u16 *ptr)
{
	CRUST_ASSERT(crustIsAlignedUSize((usize)ptr, 16));
	return crustSimd128LoadU8((const u8 *)ptr);
}

static CRUST_INLINE Crust_Simd128 crustSimd128LoadAlignedU32(const u32 *ptr)
{
	CRUST_ASSERT(crustIsAlignedUSize((usize)ptr, 16));
	return crustSimd128LoadU8((const u8 *)ptr);
}

static CRUST_INLINE Crust_Simd128 crustSimd128LoadAlignedU64(const u64 *ptr)
{
	CRUST_ASSERT(crustIsAlignedUSize((usize)ptr, 16));
	return crustSimd128LoadU8((const u8 *)ptr);
}

static CRUST_INLINE Crust_Simd128 crustSimd128LoadAlignedF32(const f32 *ptr)
{
	CRUST_ASSERT(crustIsAlignedUSize((usize)ptr, 16));
	return crustSimd128LoadU8((const u8 *)ptr);
}

static CRUST_INLINE Crust_Simd128 crustSimd128LoadAlignedF64(const f64 *ptr)
{
	CRUST_ASSERT(crustIsAlignedUSize((usize)ptr, 16));
	return crustSimd128LoadU8((const u8 *)ptr);
}

static CRUST_INLINE void crustSimd128StoreU8(u8 *ptr, Crust_Simd128 v)
{
	for (u32 i = 0; i < 16; ++i)
		ptr[i] = v.bytes[i];
}

static CRUST_INLINE void crustSimd128StoreU16(u16 *ptr, Crust_Simd128 v)
{
	crustSimd128StoreU8((u8 *)ptr, v);
}

static CRUST_INLINE void crustSimd128StoreU32(u32 *ptr, Crust_Simd128 v)
{
	crustSimd128StoreU8((u8 *)ptr, v);
}

static CRUST_INLINE void crustSimd128StoreU64(u64 *ptr, Crust_Simd128 v)
{
	crustSimd128StoreU8((u8 *)ptr, v);
}

static CRUST_INLINE void crustSimd128StoreF32(f32 *ptr, Crust_Simd128 v)
{
	crustSimd128StoreU8((u8 *)ptr, v);
}

static CRUST_INLINE void crustSimd128StoreF64(f64 *ptr, Crust_Simd128 v)
{
	crustSimd128StoreU8((u8 *)ptr, v);
}

static CRUST_INLINE void crustSimd128StoreAlignedU8(u8 *ptr, Crust_Simd128 v)
{
	CRUST_ASSERT(crustIsAlignedUSize((usize)ptr, 16));
	crustSimd128StoreU8(ptr, v);
}

static CRUST_INLINE void crustSimd128StoreAlignedU16(u16 *ptr, Crust_Simd128 v)
{
	CRUST_ASSERT(crustIsAlignedUSize((usize)ptr, 16));
	crustSimd128StoreU8((u8 *)ptr, v);
}

static CRUST_INLINE void crustSimd128StoreAlignedU32(u32 *ptr, Crust_Simd128 v)
{
	CRUST_ASSERT(crustIsAlignedUSize((usize)ptr, 16));
	crustSimd128StoreU8((u8 *)ptr, v);
}

static CRUST_INLINE void crustSimd128StoreAlignedU64(u64 *ptr, Crust_Simd128 v)
{
	CRUST_ASSERT(crustIsAlignedUSize((usize)ptr, 16));
	crustSimd128StoreU8((u8 *)ptr, v);
}

static CRUST_INLINE void crustSimd128StoreAlignedF32(f32 *ptr, Crust_Simd128 v)
{
	CRUST_ASSERT(crustIsAlignedUSize((usize)ptr, 16));
	crustSimd128StoreU8((u8 *)ptr, v);
}

static CRUST_INLINE void crustSimd128StoreAlignedF64(f64 *ptr, Crust_Simd128 v)
{
	CRUST_ASSERT(crustIsAlignedUSize((usize)ptr, 16));
	crustSimd128StoreU8((u8 *)ptr, v);
}

//
static CRUST_INLINE Crust_Simd128 crustSimd128SplatU8(u8 v)
{
	Crust_Simd128 result;
	for (u32 i = 0; i < 16; ++i)
		result.bytes[i] = v;

	return result;
}

static CRUST_INLINE Crust_Simd128 crustSimd128SplatU16(u16 v)
{
	u16 lanes[8];
	for (u32 i = 0; i < 8; ++i)
		lanes[i] = v;

	return crustSimd128LoadU16(lanes);
}

static CRUST_INLINE Crust_Simd128 crustSimd128SplatU32(u32 v)
{
	u32 lanes[4] = { v, v, v, v };
	return crustSimd128LoadU32(lanes);
}

static CRUST_INLINE Crust_Simd128 crustSimd128SplatU64(u64 v)
{
	u64 lanes[2] = { v, v };
	return crustSimd128LoadU64(lanes);
}

static CRUST_INLINE Crust_Simd128 crustSimd128SplatF32(f32 v)
{
	f32 lanes[4] = { v, v, v, v };
	return crustSimd128LoadF32(lanes);
}

static CRUST_INLINE Crust_Simd128 crustSimd128SplatF64(f64 v)
{
	f64 lanes[2] = { v, v };
	return crustSimd128LoadF64(lanes);
}

static CRUST_INLINE Crust_Simd128 crustSimd128MakeU32(u32 v0, u32 v1, u32 v2, u32 v3)
{
	u32 lanes[4] = { v0, v1, v2, v3 };
	return crustSimd128LoadU32(lanes);
}

static CRUST_INLINE Crust_Simd128 crustSimd128MakeF32(f32 v0, f32 v1, f32 v2, f32 v3)
{
	f32 lanes[4] = { v0, v1, v2, v3 };
	return crustSimd128LoadF32(lanes);
}

static CRUST_INLINE Crust_Simd128 crustSimd128MakeU64(u64 v0, u64 v1)
{
	u64 lanes[2] = { v0, v1 };
	return crustSimd128LoadU64(lanes);
}

static CRUST_INLINE Crust_Simd128 crustSimd128MakeF64(f64 v0, f64 v1)
{
	f64 lanes[2] = { v0, v1 };
	return crustSimd128LoadF64(lanes);
}

//
static CRUST_INLINE Crust_Simd128 crustSimd128And(Crust_Simd128 a, Crust_Simd128 b)
{
	Crust_Simd128 result;
	for (u32 i = 0; i < 16; ++i)
		result.bytes[i] = (u8)(a.bytes[i] & b.bytes[i]);

	return result;
}

static CRUST_INLINE Crust_Simd128 crustSimd128Or(Crust_Simd128 a, Crust_Simd128 b)
{
	Crust_Simd128 result;
	for (u32 i = 0; i < 16; ++i)
		result.bytes[i] = (u8)(a.bytes[i] | b.bytes[i]);

	return result;
}

static CRUST_INLINE Crust_Simd128 crustSimd128Xor(Crust_Simd128 a, Crust_Simd128 b)
{
	Crust_Simd128 result;
	for (u32 i = 0; i < 16; ++i)
		result.bytes[i] = (u8)(a.bytes[i] ^ b.bytes[i]);

	return result;
}

static CRUST_INLINE Crust_Simd128 crustSimd128AndNot(Crust_Simd128 a, Crust_Simd128 b)
{
	Crust_Simd128 result;
	for (u32 i = 0; i < 16; ++i)
		result.bytes[i] = (u8)(a.bytes[i] & ~b.bytes[i]);

	return result;
}

static CRUST_INLINE Crust_Simd128 crustSimd128Not(Crust_Simd128 v)
{
	Crust_Simd128 result;
	for (u32 i = 0; i < 16; ++i)
		result.bytes[i] = (u8)~v.bytes[i];

	return result;
}

static CRUST_INLINE Crust_Simd128 crustSimd128Select(Crust_Simd128 mask, Crust_Simd128 a, Crust_Simd128 b)
{
	Crust_Simd128 result;
	for (u32 i = 0; i < 16; ++i)
		result.bytes[i] = (u8)((mask.bytes[i] & a.bytes[i]) | (~mask.bytes[i] & b.bytes[i]));

	return result;
}

//
static CRUST_INLINE u32 crustSimd128Any(Crust_Simd128 v)
{
	for (u32 i = 0; i < 16; ++i)
	{
		if (v.bytes[i] != 0)
			return 1;
	}

	return 0;
}

static CRUST_INLINE u32 crustSimd128All(Crust_Simd128 v)
{
	for (u32 i = 0; i < 16; ++i)
	{
		if (v.bytes[i] != CRUST_U8_MAX)
			return 0;
	}

	return 1;
}

static CRUST_INLINE u32 crustSimd128BitmaskU8(Crust_Simd128 v)
{
	u32 result = 0;
	for (u32 i = 0; i < 16; ++i)
		result |= (u32)(v.bytes[i] >> 7) << i;

	return result;
}

static CRUST_INLINE u32 crustSimd128BitmaskU16(Crust_Simd128 v)
{
	u16 lanes[8];
	crustSimd128StoreU16(lanes, v);

	u32 result = 0;
	for (u32 i = 0; i < 8; ++i)
		result |= (u32)(lanes[i] >> 15) << i;

	return result;
}

static CRUST_INLINE u32 crustSimd128BitmaskU32(Crust_Simd128 v)
{
	u32 lanes[4];
	crustSimd128StoreU32(lanes, v);

	u32 result = 0;
	for (u32 i = 0; i < 4; ++i)
		result |= (lanes[i] >> 31) << i;

	return result;
}

static CRUST_INLINE u32 crustSimd128BitmaskU64(Crust_Simd128 v)
{
	u64 lanes[2];
	crustSimd128StoreU64(lanes, v);

	u32 result = 0;
	for (u32 i = 0; i < 2; ++i)
		result |= (u32)(lanes[i] >> 63) << i;

	return result;
}

//
static CRUST_INLINE Crust_Simd128 crustSimd128AddU32(Crust_Simd128 a, Crust_Simd128 b)
{
	u32 x[4];
	u32 y[4];
	crustSimd128StoreU32(x, a);
	crustSimd128StoreU32(y, b);

	for (u32 i = 0; i < 4; ++i)
		x[i] += y[i];

	return crustSimd128LoadU32(x);
}

//
static CRUST_INLINE Crust_Simd128 crustSimd128AddF32(Crust_Simd128 a, Crust_Simd128 b)
{
	f32 x[4];
	f32 y[4];
	crustSimd128StoreF32(x, a);
	crustSimd128StoreF32(y, b);

	for (u32 i = 0; i < 4; ++i)
		x[i] += y[i];

	return crustSimd128LoadF32(x);
}

static CRUST_INLINE Crust_Simd128 crustSimd128SubF32(Crust_Simd128 a, Crust_Simd128 b)
{
	f32 x[4];
	f32 y[4];
	crustSimd128StoreF32(x, a);
	crustSimd128StoreF32(y, b);

	for (u32 i = 0; i < 4; ++i)
		x[i] -= y[i];

	return crustSimd128LoadF32(x);
}

static CRUST_INLINE Crust_Simd128 crustSimd128MulF32(Crust_Simd128 a, Crust_Simd128 b)
{
	f32 x[4];
	f32 y[4];
	crustSimd128StoreF32(x, a);
	crustSimd128StoreF32(y, b);

	for (u32 i = 0; i < 4; ++i)
		x[i] *= y[i];

	return crustSimd128LoadF32(x);
}

static CRUST_INLINE Crust_Simd128 crustSimd128DivF32(Crust_Simd128 a, Crust_Simd128 b)
{
	f32 x[4];
	f32 y[4];
	crustSimd128StoreF32(x, a);
	crustSimd128StoreF32(y, b);

	for (u32 i = 0; i < 4; ++i)
		x[i] /= y[i];

	return crustSimd128LoadF32(x);
}

static CRUST_INLINE Crust_Simd128 crustSimd128SqrtF32(Crust_Simd128 v)
{
	f32 x[4];
	crustSimd128StoreF32(x, v);

	for (u32 i = 0; i < 4; ++i)
		x[i] = crustSqrtF32(x[i]);

	return crustSimd128LoadF32(x);
}

static CRUST_INLINE Crust_Simd128 crustSimd128AbsF32(Crust_Simd128 v)
{
	f32 x[4];
	crustSimd128StoreF32(x, v);

	for (u32 i = 0; i < 4; ++i)
		x[i] = crustAbsF32(x[i]);

	return crustSimd128LoadF32(x);
}

static CRUST_INLINE Crust_Simd128 crustSimd128NegateF32(Crust_Simd128 v)
{
	f32 x[4];
	crustSimd128StoreF32(x, v);

	for (u32 i = 0; i < 4; ++i)
		x[i] = -x[i];

	return crustSimd128LoadF32(x);
}

static CRUST_INLINE Crust_Simd128 crustSimd128MinF32(Crust_Simd128 a, Crust_Simd128 b)
{
	f32 x[4];
	f32 y[4];
	crustSimd128StoreF32(x, a);
	crustSimd128StoreF32(y, b);

	for (u32 i = 0; i < 4; ++i)
		x[i] = crustMinF32(x[i], y[i]);

	return crustSimd128LoadF32(x);
}

static CRUST_INLINE Crust_Simd128 crustSimd128MaxF32(Crust_Simd128 a, Crust_Simd128 b)
{
	f32 x[4];
	f32 y[4];
	crustSimd128StoreF32(x, a);
	crustSimd128StoreF32(y, b);

	for (u32 i = 0; i < 4; ++i)
		x[i] = crustMaxF32(x[i], y[i]);

	return crustSimd128LoadF32(x);
}

static CRUST_INLINE Crust_Simd128 crustSimd128EqualF32(Crust_Simd128 a, Crust_Simd128 b)
{
	f32 x[4];
	f32 y[4];
	crustSimd128StoreF32(x, a);
	crustSimd128StoreF32(y, b);

	u32 result[4];
	for (u32 i = 0; i < 4; ++i)
		result[i] = (x[i] == y[i]) ? CRUST_U32_MAX : 0;

	return crustSimd128LoadU32(result);
}

static CRUST_INLINE Crust_Simd128 crustSimd128NotEqualF32(Crust_Simd128 a, Crust_Simd128 b)
{
	f32 x[4];
	f32 y[4];
	crustSimd128StoreF32(x, a);
	crustSimd128StoreF32(y, b);

	u32 result[4];
	for (u32 i = 0; i < 4; ++i)
		result[i] = (x[i] != y[i]) ? CRUST_U32_MAX : 0;

	return crustSimd128LoadU32(result);
}

static CRUST_INLINE Crust_Simd128 crustSimd128LessF32(Crust_Simd128 a, Crust_Simd128 b)
{
	f32 x[4];
	f32 y[4];
	crustSimd128StoreF32(x, a);
	crustSimd128StoreF32(y, b);

	u32 result[4];
	for (u32 i = 0; i < 4; ++i)
		result[i] = (x[i] < y[i]) ? CRUST_U32_MAX : 0;

	return crustSimd128LoadU32(result);
}

static CRUST_INLINE Crust_Simd128 crustSimd128LessEqualF32(Crust_Simd128 a, Crust_Simd128 b)
{
	f32 x[4];
	f32 y[4];
	crustSimd128StoreF32(x, a);
	crustSimd128StoreF32(y, b);

	u32 result[4];
	for (u32 i = 0; i < 4; ++i)
		result[i] = (x[i] <= y[i]) ? CRUST_U32_MAX : 0;

	return crustSimd128LoadU32(result);
}

static CRUST_INLINE Crust_Simd128 crustSimd128FloorF32(Crust_Simd128 v)
{
	f32 x[4];
	crustSimd128StoreF32(x, v);

	for (u32 i = 0; i < 4; ++i)
		x[i] = crustFloorF32(x[i]);

	return crustSimd128LoadF32(x);
}

static CRUST_INLINE Crust_Simd128 crustSimd128CeilF32(Crust_Simd128 v)
{
	f32 x[4];
	crustSimd128StoreF32(x, v);

	for (u32 i = 0; i < 4; ++i)
		x[i] = crustCeilF32(x[i]);

	return crustSimd128LoadF32(x);
}

static CRUST_INLINE Crust_Simd128 crustSimd128RoundEvenF32(Crust_Simd128 v)
{
	f32 x[4];
	crustSimd128StoreF32(x, v);

	for (u32 i = 0; i < 4; ++i)
		x[i] = crustRoundEvenF32(x[i]);

	return crustSimd128LoadF32(x);
}

static CRUST_INLINE Crust_Simd128 crustSimd128TruncateF32(Crust_Simd128 v)
{
	f32 x[4];
	crustSimd128StoreF32(x, v);

	for (u32 i = 0; i < 4; ++i)
		x[i] = crustTruncateF32(x[i]);

	return crustSimd128LoadF32(x);
}

//
static CRUST_INLINE Crust_Simd128 crustSimd128AddF64(Crust_Simd128 a, Crust_Simd128 b)
{
	f64 x[2];
	f64 y[2];
	crustSimd128StoreF64(x, a);
	crustSimd128StoreF64(y, b);

	for (u32 i = 0; i < 2; ++i)
		x[i] += y[i];

	return crustSimd128LoadF64(x);
}

static CRUST_INLINE Crust_Simd128 crustSimd128SubF64(Crust_Simd128 a, Crust_Simd128 b)
{
	f64 x[2];
	f64 y[2];
	crustSimd128StoreF64(x, a);
	crustSimd128StoreF64(y, b);

	for (u32 i = 0; i < 2; ++i)
		x[i] -= y[i];

	return crustSimd128LoadF64(x);
}

static CRUST_INLINE Crust_Simd128 crustSimd128MulF64(Crust_Simd128 a, Crust_Simd128 b)
{
	f64 x[2];
	f64 y[2];
	crustSimd128StoreF64(x, a);
	crustSimd128StoreF64(y, b);

	for (u32 i = 0; i < 2; ++i)
		x[i] *= y[i];

	return crustSimd128LoadF64(x);
}

static CRUST_INLINE Crust_Simd128 crustSimd128DivF64(Crust_Simd128 a, Crust_Simd128 b)
{
	f64 x[2];
	f64 y[2];
	crustSimd128StoreF64(x, a);
	crustSimd128StoreF64(y, b);

	for (u32 i = 0; i < 2; ++i)
		x[i] /= y[i];

	return crustSimd128LoadF64(x);
}

static CRUST_INLINE Crust_Simd128 crustSimd128SqrtF64(Crust_Simd128 v)
{
	f64 x[2];
	crustSimd128StoreF64(x, v);

	for (u32 i = 0; i < 2; ++i)
		x[i] = crustSqrtF64(x[i]);

	return crustSimd128LoadF64(x);
}

static CRUST_INLINE Crust_Simd128 crustSimd128AbsF64(Crust_Simd128 v)
{
	f64 x[2];
	crustSimd128StoreF64(x, v);

	for (u32 i = 0; i < 2; ++i)
		x[i] = crustAbsF64(x[i]);

	return crustSimd128LoadF64(x);
}

static CRUST_INLINE Crust_Simd128 crustSimd128NegateF64(Crust_Simd128 v)
{
	f64 x[2];
	crustSimd128StoreF64(x, v);

	for (u32 i = 0; i < 2; ++i)
		x[i] = -x[i];

	return crustSimd128LoadF64(x);
}

static CRUST_INLINE Crust_Simd128 crustSimd128MinF64(Crust_Simd128 a, Crust_Simd128 b)
{
	f64 x[2];
	f64 y[2];
	crustSimd128StoreF64(x, a);
	crustSimd128StoreF64(y, b);

	for (u32 i = 0; i < 2; ++i)
		x[i] = crustMinF64(x[i], y[i]);

	return crustSimd128LoadF64(x);
}

static CRUST_INLINE Crust_Simd128 crustSimd128MaxF64(Crust_Simd128 a, Crust_Simd128 b)
{
	f64 x[2];
	f64 y[2];
	crustSimd128StoreF64(x, a);
	crustSimd128StoreF64(y, b);

	for (u32 i = 0; i < 2; ++i)
		x[i] = crustMaxF64(x[i], y[i]);

	return crustSimd128LoadF64(x);
}

static CRUST_INLINE Crust_Simd128 crustSimd128EqualF64(Crust_Simd128 a, Crust_Simd128 b)
{
	f64 x[2];
	f64 y[2];
	crustSimd128StoreF64(x, a);
	crustSimd128StoreF64(y, b);

	u64 result[2];
	for (u32 i = 0; i < 2; ++i)
		result[i] = (x[i] == y[i]) ? CRUST_U64_MAX : 0;

	return crustSimd128LoadU64(result);
}

static CRUST_INLINE Crust_Simd128 crustSimd128NotEqualF64(Crust_Simd128 a, Crust_Simd128 b)
{
	f64 x[2];
	f64 y[2];
	crustSimd128StoreF64(x, a);
	crustSimd128StoreF64(y, b);

	u64 result[2];
	for (u32 i = 0; i < 2; ++i)
		result[i] = (x[i] != y[i]) ? CRUST_U64_MAX : 0;

	return crustSimd128LoadU64(result);
}

static CRUST_INLINE Crust_Simd128 crustSimd128LessF64(Crust_Simd128 a, Crust_Simd128 b)
{
	f64 x[2];
	f64 y[2];
	crustSimd128StoreF64(x, a);
	crustSimd128StoreF64(y, b);

	u64 result[2];
	for (u32 i = 0; i < 2; ++i)
		result[i] = (x[i] < y[i]) ? CRUST_U64_MAX : 0;

	return crustSimd128LoadU64(result);
}

static CRUST_INLINE Crust_Simd128 crustSimd128LessEqualF64(Crust_Simd128 a, Crust_Simd128 b)
{
	f64 x[2];
	f64 y[2];
	crustSimd128StoreF64(x, a);
	crustSimd128StoreF64(y, b);

	u64 result[2];
	for (u32 i = 0; i < 2; ++i)
		result[i] = (x[i] <= y[i]) ? CRUST_U64_MAX : 0;

	return crustSimd128LoadU64(result);
}

static CRUST_INLINE Crust_Simd128 crustSimd128FloorF64(Crust_Simd128 v)
{
	f64 x[2];
	crustSimd128StoreF64(x, v);

	for (u32 i = 0; i < 2; ++i)
		x[i] = crustFloorF64(x[i]);

	return crustSimd128LoadF64(x);
}

static CRUST_INLINE Crust_Simd128 crustSimd128CeilF64(Crust_Simd128 v)
{
	f64 x[2];
	crustSimd128StoreF64(x, v);

	for (u32 i = 0; i < 2; ++i)
		x[i] = crustCeilF64(x[i]);

	return crustSimd128LoadF64(x);
}

static CRUST_INLINE Crust_Simd128 crustSimd128RoundEvenF64(Crust_Simd128 v)
{
	f64 x[2];
	crustSimd128StoreF64(x, v);

	for (u32 i = 0; i < 2; ++i)
		x[i] = crustRoundEvenF64(x[i]);

	return crustSimd128LoadF64(x);
}

static CRUST_INLINE Crust_Simd128 crustSimd128TruncateF64(Crust_Simd128 v)
{
	f64 x[2];
	crustSimd128StoreF64(x, v);

	for (u32 i = 0; i < 2; ++i)
		x[i] = crustTruncateF64(x[i]);

	return crustSimd128LoadF64(x);
}

//
static CRUST_INLINE Crust_Simd128 crustSimd128ConvertF32FromI32(Crust_Simd128 v)
{
	i32 x[4];
	crustSimd128StoreI32(x, v);

	f32 result[4];
	for (u32 i = 0; i < 4; ++i)
		result[i] = (f32)x[i];

	return crustSimd128LoadF32(result);
}

static CRUST_INLINE Crust_Simd128 crustSimd128ConvertI32FromF32(Crust_Simd128 v)
{
	f32 x[4];
	crustSimd128StoreF32(x, v);

	i32 result[4];
	for (u32 i = 0; i < 4; ++i)
	{
		CRUST_ASSERT(crustAbsF32(x[i]) < 2147483648.0f);
		result[i] = (i32)x[i];
	}

	return crustSimd128LoadI32(result);
}

static CRUST_INLINE Crust_Simd128 crustSimd128ConvertI32FromF32Saturate(Crust_Simd128 v)
{
	f32 x[4];
	crustSimd128StoreF32(x, v);

	i32 result[4];
	for (u32 i = 0; i < 4; ++i)
	{
		if (x[i] != x[i])
			result[i] = 0;
		else if (x[i] >= 2147483648.0f)
			result[i] = CRUST_I32_MAX;
		else if (x[i] <= -2147483648.0f)
			result[i] = CRUST_I32_MIN;
		else
			result[i] = (i32)x[i];
	}

	return crustSimd128LoadI32(result);
}

static CRUST_INLINE Crust_Simd128 crustSimd128ConvertF64FromF32Low(Crust_Simd128 v)
{
	f32 x[4];
	crustSimd128StoreF32(x, v);

	f64 result[2] = { (f64)x[0], (f64)x[1] };
	return crustSimd128LoadF64(result);
}

static CRUST_INLINE Crust_Simd128 crustSimd128ConvertF64FromF32High(Crust_Simd128 v)
{
	f32 x[4];
	crustSimd128StoreF32(x, v);

	f64 result[2] = { (f64)x[2], (f64)x[3] };
	return crustSimd128LoadF64(result);
}

static CRUST_INLINE Crust_Simd128 crustSimd128ConvertF32FromF64(Crust_Simd128 low, Crust_Simd128 high)
{
	f64 x[2];
	f64 y[2];
	crustSimd128StoreF64(x, low);
	crustSimd128StoreF64(y, high);

	f32 result[4] = { (f32)x[0], (f32)x[1], (f32)y[0], (f32)y[1] };
	return crustSimd128LoadF32(result);
}
