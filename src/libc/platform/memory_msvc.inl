#include <malloc.h>

static CRUST_INLINE void *crustAlignedMalloc(usize size, usize alignment)
{
	CRUST_ASSERT(alignment > 0);
	CRUST_ASSERT(crustIsPow2USize(alignment) != 0);

	if (size == 0)
		return CRUST_NULL;

	return _aligned_malloc(size, alignment);
}

static CRUST_INLINE void *crustAlignedRealloc(void *ptr, usize old_size, usize size, usize alignment)
{
	CRUST_ASSERT(alignment > 0);
	CRUST_ASSERT(crustIsPow2USize(alignment) != 0);
	CRUST_ASSERT(ptr == CRUST_NULL || old_size > 0);
	CRUST_UNUSED(old_size);

	if (size == 0)
	{
		_aligned_free(ptr);
		return CRUST_NULL;
	}

	return _aligned_realloc(ptr, size, alignment);
}

static CRUST_INLINE void crustAlignedFree(void *ptr)
{
	_aligned_free(ptr);
}
