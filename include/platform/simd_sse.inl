//
typedef __m128i Crust_Simd128;

//
static CRUST_INLINE Crust_Simd128 crustSimd128Zero(void)
{
	return _mm_setzero_si128();
}

static CRUST_INLINE Crust_Simd128 crustSimd128LoadU(const void *ptr)
{
	return _mm_loadu_si128((const __m128i *)ptr);
}

static CRUST_INLINE void crustSimd128StoreU(void *ptr, Crust_Simd128 v)
{
	_mm_storeu_si128((__m128i *)ptr, v);
}

//
static CRUST_INLINE Crust_Simd128 crustSimd128SplatU32(u32 v)
{
	return _mm_set1_epi32((int)v);
}

static CRUST_INLINE Crust_Simd128 crustSimd128SplatF32(f32 v)
{
	return _mm_castps_si128(_mm_set1_ps(v));
}

static CRUST_INLINE Crust_Simd128 crustSimd128SetU32(u32 v0, u32 v1, u32 v2, u32 v3)
{
	return _mm_setr_epi32((int)v0, (int)v1, (int)v2, (int)v3);
}

static CRUST_INLINE Crust_Simd128 crustSimd128SetF32(f32 v0, f32 v1, f32 v2, f32 v3)
{
	return _mm_castps_si128(_mm_setr_ps(v0, v1, v2, v3));
}

//
static CRUST_INLINE Crust_Simd128 crustSimd128Select(Crust_Simd128 mask, Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_or_si128(_mm_and_si128(mask, a), _mm_andnot_si128(mask, b));
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

static CRUST_INLINE Crust_Simd128 crustSimd128MulF32(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_castps_si128(_mm_mul_ps(_mm_castsi128_ps(a), _mm_castsi128_ps(b)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128LtF32(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_castps_si128(_mm_cmplt_ps(_mm_castsi128_ps(a), _mm_castsi128_ps(b)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128MinF32(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_castps_si128(_mm_min_ps(_mm_castsi128_ps(a), _mm_castsi128_ps(b)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128MaxF32(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_castps_si128(_mm_max_ps(_mm_castsi128_ps(b), _mm_castsi128_ps(a)));
}
