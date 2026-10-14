#pragma once

#include "core.h"

#ifdef __cplusplus
extern "C" {
#endif

//
typedef void *(*PFN_crustAllocatorAlloc)(void *context, usize size, usize alignment);
typedef void *(*PFN_crustAllocatorRealloc)(void *context, void *ptr, usize old_size, usize new_size, usize alignment);
typedef void (*PFN_crustAllocatorFree)(void *context, void *ptr, usize size, usize alignment);

typedef struct Crust_AllocatorVtbl_t
{
	PFN_crustAllocatorAlloc alloc;
	PFN_crustAllocatorRealloc realloc;
	PFN_crustAllocatorFree free;
} Crust_AllocatorVtbl;

typedef struct Crust_Allocator_t
{
	void *context;
	const Crust_AllocatorVtbl *vtbl;
} Crust_Allocator;

static CRUST_INLINE void *crustAllocatorAlloc(Crust_Allocator allocator, usize size, usize alignment)
{
	CRUST_ASSERT(allocator.vtbl != CRUST_NULL);
	CRUST_ASSERT(allocator.vtbl->alloc != CRUST_NULL);

	CRUST_ASSERT(size > 0);
	CRUST_ASSERT(alignment > 0);
	CRUST_ASSERT(crustIsPow2USize(alignment));

	return allocator.vtbl->alloc(allocator.context, size, alignment);
}

static CRUST_INLINE void *crustAllocatorRealloc(Crust_Allocator allocator, void *ptr, usize old_size, usize new_size, usize alignment)
{
	CRUST_ASSERT(allocator.vtbl != CRUST_NULL);
	CRUST_ASSERT(allocator.vtbl->alloc != CRUST_NULL);
	CRUST_ASSERT(allocator.vtbl->realloc != CRUST_NULL);
	CRUST_ASSERT(allocator.vtbl->free != CRUST_NULL);

	CRUST_ASSERT(alignment > 0);
	CRUST_ASSERT(crustIsPow2USize(alignment));
	
	if (ptr == CRUST_NULL)
	{
		CRUST_ASSERT(old_size == 0);

		if (new_size == 0)
			return CRUST_NULL;

		return allocator.vtbl->alloc(allocator.context, new_size, alignment);
	}

	CRUST_ASSERT(old_size > 0);

	if (new_size == 0)
	{
		allocator.vtbl->free(allocator.context, ptr, old_size, alignment);
		return CRUST_NULL;
	}

	return allocator.vtbl->realloc(allocator.context, ptr, old_size, new_size, alignment);
}

static CRUST_INLINE void crustAllocatorFree(Crust_Allocator allocator, void *ptr, usize size, usize alignment)
{
	CRUST_ASSERT(allocator.vtbl != CRUST_NULL);
	CRUST_ASSERT(allocator.vtbl->free != CRUST_NULL);

	if (ptr == CRUST_NULL)
		return;

	CRUST_ASSERT(size > 0);
	CRUST_ASSERT(alignment > 0);
	CRUST_ASSERT(crustIsPow2USize(alignment));

	allocator.vtbl->free(allocator.context, ptr, size, alignment);
}

static CRUST_INLINE usize crustAllocatorSizeMul(usize a, usize b)
{
	CRUST_ASSERT(a == 0 || b <= CRUST_USIZE_MAX / a);

	if (a != 0 && b > CRUST_USIZE_MAX / a)
		return CRUST_USIZE_MAX;

	return a * b;
}

#define crustAlloc(allocator, T)                                     \
	((T *)crustAllocatorAlloc(                                       \
		(allocator),                                                 \
		sizeof(T),                                                   \
		CRUST_ALIGNOF(T)))

#define crustAllocArray(allocator, T, count)                         \
	((T *)crustAllocatorAlloc(                                       \
		(allocator),                                                 \
		crustAllocatorSizeMul(sizeof(T), (count)),                   \
		CRUST_ALIGNOF(T)))

#define crustReallocArray(allocator, ptr, T, old_count, new_count)   \
	((T *)crustAllocatorRealloc(                                     \
		(allocator),                                                 \
		(ptr),                                                       \
		crustAllocatorSizeMul(sizeof(T), (old_count)),               \
		crustAllocatorSizeMul(sizeof(T), (new_count)),               \
		CRUST_ALIGNOF(T)))

#define crustFree(allocator, ptr, T)                                 \
	crustAllocatorFree(                                              \
		(allocator),                                                 \
		(ptr),                                                       \
		sizeof(T),                                                   \
		CRUST_ALIGNOF(T))

#define crustFreeArray(allocator, ptr, T, count)                     \
	crustAllocatorFree(                                              \
		(allocator),                                                 \
		(ptr),                                                       \
		crustAllocatorSizeMul(sizeof(T), (count)),                   \
		CRUST_ALIGNOF(T))

CRUST_APIENTRY Crust_Allocator crustSystemAllocatorInit(void);

#ifdef __cplusplus
}
#endif
