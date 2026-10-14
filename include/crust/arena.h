#pragma once

#include "allocator.h"

#ifdef __cplusplus
extern "C" {
#endif

//
typedef struct Crust_Arena_t
{
	void *memory;
	usize capacity;
	usize size;
} Crust_Arena;

CRUST_APIENTRY Crust_Arena crustArenaAttach(void *memory, usize capacity);
CRUST_APIENTRY void crustArenaReset(Crust_Arena *arena);
CRUST_APIENTRY void *crustArenaAlloc(Crust_Arena *arena, usize size, usize alignment);

//
CRUST_APIENTRY Crust_Allocator crustArenaAllocatorInit(Crust_Arena *arena);

#ifdef __cplusplus
}
#endif
