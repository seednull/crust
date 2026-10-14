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
static CRUST_INLINE Crust_Simd128 crustSimd128ZipLowU8(Crust_Simd128 a, Crust_Simd128 b)
{
	u8 x[16];
	u8 y[16];
	crustSimd128StoreU8(x, a);
	crustSimd128StoreU8(y, b);

	u8 result[16];
	for (u32 i = 0; i < 8; ++i)
	{
		result[2 * i] = x[i];
		result[2 * i + 1] = y[i];
	}

	return crustSimd128LoadU8(result);
}

static CRUST_INLINE Crust_Simd128 crustSimd128ZipHighU8(Crust_Simd128 a, Crust_Simd128 b)
{
	u8 x[16];
	u8 y[16];
	crustSimd128StoreU8(x, a);
	crustSimd128StoreU8(y, b);

	u8 result[16];
	for (u32 i = 0; i < 8; ++i)
	{
		result[2 * i] = x[i + 8];
		result[2 * i + 1] = y[i + 8];
	}

	return crustSimd128LoadU8(result);
}

static CRUST_INLINE Crust_Simd128 crustSimd128ZipLowU16(Crust_Simd128 a, Crust_Simd128 b)
{
	u16 x[8];
	u16 y[8];
	crustSimd128StoreU16(x, a);
	crustSimd128StoreU16(y, b);

	u16 result[8];
	for (u32 i = 0; i < 4; ++i)
	{
		result[2 * i] = x[i];
		result[2 * i + 1] = y[i];
	}

	return crustSimd128LoadU16(result);
}

static CRUST_INLINE Crust_Simd128 crustSimd128ZipHighU16(Crust_Simd128 a, Crust_Simd128 b)
{
	u16 x[8];
	u16 y[8];
	crustSimd128StoreU16(x, a);
	crustSimd128StoreU16(y, b);

	u16 result[8];
	for (u32 i = 0; i < 4; ++i)
	{
		result[2 * i] = x[i + 4];
		result[2 * i + 1] = y[i + 4];
	}

	return crustSimd128LoadU16(result);
}

static CRUST_INLINE Crust_Simd128 crustSimd128ZipLowU32(Crust_Simd128 a, Crust_Simd128 b)
{
	u32 x[4];
	u32 y[4];
	crustSimd128StoreU32(x, a);
	crustSimd128StoreU32(y, b);

	u32 result[4];
	for (u32 i = 0; i < 2; ++i)
	{
		result[2 * i] = x[i];
		result[2 * i + 1] = y[i];
	}

	return crustSimd128LoadU32(result);
}

static CRUST_INLINE Crust_Simd128 crustSimd128ZipHighU32(Crust_Simd128 a, Crust_Simd128 b)
{
	u32 x[4];
	u32 y[4];
	crustSimd128StoreU32(x, a);
	crustSimd128StoreU32(y, b);

	u32 result[4];
	for (u32 i = 0; i < 2; ++i)
	{
		result[2 * i] = x[i + 2];
		result[2 * i + 1] = y[i + 2];
	}

	return crustSimd128LoadU32(result);
}

static CRUST_INLINE Crust_Simd128 crustSimd128ZipLowU64(Crust_Simd128 a, Crust_Simd128 b)
{
	u64 x[2];
	u64 y[2];
	crustSimd128StoreU64(x, a);
	crustSimd128StoreU64(y, b);

	u64 result[2];
	for (u32 i = 0; i < 1; ++i)
	{
		result[2 * i] = x[i];
		result[2 * i + 1] = y[i];
	}

	return crustSimd128LoadU64(result);
}

static CRUST_INLINE Crust_Simd128 crustSimd128ZipHighU64(Crust_Simd128 a, Crust_Simd128 b)
{
	u64 x[2];
	u64 y[2];
	crustSimd128StoreU64(x, a);
	crustSimd128StoreU64(y, b);

	u64 result[2];
	for (u32 i = 0; i < 1; ++i)
	{
		result[2 * i] = x[i + 1];
		result[2 * i + 1] = y[i + 1];
	}

	return crustSimd128LoadU64(result);
}

//
static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32XYWZ(Crust_Simd128 v)
{
	u32 x[4];
	crustSimd128StoreU32(x, v);

	return crustSimd128MakeU32(x[0], x[1], x[3], x[2]);
}

static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32XZYW(Crust_Simd128 v)
{
	u32 x[4];
	crustSimd128StoreU32(x, v);

	return crustSimd128MakeU32(x[0], x[2], x[1], x[3]);
}

static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32XZWY(Crust_Simd128 v)
{
	u32 x[4];
	crustSimd128StoreU32(x, v);

	return crustSimd128MakeU32(x[0], x[2], x[3], x[1]);
}

static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32XWYZ(Crust_Simd128 v)
{
	u32 x[4];
	crustSimd128StoreU32(x, v);

	return crustSimd128MakeU32(x[0], x[3], x[1], x[2]);
}

static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32XWZY(Crust_Simd128 v)
{
	u32 x[4];
	crustSimd128StoreU32(x, v);

	return crustSimd128MakeU32(x[0], x[3], x[2], x[1]);
}

static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32YXZW(Crust_Simd128 v)
{
	u32 x[4];
	crustSimd128StoreU32(x, v);

	return crustSimd128MakeU32(x[1], x[0], x[2], x[3]);
}

static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32YXWZ(Crust_Simd128 v)
{
	u32 x[4];
	crustSimd128StoreU32(x, v);

	return crustSimd128MakeU32(x[1], x[0], x[3], x[2]);
}

static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32YZXW(Crust_Simd128 v)
{
	u32 x[4];
	crustSimd128StoreU32(x, v);

	return crustSimd128MakeU32(x[1], x[2], x[0], x[3]);
}

static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32YZWX(Crust_Simd128 v)
{
	u32 x[4];
	crustSimd128StoreU32(x, v);

	return crustSimd128MakeU32(x[1], x[2], x[3], x[0]);
}

static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32YWXZ(Crust_Simd128 v)
{
	u32 x[4];
	crustSimd128StoreU32(x, v);

	return crustSimd128MakeU32(x[1], x[3], x[0], x[2]);
}

static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32YWZX(Crust_Simd128 v)
{
	u32 x[4];
	crustSimd128StoreU32(x, v);

	return crustSimd128MakeU32(x[1], x[3], x[2], x[0]);
}

static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32ZXYW(Crust_Simd128 v)
{
	u32 x[4];
	crustSimd128StoreU32(x, v);

	return crustSimd128MakeU32(x[2], x[0], x[1], x[3]);
}

static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32ZXWY(Crust_Simd128 v)
{
	u32 x[4];
	crustSimd128StoreU32(x, v);

	return crustSimd128MakeU32(x[2], x[0], x[3], x[1]);
}

static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32ZYXW(Crust_Simd128 v)
{
	u32 x[4];
	crustSimd128StoreU32(x, v);

	return crustSimd128MakeU32(x[2], x[1], x[0], x[3]);
}

static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32ZYWX(Crust_Simd128 v)
{
	u32 x[4];
	crustSimd128StoreU32(x, v);

	return crustSimd128MakeU32(x[2], x[1], x[3], x[0]);
}

static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32ZWXY(Crust_Simd128 v)
{
	u32 x[4];
	crustSimd128StoreU32(x, v);

	return crustSimd128MakeU32(x[2], x[3], x[0], x[1]);
}

static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32ZWYX(Crust_Simd128 v)
{
	u32 x[4];
	crustSimd128StoreU32(x, v);

	return crustSimd128MakeU32(x[2], x[3], x[1], x[0]);
}

static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32WXYZ(Crust_Simd128 v)
{
	u32 x[4];
	crustSimd128StoreU32(x, v);

	return crustSimd128MakeU32(x[3], x[0], x[1], x[2]);
}

static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32WXZY(Crust_Simd128 v)
{
	u32 x[4];
	crustSimd128StoreU32(x, v);

	return crustSimd128MakeU32(x[3], x[0], x[2], x[1]);
}

static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32WYXZ(Crust_Simd128 v)
{
	u32 x[4];
	crustSimd128StoreU32(x, v);

	return crustSimd128MakeU32(x[3], x[1], x[0], x[2]);
}

static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32WYZX(Crust_Simd128 v)
{
	u32 x[4];
	crustSimd128StoreU32(x, v);

	return crustSimd128MakeU32(x[3], x[1], x[2], x[0]);
}

static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32WZXY(Crust_Simd128 v)
{
	u32 x[4];
	crustSimd128StoreU32(x, v);

	return crustSimd128MakeU32(x[3], x[2], x[0], x[1]);
}

static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32WZYX(Crust_Simd128 v)
{
	u32 x[4];
	crustSimd128StoreU32(x, v);

	return crustSimd128MakeU32(x[3], x[2], x[1], x[0]);
}

static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32XXXX(Crust_Simd128 v)
{
	u32 x[4];
	crustSimd128StoreU32(x, v);

	return crustSimd128MakeU32(x[0], x[0], x[0], x[0]);
}

static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32YYYY(Crust_Simd128 v)
{
	u32 x[4];
	crustSimd128StoreU32(x, v);

	return crustSimd128MakeU32(x[1], x[1], x[1], x[1]);
}

static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32ZZZZ(Crust_Simd128 v)
{
	u32 x[4];
	crustSimd128StoreU32(x, v);

	return crustSimd128MakeU32(x[2], x[2], x[2], x[2]);
}

static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32WWWW(Crust_Simd128 v)
{
	u32 x[4];
	crustSimd128StoreU32(x, v);

	return crustSimd128MakeU32(x[3], x[3], x[3], x[3]);
}

static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle64YX(Crust_Simd128 v)
{
	u64 x[2];
	crustSimd128StoreU64(x, v);

	return crustSimd128MakeU64(x[1], x[0]);
}

static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle64XX(Crust_Simd128 v)
{
	u64 x[2];
	crustSimd128StoreU64(x, v);

	return crustSimd128MakeU64(x[0], x[0]);
}

static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle64YY(Crust_Simd128 v)
{
	u64 x[2];
	crustSimd128StoreU64(x, v);

	return crustSimd128MakeU64(x[1], x[1]);
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
static CRUST_INLINE Crust_Simd128 crustSimd128AddU8(Crust_Simd128 a, Crust_Simd128 b)
{
	u8 x[16];
	u8 y[16];
	crustSimd128StoreU8(x, a);
	crustSimd128StoreU8(y, b);

	for (u32 i = 0; i < 16; ++i)
		x[i] = (u8)(x[i] + y[i]);

	return crustSimd128LoadU8(x);
}

static CRUST_INLINE Crust_Simd128 crustSimd128AddU16(Crust_Simd128 a, Crust_Simd128 b)
{
	u16 x[8];
	u16 y[8];
	crustSimd128StoreU16(x, a);
	crustSimd128StoreU16(y, b);

	for (u32 i = 0; i < 8; ++i)
		x[i] = (u16)(x[i] + y[i]);

	return crustSimd128LoadU16(x);
}

static CRUST_INLINE Crust_Simd128 crustSimd128AddU32(Crust_Simd128 a, Crust_Simd128 b)
{
	u32 x[4];
	u32 y[4];
	crustSimd128StoreU32(x, a);
	crustSimd128StoreU32(y, b);

	for (u32 i = 0; i < 4; ++i)
		x[i] = (u32)(x[i] + y[i]);

	return crustSimd128LoadU32(x);
}

static CRUST_INLINE Crust_Simd128 crustSimd128AddU64(Crust_Simd128 a, Crust_Simd128 b)
{
	u64 x[2];
	u64 y[2];
	crustSimd128StoreU64(x, a);
	crustSimd128StoreU64(y, b);

	for (u32 i = 0; i < 2; ++i)
		x[i] = (u64)(x[i] + y[i]);

	return crustSimd128LoadU64(x);
}

//
static CRUST_INLINE Crust_Simd128 crustSimd128SubU8(Crust_Simd128 a, Crust_Simd128 b)
{
	u8 x[16];
	u8 y[16];
	crustSimd128StoreU8(x, a);
	crustSimd128StoreU8(y, b);

	for (u32 i = 0; i < 16; ++i)
		x[i] = (u8)(x[i] - y[i]);

	return crustSimd128LoadU8(x);
}

static CRUST_INLINE Crust_Simd128 crustSimd128SubU16(Crust_Simd128 a, Crust_Simd128 b)
{
	u16 x[8];
	u16 y[8];
	crustSimd128StoreU16(x, a);
	crustSimd128StoreU16(y, b);

	for (u32 i = 0; i < 8; ++i)
		x[i] = (u16)(x[i] - y[i]);

	return crustSimd128LoadU16(x);
}

static CRUST_INLINE Crust_Simd128 crustSimd128SubU32(Crust_Simd128 a, Crust_Simd128 b)
{
	u32 x[4];
	u32 y[4];
	crustSimd128StoreU32(x, a);
	crustSimd128StoreU32(y, b);

	for (u32 i = 0; i < 4; ++i)
		x[i] = (u32)(x[i] - y[i]);

	return crustSimd128LoadU32(x);
}

static CRUST_INLINE Crust_Simd128 crustSimd128SubU64(Crust_Simd128 a, Crust_Simd128 b)
{
	u64 x[2];
	u64 y[2];
	crustSimd128StoreU64(x, a);
	crustSimd128StoreU64(y, b);

	for (u32 i = 0; i < 2; ++i)
		x[i] = (u64)(x[i] - y[i]);

	return crustSimd128LoadU64(x);
}

//
static CRUST_INLINE Crust_Simd128 crustSimd128AddSaturateU8(Crust_Simd128 a, Crust_Simd128 b)
{
	u8 x[16];
	u8 y[16];
	crustSimd128StoreU8(x, a);
	crustSimd128StoreU8(y, b);

	for (u32 i = 0; i < 16; ++i)
		x[i] = (u8)crustMinU32((u32)x[i] + y[i], CRUST_U8_MAX);

	return crustSimd128LoadU8(x);
}

static CRUST_INLINE Crust_Simd128 crustSimd128AddSaturateI8(Crust_Simd128 a, Crust_Simd128 b)
{
	i8 x[16];
	i8 y[16];
	crustSimd128StoreI8(x, a);
	crustSimd128StoreI8(y, b);

	for (u32 i = 0; i < 16; ++i)
		x[i] = (i8)crustClampI32((i32)x[i] + y[i], CRUST_I8_MIN, CRUST_I8_MAX);

	return crustSimd128LoadI8(x);
}

static CRUST_INLINE Crust_Simd128 crustSimd128AddSaturateU16(Crust_Simd128 a, Crust_Simd128 b)
{
	u16 x[8];
	u16 y[8];
	crustSimd128StoreU16(x, a);
	crustSimd128StoreU16(y, b);

	for (u32 i = 0; i < 8; ++i)
		x[i] = (u16)crustMinU32((u32)x[i] + y[i], CRUST_U16_MAX);

	return crustSimd128LoadU16(x);
}

static CRUST_INLINE Crust_Simd128 crustSimd128AddSaturateI16(Crust_Simd128 a, Crust_Simd128 b)
{
	i16 x[8];
	i16 y[8];
	crustSimd128StoreI16(x, a);
	crustSimd128StoreI16(y, b);

	for (u32 i = 0; i < 8; ++i)
		x[i] = (i16)crustClampI32((i32)x[i] + y[i], CRUST_I16_MIN, CRUST_I16_MAX);

	return crustSimd128LoadI16(x);
}

static CRUST_INLINE Crust_Simd128 crustSimd128SubSaturateU8(Crust_Simd128 a, Crust_Simd128 b)
{
	u8 x[16];
	u8 y[16];
	crustSimd128StoreU8(x, a);
	crustSimd128StoreU8(y, b);

	for (u32 i = 0; i < 16; ++i)
		x[i] = (x[i] > y[i]) ? (u8)(x[i] - y[i]) : 0;

	return crustSimd128LoadU8(x);
}

static CRUST_INLINE Crust_Simd128 crustSimd128SubSaturateI8(Crust_Simd128 a, Crust_Simd128 b)
{
	i8 x[16];
	i8 y[16];
	crustSimd128StoreI8(x, a);
	crustSimd128StoreI8(y, b);

	for (u32 i = 0; i < 16; ++i)
		x[i] = (i8)crustClampI32((i32)x[i] - y[i], CRUST_I8_MIN, CRUST_I8_MAX);

	return crustSimd128LoadI8(x);
}

static CRUST_INLINE Crust_Simd128 crustSimd128SubSaturateU16(Crust_Simd128 a, Crust_Simd128 b)
{
	u16 x[8];
	u16 y[8];
	crustSimd128StoreU16(x, a);
	crustSimd128StoreU16(y, b);

	for (u32 i = 0; i < 8; ++i)
		x[i] = (x[i] > y[i]) ? (u16)(x[i] - y[i]) : 0;

	return crustSimd128LoadU16(x);
}

static CRUST_INLINE Crust_Simd128 crustSimd128SubSaturateI16(Crust_Simd128 a, Crust_Simd128 b)
{
	i16 x[8];
	i16 y[8];
	crustSimd128StoreI16(x, a);
	crustSimd128StoreI16(y, b);

	for (u32 i = 0; i < 8; ++i)
		x[i] = (i16)crustClampI32((i32)x[i] - y[i], CRUST_I16_MIN, CRUST_I16_MAX);

	return crustSimd128LoadI16(x);
}

//
static CRUST_INLINE Crust_Simd128 crustSimd128MulU16(Crust_Simd128 a, Crust_Simd128 b)
{
	u16 x[8];
	u16 y[8];
	crustSimd128StoreU16(x, a);
	crustSimd128StoreU16(y, b);

	for (u32 i = 0; i < 8; ++i)
		x[i] = (u16)((u32)x[i] * y[i]);

	return crustSimd128LoadU16(x);
}

static CRUST_INLINE Crust_Simd128 crustSimd128MulU32(Crust_Simd128 a, Crust_Simd128 b)
{
	u32 x[4];
	u32 y[4];
	crustSimd128StoreU32(x, a);
	crustSimd128StoreU32(y, b);

	for (u32 i = 0; i < 4; ++i)
		x[i] = x[i] * y[i];

	return crustSimd128LoadU32(x);
}

//
static CRUST_INLINE Crust_Simd128 crustSimd128MinU8(Crust_Simd128 a, Crust_Simd128 b)
{
	u8 x[16];
	u8 y[16];
	crustSimd128StoreU8(x, a);
	crustSimd128StoreU8(y, b);

	for (u32 i = 0; i < 16; ++i)
		x[i] = crustMinU8(x[i], y[i]);

	return crustSimd128LoadU8(x);
}

static CRUST_INLINE Crust_Simd128 crustSimd128MinI8(Crust_Simd128 a, Crust_Simd128 b)
{
	i8 x[16];
	i8 y[16];
	crustSimd128StoreI8(x, a);
	crustSimd128StoreI8(y, b);

	for (u32 i = 0; i < 16; ++i)
		x[i] = crustMinI8(x[i], y[i]);

	return crustSimd128LoadI8(x);
}

static CRUST_INLINE Crust_Simd128 crustSimd128MinU16(Crust_Simd128 a, Crust_Simd128 b)
{
	u16 x[8];
	u16 y[8];
	crustSimd128StoreU16(x, a);
	crustSimd128StoreU16(y, b);

	for (u32 i = 0; i < 8; ++i)
		x[i] = crustMinU16(x[i], y[i]);

	return crustSimd128LoadU16(x);
}

static CRUST_INLINE Crust_Simd128 crustSimd128MinI16(Crust_Simd128 a, Crust_Simd128 b)
{
	i16 x[8];
	i16 y[8];
	crustSimd128StoreI16(x, a);
	crustSimd128StoreI16(y, b);

	for (u32 i = 0; i < 8; ++i)
		x[i] = crustMinI16(x[i], y[i]);

	return crustSimd128LoadI16(x);
}

static CRUST_INLINE Crust_Simd128 crustSimd128MinU32(Crust_Simd128 a, Crust_Simd128 b)
{
	u32 x[4];
	u32 y[4];
	crustSimd128StoreU32(x, a);
	crustSimd128StoreU32(y, b);

	for (u32 i = 0; i < 4; ++i)
		x[i] = crustMinU32(x[i], y[i]);

	return crustSimd128LoadU32(x);
}

static CRUST_INLINE Crust_Simd128 crustSimd128MinI32(Crust_Simd128 a, Crust_Simd128 b)
{
	i32 x[4];
	i32 y[4];
	crustSimd128StoreI32(x, a);
	crustSimd128StoreI32(y, b);

	for (u32 i = 0; i < 4; ++i)
		x[i] = crustMinI32(x[i], y[i]);

	return crustSimd128LoadI32(x);
}

static CRUST_INLINE Crust_Simd128 crustSimd128MaxU8(Crust_Simd128 a, Crust_Simd128 b)
{
	u8 x[16];
	u8 y[16];
	crustSimd128StoreU8(x, a);
	crustSimd128StoreU8(y, b);

	for (u32 i = 0; i < 16; ++i)
		x[i] = crustMaxU8(x[i], y[i]);

	return crustSimd128LoadU8(x);
}

static CRUST_INLINE Crust_Simd128 crustSimd128MaxI8(Crust_Simd128 a, Crust_Simd128 b)
{
	i8 x[16];
	i8 y[16];
	crustSimd128StoreI8(x, a);
	crustSimd128StoreI8(y, b);

	for (u32 i = 0; i < 16; ++i)
		x[i] = crustMaxI8(x[i], y[i]);

	return crustSimd128LoadI8(x);
}

static CRUST_INLINE Crust_Simd128 crustSimd128MaxU16(Crust_Simd128 a, Crust_Simd128 b)
{
	u16 x[8];
	u16 y[8];
	crustSimd128StoreU16(x, a);
	crustSimd128StoreU16(y, b);

	for (u32 i = 0; i < 8; ++i)
		x[i] = crustMaxU16(x[i], y[i]);

	return crustSimd128LoadU16(x);
}

static CRUST_INLINE Crust_Simd128 crustSimd128MaxI16(Crust_Simd128 a, Crust_Simd128 b)
{
	i16 x[8];
	i16 y[8];
	crustSimd128StoreI16(x, a);
	crustSimd128StoreI16(y, b);

	for (u32 i = 0; i < 8; ++i)
		x[i] = crustMaxI16(x[i], y[i]);

	return crustSimd128LoadI16(x);
}

static CRUST_INLINE Crust_Simd128 crustSimd128MaxU32(Crust_Simd128 a, Crust_Simd128 b)
{
	u32 x[4];
	u32 y[4];
	crustSimd128StoreU32(x, a);
	crustSimd128StoreU32(y, b);

	for (u32 i = 0; i < 4; ++i)
		x[i] = crustMaxU32(x[i], y[i]);

	return crustSimd128LoadU32(x);
}

static CRUST_INLINE Crust_Simd128 crustSimd128MaxI32(Crust_Simd128 a, Crust_Simd128 b)
{
	i32 x[4];
	i32 y[4];
	crustSimd128StoreI32(x, a);
	crustSimd128StoreI32(y, b);

	for (u32 i = 0; i < 4; ++i)
		x[i] = crustMaxI32(x[i], y[i]);

	return crustSimd128LoadI32(x);
}

//
static CRUST_INLINE Crust_Simd128 crustSimd128ShiftLeftU8(Crust_Simd128 v, u32 count)
{
	CRUST_ASSERT(count < 8);

	u8 x[16];
	crustSimd128StoreU8(x, v);

	for (u32 i = 0; i < 16; ++i)
		x[i] = (u8)(x[i] << count);

	return crustSimd128LoadU8(x);
}

static CRUST_INLINE Crust_Simd128 crustSimd128ShiftLeftU16(Crust_Simd128 v, u32 count)
{
	CRUST_ASSERT(count < 16);

	u16 x[8];
	crustSimd128StoreU16(x, v);

	for (u32 i = 0; i < 8; ++i)
		x[i] = (u16)(x[i] << count);

	return crustSimd128LoadU16(x);
}

static CRUST_INLINE Crust_Simd128 crustSimd128ShiftLeftU32(Crust_Simd128 v, u32 count)
{
	CRUST_ASSERT(count < 32);

	u32 x[4];
	crustSimd128StoreU32(x, v);

	for (u32 i = 0; i < 4; ++i)
		x[i] = (u32)(x[i] << count);

	return crustSimd128LoadU32(x);
}

static CRUST_INLINE Crust_Simd128 crustSimd128ShiftLeftU64(Crust_Simd128 v, u32 count)
{
	CRUST_ASSERT(count < 64);

	u64 x[2];
	crustSimd128StoreU64(x, v);

	for (u32 i = 0; i < 2; ++i)
		x[i] = (u64)(x[i] << count);

	return crustSimd128LoadU64(x);
}

static CRUST_INLINE Crust_Simd128 crustSimd128ShiftRightU8(Crust_Simd128 v, u32 count)
{
	CRUST_ASSERT(count < 8);

	u8 x[16];
	crustSimd128StoreU8(x, v);

	for (u32 i = 0; i < 16; ++i)
		x[i] = (u8)(x[i] >> count);

	return crustSimd128LoadU8(x);
}

static CRUST_INLINE Crust_Simd128 crustSimd128ShiftRightI8(Crust_Simd128 v, u32 count)
{
	CRUST_ASSERT(count < 8);

	u8 x[16];
	crustSimd128StoreU8(x, v);

	for (u32 i = 0; i < 16; ++i)
	{
		u8 sign = (x[i] >> 7) ? CRUST_U8_MAX : 0;
		x[i] = (u8)(((x[i] ^ sign) >> count) ^ sign);
	}

	return crustSimd128LoadU8(x);
}

static CRUST_INLINE Crust_Simd128 crustSimd128ShiftRightU16(Crust_Simd128 v, u32 count)
{
	CRUST_ASSERT(count < 16);

	u16 x[8];
	crustSimd128StoreU16(x, v);

	for (u32 i = 0; i < 8; ++i)
		x[i] = (u16)(x[i] >> count);

	return crustSimd128LoadU16(x);
}

static CRUST_INLINE Crust_Simd128 crustSimd128ShiftRightI16(Crust_Simd128 v, u32 count)
{
	CRUST_ASSERT(count < 16);

	u16 x[8];
	crustSimd128StoreU16(x, v);

	for (u32 i = 0; i < 8; ++i)
	{
		u16 sign = (x[i] >> 15) ? CRUST_U16_MAX : 0;
		x[i] = (u16)(((x[i] ^ sign) >> count) ^ sign);
	}

	return crustSimd128LoadU16(x);
}

static CRUST_INLINE Crust_Simd128 crustSimd128ShiftRightU32(Crust_Simd128 v, u32 count)
{
	CRUST_ASSERT(count < 32);

	u32 x[4];
	crustSimd128StoreU32(x, v);

	for (u32 i = 0; i < 4; ++i)
		x[i] = (u32)(x[i] >> count);

	return crustSimd128LoadU32(x);
}

static CRUST_INLINE Crust_Simd128 crustSimd128ShiftRightI32(Crust_Simd128 v, u32 count)
{
	CRUST_ASSERT(count < 32);

	u32 x[4];
	crustSimd128StoreU32(x, v);

	for (u32 i = 0; i < 4; ++i)
	{
		u32 sign = (x[i] >> 31) ? CRUST_U32_MAX : 0;
		x[i] = (u32)(((x[i] ^ sign) >> count) ^ sign);
	}

	return crustSimd128LoadU32(x);
}

static CRUST_INLINE Crust_Simd128 crustSimd128ShiftRightU64(Crust_Simd128 v, u32 count)
{
	CRUST_ASSERT(count < 64);

	u64 x[2];
	crustSimd128StoreU64(x, v);

	for (u32 i = 0; i < 2; ++i)
		x[i] = (u64)(x[i] >> count);

	return crustSimd128LoadU64(x);
}

static CRUST_INLINE Crust_Simd128 crustSimd128ShiftRightI64(Crust_Simd128 v, u32 count)
{
	CRUST_ASSERT(count < 64);

	u64 x[2];
	crustSimd128StoreU64(x, v);

	for (u32 i = 0; i < 2; ++i)
	{
		u64 sign = (x[i] >> 63) ? CRUST_U64_MAX : 0;
		x[i] = (u64)(((x[i] ^ sign) >> count) ^ sign);
	}

	return crustSimd128LoadU64(x);
}

//
static CRUST_INLINE Crust_Simd128 crustSimd128EqualU8(Crust_Simd128 a, Crust_Simd128 b)
{
	u8 x[16];
	u8 y[16];
	crustSimd128StoreU8(x, a);
	crustSimd128StoreU8(y, b);

	u8 result[16];
	for (u32 i = 0; i < 16; ++i)
		result[i] = (x[i] == y[i]) ? CRUST_U8_MAX : 0;

	return crustSimd128LoadU8(result);
}

static CRUST_INLINE Crust_Simd128 crustSimd128EqualU16(Crust_Simd128 a, Crust_Simd128 b)
{
	u16 x[8];
	u16 y[8];
	crustSimd128StoreU16(x, a);
	crustSimd128StoreU16(y, b);

	u16 result[8];
	for (u32 i = 0; i < 8; ++i)
		result[i] = (x[i] == y[i]) ? CRUST_U16_MAX : 0;

	return crustSimd128LoadU16(result);
}

static CRUST_INLINE Crust_Simd128 crustSimd128EqualU32(Crust_Simd128 a, Crust_Simd128 b)
{
	u32 x[4];
	u32 y[4];
	crustSimd128StoreU32(x, a);
	crustSimd128StoreU32(y, b);

	u32 result[4];
	for (u32 i = 0; i < 4; ++i)
		result[i] = (x[i] == y[i]) ? CRUST_U32_MAX : 0;

	return crustSimd128LoadU32(result);
}

static CRUST_INLINE Crust_Simd128 crustSimd128EqualU64(Crust_Simd128 a, Crust_Simd128 b)
{
	u64 x[2];
	u64 y[2];
	crustSimd128StoreU64(x, a);
	crustSimd128StoreU64(y, b);

	u64 result[2];
	for (u32 i = 0; i < 2; ++i)
		result[i] = (x[i] == y[i]) ? CRUST_U64_MAX : 0;

	return crustSimd128LoadU64(result);
}

static CRUST_INLINE Crust_Simd128 crustSimd128LessU8(Crust_Simd128 a, Crust_Simd128 b)
{
	u8 x[16];
	u8 y[16];
	crustSimd128StoreU8(x, a);
	crustSimd128StoreU8(y, b);

	u8 result[16];
	for (u32 i = 0; i < 16; ++i)
		result[i] = (x[i] < y[i]) ? CRUST_U8_MAX : 0;

	return crustSimd128LoadU8(result);
}

static CRUST_INLINE Crust_Simd128 crustSimd128LessI8(Crust_Simd128 a, Crust_Simd128 b)
{
	i8 x[16];
	i8 y[16];
	crustSimd128StoreI8(x, a);
	crustSimd128StoreI8(y, b);

	u8 result[16];
	for (u32 i = 0; i < 16; ++i)
		result[i] = (x[i] < y[i]) ? CRUST_U8_MAX : 0;

	return crustSimd128LoadU8(result);
}

static CRUST_INLINE Crust_Simd128 crustSimd128LessU16(Crust_Simd128 a, Crust_Simd128 b)
{
	u16 x[8];
	u16 y[8];
	crustSimd128StoreU16(x, a);
	crustSimd128StoreU16(y, b);

	u16 result[8];
	for (u32 i = 0; i < 8; ++i)
		result[i] = (x[i] < y[i]) ? CRUST_U16_MAX : 0;

	return crustSimd128LoadU16(result);
}

static CRUST_INLINE Crust_Simd128 crustSimd128LessI16(Crust_Simd128 a, Crust_Simd128 b)
{
	i16 x[8];
	i16 y[8];
	crustSimd128StoreI16(x, a);
	crustSimd128StoreI16(y, b);

	u16 result[8];
	for (u32 i = 0; i < 8; ++i)
		result[i] = (x[i] < y[i]) ? CRUST_U16_MAX : 0;

	return crustSimd128LoadU16(result);
}

static CRUST_INLINE Crust_Simd128 crustSimd128LessU32(Crust_Simd128 a, Crust_Simd128 b)
{
	u32 x[4];
	u32 y[4];
	crustSimd128StoreU32(x, a);
	crustSimd128StoreU32(y, b);

	u32 result[4];
	for (u32 i = 0; i < 4; ++i)
		result[i] = (x[i] < y[i]) ? CRUST_U32_MAX : 0;

	return crustSimd128LoadU32(result);
}

static CRUST_INLINE Crust_Simd128 crustSimd128LessI32(Crust_Simd128 a, Crust_Simd128 b)
{
	i32 x[4];
	i32 y[4];
	crustSimd128StoreI32(x, a);
	crustSimd128StoreI32(y, b);

	u32 result[4];
	for (u32 i = 0; i < 4; ++i)
		result[i] = (x[i] < y[i]) ? CRUST_U32_MAX : 0;

	return crustSimd128LoadU32(result);
}

static CRUST_INLINE Crust_Simd128 crustSimd128LessU64(Crust_Simd128 a, Crust_Simd128 b)
{
	u64 x[2];
	u64 y[2];
	crustSimd128StoreU64(x, a);
	crustSimd128StoreU64(y, b);

	u64 result[2];
	for (u32 i = 0; i < 2; ++i)
		result[i] = (x[i] < y[i]) ? CRUST_U64_MAX : 0;

	return crustSimd128LoadU64(result);
}

static CRUST_INLINE Crust_Simd128 crustSimd128LessI64(Crust_Simd128 a, Crust_Simd128 b)
{
	i64 x[2];
	i64 y[2];
	crustSimd128StoreI64(x, a);
	crustSimd128StoreI64(y, b);

	u64 result[2];
	for (u32 i = 0; i < 2; ++i)
		result[i] = (x[i] < y[i]) ? CRUST_U64_MAX : 0;

	return crustSimd128LoadU64(result);
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
	return crustSimd128Select(crustSimd128LessF32(a, b), a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128MaxF32(Crust_Simd128 a, Crust_Simd128 b)
{
	return crustSimd128Select(crustSimd128LessF32(a, b), b, a);
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
	return crustSimd128Select(crustSimd128LessF64(a, b), a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128MaxF64(Crust_Simd128 a, Crust_Simd128 b)
{
	return crustSimd128Select(crustSimd128LessF64(a, b), b, a);
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

//
static CRUST_INLINE Crust_Simd128 crustSimd128ConvertU16FromU8Low(Crust_Simd128 v)
{
	u8 x[16];
	crustSimd128StoreU8(x, v);

	u16 result[8];
	for (u32 i = 0; i < 8; ++i)
		result[i] = x[i];

	return crustSimd128LoadU16(result);
}

static CRUST_INLINE Crust_Simd128 crustSimd128ConvertU16FromU8High(Crust_Simd128 v)
{
	u8 x[16];
	crustSimd128StoreU8(x, v);

	u16 result[8];
	for (u32 i = 0; i < 8; ++i)
		result[i] = x[i + 8];

	return crustSimd128LoadU16(result);
}

static CRUST_INLINE Crust_Simd128 crustSimd128ConvertI16FromI8Low(Crust_Simd128 v)
{
	i8 x[16];
	crustSimd128StoreI8(x, v);

	i16 result[8];
	for (u32 i = 0; i < 8; ++i)
		result[i] = x[i];

	return crustSimd128LoadI16(result);
}

static CRUST_INLINE Crust_Simd128 crustSimd128ConvertI16FromI8High(Crust_Simd128 v)
{
	i8 x[16];
	crustSimd128StoreI8(x, v);

	i16 result[8];
	for (u32 i = 0; i < 8; ++i)
		result[i] = x[i + 8];

	return crustSimd128LoadI16(result);
}

static CRUST_INLINE Crust_Simd128 crustSimd128ConvertU32FromU16Low(Crust_Simd128 v)
{
	u16 x[8];
	crustSimd128StoreU16(x, v);

	u32 result[4];
	for (u32 i = 0; i < 4; ++i)
		result[i] = x[i];

	return crustSimd128LoadU32(result);
}

static CRUST_INLINE Crust_Simd128 crustSimd128ConvertU32FromU16High(Crust_Simd128 v)
{
	u16 x[8];
	crustSimd128StoreU16(x, v);

	u32 result[4];
	for (u32 i = 0; i < 4; ++i)
		result[i] = x[i + 4];

	return crustSimd128LoadU32(result);
}

static CRUST_INLINE Crust_Simd128 crustSimd128ConvertI32FromI16Low(Crust_Simd128 v)
{
	i16 x[8];
	crustSimd128StoreI16(x, v);

	i32 result[4];
	for (u32 i = 0; i < 4; ++i)
		result[i] = x[i];

	return crustSimd128LoadI32(result);
}

static CRUST_INLINE Crust_Simd128 crustSimd128ConvertI32FromI16High(Crust_Simd128 v)
{
	i16 x[8];
	crustSimd128StoreI16(x, v);

	i32 result[4];
	for (u32 i = 0; i < 4; ++i)
		result[i] = x[i + 4];

	return crustSimd128LoadI32(result);
}

static CRUST_INLINE Crust_Simd128 crustSimd128ConvertU64FromU32Low(Crust_Simd128 v)
{
	u32 x[4];
	crustSimd128StoreU32(x, v);

	u64 result[2];
	for (u32 i = 0; i < 2; ++i)
		result[i] = x[i];

	return crustSimd128LoadU64(result);
}

static CRUST_INLINE Crust_Simd128 crustSimd128ConvertU64FromU32High(Crust_Simd128 v)
{
	u32 x[4];
	crustSimd128StoreU32(x, v);

	u64 result[2];
	for (u32 i = 0; i < 2; ++i)
		result[i] = x[i + 2];

	return crustSimd128LoadU64(result);
}

static CRUST_INLINE Crust_Simd128 crustSimd128ConvertI64FromI32Low(Crust_Simd128 v)
{
	i32 x[4];
	crustSimd128StoreI32(x, v);

	i64 result[2];
	for (u32 i = 0; i < 2; ++i)
		result[i] = x[i];

	return crustSimd128LoadI64(result);
}

static CRUST_INLINE Crust_Simd128 crustSimd128ConvertI64FromI32High(Crust_Simd128 v)
{
	i32 x[4];
	crustSimd128StoreI32(x, v);

	i64 result[2];
	for (u32 i = 0; i < 2; ++i)
		result[i] = x[i + 2];

	return crustSimd128LoadI64(result);
}

//
static CRUST_INLINE Crust_Simd128 crustSimd128ConvertI8FromI16Saturate(Crust_Simd128 low, Crust_Simd128 high)
{
	i16 x[8];
	i16 y[8];
	crustSimd128StoreI16(x, low);
	crustSimd128StoreI16(y, high);

	i8 result[16];
	for (u32 i = 0; i < 8; ++i)
	{
		result[i] = (i8)crustClampI16(x[i], CRUST_I8_MIN, CRUST_I8_MAX);
		result[i + 8] = (i8)crustClampI16(y[i], CRUST_I8_MIN, CRUST_I8_MAX);
	}

	return crustSimd128LoadI8(result);
}

static CRUST_INLINE Crust_Simd128 crustSimd128ConvertU8FromI16Saturate(Crust_Simd128 low, Crust_Simd128 high)
{
	i16 x[8];
	i16 y[8];
	crustSimd128StoreI16(x, low);
	crustSimd128StoreI16(y, high);

	u8 result[16];
	for (u32 i = 0; i < 8; ++i)
	{
		result[i] = (u8)crustClampI16(x[i], 0, CRUST_U8_MAX);
		result[i + 8] = (u8)crustClampI16(y[i], 0, CRUST_U8_MAX);
	}

	return crustSimd128LoadU8(result);
}

static CRUST_INLINE Crust_Simd128 crustSimd128ConvertI16FromI32Saturate(Crust_Simd128 low, Crust_Simd128 high)
{
	i32 x[4];
	i32 y[4];
	crustSimd128StoreI32(x, low);
	crustSimd128StoreI32(y, high);

	i16 result[8];
	for (u32 i = 0; i < 4; ++i)
	{
		result[i] = (i16)crustClampI32(x[i], CRUST_I16_MIN, CRUST_I16_MAX);
		result[i + 4] = (i16)crustClampI32(y[i], CRUST_I16_MIN, CRUST_I16_MAX);
	}

	return crustSimd128LoadI16(result);
}

static CRUST_INLINE Crust_Simd128 crustSimd128ConvertU16FromI32Saturate(Crust_Simd128 low, Crust_Simd128 high)
{
	i32 x[4];
	i32 y[4];
	crustSimd128StoreI32(x, low);
	crustSimd128StoreI32(y, high);

	u16 result[8];
	for (u32 i = 0; i < 4; ++i)
	{
		result[i] = (u16)crustClampI32(x[i], 0, CRUST_U16_MAX);
		result[i + 4] = (u16)crustClampI32(y[i], 0, CRUST_U16_MAX);
	}

	return crustSimd128LoadU16(result);
}
