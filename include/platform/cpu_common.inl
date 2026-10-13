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
