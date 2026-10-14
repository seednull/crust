//
static CRUST_INLINE f32 crustAbsF32(f32 v)
{
	float32x2_t x = vdup_n_f32(v);
	return vget_lane_f32(vabs_f32(x), 0);
}

static CRUST_INLINE f32 crustSqrtF32(f32 v)
{
	float32x2_t x = vdup_n_f32(v);
	return vget_lane_f32(vsqrt_f32(x), 0);
}

static CRUST_INLINE f32 crustFloorF32(f32 v)
{
	float32x2_t x = vdup_n_f32(v);
	return vget_lane_f32(vrndm_f32(x), 0);
}

static CRUST_INLINE f32 crustCeilF32(f32 v)
{
	float32x2_t x = vdup_n_f32(v);
	return vget_lane_f32(vrndp_f32(x), 0);
}

static CRUST_INLINE f32 crustRoundEvenF32(f32 v)
{
	float32x2_t x = vdup_n_f32(v);
	return vget_lane_f32(vrndn_f32(x), 0);
}

static CRUST_INLINE f32 crustTruncateF32(f32 v)
{
	float32x2_t x = vdup_n_f32(v);
	return vget_lane_f32(vrnd_f32(x), 0);
}

static CRUST_INLINE f64 crustAbsF64(f64 v)
{
	float64x1_t x = vdup_n_f64(v);
	return vget_lane_f64(vabs_f64(x), 0);
}

static CRUST_INLINE f64 crustSqrtF64(f64 v)
{
	float64x1_t x = vdup_n_f64(v);
	return vget_lane_f64(vsqrt_f64(x), 0);
}

static CRUST_INLINE f64 crustFloorF64(f64 v)
{
	float64x1_t x = vdup_n_f64(v);
	return vget_lane_f64(vrndm_f64(x), 0);
}

static CRUST_INLINE f64 crustCeilF64(f64 v)
{
	float64x1_t x = vdup_n_f64(v);
	return vget_lane_f64(vrndp_f64(x), 0);
}

static CRUST_INLINE f64 crustRoundEvenF64(f64 v)
{
	float64x1_t x = vdup_n_f64(v);
	return vget_lane_f64(vrndn_f64(x), 0);
}

static CRUST_INLINE f64 crustTruncateF64(f64 v)
{
	float64x1_t x = vdup_n_f64(v);
	return vget_lane_f64(vrnd_f64(x), 0);
}

//
static CRUST_INLINE u32 crustLzcntU32(u32 value)
{
	CRUST_ASSERT(value != 0);
	return (u32)_CountLeadingZeros(value);
}

static CRUST_INLINE u32 crustTzcntU32(u32 value)
{
	CRUST_ASSERT(value != 0);
	return (u32)_CountTrailingZeros(value);
}

static CRUST_INLINE u32 crustPopcntU32(u32 value)
{
	return (u32)_CountOneBits(value);
}
