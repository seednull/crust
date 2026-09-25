static CRUST_INLINE u32 crustAtomicSwapU32(volatile u32 *p, u32 v, Crust_MemoryOrder order)
{
	return __atomic_exchange_n((u32 *)p, v, (int)order);
}

static CRUST_INLINE u32 crustAtomicCompareAndSwapU32(volatile u32 *p, u32 desired, u32 expected, Crust_MemoryOrder success_order, Crust_MemoryOrder failure_order)
{
	CRUST_ASSERT(crustIsValidFailureOrder(success_order, failure_order));

	u32 e = expected;
	__atomic_compare_exchange_n((u32 *)p, &e, desired, 0, (int)success_order, (int)failure_order);

	return e;
}

static CRUST_INLINE u32 crustAtomicLoadU32(volatile u32 *p, Crust_MemoryOrder order)
{
	CRUST_ASSERT(order != CRUST_MEMORY_ORDER_RELEASE);
	CRUST_ASSERT(order != CRUST_MEMORY_ORDER_ACQ_REL);

	return __atomic_load_n((u32 *)p, (int)order);
}

static CRUST_INLINE void crustAtomicStoreU32(volatile u32 *p, u32 v, Crust_MemoryOrder order)
{
	CRUST_ASSERT(order != CRUST_MEMORY_ORDER_ACQUIRE);
	CRUST_ASSERT(order != CRUST_MEMORY_ORDER_ACQ_REL);
	CRUST_ASSERT(order != CRUST_MEMORY_ORDER_CONSUME);

	__atomic_store_n((u32 *)p, v, (int)order);
}

static CRUST_INLINE u32 crustAtomicIncrementU32(volatile u32 *p, Crust_MemoryOrder order)
{
	return __atomic_add_fetch((u32 *)p, 1, (int)order);
}

static CRUST_INLINE u32 crustAtomicDecrementU32(volatile u32 *p, Crust_MemoryOrder order)
{
	return __atomic_sub_fetch((u32 *)p, 1, (int)order);
}

static CRUST_INLINE u64 crustAtomicSwapU64(volatile u64 *p, u64 v, Crust_MemoryOrder order)
{
	return __atomic_exchange_n((u64 *)p, v, (int)order);
}

static CRUST_INLINE u64 crustAtomicCompareAndSwapU64(volatile u64 *p, u64 desired, u64 expected, Crust_MemoryOrder success_order, Crust_MemoryOrder failure_order)
{
	CRUST_ASSERT(crustIsValidFailureOrder(success_order, failure_order));

	u64 e = expected;
	__atomic_compare_exchange_n((u64 *)p, &e, desired, 0, (int)success_order, (int)failure_order);

	return e;
}

static CRUST_INLINE u64 crustAtomicLoadU64(volatile u64 *p, Crust_MemoryOrder order)
{
	CRUST_ASSERT(order != CRUST_MEMORY_ORDER_RELEASE);
	CRUST_ASSERT(order != CRUST_MEMORY_ORDER_ACQ_REL);

	return __atomic_load_n((u64 *)p, (int)order);
}

static CRUST_INLINE void crustAtomicStoreU64(volatile u64 *p, u64 v, Crust_MemoryOrder order)
{
	CRUST_ASSERT(order != CRUST_MEMORY_ORDER_ACQUIRE);
	CRUST_ASSERT(order != CRUST_MEMORY_ORDER_ACQ_REL);
	CRUST_ASSERT(order != CRUST_MEMORY_ORDER_CONSUME);

	__atomic_store_n((u64 *)p, v, (int)order);
}

static CRUST_INLINE u64 crustAtomicIncrementU64(volatile u64 *p, Crust_MemoryOrder order)
{
	return __atomic_add_fetch((u64 *)p, 1, (int)order);
}

static CRUST_INLINE u64 crustAtomicDecrementU64(volatile u64 *p, Crust_MemoryOrder order)
{
	return __atomic_sub_fetch((u64 *)p, 1, (int)order);
}

static CRUST_INLINE void crustCpuRelax(void)
{
#if CRUST_ARCH_X86 || CRUST_ARCH_X64
	__builtin_ia32_pause();
#elif CRUST_ARCH_ARM64
	__asm__ __volatile__("yield" ::: "memory");
#elif CRUST_ARCH_ARM
	__asm__ __volatile__("yield" ::: "memory");
#else
	__asm__ __volatile__("" ::: "memory");
#endif
}

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
