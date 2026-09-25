#include <crust.h>

//
CRUST_APIENTRY Crust_Stack crustStackAttach(void *memory, usize capacity)
{
	CRUST_ASSERT(memory != CRUST_NULL);
	CRUST_ASSERT(capacity > 0);

	Crust_Stack result;
	result.memory = memory;
	result.capacity = capacity;
	result.head = 0;

	return result;
}

CRUST_APIENTRY void crustStackReset(Crust_Stack *stack)
{
	CRUST_ASSERT(stack != CRUST_NULL);
	CRUST_ASSERT(stack->memory != CRUST_NULL);

	stack->head = 0;
}

CRUST_APIENTRY void crustStackPush(Crust_Stack *stack, const void *data, usize size)
{
	CRUST_ASSERT(stack != CRUST_NULL);
	CRUST_ASSERT(stack->memory != CRUST_NULL);
	CRUST_ASSERT(stack->head + size <= stack->capacity);
	CRUST_ASSERT(data != CRUST_NULL);

	if (size == 0)
		return;

	u8 *ptr = (u8 *)stack->memory + stack->head;
	crustMemcpy(ptr, data, size);

	stack->head += size;
}

CRUST_APIENTRY void crustStackPeek(const Crust_Stack *stack, void *data, usize size)
{
	CRUST_ASSERT(stack != CRUST_NULL);
	CRUST_ASSERT(stack->memory != CRUST_NULL);
	CRUST_ASSERT(stack->head >= size);
	CRUST_ASSERT(data != CRUST_NULL);

	usize offset = stack->head - size;

	const u8 *ptr = (const u8 *)stack->memory + offset;
	crustMemcpy(data, ptr, size);
}

CRUST_APIENTRY void crustStackPop(Crust_Stack *stack, void *data, usize size)
{
	CRUST_ASSERT(stack != CRUST_NULL);
	CRUST_ASSERT(stack->memory != CRUST_NULL);
	CRUST_ASSERT(stack->head >= size);
	CRUST_ASSERT(data != CRUST_NULL);

	stack->head -= size;

	u8 *ptr = (u8 *)stack->memory + stack->head;
	crustMemcpy(data, ptr, size);
}
