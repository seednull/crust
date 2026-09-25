#include <crust.h>

//
static void crustGenerationalHandlePoolAddMask(Crust_GenerationalHandlePool *pool, usize index)
{
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(pool->masks != CRUST_NULL);
	CRUST_ASSERT(pool->capacity > index);

	usize mask_index = index / 32;
	usize bit_index = index % 32;

	pool->masks[mask_index] |= (1u << bit_index);
}

static void crustGenerationalHandlePoolRemoveMask(Crust_GenerationalHandlePool *pool, usize index)
{
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(pool->masks != CRUST_NULL);
	CRUST_ASSERT(pool->capacity > index);

	usize mask_index = index / 32;
	usize bit_index = index % 32;

	pool->masks[mask_index] &= ~(1u << bit_index);
}

static u8 crustGenerationalHandlePoolCheckMask(const Crust_GenerationalHandlePool *pool, usize index)
{
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(pool->masks != CRUST_NULL);
	CRUST_ASSERT(index < pool->capacity);

	usize mask_index = index / 32;
	usize bit_index = index % 32;

	return (pool->masks[mask_index] & (1u << bit_index)) != 0;
}

//
CRUST_APIENTRY void crustGenerationalHandlePoolAlloc(Crust_Allocator allocator, Crust_GenerationalHandlePool *pool, usize capacity)
{
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(capacity > 0);

	usize masks_capacity = crustAlignUpUSize(capacity, 32) / 32;

	pool->generations = crustAllocArray(allocator, usize, capacity);
	pool->prevs = crustAllocArray(allocator, usize, capacity);
	pool->nexts = crustAllocArray(allocator, usize, capacity);
	pool->masks = crustAllocArray(allocator, u32, masks_capacity);
	pool->capacity = capacity;

	for (usize i = 0; i < capacity; ++i)
	{
		usize next = (i + 1 < capacity) ? i + 1 : CRUST_USIZE_MAX;
		usize prev = (i > 0) ? i - 1 : CRUST_USIZE_MAX;

		pool->nexts[i] = next;
		pool->prevs[i] = prev;
		pool->generations[i] = 0;
	}

	for (usize i = 0; i < masks_capacity; ++i)
		pool->masks[i] = 0;

	pool->free_head = 0;
	pool->free_tail = capacity - 1;

	pool->used_head = CRUST_USIZE_MAX;
	pool->used_tail = CRUST_USIZE_MAX;
}

CRUST_APIENTRY void crustGenerationalHandlePoolFree(Crust_Allocator allocator, Crust_GenerationalHandlePool *pool)
{
	CRUST_ASSERT(pool != CRUST_NULL);

	usize masks_capacity = crustAlignUpUSize(pool->capacity, 32) / 32;

	crustFreeArray(allocator, pool->generations, usize, pool->capacity);
	crustFreeArray(allocator, pool->prevs, usize, pool->capacity);
	crustFreeArray(allocator, pool->nexts, usize, pool->capacity);
	crustFreeArray(allocator, pool->masks, u32, masks_capacity);

	pool->generations = CRUST_NULL;
	pool->prevs = CRUST_NULL;
	pool->nexts = CRUST_NULL;
	pool->masks = CRUST_NULL;

	pool->capacity = 0;

	pool->free_head = CRUST_USIZE_MAX;
	pool->free_tail = CRUST_USIZE_MAX;

	pool->used_head = CRUST_USIZE_MAX;
	pool->used_tail = CRUST_USIZE_MAX;
}

CRUST_APIENTRY void crustGenerationalHandlePoolGrow(Crust_Allocator allocator, Crust_GenerationalHandlePool *pool, usize new_capacity)
{
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(pool->capacity < new_capacity);

	usize masks_capacity = crustAlignUpUSize(pool->capacity, 32) / 32;
	usize new_masks_capacity = crustAlignUpUSize(new_capacity, 32) / 32;

	pool->generations = crustReallocArray(allocator, pool->generations, usize, pool->capacity, new_capacity);
	pool->prevs = crustReallocArray(allocator, pool->prevs, usize, pool->capacity, new_capacity);
	pool->nexts = crustReallocArray(allocator, pool->nexts, usize, pool->capacity, new_capacity);

	if (masks_capacity != new_masks_capacity)
		pool->masks = crustReallocArray(allocator, pool->masks, u32, masks_capacity, new_masks_capacity);

	for (usize i = masks_capacity; i < new_masks_capacity; ++i)
		pool->masks[i] = 0;

	for (usize i = pool->capacity; i < new_capacity; ++i)
	{
		usize next = (i + 1 < new_capacity) ? i + 1 : CRUST_USIZE_MAX;
		usize prev = (i > pool->capacity) ? i - 1 : pool->free_tail;

		pool->nexts[i] = next;
		pool->prevs[i] = prev;
		pool->generations[i] = 0;
	}

	if (pool->free_tail != CRUST_USIZE_MAX)
		pool->nexts[pool->free_tail] = pool->capacity;

	if (pool->free_head == CRUST_USIZE_MAX)
		pool->free_head = pool->capacity;

	pool->free_tail = new_capacity - 1;
	pool->capacity = new_capacity;
}

CRUST_APIENTRY u32 crustGenerationalHandlePoolCheck(const Crust_GenerationalHandlePool *pool, Crust_GenerationalHandle handle)
{
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(handle.index < pool->capacity);

	usize generation = pool->generations[handle.index];

	if (generation == 0 || generation != handle.generation)
		return 0;

	if (crustGenerationalHandlePoolCheckMask(pool, handle.index) == 0)
		return 0;

	return 1;
}

CRUST_APIENTRY Crust_GenerationalHandle crustGenerationalHandlePoolAdd(Crust_GenerationalHandlePool *pool, usize max_generation)
{
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(pool->prevs != CRUST_NULL);
	CRUST_ASSERT(pool->nexts != CRUST_NULL);
	CRUST_ASSERT(pool->generations != CRUST_NULL);
	CRUST_ASSERT(pool->free_head != CRUST_USIZE_MAX);

	usize index = pool->free_head;
	usize next = pool->nexts[index];
	usize prev = pool->prevs[index];

	pool->prevs[index] = CRUST_USIZE_MAX;
	pool->nexts[index] = CRUST_USIZE_MAX;

	if (prev != CRUST_USIZE_MAX)
		pool->nexts[prev] = next;

	if (next != CRUST_USIZE_MAX)
		pool->prevs[next] = prev;

	if (pool->free_head == index)
		pool->free_head = next;

	if (pool->free_tail == index)
		pool->free_tail = prev;

	crustGenerationalHandlePoolAddMask(pool, index);

	usize generation = pool->generations[index];
	generation = crustMaxUSize(1, (generation + 1) % max_generation);

	pool->generations[index] = generation;

	if (pool->used_tail == CRUST_USIZE_MAX)
	{
		CRUST_ASSERT(pool->used_head == CRUST_USIZE_MAX);

		pool->used_head = index;
		pool->used_tail = index;
	}
	else
	{
		usize used_tail = pool->used_tail;

		pool->nexts[used_tail] = index;
		pool->prevs[index] = used_tail;

		pool->used_tail = index;
	}

	Crust_GenerationalHandle result;
	result.index = index;
	result.generation = generation;

	return result;
}

CRUST_APIENTRY void crustGenerationalHandlePoolRemove(Crust_GenerationalHandlePool *pool, Crust_GenerationalHandle handle)
{
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(pool->prevs != CRUST_NULL);
	CRUST_ASSERT(pool->nexts != CRUST_NULL);
	CRUST_ASSERT(pool->generations != CRUST_NULL);
	CRUST_ASSERT(pool->used_tail != CRUST_USIZE_MAX);
	CRUST_ASSERT(crustGenerationalHandlePoolCheck(pool, handle) != 0);

	usize index = handle.index;
	usize next = pool->nexts[index];
	usize prev = pool->prevs[index];

	pool->nexts[index] = CRUST_USIZE_MAX;
	pool->prevs[index] = CRUST_USIZE_MAX;

	if (prev != CRUST_USIZE_MAX)
		pool->nexts[prev] = next;

	if (next != CRUST_USIZE_MAX)
		pool->prevs[next] = prev;

	if (pool->used_head == index)
		pool->used_head = next;

	if (pool->used_tail == index)
		pool->used_tail = prev;

	if (pool->free_head == CRUST_USIZE_MAX)
	{
		CRUST_ASSERT(pool->free_tail == CRUST_USIZE_MAX);

		pool->free_head = index;
		pool->free_tail = index;
	}
	else
	{
		usize free_head = pool->free_head;

		pool->prevs[free_head] = index;
		pool->nexts[index] = free_head;

		pool->free_head = index;
	}

	crustGenerationalHandlePoolRemoveMask(pool, index);
}

CRUST_APIENTRY Crust_GenerationalHandle crustGenerationalHandlePoolHead(const Crust_GenerationalHandlePool *pool)
{
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(pool->generations != CRUST_NULL);

	Crust_GenerationalHandle result;
	result.index = pool->used_head;
	result.generation = 0;

	if (result.index != CRUST_USIZE_MAX)
		result.generation = pool->generations[result.index];

	return result;
}

CRUST_APIENTRY Crust_GenerationalHandle crustGenerationalHandlePoolTail(const Crust_GenerationalHandlePool *pool)
{
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(pool->generations != CRUST_NULL);

	Crust_GenerationalHandle result;
	result.index = pool->used_tail;
	result.generation = 0;

	if (result.index != CRUST_USIZE_MAX)
		result.generation = pool->generations[result.index];

	return result;
}

CRUST_APIENTRY Crust_GenerationalHandle crustGenerationalHandlePoolNext(const Crust_GenerationalHandlePool *pool, Crust_GenerationalHandle handle)
{
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(pool->nexts != CRUST_NULL);
	CRUST_ASSERT(pool->generations != CRUST_NULL);
	CRUST_ASSERT(crustGenerationalHandlePoolCheck(pool, handle) != 0);

	Crust_GenerationalHandle result;
	result.index = pool->nexts[handle.index];
	result.generation = 0;

	if (result.index != CRUST_USIZE_MAX)
		result.generation = pool->generations[result.index];

	return result;
}

CRUST_APIENTRY Crust_GenerationalHandle crustGenerationalHandlePoolPrev(const Crust_GenerationalHandlePool *pool, Crust_GenerationalHandle handle)
{
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(pool->prevs != CRUST_NULL);
	CRUST_ASSERT(pool->generations != CRUST_NULL);
	CRUST_ASSERT(crustGenerationalHandlePoolCheck(pool, handle) != 0);

	Crust_GenerationalHandle result;
	result.index = pool->prevs[handle.index];
	result.generation = 0;

	if (result.index != CRUST_USIZE_MAX)
		result.generation = pool->generations[result.index];

	return result;
}
