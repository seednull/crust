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
