//
static CRUST_INLINE Crust_Simd128 crustSimd128AllZeros(void)
{
	return vdupq_n_u8(0);
}

static CRUST_INLINE Crust_Simd128 crustSimd128AllOnes(void)
{
	return vdupq_n_u8(CRUST_U8_MAX);
}

//
static CRUST_INLINE Crust_Simd128 crustSimd128LoadU8(const u8 *ptr)
{
	return vld1q_u8(ptr);
}

static CRUST_INLINE Crust_Simd128 crustSimd128LoadU16(const u16 *ptr)
{
	return vreinterpretq_u8_u16(vld1q_u16(ptr));
}

static CRUST_INLINE Crust_Simd128 crustSimd128LoadU32(const u32 *ptr)
{
	return vreinterpretq_u8_u32(vld1q_u32(ptr));
}

static CRUST_INLINE Crust_Simd128 crustSimd128LoadU64(const u64 *ptr)
{
	return vreinterpretq_u8_u64(vld1q_u64(ptr));
}

static CRUST_INLINE Crust_Simd128 crustSimd128LoadF32(const f32 *ptr)
{
	return vreinterpretq_u8_f32(vld1q_f32(ptr));
}

static CRUST_INLINE Crust_Simd128 crustSimd128LoadF64(const f64 *ptr)
{
	return vreinterpretq_u8_f64(vld1q_f64(ptr));
}

static CRUST_INLINE Crust_Simd128 crustSimd128LoadAlignedU8(const u8 *ptr)
{
	CRUST_ASSERT(crustIsAlignedUSize((usize)ptr, 16));
	return vld1q_u8(ptr);
}

static CRUST_INLINE Crust_Simd128 crustSimd128LoadAlignedU16(const u16 *ptr)
{
	CRUST_ASSERT(crustIsAlignedUSize((usize)ptr, 16));
	return vreinterpretq_u8_u16(vld1q_u16(ptr));
}

static CRUST_INLINE Crust_Simd128 crustSimd128LoadAlignedU32(const u32 *ptr)
{
	CRUST_ASSERT(crustIsAlignedUSize((usize)ptr, 16));
	return vreinterpretq_u8_u32(vld1q_u32(ptr));
}

static CRUST_INLINE Crust_Simd128 crustSimd128LoadAlignedU64(const u64 *ptr)
{
	CRUST_ASSERT(crustIsAlignedUSize((usize)ptr, 16));
	return vreinterpretq_u8_u64(vld1q_u64(ptr));
}

static CRUST_INLINE Crust_Simd128 crustSimd128LoadAlignedF32(const f32 *ptr)
{
	CRUST_ASSERT(crustIsAlignedUSize((usize)ptr, 16));
	return vreinterpretq_u8_f32(vld1q_f32(ptr));
}

static CRUST_INLINE Crust_Simd128 crustSimd128LoadAlignedF64(const f64 *ptr)
{
	CRUST_ASSERT(crustIsAlignedUSize((usize)ptr, 16));
	return vreinterpretq_u8_f64(vld1q_f64(ptr));
}

static CRUST_INLINE void crustSimd128StoreU8(u8 *ptr, Crust_Simd128 v)
{
	vst1q_u8(ptr, v);
}

static CRUST_INLINE void crustSimd128StoreU16(u16 *ptr, Crust_Simd128 v)
{
	vst1q_u16(ptr, vreinterpretq_u16_u8(v));
}

static CRUST_INLINE void crustSimd128StoreU32(u32 *ptr, Crust_Simd128 v)
{
	vst1q_u32(ptr, vreinterpretq_u32_u8(v));
}

static CRUST_INLINE void crustSimd128StoreU64(u64 *ptr, Crust_Simd128 v)
{
	vst1q_u64(ptr, vreinterpretq_u64_u8(v));
}

static CRUST_INLINE void crustSimd128StoreF32(f32 *ptr, Crust_Simd128 v)
{
	vst1q_f32(ptr, vreinterpretq_f32_u8(v));
}

static CRUST_INLINE void crustSimd128StoreF64(f64 *ptr, Crust_Simd128 v)
{
	vst1q_f64(ptr, vreinterpretq_f64_u8(v));
}

static CRUST_INLINE void crustSimd128StoreAlignedU8(u8 *ptr, Crust_Simd128 v)
{
	CRUST_ASSERT(crustIsAlignedUSize((usize)ptr, 16));
	vst1q_u8(ptr, v);
}

static CRUST_INLINE void crustSimd128StoreAlignedU16(u16 *ptr, Crust_Simd128 v)
{
	CRUST_ASSERT(crustIsAlignedUSize((usize)ptr, 16));
	vst1q_u16(ptr, vreinterpretq_u16_u8(v));
}

static CRUST_INLINE void crustSimd128StoreAlignedU32(u32 *ptr, Crust_Simd128 v)
{
	CRUST_ASSERT(crustIsAlignedUSize((usize)ptr, 16));
	vst1q_u32(ptr, vreinterpretq_u32_u8(v));
}

static CRUST_INLINE void crustSimd128StoreAlignedU64(u64 *ptr, Crust_Simd128 v)
{
	CRUST_ASSERT(crustIsAlignedUSize((usize)ptr, 16));
	vst1q_u64(ptr, vreinterpretq_u64_u8(v));
}

static CRUST_INLINE void crustSimd128StoreAlignedF32(f32 *ptr, Crust_Simd128 v)
{
	CRUST_ASSERT(crustIsAlignedUSize((usize)ptr, 16));
	vst1q_f32(ptr, vreinterpretq_f32_u8(v));
}

static CRUST_INLINE void crustSimd128StoreAlignedF64(f64 *ptr, Crust_Simd128 v)
{
	CRUST_ASSERT(crustIsAlignedUSize((usize)ptr, 16));
	vst1q_f64(ptr, vreinterpretq_f64_u8(v));
}

//
static CRUST_INLINE Crust_Simd128 crustSimd128SplatU8(u8 v)
{
	return vdupq_n_u8(v);
}

static CRUST_INLINE Crust_Simd128 crustSimd128SplatU16(u16 v)
{
	return vreinterpretq_u8_u16(vdupq_n_u16(v));
}

static CRUST_INLINE Crust_Simd128 crustSimd128SplatU32(u32 v)
{
	return vreinterpretq_u8_u32(vdupq_n_u32(v));
}

static CRUST_INLINE Crust_Simd128 crustSimd128SplatU64(u64 v)
{
	return vreinterpretq_u8_u64(vdupq_n_u64(v));
}

static CRUST_INLINE Crust_Simd128 crustSimd128SplatF32(f32 v)
{
	return vreinterpretq_u8_f32(vdupq_n_f32(v));
}

static CRUST_INLINE Crust_Simd128 crustSimd128SplatF64(f64 v)
{
	return vreinterpretq_u8_f64(vdupq_n_f64(v));
}

static CRUST_INLINE Crust_Simd128 crustSimd128MakeU32(u32 v0, u32 v1, u32 v2, u32 v3)
{
	u32 lanes[4] = { v0, v1, v2, v3 };
	return vreinterpretq_u8_u32(vld1q_u32(lanes));
}

static CRUST_INLINE Crust_Simd128 crustSimd128MakeF32(f32 v0, f32 v1, f32 v2, f32 v3)
{
	f32 lanes[4] = { v0, v1, v2, v3 };
	return vreinterpretq_u8_f32(vld1q_f32(lanes));
}

static CRUST_INLINE Crust_Simd128 crustSimd128MakeU64(u64 v0, u64 v1)
{
	return vreinterpretq_u8_u64(vcombine_u64(vcreate_u64(v0), vcreate_u64(v1)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128MakeF64(f64 v0, f64 v1)
{
	return vreinterpretq_u8_f64(vcombine_f64(vdup_n_f64(v0), vdup_n_f64(v1)));
}

//
static CRUST_INLINE Crust_Simd128 crustSimd128ZipLowU8(Crust_Simd128 a, Crust_Simd128 b)
{
	return vzip1q_u8(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128ZipHighU8(Crust_Simd128 a, Crust_Simd128 b)
{
	return vzip2q_u8(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128ZipLowU16(Crust_Simd128 a, Crust_Simd128 b)
{
	return vreinterpretq_u8_u16(vzip1q_u16(vreinterpretq_u16_u8(a), vreinterpretq_u16_u8(b)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128ZipHighU16(Crust_Simd128 a, Crust_Simd128 b)
{
	return vreinterpretq_u8_u16(vzip2q_u16(vreinterpretq_u16_u8(a), vreinterpretq_u16_u8(b)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128ZipLowU32(Crust_Simd128 a, Crust_Simd128 b)
{
	return vreinterpretq_u8_u32(vzip1q_u32(vreinterpretq_u32_u8(a), vreinterpretq_u32_u8(b)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128ZipHighU32(Crust_Simd128 a, Crust_Simd128 b)
{
	return vreinterpretq_u8_u32(vzip2q_u32(vreinterpretq_u32_u8(a), vreinterpretq_u32_u8(b)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128ZipLowU64(Crust_Simd128 a, Crust_Simd128 b)
{
	return vreinterpretq_u8_u64(vzip1q_u64(vreinterpretq_u64_u8(a), vreinterpretq_u64_u8(b)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128ZipHighU64(Crust_Simd128 a, Crust_Simd128 b)
{
	return vreinterpretq_u8_u64(vzip2q_u64(vreinterpretq_u64_u8(a), vreinterpretq_u64_u8(b)));
}

//
static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32XYWZ(Crust_Simd128 v)
{
	static const u8 indices[16] = { 0, 1, 2, 3, 4, 5, 6, 7, 12, 13, 14, 15, 8, 9, 10, 11 };
	return vqtbl1q_u8(v, vld1q_u8(indices));
}

static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32XZYW(Crust_Simd128 v)
{
	static const u8 indices[16] = { 0, 1, 2, 3, 8, 9, 10, 11, 4, 5, 6, 7, 12, 13, 14, 15 };
	return vqtbl1q_u8(v, vld1q_u8(indices));
}

static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32XZWY(Crust_Simd128 v)
{
	static const u8 indices[16] = { 0, 1, 2, 3, 8, 9, 10, 11, 12, 13, 14, 15, 4, 5, 6, 7 };
	return vqtbl1q_u8(v, vld1q_u8(indices));
}

static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32XWYZ(Crust_Simd128 v)
{
	static const u8 indices[16] = { 0, 1, 2, 3, 12, 13, 14, 15, 4, 5, 6, 7, 8, 9, 10, 11 };
	return vqtbl1q_u8(v, vld1q_u8(indices));
}

static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32XWZY(Crust_Simd128 v)
{
	static const u8 indices[16] = { 0, 1, 2, 3, 12, 13, 14, 15, 8, 9, 10, 11, 4, 5, 6, 7 };
	return vqtbl1q_u8(v, vld1q_u8(indices));
}

static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32YXZW(Crust_Simd128 v)
{
	static const u8 indices[16] = { 4, 5, 6, 7, 0, 1, 2, 3, 8, 9, 10, 11, 12, 13, 14, 15 };
	return vqtbl1q_u8(v, vld1q_u8(indices));
}

static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32YXWZ(Crust_Simd128 v)
{
	return vreinterpretq_u8_u32(vrev64q_u32(vreinterpretq_u32_u8(v)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32YZXW(Crust_Simd128 v)
{
	static const u8 indices[16] = { 4, 5, 6, 7, 8, 9, 10, 11, 0, 1, 2, 3, 12, 13, 14, 15 };
	return vqtbl1q_u8(v, vld1q_u8(indices));
}

static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32YZWX(Crust_Simd128 v)
{
	return vextq_u8(v, v, 4);
}

static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32YWXZ(Crust_Simd128 v)
{
	static const u8 indices[16] = { 4, 5, 6, 7, 12, 13, 14, 15, 0, 1, 2, 3, 8, 9, 10, 11 };
	return vqtbl1q_u8(v, vld1q_u8(indices));
}

static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32YWZX(Crust_Simd128 v)
{
	static const u8 indices[16] = { 4, 5, 6, 7, 12, 13, 14, 15, 8, 9, 10, 11, 0, 1, 2, 3 };
	return vqtbl1q_u8(v, vld1q_u8(indices));
}

static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32ZXYW(Crust_Simd128 v)
{
	static const u8 indices[16] = { 8, 9, 10, 11, 0, 1, 2, 3, 4, 5, 6, 7, 12, 13, 14, 15 };
	return vqtbl1q_u8(v, vld1q_u8(indices));
}

static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32ZXWY(Crust_Simd128 v)
{
	static const u8 indices[16] = { 8, 9, 10, 11, 0, 1, 2, 3, 12, 13, 14, 15, 4, 5, 6, 7 };
	return vqtbl1q_u8(v, vld1q_u8(indices));
}

static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32ZYXW(Crust_Simd128 v)
{
	static const u8 indices[16] = { 8, 9, 10, 11, 4, 5, 6, 7, 0, 1, 2, 3, 12, 13, 14, 15 };
	return vqtbl1q_u8(v, vld1q_u8(indices));
}

static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32ZYWX(Crust_Simd128 v)
{
	static const u8 indices[16] = { 8, 9, 10, 11, 4, 5, 6, 7, 12, 13, 14, 15, 0, 1, 2, 3 };
	return vqtbl1q_u8(v, vld1q_u8(indices));
}

static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32ZWXY(Crust_Simd128 v)
{
	return vextq_u8(v, v, 8);
}

static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32ZWYX(Crust_Simd128 v)
{
	static const u8 indices[16] = { 8, 9, 10, 11, 12, 13, 14, 15, 4, 5, 6, 7, 0, 1, 2, 3 };
	return vqtbl1q_u8(v, vld1q_u8(indices));
}

static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32WXYZ(Crust_Simd128 v)
{
	return vextq_u8(v, v, 12);
}

static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32WXZY(Crust_Simd128 v)
{
	static const u8 indices[16] = { 12, 13, 14, 15, 0, 1, 2, 3, 8, 9, 10, 11, 4, 5, 6, 7 };
	return vqtbl1q_u8(v, vld1q_u8(indices));
}

static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32WYXZ(Crust_Simd128 v)
{
	static const u8 indices[16] = { 12, 13, 14, 15, 4, 5, 6, 7, 0, 1, 2, 3, 8, 9, 10, 11 };
	return vqtbl1q_u8(v, vld1q_u8(indices));
}

static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32WYZX(Crust_Simd128 v)
{
	static const u8 indices[16] = { 12, 13, 14, 15, 4, 5, 6, 7, 8, 9, 10, 11, 0, 1, 2, 3 };
	return vqtbl1q_u8(v, vld1q_u8(indices));
}

static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32WZXY(Crust_Simd128 v)
{
	static const u8 indices[16] = { 12, 13, 14, 15, 8, 9, 10, 11, 0, 1, 2, 3, 4, 5, 6, 7 };
	return vqtbl1q_u8(v, vld1q_u8(indices));
}

static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32WZYX(Crust_Simd128 v)
{
	static const u8 indices[16] = { 12, 13, 14, 15, 8, 9, 10, 11, 4, 5, 6, 7, 0, 1, 2, 3 };
	return vqtbl1q_u8(v, vld1q_u8(indices));
}

static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32XXXX(Crust_Simd128 v)
{
	return vreinterpretq_u8_u32(vdupq_laneq_u32(vreinterpretq_u32_u8(v), 0));
}

static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32YYYY(Crust_Simd128 v)
{
	return vreinterpretq_u8_u32(vdupq_laneq_u32(vreinterpretq_u32_u8(v), 1));
}

static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32ZZZZ(Crust_Simd128 v)
{
	return vreinterpretq_u8_u32(vdupq_laneq_u32(vreinterpretq_u32_u8(v), 2));
}

static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32WWWW(Crust_Simd128 v)
{
	return vreinterpretq_u8_u32(vdupq_laneq_u32(vreinterpretq_u32_u8(v), 3));
}

static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle64YX(Crust_Simd128 v)
{
	return vextq_u8(v, v, 8);
}

static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle64XX(Crust_Simd128 v)
{
	return vreinterpretq_u8_u64(vdupq_laneq_u64(vreinterpretq_u64_u8(v), 0));
}

static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle64YY(Crust_Simd128 v)
{
	return vreinterpretq_u8_u64(vdupq_laneq_u64(vreinterpretq_u64_u8(v), 1));
}

//
static CRUST_INLINE Crust_Simd128 crustSimd128And(Crust_Simd128 a, Crust_Simd128 b)
{
	return vandq_u8(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128Or(Crust_Simd128 a, Crust_Simd128 b)
{
	return vorrq_u8(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128Xor(Crust_Simd128 a, Crust_Simd128 b)
{
	return veorq_u8(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128AndNot(Crust_Simd128 a, Crust_Simd128 b)
{
	return vbicq_u8(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128Not(Crust_Simd128 v)
{
	return vmvnq_u8(v);
}

static CRUST_INLINE Crust_Simd128 crustSimd128Select(Crust_Simd128 mask, Crust_Simd128 a, Crust_Simd128 b)
{
	return vbslq_u8(mask, a, b);
}

//
static CRUST_INLINE u32 crustSimd128Any(Crust_Simd128 v)
{
	return vmaxvq_u32(vreinterpretq_u32_u8(v)) != 0;
}

static CRUST_INLINE u32 crustSimd128All(Crust_Simd128 v)
{
	return vminvq_u32(vreinterpretq_u32_u8(v)) == CRUST_U32_MAX;
}

static CRUST_INLINE u32 crustSimd128BitmaskU8(Crust_Simd128 v)
{
	static const i8 shifts[8] = { 0, 1, 2, 3, 4, 5, 6, 7 };

	int8x8_t shift = vld1_s8(shifts);
	uint8x16_t bits = vshrq_n_u8(v, 7);

	u32 low = vaddv_u8(vshl_u8(vget_low_u8(bits), shift));
	u32 high = vaddv_u8(vshl_u8(vget_high_u8(bits), shift));

	return low | (high << 8);
}

static CRUST_INLINE u32 crustSimd128BitmaskU16(Crust_Simd128 v)
{
	static const i16 shifts[8] = { 0, 1, 2, 3, 4, 5, 6, 7 };

	uint16x8_t bits = vshrq_n_u16(vreinterpretq_u16_u8(v), 15);
	return vaddvq_u16(vshlq_u16(bits, vld1q_s16(shifts)));
}

static CRUST_INLINE u32 crustSimd128BitmaskU32(Crust_Simd128 v)
{
	static const i32 shifts[4] = { 0, 1, 2, 3 };

	uint32x4_t bits = vshrq_n_u32(vreinterpretq_u32_u8(v), 31);
	return vaddvq_u32(vshlq_u32(bits, vld1q_s32(shifts)));
}

static CRUST_INLINE u32 crustSimd128BitmaskU64(Crust_Simd128 v)
{
	uint64x2_t lanes = vreinterpretq_u64_u8(v);

	u32 low = (u32)(vgetq_lane_u64(lanes, 0) >> 63);
	u32 high = (u32)(vgetq_lane_u64(lanes, 1) >> 63);

	return low | (high << 1);
}

//
static CRUST_INLINE Crust_Simd128 crustSimd128AddU8(Crust_Simd128 a, Crust_Simd128 b)
{
	return vaddq_u8(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128AddU16(Crust_Simd128 a, Crust_Simd128 b)
{
	return vreinterpretq_u8_u16(vaddq_u16(vreinterpretq_u16_u8(a), vreinterpretq_u16_u8(b)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128AddU32(Crust_Simd128 a, Crust_Simd128 b)
{
	return vreinterpretq_u8_u32(vaddq_u32(vreinterpretq_u32_u8(a), vreinterpretq_u32_u8(b)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128AddU64(Crust_Simd128 a, Crust_Simd128 b)
{
	return vreinterpretq_u8_u64(vaddq_u64(vreinterpretq_u64_u8(a), vreinterpretq_u64_u8(b)));
}

//
static CRUST_INLINE Crust_Simd128 crustSimd128SubU8(Crust_Simd128 a, Crust_Simd128 b)
{
	return vsubq_u8(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128SubU16(Crust_Simd128 a, Crust_Simd128 b)
{
	return vreinterpretq_u8_u16(vsubq_u16(vreinterpretq_u16_u8(a), vreinterpretq_u16_u8(b)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128SubU32(Crust_Simd128 a, Crust_Simd128 b)
{
	return vreinterpretq_u8_u32(vsubq_u32(vreinterpretq_u32_u8(a), vreinterpretq_u32_u8(b)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128SubU64(Crust_Simd128 a, Crust_Simd128 b)
{
	return vreinterpretq_u8_u64(vsubq_u64(vreinterpretq_u64_u8(a), vreinterpretq_u64_u8(b)));
}

//
static CRUST_INLINE Crust_Simd128 crustSimd128AddSaturateU8(Crust_Simd128 a, Crust_Simd128 b)
{
	return vqaddq_u8(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128AddSaturateI8(Crust_Simd128 a, Crust_Simd128 b)
{
	return vreinterpretq_u8_s8(vqaddq_s8(vreinterpretq_s8_u8(a), vreinterpretq_s8_u8(b)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128AddSaturateU16(Crust_Simd128 a, Crust_Simd128 b)
{
	return vreinterpretq_u8_u16(vqaddq_u16(vreinterpretq_u16_u8(a), vreinterpretq_u16_u8(b)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128AddSaturateI16(Crust_Simd128 a, Crust_Simd128 b)
{
	return vreinterpretq_u8_s16(vqaddq_s16(vreinterpretq_s16_u8(a), vreinterpretq_s16_u8(b)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128SubSaturateU8(Crust_Simd128 a, Crust_Simd128 b)
{
	return vqsubq_u8(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128SubSaturateI8(Crust_Simd128 a, Crust_Simd128 b)
{
	return vreinterpretq_u8_s8(vqsubq_s8(vreinterpretq_s8_u8(a), vreinterpretq_s8_u8(b)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128SubSaturateU16(Crust_Simd128 a, Crust_Simd128 b)
{
	return vreinterpretq_u8_u16(vqsubq_u16(vreinterpretq_u16_u8(a), vreinterpretq_u16_u8(b)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128SubSaturateI16(Crust_Simd128 a, Crust_Simd128 b)
{
	return vreinterpretq_u8_s16(vqsubq_s16(vreinterpretq_s16_u8(a), vreinterpretq_s16_u8(b)));
}

//
static CRUST_INLINE Crust_Simd128 crustSimd128MulU16(Crust_Simd128 a, Crust_Simd128 b)
{
	return vreinterpretq_u8_u16(vmulq_u16(vreinterpretq_u16_u8(a), vreinterpretq_u16_u8(b)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128MulU32(Crust_Simd128 a, Crust_Simd128 b)
{
	return vreinterpretq_u8_u32(vmulq_u32(vreinterpretq_u32_u8(a), vreinterpretq_u32_u8(b)));
}

//
static CRUST_INLINE Crust_Simd128 crustSimd128MinU8(Crust_Simd128 a, Crust_Simd128 b)
{
	return vminq_u8(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128MinI8(Crust_Simd128 a, Crust_Simd128 b)
{
	return vreinterpretq_u8_s8(vminq_s8(vreinterpretq_s8_u8(a), vreinterpretq_s8_u8(b)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128MinU16(Crust_Simd128 a, Crust_Simd128 b)
{
	return vreinterpretq_u8_u16(vminq_u16(vreinterpretq_u16_u8(a), vreinterpretq_u16_u8(b)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128MinI16(Crust_Simd128 a, Crust_Simd128 b)
{
	return vreinterpretq_u8_s16(vminq_s16(vreinterpretq_s16_u8(a), vreinterpretq_s16_u8(b)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128MinU32(Crust_Simd128 a, Crust_Simd128 b)
{
	return vreinterpretq_u8_u32(vminq_u32(vreinterpretq_u32_u8(a), vreinterpretq_u32_u8(b)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128MinI32(Crust_Simd128 a, Crust_Simd128 b)
{
	return vreinterpretq_u8_s32(vminq_s32(vreinterpretq_s32_u8(a), vreinterpretq_s32_u8(b)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128MaxU8(Crust_Simd128 a, Crust_Simd128 b)
{
	return vmaxq_u8(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128MaxI8(Crust_Simd128 a, Crust_Simd128 b)
{
	return vreinterpretq_u8_s8(vmaxq_s8(vreinterpretq_s8_u8(a), vreinterpretq_s8_u8(b)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128MaxU16(Crust_Simd128 a, Crust_Simd128 b)
{
	return vreinterpretq_u8_u16(vmaxq_u16(vreinterpretq_u16_u8(a), vreinterpretq_u16_u8(b)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128MaxI16(Crust_Simd128 a, Crust_Simd128 b)
{
	return vreinterpretq_u8_s16(vmaxq_s16(vreinterpretq_s16_u8(a), vreinterpretq_s16_u8(b)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128MaxU32(Crust_Simd128 a, Crust_Simd128 b)
{
	return vreinterpretq_u8_u32(vmaxq_u32(vreinterpretq_u32_u8(a), vreinterpretq_u32_u8(b)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128MaxI32(Crust_Simd128 a, Crust_Simd128 b)
{
	return vreinterpretq_u8_s32(vmaxq_s32(vreinterpretq_s32_u8(a), vreinterpretq_s32_u8(b)));
}

//
static CRUST_INLINE Crust_Simd128 crustSimd128ShiftLeftU8(Crust_Simd128 v, u32 count)
{
	CRUST_ASSERT(count < 8);
	return vshlq_u8(v, vdupq_n_s8((i8)count));
}

static CRUST_INLINE Crust_Simd128 crustSimd128ShiftLeftU16(Crust_Simd128 v, u32 count)
{
	CRUST_ASSERT(count < 16);
	return vreinterpretq_u8_u16(vshlq_u16(vreinterpretq_u16_u8(v), vdupq_n_s16((i16)count)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128ShiftLeftU32(Crust_Simd128 v, u32 count)
{
	CRUST_ASSERT(count < 32);
	return vreinterpretq_u8_u32(vshlq_u32(vreinterpretq_u32_u8(v), vdupq_n_s32((i32)count)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128ShiftLeftU64(Crust_Simd128 v, u32 count)
{
	CRUST_ASSERT(count < 64);
	return vreinterpretq_u8_u64(vshlq_u64(vreinterpretq_u64_u8(v), vdupq_n_s64((i64)count)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128ShiftRightU8(Crust_Simd128 v, u32 count)
{
	CRUST_ASSERT(count < 8);
	return vshlq_u8(v, vdupq_n_s8((i8)(-(i32)count)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128ShiftRightI8(Crust_Simd128 v, u32 count)
{
	CRUST_ASSERT(count < 8);
	return vreinterpretq_u8_s8(vshlq_s8(vreinterpretq_s8_u8(v), vdupq_n_s8((i8)(-(i32)count))));
}

static CRUST_INLINE Crust_Simd128 crustSimd128ShiftRightU16(Crust_Simd128 v, u32 count)
{
	CRUST_ASSERT(count < 16);
	return vreinterpretq_u8_u16(vshlq_u16(vreinterpretq_u16_u8(v), vdupq_n_s16((i16)(-(i32)count))));
}

static CRUST_INLINE Crust_Simd128 crustSimd128ShiftRightI16(Crust_Simd128 v, u32 count)
{
	CRUST_ASSERT(count < 16);
	return vreinterpretq_u8_s16(vshlq_s16(vreinterpretq_s16_u8(v), vdupq_n_s16((i16)(-(i32)count))));
}

static CRUST_INLINE Crust_Simd128 crustSimd128ShiftRightU32(Crust_Simd128 v, u32 count)
{
	CRUST_ASSERT(count < 32);
	return vreinterpretq_u8_u32(vshlq_u32(vreinterpretq_u32_u8(v), vdupq_n_s32(-(i32)count)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128ShiftRightI32(Crust_Simd128 v, u32 count)
{
	CRUST_ASSERT(count < 32);
	return vreinterpretq_u8_s32(vshlq_s32(vreinterpretq_s32_u8(v), vdupq_n_s32(-(i32)count)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128ShiftRightU64(Crust_Simd128 v, u32 count)
{
	CRUST_ASSERT(count < 64);
	return vreinterpretq_u8_u64(vshlq_u64(vreinterpretq_u64_u8(v), vdupq_n_s64(-(i64)count)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128ShiftRightI64(Crust_Simd128 v, u32 count)
{
	CRUST_ASSERT(count < 64);
	return vreinterpretq_u8_s64(vshlq_s64(vreinterpretq_s64_u8(v), vdupq_n_s64(-(i64)count)));
}

//
static CRUST_INLINE Crust_Simd128 crustSimd128EqualU8(Crust_Simd128 a, Crust_Simd128 b)
{
	return vceqq_u8(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128EqualU16(Crust_Simd128 a, Crust_Simd128 b)
{
	return vreinterpretq_u8_u16(vceqq_u16(vreinterpretq_u16_u8(a), vreinterpretq_u16_u8(b)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128EqualU32(Crust_Simd128 a, Crust_Simd128 b)
{
	return vreinterpretq_u8_u32(vceqq_u32(vreinterpretq_u32_u8(a), vreinterpretq_u32_u8(b)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128EqualU64(Crust_Simd128 a, Crust_Simd128 b)
{
	return vreinterpretq_u8_u64(vceqq_u64(vreinterpretq_u64_u8(a), vreinterpretq_u64_u8(b)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128LessU8(Crust_Simd128 a, Crust_Simd128 b)
{
	return vcltq_u8(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128LessI8(Crust_Simd128 a, Crust_Simd128 b)
{
	return vcltq_s8(vreinterpretq_s8_u8(a), vreinterpretq_s8_u8(b));
}

static CRUST_INLINE Crust_Simd128 crustSimd128LessU16(Crust_Simd128 a, Crust_Simd128 b)
{
	return vreinterpretq_u8_u16(vcltq_u16(vreinterpretq_u16_u8(a), vreinterpretq_u16_u8(b)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128LessI16(Crust_Simd128 a, Crust_Simd128 b)
{
	return vreinterpretq_u8_u16(vcltq_s16(vreinterpretq_s16_u8(a), vreinterpretq_s16_u8(b)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128LessU32(Crust_Simd128 a, Crust_Simd128 b)
{
	return vreinterpretq_u8_u32(vcltq_u32(vreinterpretq_u32_u8(a), vreinterpretq_u32_u8(b)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128LessI32(Crust_Simd128 a, Crust_Simd128 b)
{
	return vreinterpretq_u8_u32(vcltq_s32(vreinterpretq_s32_u8(a), vreinterpretq_s32_u8(b)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128LessU64(Crust_Simd128 a, Crust_Simd128 b)
{
	return vreinterpretq_u8_u64(vcltq_u64(vreinterpretq_u64_u8(a), vreinterpretq_u64_u8(b)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128LessI64(Crust_Simd128 a, Crust_Simd128 b)
{
	return vreinterpretq_u8_u64(vcltq_s64(vreinterpretq_s64_u8(a), vreinterpretq_s64_u8(b)));
}

//
static CRUST_INLINE Crust_Simd128 crustSimd128AddF32(Crust_Simd128 a, Crust_Simd128 b)
{
	return vreinterpretq_u8_f32(vaddq_f32(vreinterpretq_f32_u8(a), vreinterpretq_f32_u8(b)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128SubF32(Crust_Simd128 a, Crust_Simd128 b)
{
	return vreinterpretq_u8_f32(vsubq_f32(vreinterpretq_f32_u8(a), vreinterpretq_f32_u8(b)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128MulF32(Crust_Simd128 a, Crust_Simd128 b)
{
	return vreinterpretq_u8_f32(vmulq_f32(vreinterpretq_f32_u8(a), vreinterpretq_f32_u8(b)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128DivF32(Crust_Simd128 a, Crust_Simd128 b)
{
	return vreinterpretq_u8_f32(vdivq_f32(vreinterpretq_f32_u8(a), vreinterpretq_f32_u8(b)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128SqrtF32(Crust_Simd128 v)
{
	return vreinterpretq_u8_f32(vsqrtq_f32(vreinterpretq_f32_u8(v)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128AbsF32(Crust_Simd128 v)
{
	return vreinterpretq_u8_f32(vabsq_f32(vreinterpretq_f32_u8(v)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128NegateF32(Crust_Simd128 v)
{
	return vreinterpretq_u8_f32(vnegq_f32(vreinterpretq_f32_u8(v)));
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
	return vreinterpretq_u8_u32(vceqq_f32(vreinterpretq_f32_u8(a), vreinterpretq_f32_u8(b)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128NotEqualF32(Crust_Simd128 a, Crust_Simd128 b)
{
	return vmvnq_u8(crustSimd128EqualF32(a, b));
}

static CRUST_INLINE Crust_Simd128 crustSimd128LessF32(Crust_Simd128 a, Crust_Simd128 b)
{
	return vreinterpretq_u8_u32(vcltq_f32(vreinterpretq_f32_u8(a), vreinterpretq_f32_u8(b)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128LessEqualF32(Crust_Simd128 a, Crust_Simd128 b)
{
	return vreinterpretq_u8_u32(vcleq_f32(vreinterpretq_f32_u8(a), vreinterpretq_f32_u8(b)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128FloorF32(Crust_Simd128 v)
{
	return vreinterpretq_u8_f32(vrndmq_f32(vreinterpretq_f32_u8(v)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128CeilF32(Crust_Simd128 v)
{
	return vreinterpretq_u8_f32(vrndpq_f32(vreinterpretq_f32_u8(v)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128RoundEvenF32(Crust_Simd128 v)
{
	return vreinterpretq_u8_f32(vrndnq_f32(vreinterpretq_f32_u8(v)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128TruncateF32(Crust_Simd128 v)
{
	return vreinterpretq_u8_f32(vrndq_f32(vreinterpretq_f32_u8(v)));
}

//
static CRUST_INLINE Crust_Simd128 crustSimd128AddF64(Crust_Simd128 a, Crust_Simd128 b)
{
	return vreinterpretq_u8_f64(vaddq_f64(vreinterpretq_f64_u8(a), vreinterpretq_f64_u8(b)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128SubF64(Crust_Simd128 a, Crust_Simd128 b)
{
	return vreinterpretq_u8_f64(vsubq_f64(vreinterpretq_f64_u8(a), vreinterpretq_f64_u8(b)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128MulF64(Crust_Simd128 a, Crust_Simd128 b)
{
	return vreinterpretq_u8_f64(vmulq_f64(vreinterpretq_f64_u8(a), vreinterpretq_f64_u8(b)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128DivF64(Crust_Simd128 a, Crust_Simd128 b)
{
	return vreinterpretq_u8_f64(vdivq_f64(vreinterpretq_f64_u8(a), vreinterpretq_f64_u8(b)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128SqrtF64(Crust_Simd128 v)
{
	return vreinterpretq_u8_f64(vsqrtq_f64(vreinterpretq_f64_u8(v)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128AbsF64(Crust_Simd128 v)
{
	return vreinterpretq_u8_f64(vabsq_f64(vreinterpretq_f64_u8(v)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128NegateF64(Crust_Simd128 v)
{
	return vreinterpretq_u8_f64(vnegq_f64(vreinterpretq_f64_u8(v)));
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
	return vreinterpretq_u8_u64(vceqq_f64(vreinterpretq_f64_u8(a), vreinterpretq_f64_u8(b)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128NotEqualF64(Crust_Simd128 a, Crust_Simd128 b)
{
	return vmvnq_u8(crustSimd128EqualF64(a, b));
}

static CRUST_INLINE Crust_Simd128 crustSimd128LessF64(Crust_Simd128 a, Crust_Simd128 b)
{
	return vreinterpretq_u8_u64(vcltq_f64(vreinterpretq_f64_u8(a), vreinterpretq_f64_u8(b)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128LessEqualF64(Crust_Simd128 a, Crust_Simd128 b)
{
	return vreinterpretq_u8_u64(vcleq_f64(vreinterpretq_f64_u8(a), vreinterpretq_f64_u8(b)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128FloorF64(Crust_Simd128 v)
{
	return vreinterpretq_u8_f64(vrndmq_f64(vreinterpretq_f64_u8(v)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128CeilF64(Crust_Simd128 v)
{
	return vreinterpretq_u8_f64(vrndpq_f64(vreinterpretq_f64_u8(v)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128RoundEvenF64(Crust_Simd128 v)
{
	return vreinterpretq_u8_f64(vrndnq_f64(vreinterpretq_f64_u8(v)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128TruncateF64(Crust_Simd128 v)
{
	return vreinterpretq_u8_f64(vrndq_f64(vreinterpretq_f64_u8(v)));
}

//
static CRUST_INLINE Crust_Simd128 crustSimd128ConvertF32FromI32(Crust_Simd128 v)
{
	return vreinterpretq_u8_f32(vcvtq_f32_s32(vreinterpretq_s32_u8(v)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128ConvertI32FromF32(Crust_Simd128 v)
{
	CRUST_ASSERT(crustSimd128All(crustSimd128LessF32(crustSimd128AbsF32(v), crustSimd128SplatF32(2147483648.0f))));
	return vreinterpretq_u8_s32(vcvtq_s32_f32(vreinterpretq_f32_u8(v)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128ConvertI32FromF32Saturate(Crust_Simd128 v)
{
	return vreinterpretq_u8_s32(vcvtq_s32_f32(vreinterpretq_f32_u8(v)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128ConvertF64FromF32Low(Crust_Simd128 v)
{
	return vreinterpretq_u8_f64(vcvt_f64_f32(vget_low_f32(vreinterpretq_f32_u8(v))));
}

static CRUST_INLINE Crust_Simd128 crustSimd128ConvertF64FromF32High(Crust_Simd128 v)
{
	return vreinterpretq_u8_f64(vcvt_high_f64_f32(vreinterpretq_f32_u8(v)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128ConvertF32FromF64(Crust_Simd128 low, Crust_Simd128 high)
{
	return vreinterpretq_u8_f32(vcvt_high_f32_f64(vcvt_f32_f64(vreinterpretq_f64_u8(low)), vreinterpretq_f64_u8(high)));
}

//
static CRUST_INLINE Crust_Simd128 crustSimd128ConvertU16FromU8Low(Crust_Simd128 v)
{
	return vreinterpretq_u8_u16(vmovl_u8(vget_low_u8(v)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128ConvertU16FromU8High(Crust_Simd128 v)
{
	return vreinterpretq_u8_u16(vmovl_high_u8(v));
}

static CRUST_INLINE Crust_Simd128 crustSimd128ConvertI16FromI8Low(Crust_Simd128 v)
{
	return vreinterpretq_u8_s16(vmovl_s8(vget_low_s8(vreinterpretq_s8_u8(v))));
}

static CRUST_INLINE Crust_Simd128 crustSimd128ConvertI16FromI8High(Crust_Simd128 v)
{
	return vreinterpretq_u8_s16(vmovl_high_s8(vreinterpretq_s8_u8(v)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128ConvertU32FromU16Low(Crust_Simd128 v)
{
	return vreinterpretq_u8_u32(vmovl_u16(vget_low_u16(vreinterpretq_u16_u8(v))));
}

static CRUST_INLINE Crust_Simd128 crustSimd128ConvertU32FromU16High(Crust_Simd128 v)
{
	return vreinterpretq_u8_u32(vmovl_high_u16(vreinterpretq_u16_u8(v)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128ConvertI32FromI16Low(Crust_Simd128 v)
{
	return vreinterpretq_u8_s32(vmovl_s16(vget_low_s16(vreinterpretq_s16_u8(v))));
}

static CRUST_INLINE Crust_Simd128 crustSimd128ConvertI32FromI16High(Crust_Simd128 v)
{
	return vreinterpretq_u8_s32(vmovl_high_s16(vreinterpretq_s16_u8(v)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128ConvertU64FromU32Low(Crust_Simd128 v)
{
	return vreinterpretq_u8_u64(vmovl_u32(vget_low_u32(vreinterpretq_u32_u8(v))));
}

static CRUST_INLINE Crust_Simd128 crustSimd128ConvertU64FromU32High(Crust_Simd128 v)
{
	return vreinterpretq_u8_u64(vmovl_high_u32(vreinterpretq_u32_u8(v)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128ConvertI64FromI32Low(Crust_Simd128 v)
{
	return vreinterpretq_u8_s64(vmovl_s32(vget_low_s32(vreinterpretq_s32_u8(v))));
}

static CRUST_INLINE Crust_Simd128 crustSimd128ConvertI64FromI32High(Crust_Simd128 v)
{
	return vreinterpretq_u8_s64(vmovl_high_s32(vreinterpretq_s32_u8(v)));
}

//
static CRUST_INLINE Crust_Simd128 crustSimd128ConvertI8FromI16Saturate(Crust_Simd128 low, Crust_Simd128 high)
{
	return vreinterpretq_u8_s8(vqmovn_high_s16(vqmovn_s16(vreinterpretq_s16_u8(low)), vreinterpretq_s16_u8(high)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128ConvertU8FromI16Saturate(Crust_Simd128 low, Crust_Simd128 high)
{
	return vqmovun_high_s16(vqmovun_s16(vreinterpretq_s16_u8(low)), vreinterpretq_s16_u8(high));
}

static CRUST_INLINE Crust_Simd128 crustSimd128ConvertI16FromI32Saturate(Crust_Simd128 low, Crust_Simd128 high)
{
	return vreinterpretq_u8_s16(vqmovn_high_s32(vqmovn_s32(vreinterpretq_s32_u8(low)), vreinterpretq_s32_u8(high)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128ConvertU16FromI32Saturate(Crust_Simd128 low, Crust_Simd128 high)
{
	return vreinterpretq_u8_u16(vqmovun_high_s32(vqmovun_s32(vreinterpretq_s32_u8(low)), vreinterpretq_s32_u8(high)));
}
