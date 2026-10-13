//
static CRUST_INLINE Crust_Simd128 crustSimd128Zero(void)
{
	return vdupq_n_u8(0);
}

static CRUST_INLINE Crust_Simd128 crustSimd128LoadU(const void *ptr)
{
	return vld1q_u8((const u8 *)ptr);
}

static CRUST_INLINE void crustSimd128StoreU(void *ptr, Crust_Simd128 v)
{
	vst1q_u8((u8 *)ptr, v);
}

//
static CRUST_INLINE Crust_Simd128 crustSimd128SplatU32(u32 v)
{
	return vreinterpretq_u8_u32(vdupq_n_u32(v));
}

static CRUST_INLINE Crust_Simd128 crustSimd128SplatF32(f32 v)
{
	return vreinterpretq_u8_f32(vdupq_n_f32(v));
}

static CRUST_INLINE Crust_Simd128 crustSimd128SetU32(u32 v0, u32 v1, u32 v2, u32 v3)
{
	u32 lanes[4] = { v0, v1, v2, v3 };
	return vreinterpretq_u8_u32(vld1q_u32(lanes));
}

static CRUST_INLINE Crust_Simd128 crustSimd128SetF32(f32 v0, f32 v1, f32 v2, f32 v3)
{
	f32 lanes[4] = { v0, v1, v2, v3 };
	return vreinterpretq_u8_f32(vld1q_f32(lanes));
}

//
static CRUST_INLINE Crust_Simd128 crustSimd128Select(Crust_Simd128 mask, Crust_Simd128 a, Crust_Simd128 b)
{
	return vbslq_u8(mask, a, b);
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

static CRUST_INLINE Crust_Simd128 crustSimd128MulF32(Crust_Simd128 a, Crust_Simd128 b)
{
	return vreinterpretq_u8_f32(vmulq_f32(vreinterpretq_f32_u8(a), vreinterpretq_f32_u8(b)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128LtF32(Crust_Simd128 a, Crust_Simd128 b)
{
	return vreinterpretq_u8_u32(vcltq_f32(vreinterpretq_f32_u8(a), vreinterpretq_f32_u8(b)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128MinF32(Crust_Simd128 a, Crust_Simd128 b)
{
	return crustSimd128Select(crustSimd128LtF32(a, b), a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128MaxF32(Crust_Simd128 a, Crust_Simd128 b)
{
	return crustSimd128Select(crustSimd128LtF32(a, b), b, a);
}
