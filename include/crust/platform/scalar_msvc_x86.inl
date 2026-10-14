//
static CRUST_INLINE f32 crustAbsF32(f32 v)
{
	__m128 x = _mm_set_ss(v);
	__m128 sign = _mm_set1_ps(-0.0f);

	return _mm_cvtss_f32(_mm_andnot_ps(sign, x));
}

static CRUST_INLINE f32 crustSqrtF32(f32 v)
{
	__m128 x = _mm_set_ss(v);
	return _mm_cvtss_f32(_mm_sqrt_ss(x));
}

static CRUST_INLINE f32 crustFloorF32(f32 v)
{
	__m128 x = _mm_set_ss(v);
	return _mm_cvtss_f32(_mm_round_ss(x, x, _MM_FROUND_TO_NEG_INF | _MM_FROUND_NO_EXC));
}

static CRUST_INLINE f32 crustCeilF32(f32 v)
{
	__m128 x = _mm_set_ss(v);
	return _mm_cvtss_f32(_mm_round_ss(x, x, _MM_FROUND_TO_POS_INF | _MM_FROUND_NO_EXC));
}

static CRUST_INLINE f32 crustRoundEvenF32(f32 v)
{
	__m128 x = _mm_set_ss(v);
	return _mm_cvtss_f32(_mm_round_ss(x, x, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC));
}

static CRUST_INLINE f32 crustTruncateF32(f32 v)
{
	__m128 x = _mm_set_ss(v);
	return _mm_cvtss_f32(_mm_round_ss(x, x, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC));
}

static CRUST_INLINE f64 crustAbsF64(f64 v)
{
	__m128d x = _mm_set_sd(v);
	__m128d sign = _mm_set1_pd(-0.0);

	return _mm_cvtsd_f64(_mm_andnot_pd(sign, x));
}

static CRUST_INLINE f64 crustSqrtF64(f64 v)
{
	__m128d x = _mm_set_sd(v);
	return _mm_cvtsd_f64(_mm_sqrt_sd(x, x));
}

static CRUST_INLINE f64 crustFloorF64(f64 v)
{
	__m128d x = _mm_set_sd(v);
	return _mm_cvtsd_f64(_mm_round_sd(x, x, _MM_FROUND_TO_NEG_INF | _MM_FROUND_NO_EXC));
}

static CRUST_INLINE f64 crustCeilF64(f64 v)
{
	__m128d x = _mm_set_sd(v);
	return _mm_cvtsd_f64(_mm_round_sd(x, x, _MM_FROUND_TO_POS_INF | _MM_FROUND_NO_EXC));
}

static CRUST_INLINE f64 crustRoundEvenF64(f64 v)
{
	__m128d x = _mm_set_sd(v);
	return _mm_cvtsd_f64(_mm_round_sd(x, x, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC));
}

static CRUST_INLINE f64 crustTruncateF64(f64 v)
{
	__m128d x = _mm_set_sd(v);
	return _mm_cvtsd_f64(_mm_round_sd(x, x, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC));
}

//
static CRUST_INLINE u32 crustLzcntU32(u32 value)
{
	CRUST_ASSERT(value != 0);

	unsigned long result;
	_BitScanReverse(&result, value);
	return 31u - (u32)result;
}

static CRUST_INLINE u32 crustTzcntU32(u32 value)
{
	CRUST_ASSERT(value != 0);

	unsigned long result;
	_BitScanForward(&result, value);
	return (u32)result;
}

static CRUST_INLINE u32 crustPopcntU32(u32 value)
{
	return (u32)__popcnt(value);
}
