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
static CRUST_INLINE Crust_Simd128 crustSimd128AddU32(Crust_Simd128 a, Crust_Simd128 b)
{
	return vreinterpretq_u8_u32(vaddq_u32(vreinterpretq_u32_u8(a), vreinterpretq_u32_u8(b)));
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
