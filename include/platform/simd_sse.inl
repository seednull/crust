//
static CRUST_INLINE Crust_Simd128 crustSimd128AllZeros(void)
{
	return _mm_setzero_si128();
}

static CRUST_INLINE Crust_Simd128 crustSimd128AllOnes(void)
{
	return _mm_set1_epi32(-1);
}

//
static CRUST_INLINE Crust_Simd128 crustSimd128LoadU8(const u8 *ptr)
{
#if CRUST_COMPILER_MSVC
	return _mm_loadu_si128((const __m128i *)ptr);
#else
	return _mm_loadu_si128((const __m128i_u *)ptr);
#endif
}

static CRUST_INLINE Crust_Simd128 crustSimd128LoadU16(const u16 *ptr)
{
#if CRUST_COMPILER_MSVC
	return _mm_loadu_si128((const __m128i *)ptr);
#else
	return _mm_loadu_si128((const __m128i_u *)ptr);
#endif
}

static CRUST_INLINE Crust_Simd128 crustSimd128LoadU32(const u32 *ptr)
{
#if CRUST_COMPILER_MSVC
	return _mm_loadu_si128((const __m128i *)ptr);
#else
	return _mm_loadu_si128((const __m128i_u *)ptr);
#endif
}

static CRUST_INLINE Crust_Simd128 crustSimd128LoadU64(const u64 *ptr)
{
#if CRUST_COMPILER_MSVC
	return _mm_loadu_si128((const __m128i *)ptr);
#else
	return _mm_loadu_si128((const __m128i_u *)ptr);
#endif
}

static CRUST_INLINE Crust_Simd128 crustSimd128LoadF32(const f32 *ptr)
{
	return _mm_castps_si128(_mm_loadu_ps(ptr));
}

static CRUST_INLINE Crust_Simd128 crustSimd128LoadF64(const f64 *ptr)
{
	return _mm_castpd_si128(_mm_loadu_pd(ptr));
}

static CRUST_INLINE Crust_Simd128 crustSimd128LoadAlignedU8(const u8 *ptr)
{
	CRUST_ASSERT(crustIsAlignedUSize((usize)ptr, 16));
	return _mm_load_si128((const __m128i *)(const void *)ptr);
}

static CRUST_INLINE Crust_Simd128 crustSimd128LoadAlignedU16(const u16 *ptr)
{
	CRUST_ASSERT(crustIsAlignedUSize((usize)ptr, 16));
	return _mm_load_si128((const __m128i *)(const void *)ptr);
}

static CRUST_INLINE Crust_Simd128 crustSimd128LoadAlignedU32(const u32 *ptr)
{
	CRUST_ASSERT(crustIsAlignedUSize((usize)ptr, 16));
	return _mm_load_si128((const __m128i *)(const void *)ptr);
}

static CRUST_INLINE Crust_Simd128 crustSimd128LoadAlignedU64(const u64 *ptr)
{
	CRUST_ASSERT(crustIsAlignedUSize((usize)ptr, 16));
	return _mm_load_si128((const __m128i *)(const void *)ptr);
}

static CRUST_INLINE Crust_Simd128 crustSimd128LoadAlignedF32(const f32 *ptr)
{
	CRUST_ASSERT(crustIsAlignedUSize((usize)ptr, 16));
	return _mm_castps_si128(_mm_load_ps(ptr));
}

static CRUST_INLINE Crust_Simd128 crustSimd128LoadAlignedF64(const f64 *ptr)
{
	CRUST_ASSERT(crustIsAlignedUSize((usize)ptr, 16));
	return _mm_castpd_si128(_mm_load_pd(ptr));
}

static CRUST_INLINE void crustSimd128StoreU8(u8 *ptr, Crust_Simd128 v)
{
#if CRUST_COMPILER_MSVC
	_mm_storeu_si128((__m128i *)ptr, v);
#else
	_mm_storeu_si128((__m128i_u *)ptr, v);
#endif
}

static CRUST_INLINE void crustSimd128StoreU16(u16 *ptr, Crust_Simd128 v)
{
#if CRUST_COMPILER_MSVC
	_mm_storeu_si128((__m128i *)ptr, v);
#else
	_mm_storeu_si128((__m128i_u *)ptr, v);
#endif
}

static CRUST_INLINE void crustSimd128StoreU32(u32 *ptr, Crust_Simd128 v)
{
#if CRUST_COMPILER_MSVC
	_mm_storeu_si128((__m128i *)ptr, v);
#else
	_mm_storeu_si128((__m128i_u *)ptr, v);
#endif
}

static CRUST_INLINE void crustSimd128StoreU64(u64 *ptr, Crust_Simd128 v)
{
#if CRUST_COMPILER_MSVC
	_mm_storeu_si128((__m128i *)ptr, v);
#else
	_mm_storeu_si128((__m128i_u *)ptr, v);
#endif
}

static CRUST_INLINE void crustSimd128StoreF32(f32 *ptr, Crust_Simd128 v)
{
	_mm_storeu_ps(ptr, _mm_castsi128_ps(v));
}

static CRUST_INLINE void crustSimd128StoreF64(f64 *ptr, Crust_Simd128 v)
{
	_mm_storeu_pd(ptr, _mm_castsi128_pd(v));
}

static CRUST_INLINE void crustSimd128StoreAlignedU8(u8 *ptr, Crust_Simd128 v)
{
	CRUST_ASSERT(crustIsAlignedUSize((usize)ptr, 16));
	_mm_store_si128((__m128i *)(void *)ptr, v);
}

static CRUST_INLINE void crustSimd128StoreAlignedU16(u16 *ptr, Crust_Simd128 v)
{
	CRUST_ASSERT(crustIsAlignedUSize((usize)ptr, 16));
	_mm_store_si128((__m128i *)(void *)ptr, v);
}

static CRUST_INLINE void crustSimd128StoreAlignedU32(u32 *ptr, Crust_Simd128 v)
{
	CRUST_ASSERT(crustIsAlignedUSize((usize)ptr, 16));
	_mm_store_si128((__m128i *)(void *)ptr, v);
}

static CRUST_INLINE void crustSimd128StoreAlignedU64(u64 *ptr, Crust_Simd128 v)
{
	CRUST_ASSERT(crustIsAlignedUSize((usize)ptr, 16));
	_mm_store_si128((__m128i *)(void *)ptr, v);
}

static CRUST_INLINE void crustSimd128StoreAlignedF32(f32 *ptr, Crust_Simd128 v)
{
	CRUST_ASSERT(crustIsAlignedUSize((usize)ptr, 16));
	_mm_store_ps(ptr, _mm_castsi128_ps(v));
}

static CRUST_INLINE void crustSimd128StoreAlignedF64(f64 *ptr, Crust_Simd128 v)
{
	CRUST_ASSERT(crustIsAlignedUSize((usize)ptr, 16));
	_mm_store_pd(ptr, _mm_castsi128_pd(v));
}

//
static CRUST_INLINE Crust_Simd128 crustSimd128SplatU8(u8 v)
{
	return _mm_set1_epi8((char)v);
}

static CRUST_INLINE Crust_Simd128 crustSimd128SplatU16(u16 v)
{
	return _mm_set1_epi16((short)v);
}

static CRUST_INLINE Crust_Simd128 crustSimd128SplatU32(u32 v)
{
	return _mm_set1_epi32((int)v);
}

static CRUST_INLINE Crust_Simd128 crustSimd128SplatU64(u64 v)
{
	return _mm_set1_epi64x((long long)v);
}

static CRUST_INLINE Crust_Simd128 crustSimd128SplatF32(f32 v)
{
	return _mm_castps_si128(_mm_set1_ps(v));
}

static CRUST_INLINE Crust_Simd128 crustSimd128SplatF64(f64 v)
{
	return _mm_castpd_si128(_mm_set1_pd(v));
}

static CRUST_INLINE Crust_Simd128 crustSimd128MakeU32(u32 v0, u32 v1, u32 v2, u32 v3)
{
	return _mm_setr_epi32((int)v0, (int)v1, (int)v2, (int)v3);
}

static CRUST_INLINE Crust_Simd128 crustSimd128MakeF32(f32 v0, f32 v1, f32 v2, f32 v3)
{
	return _mm_castps_si128(_mm_setr_ps(v0, v1, v2, v3));
}

static CRUST_INLINE Crust_Simd128 crustSimd128MakeU64(u64 v0, u64 v1)
{
	return _mm_set_epi64x((long long)v1, (long long)v0);
}

static CRUST_INLINE Crust_Simd128 crustSimd128MakeF64(f64 v0, f64 v1)
{
	return _mm_castpd_si128(_mm_setr_pd(v0, v1));
}

//
static CRUST_INLINE Crust_Simd128 crustSimd128And(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_and_si128(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128Or(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_or_si128(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128Xor(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_xor_si128(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128AndNot(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_andnot_si128(b, a);
}

static CRUST_INLINE Crust_Simd128 crustSimd128Not(Crust_Simd128 v)
{
	return _mm_xor_si128(v, _mm_set1_epi32(-1));
}

static CRUST_INLINE Crust_Simd128 crustSimd128Select(Crust_Simd128 mask, Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_or_si128(_mm_and_si128(mask, a), _mm_andnot_si128(mask, b));
}

//
static CRUST_INLINE u32 crustSimd128Any(Crust_Simd128 v)
{
	return (u32)!_mm_testz_si128(v, v);
}

static CRUST_INLINE u32 crustSimd128All(Crust_Simd128 v)
{
	return (u32)_mm_testc_si128(v, _mm_set1_epi32(-1));
}

static CRUST_INLINE u32 crustSimd128BitmaskU8(Crust_Simd128 v)
{
	return (u32)_mm_movemask_epi8(v);
}

static CRUST_INLINE u32 crustSimd128BitmaskU16(Crust_Simd128 v)
{
	return (u32)_mm_movemask_epi8(_mm_packs_epi16(v, _mm_setzero_si128()));
}

static CRUST_INLINE u32 crustSimd128BitmaskU32(Crust_Simd128 v)
{
	return (u32)_mm_movemask_ps(_mm_castsi128_ps(v));
}

static CRUST_INLINE u32 crustSimd128BitmaskU64(Crust_Simd128 v)
{
	return (u32)_mm_movemask_pd(_mm_castsi128_pd(v));
}

//
static CRUST_INLINE Crust_Simd128 crustSimd128AddU32(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_add_epi32(a, b);
}

//
static CRUST_INLINE Crust_Simd128 crustSimd128AddF32(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_castps_si128(_mm_add_ps(_mm_castsi128_ps(a), _mm_castsi128_ps(b)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128SubF32(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_castps_si128(_mm_sub_ps(_mm_castsi128_ps(a), _mm_castsi128_ps(b)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128MulF32(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_castps_si128(_mm_mul_ps(_mm_castsi128_ps(a), _mm_castsi128_ps(b)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128DivF32(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_castps_si128(_mm_div_ps(_mm_castsi128_ps(a), _mm_castsi128_ps(b)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128SqrtF32(Crust_Simd128 v)
{
	return _mm_castps_si128(_mm_sqrt_ps(_mm_castsi128_ps(v)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128AbsF32(Crust_Simd128 v)
{
	return _mm_andnot_si128(_mm_set1_epi32(CRUST_I32_MIN), v);
}

static CRUST_INLINE Crust_Simd128 crustSimd128NegateF32(Crust_Simd128 v)
{
	return _mm_xor_si128(v, _mm_set1_epi32(CRUST_I32_MIN));
}

static CRUST_INLINE Crust_Simd128 crustSimd128MinF32(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_castps_si128(_mm_min_ps(_mm_castsi128_ps(a), _mm_castsi128_ps(b)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128MaxF32(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_castps_si128(_mm_max_ps(_mm_castsi128_ps(b), _mm_castsi128_ps(a)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128EqualF32(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_castps_si128(_mm_cmpeq_ps(_mm_castsi128_ps(a), _mm_castsi128_ps(b)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128NotEqualF32(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_castps_si128(_mm_cmpneq_ps(_mm_castsi128_ps(a), _mm_castsi128_ps(b)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128LessF32(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_castps_si128(_mm_cmplt_ps(_mm_castsi128_ps(a), _mm_castsi128_ps(b)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128LessEqualF32(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_castps_si128(_mm_cmple_ps(_mm_castsi128_ps(a), _mm_castsi128_ps(b)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128FloorF32(Crust_Simd128 v)
{
	return _mm_castps_si128(_mm_round_ps(_mm_castsi128_ps(v), _MM_FROUND_TO_NEG_INF | _MM_FROUND_NO_EXC));
}

static CRUST_INLINE Crust_Simd128 crustSimd128CeilF32(Crust_Simd128 v)
{
	return _mm_castps_si128(_mm_round_ps(_mm_castsi128_ps(v), _MM_FROUND_TO_POS_INF | _MM_FROUND_NO_EXC));
}

static CRUST_INLINE Crust_Simd128 crustSimd128RoundEvenF32(Crust_Simd128 v)
{
	return _mm_castps_si128(_mm_round_ps(_mm_castsi128_ps(v), _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC));
}

static CRUST_INLINE Crust_Simd128 crustSimd128TruncateF32(Crust_Simd128 v)
{
	return _mm_castps_si128(_mm_round_ps(_mm_castsi128_ps(v), _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC));
}

//
static CRUST_INLINE Crust_Simd128 crustSimd128AddF64(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_castpd_si128(_mm_add_pd(_mm_castsi128_pd(a), _mm_castsi128_pd(b)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128SubF64(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_castpd_si128(_mm_sub_pd(_mm_castsi128_pd(a), _mm_castsi128_pd(b)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128MulF64(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_castpd_si128(_mm_mul_pd(_mm_castsi128_pd(a), _mm_castsi128_pd(b)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128DivF64(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_castpd_si128(_mm_div_pd(_mm_castsi128_pd(a), _mm_castsi128_pd(b)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128SqrtF64(Crust_Simd128 v)
{
	return _mm_castpd_si128(_mm_sqrt_pd(_mm_castsi128_pd(v)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128AbsF64(Crust_Simd128 v)
{
	return _mm_andnot_si128(_mm_set1_epi64x(CRUST_I64_MIN), v);
}

static CRUST_INLINE Crust_Simd128 crustSimd128NegateF64(Crust_Simd128 v)
{
	return _mm_xor_si128(v, _mm_set1_epi64x(CRUST_I64_MIN));
}

static CRUST_INLINE Crust_Simd128 crustSimd128MinF64(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_castpd_si128(_mm_min_pd(_mm_castsi128_pd(a), _mm_castsi128_pd(b)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128MaxF64(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_castpd_si128(_mm_max_pd(_mm_castsi128_pd(b), _mm_castsi128_pd(a)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128EqualF64(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_castpd_si128(_mm_cmpeq_pd(_mm_castsi128_pd(a), _mm_castsi128_pd(b)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128NotEqualF64(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_castpd_si128(_mm_cmpneq_pd(_mm_castsi128_pd(a), _mm_castsi128_pd(b)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128LessF64(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_castpd_si128(_mm_cmplt_pd(_mm_castsi128_pd(a), _mm_castsi128_pd(b)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128LessEqualF64(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_castpd_si128(_mm_cmple_pd(_mm_castsi128_pd(a), _mm_castsi128_pd(b)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128FloorF64(Crust_Simd128 v)
{
	return _mm_castpd_si128(_mm_round_pd(_mm_castsi128_pd(v), _MM_FROUND_TO_NEG_INF | _MM_FROUND_NO_EXC));
}

static CRUST_INLINE Crust_Simd128 crustSimd128CeilF64(Crust_Simd128 v)
{
	return _mm_castpd_si128(_mm_round_pd(_mm_castsi128_pd(v), _MM_FROUND_TO_POS_INF | _MM_FROUND_NO_EXC));
}

static CRUST_INLINE Crust_Simd128 crustSimd128RoundEvenF64(Crust_Simd128 v)
{
	return _mm_castpd_si128(_mm_round_pd(_mm_castsi128_pd(v), _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC));
}

static CRUST_INLINE Crust_Simd128 crustSimd128TruncateF64(Crust_Simd128 v)
{
	return _mm_castpd_si128(_mm_round_pd(_mm_castsi128_pd(v), _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC));
}

//
static CRUST_INLINE Crust_Simd128 crustSimd128ConvertF32FromI32(Crust_Simd128 v)
{
	return _mm_castps_si128(_mm_cvtepi32_ps(v));
}

static CRUST_INLINE Crust_Simd128 crustSimd128ConvertI32FromF32(Crust_Simd128 v)
{
	CRUST_ASSERT(crustSimd128All(crustSimd128LessF32(crustSimd128AbsF32(v), crustSimd128SplatF32(2147483648.0f))));
	return _mm_cvttps_epi32(_mm_castsi128_ps(v));
}

static CRUST_INLINE Crust_Simd128 crustSimd128ConvertI32FromF32Saturate(Crust_Simd128 v)
{
	__m128 x = _mm_castsi128_ps(v);
	__m128i result = _mm_cvttps_epi32(x);
	__m128i overflow = _mm_castps_si128(_mm_cmpge_ps(x, _mm_set1_ps(2147483648.0f)));
	__m128i ordered = _mm_castps_si128(_mm_cmpeq_ps(x, x));

	return _mm_and_si128(_mm_xor_si128(result, overflow), ordered);
}

static CRUST_INLINE Crust_Simd128 crustSimd128ConvertF64FromF32Low(Crust_Simd128 v)
{
	return _mm_castpd_si128(_mm_cvtps_pd(_mm_castsi128_ps(v)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128ConvertF64FromF32High(Crust_Simd128 v)
{
	__m128 x = _mm_castsi128_ps(v);
	return _mm_castpd_si128(_mm_cvtps_pd(_mm_movehl_ps(x, x)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128ConvertF32FromF64(Crust_Simd128 low, Crust_Simd128 high)
{
	return _mm_castps_si128(_mm_movelh_ps(_mm_cvtpd_ps(_mm_castsi128_pd(low)), _mm_cvtpd_ps(_mm_castsi128_pd(high))));
}
