//
static CRUST_INLINE int crustIsValidFailureOrder(Crust_MemoryOrder success, Crust_MemoryOrder failure)
{
	if (failure == CRUST_MEMORY_ORDER_RELEASE || failure == CRUST_MEMORY_ORDER_ACQ_REL)
		return 0;

	switch (success)
	{
		case CRUST_MEMORY_ORDER_RELAXED:
			return failure == CRUST_MEMORY_ORDER_RELAXED;

		case CRUST_MEMORY_ORDER_CONSUME:
			return failure == CRUST_MEMORY_ORDER_RELAXED
				|| failure == CRUST_MEMORY_ORDER_CONSUME;

		case CRUST_MEMORY_ORDER_ACQUIRE:
			return failure == CRUST_MEMORY_ORDER_RELAXED
				|| failure == CRUST_MEMORY_ORDER_CONSUME
				|| failure == CRUST_MEMORY_ORDER_ACQUIRE;

		case CRUST_MEMORY_ORDER_RELEASE:
			return failure == CRUST_MEMORY_ORDER_RELAXED;

		case CRUST_MEMORY_ORDER_ACQ_REL:
			return failure == CRUST_MEMORY_ORDER_RELAXED
				|| failure == CRUST_MEMORY_ORDER_CONSUME
				|| failure == CRUST_MEMORY_ORDER_ACQUIRE;

		case CRUST_MEMORY_ORDER_SEQ_CST:
			return failure == CRUST_MEMORY_ORDER_RELAXED
				|| failure == CRUST_MEMORY_ORDER_CONSUME
				|| failure == CRUST_MEMORY_ORDER_ACQUIRE
				|| failure == CRUST_MEMORY_ORDER_SEQ_CST;
	}

	return 0;
}

//
#if CRUST_COMPILER_MSVC
	#if CRUST_ARCH_X86 || CRUST_ARCH_X64
		#include "cpu_msvc_x86.inl"
	#elif CRUST_ARCH_ARM64
		#include "cpu_msvc_arm64.inl"
	#else
		#error "Unsupported MSVC architecture"
	#endif
#elif CRUST_COMPILER_GCC || CRUST_COMPILER_CLANG
	#include "cpu_gcc_clang.inl"
#else
	#error "Unsupported compiler"
#endif

//
static CRUST_INLINE f32 crustRsqrtF32(f32 v) { return 1.0f / crustSqrtF32(v); }
static CRUST_INLINE f64 crustRsqrtF64(f64 v) { return 1.0 / crustSqrtF64(v); }

//
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
