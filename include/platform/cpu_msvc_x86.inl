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

//
static CRUST_INLINE u32 crustAtomicSwapU32(volatile u32 *p, u32 v, Crust_MemoryOrder order)
{
	CRUST_UNUSED(order);
	return (u32)_InterlockedExchange((volatile long *)p, (long)v);
}

static CRUST_INLINE u32 crustAtomicCompareAndSwapU32(volatile u32 *p, u32 desired, u32 expected, Crust_MemoryOrder success_order, Crust_MemoryOrder failure_order)
{
	CRUST_ASSERT(crustIsValidFailureOrder(success_order, failure_order));
	CRUST_UNUSED(success_order);
	CRUST_UNUSED(failure_order);

	return (u32)_InterlockedCompareExchange((volatile long *)p, (long)desired, (long)expected);
}

static CRUST_INLINE u32 crustAtomicLoadU32(volatile u32 *p, Crust_MemoryOrder order)
{
	CRUST_ASSERT(order != CRUST_MEMORY_ORDER_RELEASE);
	CRUST_ASSERT(order != CRUST_MEMORY_ORDER_ACQ_REL);

	if (order == CRUST_MEMORY_ORDER_SEQ_CST)
		return (u32)_InterlockedCompareExchange((volatile long *)p, 0, 0);

	if (order == CRUST_MEMORY_ORDER_RELAXED)
		return (u32)__iso_volatile_load32((const volatile int *)p);

	u32 result = (u32)__iso_volatile_load32((const volatile int *)p);
	_ReadWriteBarrier();

	return result;
}

static CRUST_INLINE void crustAtomicStoreU32(volatile u32 *p, u32 v, Crust_MemoryOrder order)
{
	CRUST_ASSERT(order != CRUST_MEMORY_ORDER_ACQUIRE);
	CRUST_ASSERT(order != CRUST_MEMORY_ORDER_ACQ_REL);
	CRUST_ASSERT(order != CRUST_MEMORY_ORDER_CONSUME);

	if (order == CRUST_MEMORY_ORDER_SEQ_CST)
	{
		_InterlockedExchange((volatile long *)p, (long)v);
		return;
	}

	if (order == CRUST_MEMORY_ORDER_RELAXED)
	{
		__iso_volatile_store32((volatile int *)p, (int)v);
		return;
	}

	_ReadWriteBarrier();
	__iso_volatile_store32((volatile int *)p, (int)v);
}

static CRUST_INLINE u32 crustAtomicIncrementU32(volatile u32 *p, Crust_MemoryOrder order)
{
	CRUST_UNUSED(order);
	return (u32)_InterlockedIncrement((volatile long *)p);
}

static CRUST_INLINE u32 crustAtomicDecrementU32(volatile u32 *p, Crust_MemoryOrder order)
{
	CRUST_UNUSED(order);
	return (u32)_InterlockedDecrement((volatile long *)p);
}

static CRUST_INLINE u64 crustAtomicSwapU64(volatile u64 *p, u64 v, Crust_MemoryOrder order)
{
	CRUST_UNUSED(order);
	return (u64)_InterlockedExchange64((volatile long long *)p, (long long)v);
}

static CRUST_INLINE u64 crustAtomicCompareAndSwapU64(volatile u64 *p, u64 desired, u64 expected, Crust_MemoryOrder success_order, Crust_MemoryOrder failure_order)
{
	CRUST_ASSERT(crustIsValidFailureOrder(success_order, failure_order));
	CRUST_UNUSED(success_order);
	CRUST_UNUSED(failure_order);

	return (u64)_InterlockedCompareExchange64((volatile long long *)p, (long long)desired, (long long)expected);
}

static CRUST_INLINE u64 crustAtomicLoadU64(volatile u64 *p, Crust_MemoryOrder order)
{
	CRUST_ASSERT(order != CRUST_MEMORY_ORDER_RELEASE);
	CRUST_ASSERT(order != CRUST_MEMORY_ORDER_ACQ_REL);

	if (order == CRUST_MEMORY_ORDER_SEQ_CST)
		return (u64)_InterlockedCompareExchange64((volatile long long *)p, 0, 0);

	if (order == CRUST_MEMORY_ORDER_RELAXED)
		return (u64)__iso_volatile_load64((const volatile long long *)p);

	u64 result = (u64)__iso_volatile_load64((const volatile long long *)p);
	_ReadWriteBarrier();

	return result;
}

static CRUST_INLINE void crustAtomicStoreU64(volatile u64 *p, u64 v, Crust_MemoryOrder order)
{
	CRUST_ASSERT(order != CRUST_MEMORY_ORDER_ACQUIRE);
	CRUST_ASSERT(order != CRUST_MEMORY_ORDER_ACQ_REL);
	CRUST_ASSERT(order != CRUST_MEMORY_ORDER_CONSUME);

	if (order == CRUST_MEMORY_ORDER_SEQ_CST)
	{
		_InterlockedExchange64((volatile long long *)p, (long long)v);
		return;
	}

	if (order == CRUST_MEMORY_ORDER_RELAXED)
	{
		__iso_volatile_store64((volatile long long *)p, (long long)v);
		return;
	}

	_ReadWriteBarrier();
	__iso_volatile_store64((volatile long long *)p, (long long)v);
}

static CRUST_INLINE u64 crustAtomicIncrementU64(volatile u64 *p, Crust_MemoryOrder order)
{
	CRUST_UNUSED(order);
	return (u64)_InterlockedIncrement64((volatile long long *)p);
}

static CRUST_INLINE u64 crustAtomicDecrementU64(volatile u64 *p, Crust_MemoryOrder order)
{
	CRUST_UNUSED(order);
	return (u64)_InterlockedDecrement64((volatile long long *)p);
}

//
static CRUST_INLINE void crustCpuRelax(void)
{
	_mm_pause();
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
