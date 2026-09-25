#include <crust.h>
#include <string.h>

//
CRUST_APIENTRY void crustMemcpy(void *dst, const void *src, usize size)
{
	memcpy(dst, src, size);
}

CRUST_APIENTRY void crustMemset(void *dst, u8 value, usize size)
{
	memset(dst, value, size);
}

//
#if CRUST_TOOLCHAIN_MSVC || CRUST_TOOLCHAIN_MINGW
	#include "platform/memory_msvc.inl"
#else
	#include "platform/memory_posix.inl"
#endif

//
static void *crustSystemAllocatorAlloc(void *context, usize size, usize alignment)
{
	CRUST_UNUSED(context);

	return crustAlignedMalloc(size, alignment);
}

static void *crustSystemAllocatorRealloc(void *context, void *ptr, usize old_size, usize new_size, usize alignment)
{
	CRUST_UNUSED(context);
	CRUST_UNUSED(old_size);

	return crustAlignedRealloc(ptr, old_size, new_size, alignment);
}

static void crustSystemAllocatorFree(void *context, void *ptr, usize size, usize alignment)
{
	CRUST_UNUSED(context);
	CRUST_UNUSED(size);
	CRUST_UNUSED(alignment);

	crustAlignedFree(ptr);
}

static const Crust_AllocatorVtbl crustSystemAllocatorVtbl =
{
	crustSystemAllocatorAlloc,
	crustSystemAllocatorRealloc,
	crustSystemAllocatorFree,
};

CRUST_APIENTRY Crust_Allocator crustSystemAllocatorInit(void)
{
	Crust_Allocator result;
	result.context = CRUST_NULL;
	result.vtbl = &crustSystemAllocatorVtbl;

	return result;
}
