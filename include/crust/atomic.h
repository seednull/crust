#pragma once

#include "core.h"
#include "platform/intrinsics_cpu.inl"

#ifdef __cplusplus
extern "C" {
#endif

//
typedef enum Crust_MemoryOrder_t
{
	CRUST_MEMORY_ORDER_RELAXED = 0,
	CRUST_MEMORY_ORDER_CONSUME = 1,
	CRUST_MEMORY_ORDER_ACQUIRE = 2,
	CRUST_MEMORY_ORDER_RELEASE = 3,
	CRUST_MEMORY_ORDER_ACQ_REL = 4,
	CRUST_MEMORY_ORDER_SEQ_CST = 5,
} Crust_MemoryOrder;

static CRUST_INLINE u32 crustAtomicSwapU32(volatile u32 *p, u32 v, Crust_MemoryOrder order);
static CRUST_INLINE u32 crustAtomicCompareAndSwapU32(volatile u32 *p, u32 desired, u32 expected, Crust_MemoryOrder success_order, Crust_MemoryOrder failure_order);
static CRUST_INLINE u32 crustAtomicLoadU32(volatile u32 *p, Crust_MemoryOrder order);
static CRUST_INLINE void crustAtomicStoreU32(volatile u32 *p, u32 v, Crust_MemoryOrder order);
static CRUST_INLINE u32 crustAtomicIncrementU32(volatile u32 *p, Crust_MemoryOrder order);
static CRUST_INLINE u32 crustAtomicDecrementU32(volatile u32 *p, Crust_MemoryOrder order);

static CRUST_INLINE u64 crustAtomicSwapU64(volatile u64 *p, u64 v, Crust_MemoryOrder order);
static CRUST_INLINE u64 crustAtomicCompareAndSwapU64(volatile u64 *p, u64 desired, u64 expected, Crust_MemoryOrder success_order, Crust_MemoryOrder failure_order);
static CRUST_INLINE u64 crustAtomicLoadU64(volatile u64 *p, Crust_MemoryOrder order);
static CRUST_INLINE void crustAtomicStoreU64(volatile u64 *p, u64 v, Crust_MemoryOrder order);
static CRUST_INLINE u64 crustAtomicIncrementU64(volatile u64 *p, Crust_MemoryOrder order);
static CRUST_INLINE u64 crustAtomicDecrementU64(volatile u64 *p, Crust_MemoryOrder order);

static CRUST_INLINE usize crustAtomicSwapUSize(volatile usize *p, usize v, Crust_MemoryOrder order);
static CRUST_INLINE usize crustAtomicCompareAndSwapUSize(volatile usize *p, usize desired, usize expected, Crust_MemoryOrder success_order, Crust_MemoryOrder failure_order);
static CRUST_INLINE usize crustAtomicLoadUSize(volatile usize *p, Crust_MemoryOrder order);
static CRUST_INLINE void crustAtomicStoreUSize(volatile usize *p, usize v, Crust_MemoryOrder order);
static CRUST_INLINE usize crustAtomicIncrementUSize(volatile usize *p, Crust_MemoryOrder order);
static CRUST_INLINE usize crustAtomicDecrementUSize(volatile usize *p, Crust_MemoryOrder order);

//
static CRUST_INLINE void crustCpuRelax(void);

//
#include "platform/atomic_common.inl"

#ifdef __cplusplus
}
#endif
