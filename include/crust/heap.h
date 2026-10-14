#pragma once

#include "allocator.h"

#ifdef __cplusplus
extern "C" {
#endif

//
typedef struct Crust_HeapNode_t
{
	u32 offset;
	u32 size;
	u32 prev_bin;
	u32 next_bin;
	u32 prev_neighbour;
	u32 next_neighbour;
} Crust_HeapNode;

typedef struct Crust_HeapNodePool_t
{
	Crust_HeapNode *nodes;
	u32 free_head;
	u32 capacity;
} Crust_HeapNodePool;

CRUST_APIENTRY void crustHeapNodePoolAlloc(Crust_Allocator allocator, Crust_HeapNodePool *pool, u32 capacity);
CRUST_APIENTRY void crustHeapNodePoolFree(Crust_Allocator allocator, Crust_HeapNodePool *pool);

//
typedef struct Crust_HeapAllocation_t
{
	u32 offset;
	u32 index;
} Crust_HeapAllocation;

typedef enum Crust_HeapParameters_t
{
	CRUST_HEAP_PARAMETERS_MANTISSA_BITS = 3,
	CRUST_HEAP_PARAMETERS_MANTISSA_MAX = 0x00000008,
	CRUST_HEAP_PARAMETERS_MANTISSA_MASK = 0x00000007,

	CRUST_HEAP_PARAMETERS_EXPONENT_BITS = 5,
	CRUST_HEAP_PARAMETERS_EXPONENT_MAX = 0x00000020,

	CRUST_HEAP_PARAMETERS_NUM_SPARSE_BINS = CRUST_HEAP_PARAMETERS_EXPONENT_MAX,
	CRUST_HEAP_PARAMETERS_NUM_LINEAR_BINS = CRUST_HEAP_PARAMETERS_MANTISSA_MAX,
	CRUST_HEAP_PARAMETERS_NUM_BINS = CRUST_HEAP_PARAMETERS_NUM_SPARSE_BINS * CRUST_HEAP_PARAMETERS_NUM_LINEAR_BINS,
} Crust_HeapParameters;

typedef struct Crust_Heap_t
{
	Crust_HeapNodePool *pool;

	u32 size;
	u32 heads[CRUST_HEAP_PARAMETERS_NUM_BINS];
	u32 used_sparse_bins;
	u8 used_linear_bins[CRUST_HEAP_PARAMETERS_NUM_SPARSE_BINS];
} Crust_Heap;

CRUST_APIENTRY Crust_Heap crustHeapInit(Crust_HeapNodePool *pool, u32 size);
CRUST_APIENTRY Crust_HeapAllocation crustHeapStageAlloc(const Crust_Heap *heap, u32 size);
CRUST_APIENTRY Crust_HeapAllocation crustHeapStageAllocAligned(const Crust_Heap *heap, u32 size, u32 alignment);
CRUST_APIENTRY void crustHeapCommitAlloc(Crust_Heap *heap, Crust_HeapAllocation allocation, u32 size);
CRUST_APIENTRY Crust_HeapAllocation crustHeapAlloc(Crust_Heap *heap, u32 size);
CRUST_APIENTRY Crust_HeapAllocation crustHeapAllocAligned(Crust_Heap *heap, u32 size, u32 alignment);
CRUST_APIENTRY void crustHeapFree(Crust_Heap *heap, Crust_HeapAllocation allocation);

#ifdef __cplusplus
}
#endif
