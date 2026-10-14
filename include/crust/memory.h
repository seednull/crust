#pragma once

#include "core.h"

#ifdef __cplusplus
extern "C" {
#endif

//
CRUST_APIENTRY void crustMemcpy(void *dst, const void *src, usize size);
CRUST_APIENTRY void crustMemset(void *dst, u8 value, usize size);

#ifdef __cplusplus
}
#endif
