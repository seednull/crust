//
static CRUST_INLINE f32 crustAbsF32(f32 v)
{
	return __builtin_fabsf(v);
}

#if CRUST_TOOLCHAIN_MSVC && CRUST_ARCH_X86
static CRUST_INLINE f32 crustSqrtF32(f32 v)
{
	return (f32)__builtin_sqrt((f64)v);
}

static CRUST_INLINE f32 crustFloorF32(f32 v)
{
	return (f32)__builtin_floor((f64)v);
}

static CRUST_INLINE f32 crustCeilF32(f32 v)
{
	return (f32)__builtin_ceil((f64)v);
}
#else
static CRUST_INLINE f32 crustSqrtF32(f32 v)
{
	return __builtin_sqrtf(v);
}

static CRUST_INLINE f32 crustFloorF32(f32 v)
{
	return __builtin_floorf(v);
}

static CRUST_INLINE f32 crustCeilF32(f32 v)
{
	return __builtin_ceilf(v);
}
#endif

static CRUST_INLINE f32 crustRoundEvenF32(f32 v)
{
	return __builtin_rintf(v);
}

static CRUST_INLINE f32 crustTruncateF32(f32 v)
{
	return __builtin_truncf(v);
}

static CRUST_INLINE f64 crustAbsF64(f64 v)
{
	return __builtin_fabs(v);
}

static CRUST_INLINE f64 crustSqrtF64(f64 v)
{
	return __builtin_sqrt(v);
}

static CRUST_INLINE f64 crustFloorF64(f64 v)
{
	return __builtin_floor(v);
}

static CRUST_INLINE f64 crustCeilF64(f64 v)
{
	return __builtin_ceil(v);
}

static CRUST_INLINE f64 crustRoundEvenF64(f64 v)
{
	return __builtin_rint(v);
}

static CRUST_INLINE f64 crustTruncateF64(f64 v)
{
	return __builtin_trunc(v);
}

//
static CRUST_INLINE u32 crustLzcntU32(u32 value)
{
	CRUST_ASSERT(value != 0);
	return (u32)__builtin_clz(value);
}

static CRUST_INLINE u32 crustTzcntU32(u32 value)
{
	CRUST_ASSERT(value != 0);
	return (u32)__builtin_ctz(value);
}

static CRUST_INLINE u32 crustPopcntU32(u32 value)
{
	return (u32)__builtin_popcount(value);
}
