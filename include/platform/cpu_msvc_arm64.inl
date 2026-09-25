#include <intrin.h>
#include <arm64_neon.h>

//
static CRUST_INLINE f32 crustAbsF32(f32 v)
{
	float32x2_t x = vdup_n_f32(v);
	return vget_lane_f32(vabs_f32(x), 0);
}

static CRUST_INLINE f32 crustSqrtF32(f32 v)
{
	float32x2_t x = vdup_n_f32(v);
	return vget_lane_f32(vsqrt_f32(x), 0);
}

static CRUST_INLINE f32 crustRsqrtF32(f32 v)
{
	return 1.0f / crustSqrtF32(v);
}

static CRUST_INLINE f64 crustAbsF64(f64 v)
{
	float64x1_t x = vdup_n_f64(v);
	return vget_lane_f64(vabs_f64(x), 0);
}

static CRUST_INLINE f64 crustSqrtF64(f64 v)
{
	float64x1_t x = vdup_n_f64(v);
	return vget_lane_f64(vsqrt_f64(x), 0);
}

static CRUST_INLINE f64 crustRsqrtF64(f64 v)
{
	return 1.0 / crustSqrt(v);
}

//
static CRUST_INLINE u32 crustAtomicSwapU32(volatile u32 *p, u32 v, Crust_MemoryOrder order)
{
	switch (order)
	{
		case CRUST_MEMORY_ORDER_RELAXED: return (u32)_InterlockedExchange_nf((volatile long *)p, (long)v);

		case CRUST_MEMORY_ORDER_CONSUME:
		case CRUST_MEMORY_ORDER_ACQUIRE: return (u32)_InterlockedExchange_acq((volatile long *)p, (long)v);

		case CRUST_MEMORY_ORDER_RELEASE: return (u32)_InterlockedExchange_rel((volatile long *)p, (long)v);

		case CRUST_MEMORY_ORDER_ACQ_REL:
		case CRUST_MEMORY_ORDER_SEQ_CST: return (u32)_InterlockedExchange((volatile long *)p, (long)v);
	}

	CRUST_ASSERT(0);
	return 0;
}

static CRUST_INLINE u32 crustAtomicCompareAndSwapU32(volatile u32 *p, u32 desired, u32 expected, Crust_MemoryOrder success_order, Crust_MemoryOrder failure_order)
{
	CRUST_ASSERT(crustIsValidFailureOrder(success_order, failure_order));
	CRUST_UNUSED(failure_order);

	switch (success_order)
	{
		case CRUST_MEMORY_ORDER_RELAXED: return (u32)_InterlockedCompareExchange_nf((volatile long *)p, (long)desired, (long)expected);

		case CRUST_MEMORY_ORDER_CONSUME:
		case CRUST_MEMORY_ORDER_ACQUIRE: return (u32)_InterlockedCompareExchange_acq((volatile long *)p, (long)desired, (long)expected);

		case CRUST_MEMORY_ORDER_RELEASE: return (u32)_InterlockedCompareExchange_rel((volatile long *)p, (long)desired, (long)expected);

		case CRUST_MEMORY_ORDER_ACQ_REL:
		case CRUST_MEMORY_ORDER_SEQ_CST: return (u32)_InterlockedCompareExchange((volatile long *)p, (long)desired, (long)expected);
	}

	CRUST_ASSERT(0);
	return 0;
}

static CRUST_INLINE u32 crustAtomicLoadU32(volatile u32 *p, Crust_MemoryOrder order)
{
	CRUST_ASSERT(order != CRUST_MEMORY_ORDER_RELEASE);
	CRUST_ASSERT(order != CRUST_MEMORY_ORDER_ACQ_REL);

	if (order == CRUST_MEMORY_ORDER_SEQ_CST)
		return (u32)_InterlockedCompareExchange((volatile long *)p, 0, 0);

	if (order == CRUST_MEMORY_ORDER_RELAXED)
		return (u32)__iso_volatile_load32((const volatile int *)p);

	return (u32)__ldar32((volatile unsigned __int32 *)p);
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

	__stlr32((volatile unsigned __int32 *)p, (unsigned __int32)v);
}

static CRUST_INLINE u32 crustAtomicIncrementU32(volatile u32 *p, Crust_MemoryOrder order)
{
	switch (order)
	{
		case CRUST_MEMORY_ORDER_RELAXED: return (u32)_InterlockedIncrement_nf((volatile long *)p);

		case CRUST_MEMORY_ORDER_CONSUME:
		case CRUST_MEMORY_ORDER_ACQUIRE: return (u32)_InterlockedIncrement_acq((volatile long *)p);

		case CRUST_MEMORY_ORDER_RELEASE: return (u32)_InterlockedIncrement_rel((volatile long *)p);

		case CRUST_MEMORY_ORDER_ACQ_REL:
		case CRUST_MEMORY_ORDER_SEQ_CST: return (u32)_InterlockedIncrement((volatile long *)p);
	}

	CRUST_ASSERT(0);
	return 0;
}

static CRUST_INLINE u32 crustAtomicDecrementU32(volatile u32 *p, Crust_MemoryOrder order)
{
	switch (order)
	{
		case CRUST_MEMORY_ORDER_RELAXED: return (u32)_InterlockedDecrement_nf((volatile long *)p);

		case CRUST_MEMORY_ORDER_CONSUME:
		case CRUST_MEMORY_ORDER_ACQUIRE: return (u32)_InterlockedDecrement_acq((volatile long *)p);

		case CRUST_MEMORY_ORDER_RELEASE: return (u32)_InterlockedDecrement_rel((volatile long *)p);

		case CRUST_MEMORY_ORDER_ACQ_REL:
		case CRUST_MEMORY_ORDER_SEQ_CST: return (u32)_InterlockedDecrement((volatile long *)p);
	}

	CRUST_ASSERT(0);
	return 0;
}

static CRUST_INLINE u64 crustAtomicSwapU64(volatile u64 *p, u64 v, Crust_MemoryOrder order)
{
	switch (order)
	{
		case CRUST_MEMORY_ORDER_RELAXED: return (u64)_InterlockedExchange64_nf((volatile long long *)p, (long long)v);

		case CRUST_MEMORY_ORDER_CONSUME:
		case CRUST_MEMORY_ORDER_ACQUIRE: return (u64)_InterlockedExchange64_acq((volatile long long *)p, (long long)v);

		case CRUST_MEMORY_ORDER_RELEASE: return (u64)_InterlockedExchange64_rel((volatile long long *)p, (long long)v);

		case CRUST_MEMORY_ORDER_ACQ_REL:
		case CRUST_MEMORY_ORDER_SEQ_CST: return (u64)_InterlockedExchange64((volatile long long *)p, (long long)v);
	}

	CRUST_ASSERT(0);
	return 0;
}

static CRUST_INLINE u64 crustAtomicCompareAndSwapU64(volatile u64 *p, u64 desired, u64 expected, Crust_MemoryOrder success_order, Crust_MemoryOrder failure_order)
{
	CRUST_ASSERT(crustIsValidFailureOrder(success_order, failure_order));
	CRUST_UNUSED(failure_order);

	switch (success_order)
	{
		case CRUST_MEMORY_ORDER_RELAXED: return (u64)_InterlockedCompareExchange64_nf((volatile long long *)p, (long long)desired, (long long)expected);

		case CRUST_MEMORY_ORDER_CONSUME:
		case CRUST_MEMORY_ORDER_ACQUIRE: return (u64)_InterlockedCompareExchange64_acq((volatile long long *)p, (long long)desired, (long long)expected);

		case CRUST_MEMORY_ORDER_RELEASE: return (u64)_InterlockedCompareExchange64_rel((volatile long long *)p, (long long)desired, (long long)expected);

		case CRUST_MEMORY_ORDER_ACQ_REL:
		case CRUST_MEMORY_ORDER_SEQ_CST: return (u64)_InterlockedCompareExchange64((volatile long long *)p, (long long)desired, (long long)expected);
	}

	CRUST_ASSERT(0);
	return 0;
}

static CRUST_INLINE u64 crustAtomicLoadU64(volatile u64 *p, Crust_MemoryOrder order)
{
	CRUST_ASSERT(order != CRUST_MEMORY_ORDER_RELEASE);
	CRUST_ASSERT(order != CRUST_MEMORY_ORDER_ACQ_REL);

	if (order == CRUST_MEMORY_ORDER_SEQ_CST)
		return (u64)_InterlockedCompareExchange64((volatile long long *)p, 0, 0);

	if (order == CRUST_MEMORY_ORDER_RELAXED)
		return (u64)__iso_volatile_load64((const volatile long long *)p);

	return (u64)__ldar64((volatile unsigned __int64 *)p);
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

	__stlr64((volatile unsigned __int64 *)p, (unsigned __int64)v);
}

static CRUST_INLINE u64 crustAtomicIncrementU64(volatile u64 *p, Crust_MemoryOrder order)
{
	switch (order)
	{
		case CRUST_MEMORY_ORDER_RELAXED: return (u64)_InterlockedIncrement64_nf((volatile long long *)p);

		case CRUST_MEMORY_ORDER_CONSUME:
		case CRUST_MEMORY_ORDER_ACQUIRE: return (u64)_InterlockedIncrement64_acq((volatile long long *)p);

		case CRUST_MEMORY_ORDER_RELEASE: return (u64)_InterlockedIncrement64_rel((volatile long long *)p);

		case CRUST_MEMORY_ORDER_ACQ_REL:
		case CRUST_MEMORY_ORDER_SEQ_CST: return (u64)_InterlockedIncrement64((volatile long long *)p);
	}

	CRUST_ASSERT(0);
	return 0;
}

static CRUST_INLINE u64 crustAtomicDecrementU64(volatile u64 *p, Crust_MemoryOrder order)
{
	switch (order)
	{
		case CRUST_MEMORY_ORDER_RELAXED: return (u64)_InterlockedDecrement64_nf((volatile long long *)p);

		case CRUST_MEMORY_ORDER_CONSUME:
		case CRUST_MEMORY_ORDER_ACQUIRE: return (u64)_InterlockedDecrement64_acq((volatile long long *)p);

		case CRUST_MEMORY_ORDER_RELEASE: return (u64)_InterlockedDecrement64_rel((volatile long long *)p);

		case CRUST_MEMORY_ORDER_ACQ_REL:
		case CRUST_MEMORY_ORDER_SEQ_CST: return (u64)_InterlockedDecrement64((volatile long long *)p);
	}

	CRUST_ASSERT(0);
	return 0;
}

//
static CRUST_INLINE void crustCpuRelax(void)
{
	__yield();
}

//
static CRUST_INLINE u32 crustLzcntU32(u32 value)
{
	CRUST_ASSERT(value != 0);
	return (u32)_CountLeadingZeros(value);
}

static CRUST_INLINE u32 crustTzcntU32(u32 value)
{
	CRUST_ASSERT(value != 0);
	return (u32)_CountTrailingZeros(value);
}

static CRUST_INLINE u32 crustPopcntU32(u32 value)
{
	return (u32)_CountOneBits(value);
}
