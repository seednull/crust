#include <crust.h>

//
CRUST_APIENTRY Crust_RingBuffer crustRingBufferAttach(void *memory, usize capacity)
{
	CRUST_ASSERT(memory != CRUST_NULL);
	CRUST_ASSERT(capacity > 0);

	Crust_RingBuffer result;
	result.memory = memory;
	result.capacity = capacity;
	result.size = 0;
	result.read = 0;
	result.write = 0;

	return result;
}

CRUST_APIENTRY void crustRingBufferReset(Crust_RingBuffer *ring)
{
	CRUST_ASSERT(ring != CRUST_NULL);
	CRUST_ASSERT(ring->memory != CRUST_NULL);

	ring->size = 0;
	ring->read = 0;
	ring->write = 0;
}

CRUST_APIENTRY void crustRingBufferRead(Crust_RingBuffer *ring, void *data, usize size)
{
	CRUST_ASSERT(ring != CRUST_NULL);
	CRUST_ASSERT(ring->memory != CRUST_NULL);
	CRUST_ASSERT(ring->size >= size);
	CRUST_ASSERT(data != CRUST_NULL);

	if (size == 0)
		return;

	usize begin = ring->read;
	usize end = (begin + size) % ring->capacity;

	ring->read = end;
	ring->size -= size;

	u8 *ptr = (u8 *)ring->memory + begin;
	if (begin < end)
	{
		crustMemcpy(data, ptr, size);
	}
	else
	{
		usize remainder = end;
		usize base = size - remainder;
		crustMemcpy(data, ptr, base);
		crustMemcpy((u8 *)data + base, ring->memory, remainder);
	}

}

CRUST_APIENTRY void crustRingBufferWrite(Crust_RingBuffer *ring, const void *data, usize size)
{
	CRUST_ASSERT(ring != CRUST_NULL);
	CRUST_ASSERT(ring->memory != CRUST_NULL);
	CRUST_ASSERT(ring->size + size <= ring->capacity);
	CRUST_ASSERT(data != CRUST_NULL);

	if (size == 0)
		return;

	usize begin = ring->write;
	usize end = (begin + size) % ring->capacity;

	ring->write = end;
	ring->size += size;

	u8 *ptr = (u8 *)ring->memory + begin;
	if (begin < end)
	{
		crustMemcpy(ptr, data, size);
	}
	else
	{
		usize remainder = end;
		usize base = size - remainder;
		crustMemcpy(ptr, data, base);
		crustMemcpy(ring->memory, (const u8 *)data + base, remainder);
	}
}

//
CRUST_APIENTRY Crust_SpscRingBuffer crustSpscRingBufferAttach(void *memory, usize capacity)
{
	CRUST_ASSERT(memory != CRUST_NULL);
	CRUST_ASSERT(capacity > 0);
	CRUST_ASSERT(crustIsPow2USize(capacity) != 0);

	Crust_SpscRingBuffer result;
	result.memory = memory;
	result.capacity = capacity;
	result.mask = capacity - 1;
	result.read = 0;
	result.write = 0;

	return result;
}

CRUST_APIENTRY void crustSpscRingBufferReset(Crust_SpscRingBuffer *ring)
{
	CRUST_ASSERT(ring != CRUST_NULL);
	CRUST_ASSERT(ring->memory != CRUST_NULL);

	crustAtomicStoreUSize(&ring->read, 0, CRUST_MEMORY_ORDER_RELAXED);
	crustAtomicStoreUSize(&ring->write, 0, CRUST_MEMORY_ORDER_RELAXED);
}

CRUST_APIENTRY usize crustSpscRingBufferRead(Crust_SpscRingBuffer *ring, void *data, usize size)
{
	CRUST_ASSERT(ring != CRUST_NULL);
	CRUST_ASSERT(ring->memory != CRUST_NULL);
	CRUST_ASSERT(data != CRUST_NULL);

	if (size == 0)
		return 0;

	usize r = crustAtomicLoadUSize(&ring->read, CRUST_MEMORY_ORDER_RELAXED);
	usize w = crustAtomicLoadUSize(&ring->write, CRUST_MEMORY_ORDER_ACQUIRE);

	usize used = w - r;
	CRUST_ASSERT(used <= ring->capacity);

	if (used == 0)
		return 0;

	size = crustMinUSize(size, used);

	usize begin = r & ring->mask;
	usize end = (begin + size) & ring->mask;

	const u8 *ptr = (const u8 *)ring->memory + begin;
	if (begin < end)
	{
		crustMemcpy(data, ptr, size);
	}
	else
	{
		usize remainder = end;
		usize base = size - remainder;
		crustMemcpy(data, ptr, base);
		crustMemcpy((u8 *)data + base, ring->memory, remainder);
	}

	crustAtomicStoreUSize(&ring->read, r + size, CRUST_MEMORY_ORDER_RELEASE);
	return size;
}

CRUST_APIENTRY usize crustSpscRingBufferWrite(Crust_SpscRingBuffer *ring, const void *data, usize size)
{
	CRUST_ASSERT(ring != CRUST_NULL);
	CRUST_ASSERT(ring->memory != CRUST_NULL);
	CRUST_ASSERT(data != CRUST_NULL);

	if (size == 0)
		return 0;

	usize r = crustAtomicLoadUSize(&ring->read, CRUST_MEMORY_ORDER_ACQUIRE);
	usize w = crustAtomicLoadUSize(&ring->write, CRUST_MEMORY_ORDER_RELAXED);

	usize used = w - r;
	CRUST_ASSERT(used <= ring->capacity);

	usize free = ring->capacity - used;

	if (free == 0)
		return 0;

	size = crustMinUSize(size, free);

	usize begin = w & ring->mask;
	usize end = (begin + size) & ring->mask;

	u8 *ptr = (u8 *)ring->memory + begin;
	if (begin < end)
	{
		crustMemcpy(ptr, data, size);
	}
	else
	{
		usize remainder = end;
		usize base = size - remainder;
		crustMemcpy(ptr, data, base);
		crustMemcpy(ring->memory, (const u8 *)data + base, remainder);
	}

	crustAtomicStoreUSize(&ring->write, w + size, CRUST_MEMORY_ORDER_RELEASE);
	return size;
}
