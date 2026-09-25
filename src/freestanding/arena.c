#include <crust.h>

//
CRUST_APIENTRY Crust_Arena crustArenaAttach(void *memory, usize capacity)
{
	CRUST_ASSERT(memory != CRUST_NULL);
	CRUST_ASSERT(capacity > 0);

	Crust_Arena result;
	result.memory = memory;
	result.capacity = capacity;
	result.size = 0;

	return result;
}

CRUST_APIENTRY void crustArenaReset(Crust_Arena *arena)
{
	CRUST_ASSERT(arena != CRUST_NULL);
	CRUST_ASSERT(arena->memory != CRUST_NULL);

	arena->size = 0;
}

CRUST_APIENTRY void *crustArenaAlloc(Crust_Arena *arena, usize size, usize alignment)
{
	CRUST_ASSERT(arena != CRUST_NULL);
	CRUST_ASSERT(arena->memory != CRUST_NULL);
	CRUST_ASSERT(alignment > 0);
	CRUST_ASSERT(crustIsPow2USize(alignment));

	u8 *base = (u8 *)arena->memory;
	u8 *current = base + arena->size;
	u8 *aligned = (u8 *)crustAlignUpUSize((usize)current, alignment);

	usize offset = (usize)(aligned - base);

	if (offset > arena->capacity)
		return CRUST_NULL;

	if (size > arena->capacity - offset)
		return CRUST_NULL;

	arena->size = offset + size;
	return (void *)aligned;
}

//
static void *crustArenaAllocatorAlloc(void *context, usize size, usize alignment)
{
	return crustArenaAlloc((Crust_Arena *)context, size, alignment);
}

static void *crustArenaAllocatorRealloc(void *context, void *ptr, usize old_size, usize new_size, usize alignment)
{
	Crust_Arena *arena = (Crust_Arena *)context;
	CRUST_ASSERT(arena != CRUST_NULL);
	CRUST_ASSERT(arena->memory != CRUST_NULL);

	u8 *base = (u8 *)arena->memory;

	u8 *end = base + arena->size;
	u8 *allocation_end = (u8 *)ptr + old_size;

	if (allocation_end == end)
	{
		usize offset = (usize)((u8 *)ptr - base);

		if (new_size <= arena->capacity - offset)
		{
			arena->size = offset + new_size;
			return ptr;
		}

		return CRUST_NULL;
	}

	void *new_ptr = crustArenaAlloc(arena, new_size, alignment);
	if (new_ptr == CRUST_NULL)
		return CRUST_NULL;

	crustMemcpy(new_ptr, ptr, crustMinUSize(old_size, new_size));
	return new_ptr;
}

static void crustArenaAllocatorFree(void *context, void *ptr, usize size, usize alignment)
{
	CRUST_UNUSED(context);
	CRUST_UNUSED(ptr);
	CRUST_UNUSED(size);
	CRUST_UNUSED(alignment);
}

static const Crust_AllocatorVtbl crustArenaAllocatorVtbl =
{
	crustArenaAllocatorAlloc,
	crustArenaAllocatorRealloc,
	crustArenaAllocatorFree,
};

CRUST_APIENTRY Crust_Allocator crustArenaAllocatorInit(Crust_Arena *arena)
{
	CRUST_ASSERT(arena != CRUST_NULL);
	CRUST_ASSERT(arena->memory != CRUST_NULL);

	Crust_Allocator result;
	result.context = arena;
	result.vtbl = &crustArenaAllocatorVtbl;

	return result;
}
