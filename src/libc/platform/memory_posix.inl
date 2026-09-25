#include <stdlib.h>

static CRUST_INLINE void *crustAlignedMalloc(usize size, usize alignment)
{
	CRUST_ASSERT(alignment > 0);
	CRUST_ASSERT(crustIsPow2USize(alignment) != 0);

	if (size == 0)
		return CRUST_NULL;

	alignment = crustMaxUSize(alignment, sizeof(void *));

	void *ptr = CRUST_NULL;
	if (posix_memalign(&ptr, alignment, size) != 0)
		return CRUST_NULL;

	return ptr;
}

static CRUST_INLINE void *crustAlignedRealloc(void *ptr, usize old_size, usize size, usize alignment)
{
	CRUST_ASSERT(alignment > 0);
	CRUST_ASSERT(crustIsPow2USize(alignment) != 0);
	CRUST_ASSERT(ptr == CRUST_NULL || old_size > 0);

	if (size == 0)
	{
		free(ptr);
		return CRUST_NULL;
	}

	void *new_ptr = crustAlignedMalloc(size, alignment);
	if (new_ptr == CRUST_NULL)
		return CRUST_NULL;

	if (ptr != CRUST_NULL)
	{
		usize copy_size = crustMinUSize(old_size, size);
		memcpy(new_ptr, ptr, copy_size);

		free(ptr);
	}

	return new_ptr;
}

static CRUST_INLINE void crustAlignedFree(void *ptr)
{
	free(ptr);
}
