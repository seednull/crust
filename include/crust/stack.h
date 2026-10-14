#pragma once

#include "core.h"

#ifdef __cplusplus
extern "C" {
#endif

//
typedef struct Crust_Stack_t
{
	void *memory;
	usize capacity;
	usize head;
} Crust_Stack;

CRUST_APIENTRY Crust_Stack crustStackAttach(void *memory, usize capacity);
CRUST_APIENTRY void crustStackReset(Crust_Stack *stack);
CRUST_APIENTRY void crustStackPush(Crust_Stack *stack, const void *data, usize size);
CRUST_APIENTRY void crustStackPeek(const Crust_Stack *stack, void *data, usize size);
CRUST_APIENTRY void crustStackPop(Crust_Stack *stack, void *data, usize size);

#ifdef __cplusplus
}
#endif
