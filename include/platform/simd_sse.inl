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
static CRUST_INLINE Crust_Simd128 crustSimd128ZipLowU8(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_unpacklo_epi8(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128ZipHighU8(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_unpackhi_epi8(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128ZipLowU16(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_unpacklo_epi16(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128ZipHighU16(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_unpackhi_epi16(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128ZipLowU32(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_unpacklo_epi32(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128ZipHighU32(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_unpackhi_epi32(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128ZipLowU64(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_unpacklo_epi64(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128ZipHighU64(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_unpackhi_epi64(a, b);
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
static CRUST_INLINE Crust_Simd128 crustSimd128AddU8(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_add_epi8(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128AddU16(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_add_epi16(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128AddU32(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_add_epi32(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128AddU64(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_add_epi64(a, b);
}

//
static CRUST_INLINE Crust_Simd128 crustSimd128SubU8(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_sub_epi8(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128SubU16(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_sub_epi16(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128SubU32(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_sub_epi32(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128SubU64(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_sub_epi64(a, b);
}

//
static CRUST_INLINE Crust_Simd128 crustSimd128AddSaturateU8(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_adds_epu8(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128AddSaturateI8(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_adds_epi8(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128AddSaturateU16(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_adds_epu16(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128AddSaturateI16(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_adds_epi16(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128SubSaturateU8(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_subs_epu8(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128SubSaturateI8(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_subs_epi8(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128SubSaturateU16(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_subs_epu16(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128SubSaturateI16(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_subs_epi16(a, b);
}

//
static CRUST_INLINE Crust_Simd128 crustSimd128MulU16(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_mullo_epi16(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128MulU32(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_mullo_epi32(a, b);
}

//
static CRUST_INLINE Crust_Simd128 crustSimd128MinU8(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_min_epu8(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128MinI8(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_min_epi8(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128MinU16(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_min_epu16(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128MinI16(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_min_epi16(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128MinU32(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_min_epu32(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128MinI32(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_min_epi32(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128MaxU8(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_max_epu8(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128MaxI8(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_max_epi8(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128MaxU16(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_max_epu16(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128MaxI16(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_max_epi16(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128MaxU32(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_max_epu32(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128MaxI32(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_max_epi32(a, b);
}

//
static CRUST_INLINE Crust_Simd128 crustSimd128ShiftLeftU8(Crust_Simd128 v, u32 count)
{
	CRUST_ASSERT(count < 8);
	return _mm_and_si128(_mm_sll_epi16(v, _mm_cvtsi32_si128((int)count)), _mm_set1_epi8((char)(CRUST_U8_MAX << count)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128ShiftLeftU16(Crust_Simd128 v, u32 count)
{
	CRUST_ASSERT(count < 16);
	return _mm_sll_epi16(v, _mm_cvtsi32_si128((int)count));
}

static CRUST_INLINE Crust_Simd128 crustSimd128ShiftLeftU32(Crust_Simd128 v, u32 count)
{
	CRUST_ASSERT(count < 32);
	return _mm_sll_epi32(v, _mm_cvtsi32_si128((int)count));
}

static CRUST_INLINE Crust_Simd128 crustSimd128ShiftLeftU64(Crust_Simd128 v, u32 count)
{
	CRUST_ASSERT(count < 64);
	return _mm_sll_epi64(v, _mm_cvtsi32_si128((int)count));
}

static CRUST_INLINE Crust_Simd128 crustSimd128ShiftRightU8(Crust_Simd128 v, u32 count)
{
	CRUST_ASSERT(count < 8);
	return _mm_and_si128(_mm_srl_epi16(v, _mm_cvtsi32_si128((int)count)), _mm_set1_epi8((char)(CRUST_U8_MAX >> count)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128ShiftRightI8(Crust_Simd128 v, u32 count)
{
	CRUST_ASSERT(count < 8);

	__m128i sign = _mm_set1_epi8(CRUST_I8_MIN);
	return _mm_sub_epi8(crustSimd128ShiftRightU8(_mm_xor_si128(v, sign), count), _mm_set1_epi8((char)(0x80u >> count)));
}

static CRUST_INLINE Crust_Simd128 crustSimd128ShiftRightU16(Crust_Simd128 v, u32 count)
{
	CRUST_ASSERT(count < 16);
	return _mm_srl_epi16(v, _mm_cvtsi32_si128((int)count));
}

static CRUST_INLINE Crust_Simd128 crustSimd128ShiftRightI16(Crust_Simd128 v, u32 count)
{
	CRUST_ASSERT(count < 16);
	return _mm_sra_epi16(v, _mm_cvtsi32_si128((int)count));
}

static CRUST_INLINE Crust_Simd128 crustSimd128ShiftRightU32(Crust_Simd128 v, u32 count)
{
	CRUST_ASSERT(count < 32);
	return _mm_srl_epi32(v, _mm_cvtsi32_si128((int)count));
}

static CRUST_INLINE Crust_Simd128 crustSimd128ShiftRightI32(Crust_Simd128 v, u32 count)
{
	CRUST_ASSERT(count < 32);
	return _mm_sra_epi32(v, _mm_cvtsi32_si128((int)count));
}

static CRUST_INLINE Crust_Simd128 crustSimd128ShiftRightU64(Crust_Simd128 v, u32 count)
{
	CRUST_ASSERT(count < 64);
	return _mm_srl_epi64(v, _mm_cvtsi32_si128((int)count));
}

static CRUST_INLINE Crust_Simd128 crustSimd128ShiftRightI64(Crust_Simd128 v, u32 count)
{
	CRUST_ASSERT(count < 64);

	__m128i sign = _mm_cmpgt_epi64(_mm_setzero_si128(), v);
	return _mm_xor_si128(_mm_srl_epi64(_mm_xor_si128(v, sign), _mm_cvtsi32_si128((int)count)), sign);
}

//
static CRUST_INLINE Crust_Simd128 crustSimd128EqualU8(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_cmpeq_epi8(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128EqualU16(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_cmpeq_epi16(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128EqualU32(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_cmpeq_epi32(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128EqualU64(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_cmpeq_epi64(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128LessU8(Crust_Simd128 a, Crust_Simd128 b)
{
	__m128i sign = _mm_set1_epi8(CRUST_I8_MIN);
	return _mm_cmplt_epi8(_mm_xor_si128(a, sign), _mm_xor_si128(b, sign));
}

static CRUST_INLINE Crust_Simd128 crustSimd128LessI8(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_cmplt_epi8(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128LessU16(Crust_Simd128 a, Crust_Simd128 b)
{
	__m128i sign = _mm_set1_epi16(CRUST_I16_MIN);
	return _mm_cmplt_epi16(_mm_xor_si128(a, sign), _mm_xor_si128(b, sign));
}

static CRUST_INLINE Crust_Simd128 crustSimd128LessI16(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_cmplt_epi16(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128LessU32(Crust_Simd128 a, Crust_Simd128 b)
{
	__m128i sign = _mm_set1_epi32(CRUST_I32_MIN);
	return _mm_cmplt_epi32(_mm_xor_si128(a, sign), _mm_xor_si128(b, sign));
}

static CRUST_INLINE Crust_Simd128 crustSimd128LessI32(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_cmplt_epi32(a, b);
}

static CRUST_INLINE Crust_Simd128 crustSimd128LessU64(Crust_Simd128 a, Crust_Simd128 b)
{
	__m128i sign = _mm_set1_epi64x(CRUST_I64_MIN);
	return _mm_cmpgt_epi64(_mm_xor_si128(b, sign), _mm_xor_si128(a, sign));
}

static CRUST_INLINE Crust_Simd128 crustSimd128LessI64(Crust_Simd128 a, Crust_Simd128 b)
{
	return _mm_cmpgt_epi64(b, a);
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

//
static CRUST_INLINE Crust_Simd128 crustSimd128ConvertU16FromU8Low(Crust_Simd128 v)
{
	return _mm_cvtepu8_epi16(v);
}

static CRUST_INLINE Crust_Simd128 crustSimd128ConvertU16FromU8High(Crust_Simd128 v)
{
	return _mm_unpackhi_epi8(v, _mm_setzero_si128());
}

static CRUST_INLINE Crust_Simd128 crustSimd128ConvertI16FromI8Low(Crust_Simd128 v)
{
	return _mm_cvtepi8_epi16(v);
}

static CRUST_INLINE Crust_Simd128 crustSimd128ConvertI16FromI8High(Crust_Simd128 v)
{
	return _mm_cvtepi8_epi16(_mm_unpackhi_epi64(v, v));
}

static CRUST_INLINE Crust_Simd128 crustSimd128ConvertU32FromU16Low(Crust_Simd128 v)
{
	return _mm_cvtepu16_epi32(v);
}

static CRUST_INLINE Crust_Simd128 crustSimd128ConvertU32FromU16High(Crust_Simd128 v)
{
	return _mm_unpackhi_epi16(v, _mm_setzero_si128());
}

static CRUST_INLINE Crust_Simd128 crustSimd128ConvertI32FromI16Low(Crust_Simd128 v)
{
	return _mm_cvtepi16_epi32(v);
}

static CRUST_INLINE Crust_Simd128 crustSimd128ConvertI32FromI16High(Crust_Simd128 v)
{
	return _mm_cvtepi16_epi32(_mm_unpackhi_epi64(v, v));
}

static CRUST_INLINE Crust_Simd128 crustSimd128ConvertU64FromU32Low(Crust_Simd128 v)
{
	return _mm_cvtepu32_epi64(v);
}

static CRUST_INLINE Crust_Simd128 crustSimd128ConvertU64FromU32High(Crust_Simd128 v)
{
	return _mm_unpackhi_epi32(v, _mm_setzero_si128());
}

static CRUST_INLINE Crust_Simd128 crustSimd128ConvertI64FromI32Low(Crust_Simd128 v)
{
	return _mm_cvtepi32_epi64(v);
}

static CRUST_INLINE Crust_Simd128 crustSimd128ConvertI64FromI32High(Crust_Simd128 v)
{
	return _mm_cvtepi32_epi64(_mm_unpackhi_epi64(v, v));
}

//
static CRUST_INLINE Crust_Simd128 crustSimd128ConvertI8FromI16Saturate(Crust_Simd128 low, Crust_Simd128 high)
{
	return _mm_packs_epi16(low, high);
}

static CRUST_INLINE Crust_Simd128 crustSimd128ConvertU8FromI16Saturate(Crust_Simd128 low, Crust_Simd128 high)
{
	return _mm_packus_epi16(low, high);
}

static CRUST_INLINE Crust_Simd128 crustSimd128ConvertI16FromI32Saturate(Crust_Simd128 low, Crust_Simd128 high)
{
	return _mm_packs_epi32(low, high);
}

static CRUST_INLINE Crust_Simd128 crustSimd128ConvertU16FromI32Saturate(Crust_Simd128 low, Crust_Simd128 high)
{
	return _mm_packus_epi32(low, high);
}
