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
	CRUST_ASSERT(crustIsAlignedUSize((usize)p, sizeof(u64)));
	CRUST_UNUSED(order);

#if CRUST_ARCH_X86
	u64 expected = (u64)__iso_volatile_load64((const volatile long long *)p);

	for (;;)
	{
		u64 actual = (u64)_InterlockedCompareExchange64((volatile long long *)p, (long long)v, (long long)expected);
		if (actual == expected)
			return actual;

		expected = actual;
	}
#else
	return (u64)_InterlockedExchange64((volatile long long *)p, (long long)v);
#endif
}

static CRUST_INLINE u64 crustAtomicCompareAndSwapU64(volatile u64 *p, u64 desired, u64 expected, Crust_MemoryOrder success_order, Crust_MemoryOrder failure_order)
{
	CRUST_ASSERT(crustIsAlignedUSize((usize)p, sizeof(u64)));
	CRUST_ASSERT(crustIsValidFailureOrder(success_order, failure_order));
	CRUST_UNUSED(success_order);
	CRUST_UNUSED(failure_order);

	return (u64)_InterlockedCompareExchange64((volatile long long *)p, (long long)desired, (long long)expected);
}

static CRUST_INLINE u64 crustAtomicLoadU64(volatile u64 *p, Crust_MemoryOrder order)
{
	CRUST_ASSERT(crustIsAlignedUSize((usize)p, sizeof(u64)));
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
	CRUST_ASSERT(crustIsAlignedUSize((usize)p, sizeof(u64)));
	CRUST_ASSERT(order != CRUST_MEMORY_ORDER_ACQUIRE);
	CRUST_ASSERT(order != CRUST_MEMORY_ORDER_ACQ_REL);
	CRUST_ASSERT(order != CRUST_MEMORY_ORDER_CONSUME);

	if (order == CRUST_MEMORY_ORDER_SEQ_CST)
	{
		crustAtomicSwapU64(p, v, order);
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
	CRUST_ASSERT(crustIsAlignedUSize((usize)p, sizeof(u64)));
	CRUST_UNUSED(order);

#if CRUST_ARCH_X86
	u64 expected = (u64)__iso_volatile_load64((const volatile long long *)p);

	for (;;)
	{
		u64 desired = expected + 1;
		u64 actual = (u64)_InterlockedCompareExchange64((volatile long long *)p, (long long)desired, (long long)expected);
		if (actual == expected)
			return desired;

		expected = actual;
	}
#else
	return (u64)_InterlockedIncrement64((volatile long long *)p);
#endif
}

static CRUST_INLINE u64 crustAtomicDecrementU64(volatile u64 *p, Crust_MemoryOrder order)
{
	CRUST_ASSERT(crustIsAlignedUSize((usize)p, sizeof(u64)));
	CRUST_UNUSED(order);

#if CRUST_ARCH_X86
	u64 expected = (u64)__iso_volatile_load64((const volatile long long *)p);

	for (;;)
	{
		u64 desired = expected - 1;
		u64 actual = (u64)_InterlockedCompareExchange64((volatile long long *)p, (long long)desired, (long long)expected);
		if (actual == expected)
			return desired;

		expected = actual;
	}
#else
	return (u64)_InterlockedDecrement64((volatile long long *)p);
#endif
}

static CRUST_INLINE usize crustAtomicSwapUSize(volatile usize *p, usize v, Crust_MemoryOrder order)
{
#if CRUST_ARCH_64BIT
	return crustAtomicSwapU64(p, v, order);
#else
	return crustAtomicSwapU32(p, v, order);
#endif
}

static CRUST_INLINE usize crustAtomicCompareAndSwapUSize(volatile usize *p, usize desired, usize expected, Crust_MemoryOrder success_order, Crust_MemoryOrder failure_order)
{
#if CRUST_ARCH_64BIT
	return crustAtomicCompareAndSwapU64(p, desired, expected, success_order, failure_order);
#else
	return crustAtomicCompareAndSwapU32(p, desired, expected, success_order, failure_order);
#endif
}

static CRUST_INLINE usize crustAtomicLoadUSize(volatile usize *p, Crust_MemoryOrder order)
{
#if CRUST_ARCH_64BIT
	return crustAtomicLoadU64(p, order);
#else
	return crustAtomicLoadU32(p, order);
#endif
}

static CRUST_INLINE void crustAtomicStoreUSize(volatile usize *p, usize v, Crust_MemoryOrder order)
{
#if CRUST_ARCH_64BIT
	crustAtomicStoreU64(p, v, order);
#else
	crustAtomicStoreU32(p, v, order);
#endif
}

static CRUST_INLINE usize crustAtomicIncrementUSize(volatile usize *p, Crust_MemoryOrder order)
{
#if CRUST_ARCH_64BIT
	return crustAtomicIncrementU64(p, order);
#else
	return crustAtomicIncrementU32(p, order);
#endif
}

static CRUST_INLINE usize crustAtomicDecrementUSize(volatile usize *p, Crust_MemoryOrder order)
{
#if CRUST_ARCH_64BIT
	return crustAtomicDecrementU64(p, order);
#else
	return crustAtomicDecrementU32(p, order);
#endif
}

//
static CRUST_INLINE void crustCpuRelax(void)
{
	_mm_pause();
}
