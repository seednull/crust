#pragma once

#include "allocator.h"

#ifdef __cplusplus
extern "C" {
#endif

//
typedef struct Crust_GenerationalHandle_t
{
	usize generation;
	usize index;
} Crust_GenerationalHandle;

typedef struct Crust_GenerationalHandlePool_t
{
	usize *generations;
	usize *prevs;
	usize *nexts;
	u32 *masks;
	usize capacity;
	usize free_head;
	usize free_tail;
	usize used_head;
	usize used_tail;
} Crust_GenerationalHandlePool;

CRUST_APIENTRY usize crustGenerationalHandlePoolFootprint(usize capacity);
CRUST_APIENTRY u8 crustGenerationalHandlePoolAlloc(Crust_Allocator allocator, Crust_GenerationalHandlePool *pool, usize capacity);
CRUST_APIENTRY void crustGenerationalHandlePoolFree(Crust_Allocator allocator, Crust_GenerationalHandlePool *pool);
CRUST_APIENTRY u8 crustGenerationalHandlePoolGrow(Crust_Allocator allocator, Crust_GenerationalHandlePool *pool, usize new_capacity);
CRUST_APIENTRY u32 crustGenerationalHandlePoolCheck(const Crust_GenerationalHandlePool *pool, Crust_GenerationalHandle handle);
CRUST_APIENTRY Crust_GenerationalHandle crustGenerationalHandlePoolAdd(Crust_GenerationalHandlePool *pool, usize max_generation);
CRUST_APIENTRY void crustGenerationalHandlePoolRemove(Crust_GenerationalHandlePool *pool, Crust_GenerationalHandle handle);
CRUST_APIENTRY Crust_GenerationalHandle crustGenerationalHandlePoolHead(const Crust_GenerationalHandlePool *pool);
CRUST_APIENTRY Crust_GenerationalHandle crustGenerationalHandlePoolTail(const Crust_GenerationalHandlePool *pool);
CRUST_APIENTRY Crust_GenerationalHandle crustGenerationalHandlePoolNext(const Crust_GenerationalHandlePool *pool, Crust_GenerationalHandle handle);
CRUST_APIENTRY Crust_GenerationalHandle crustGenerationalHandlePoolPrev(const Crust_GenerationalHandlePool *pool, Crust_GenerationalHandle handle);

#ifdef __cplusplus
}
#endif
