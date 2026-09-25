#include <crust.h>

//
CRUST_APIENTRY Crust_BipBuffer crustBipBufferAttach(void *memory, usize capacity)
{
	CRUST_ASSERT(memory != CRUST_NULL);
	CRUST_ASSERT(capacity > 0);

	Crust_BipBuffer result;
	result.memory = memory;
	result.capacity = capacity;

	for (usize i = 0; i < 2; ++i)
	{
		result.begin[i] = 0;
		result.end[i] = 0;
	}

	result.read_staged = 0;
	result.write_staged = 0;
	result.reader = 0;
	result.writer = 0;

	return result;
}

CRUST_APIENTRY void crustBipBufferReset(Crust_BipBuffer *bip)
{
	CRUST_ASSERT(bip != CRUST_NULL);
	CRUST_ASSERT(bip->memory != CRUST_NULL);

	for (usize i = 0; i < 2; ++i)
	{
		bip->begin[i] = 0;
		bip->end[i] = 0;
	}

	bip->read_staged = 0;
	bip->write_staged = 0;
	bip->reader = 0;
	bip->writer = 0;
}

CRUST_APIENTRY const void *crustBipBufferStageRead(Crust_BipBuffer *bip, usize size)
{
	CRUST_ASSERT(bip != CRUST_NULL);
	CRUST_ASSERT(bip->memory != CRUST_NULL);
	CRUST_ASSERT(bip->read_staged == 0);
	CRUST_ASSERT(size > 0);

	usize r = bip->reader;

	usize begin = bip->begin[r];
	usize end = bip->end[r];

	if (begin != end)
	{
		if (end - begin < size)
			return CRUST_NULL;

		bip->read_staged = size;
		return (const u8 *)bip->memory + begin;
	}

	usize w = bip->writer;

	if (r == w)
		return CRUST_NULL;

	usize next = (r + 1) % 2;
	CRUST_ASSERT(next == w);

	end = bip->end[next];

	if (end < size)
		return CRUST_NULL;

	bip->begin[next] = 0;
	bip->reader = next;

	bip->read_staged = size;
	return bip->memory;
}

CRUST_APIENTRY void crustBipBufferCommitRead(Crust_BipBuffer *bip, usize size)
{
	CRUST_ASSERT(bip != CRUST_NULL);
	CRUST_ASSERT(bip->memory != CRUST_NULL);
	CRUST_ASSERT(bip->read_staged != 0);
	CRUST_ASSERT(bip->read_staged >= size);
	CRUST_ASSERT(size > 0);

	usize r = bip->reader;
	usize begin = bip->begin[r];

	bip->begin[r] = begin + size;
	bip->read_staged = 0;
}

CRUST_APIENTRY void *crustBipBufferStageWrite(Crust_BipBuffer *bip, usize size)
{
	CRUST_ASSERT(bip != CRUST_NULL);
	CRUST_ASSERT(bip->memory != CRUST_NULL);
	CRUST_ASSERT(bip->write_staged == 0);
	CRUST_ASSERT(size > 0);

	usize r = bip->reader;
	usize w = bip->writer;

	usize read_begin = bip->begin[r];
	usize read_end = bip->end[r];
	usize write_end = bip->end[w];

	if (r == w)
	{
		if (bip->capacity - write_end >= size)
		{
			bip->write_staged = size;
			return (u8 *)bip->memory + write_end;
		}

		usize available = (read_begin == read_end) ? bip->capacity : read_begin;

		if (available < size)
			return CRUST_NULL;

		usize next = (w + 1) % 2;

		usize next_begin = bip->begin[next];
		usize next_end = bip->end[next];

		CRUST_ASSERT(next_begin == next_end);
		CRUST_UNUSED(next_begin);
		CRUST_UNUSED(next_end);

		bip->end[next] = 0;
		bip->writer = next;

		bip->write_staged = size;
		return bip->memory;
	}

	usize available = (read_begin == read_end) ? bip->capacity : read_begin;
	
	CRUST_ASSERT(available >= write_end);
	available -= write_end;

	if (available < size)
		return CRUST_NULL;

	bip->write_staged = size;
	return (u8 *)bip->memory + write_end;
}

CRUST_APIENTRY void crustBipBufferCommitWrite(Crust_BipBuffer *bip, usize size)
{
	CRUST_ASSERT(bip != CRUST_NULL);
	CRUST_ASSERT(bip->memory != CRUST_NULL);
	CRUST_ASSERT(bip->write_staged != 0);
	CRUST_ASSERT(bip->write_staged >= size);
	CRUST_ASSERT(size > 0);

	usize w = bip->writer;
	usize end = bip->end[w];

	bip->end[w] = end + size;
	bip->write_staged = 0;
}

//
CRUST_APIENTRY Crust_SpscBipBuffer crustSpscBipBufferAttach(void *memory, usize capacity)
{
	CRUST_ASSERT(memory != CRUST_NULL);
	CRUST_ASSERT(capacity > 0);

	Crust_SpscBipBuffer result;
	result.memory = memory;
	result.capacity = capacity;

	for (usize i = 0; i < 2; ++i)
	{
		result.begin[i] = 0;
		result.end[i] = 0;
	}

	result.read_staged = 0;
	result.write_staged = 0;
	result.reader = 0;
	result.writer = 0;

	return result;
}

CRUST_APIENTRY void crustSpscBipBufferReset(Crust_SpscBipBuffer *bip)
{
	CRUST_ASSERT(bip != CRUST_NULL);
	CRUST_ASSERT(bip->memory != CRUST_NULL);

	for (usize i = 0; i < 2; ++i)
	{
		crustAtomicStoreUSize(&bip->begin[i], 0, CRUST_MEMORY_ORDER_RELAXED);
		crustAtomicStoreUSize(&bip->end[i], 0, CRUST_MEMORY_ORDER_RELAXED);
	}

	crustAtomicStoreUSize(&bip->reader, 0, CRUST_MEMORY_ORDER_RELAXED);
	crustAtomicStoreUSize(&bip->writer, 0, CRUST_MEMORY_ORDER_RELAXED);

	bip->read_staged = 0;
	bip->write_staged = 0;
}

CRUST_APIENTRY const void *crustSpscBipBufferStageRead(Crust_SpscBipBuffer *bip, usize size)
{
	CRUST_ASSERT(bip != CRUST_NULL);
	CRUST_ASSERT(bip->memory != CRUST_NULL);
	CRUST_ASSERT(bip->read_staged == 0);
	CRUST_ASSERT(size > 0);

	usize r = crustAtomicLoadUSize(&bip->reader, CRUST_MEMORY_ORDER_RELAXED);

	usize begin = crustAtomicLoadUSize(&bip->begin[r], CRUST_MEMORY_ORDER_RELAXED);
	usize end = crustAtomicLoadUSize(&bip->end[r], CRUST_MEMORY_ORDER_ACQUIRE);

	if (begin != end)
	{
		if (end - begin < size)
			return CRUST_NULL;

		bip->read_staged = size;
		return (const u8 *)bip->memory + begin;
	}

	usize w = crustAtomicLoadUSize(&bip->writer, CRUST_MEMORY_ORDER_ACQUIRE);

	if (r == w)
		return CRUST_NULL;

	usize next = (r + 1) % 2;
	CRUST_ASSERT(next == w);

	end = crustAtomicLoadUSize(&bip->end[next], CRUST_MEMORY_ORDER_ACQUIRE);

	if (end < size)
		return CRUST_NULL;

	crustAtomicStoreUSize(&bip->begin[next], 0, CRUST_MEMORY_ORDER_RELAXED);
	crustAtomicStoreUSize(&bip->reader, next, CRUST_MEMORY_ORDER_RELEASE);

	bip->read_staged = size;
	return bip->memory;
}

CRUST_APIENTRY void crustSpscBipBufferCommitRead(Crust_SpscBipBuffer *bip, usize size)
{
	CRUST_ASSERT(bip != CRUST_NULL);
	CRUST_ASSERT(bip->memory != CRUST_NULL);
	CRUST_ASSERT(bip->read_staged != 0);
	CRUST_ASSERT(bip->read_staged >= size);
	CRUST_ASSERT(size > 0);

	usize r = crustAtomicLoadUSize(&bip->reader, CRUST_MEMORY_ORDER_RELAXED);
	usize begin = crustAtomicLoadUSize(&bip->begin[r], CRUST_MEMORY_ORDER_RELAXED);

	crustAtomicStoreUSize(&bip->begin[r], begin + size, CRUST_MEMORY_ORDER_RELEASE);
	bip->read_staged = 0;
}

CRUST_APIENTRY void *crustSpscBipBufferStageWrite(Crust_SpscBipBuffer *bip, usize size)
{
	CRUST_ASSERT(bip != CRUST_NULL);
	CRUST_ASSERT(bip->memory != CRUST_NULL);
	CRUST_ASSERT(bip->write_staged == 0);
	CRUST_ASSERT(size > 0);

	usize r = crustAtomicLoadUSize(&bip->reader, CRUST_MEMORY_ORDER_ACQUIRE);
	usize w = crustAtomicLoadUSize(&bip->writer, CRUST_MEMORY_ORDER_RELAXED);

	usize read_begin = crustAtomicLoadUSize(&bip->begin[r], CRUST_MEMORY_ORDER_ACQUIRE);
	usize read_end = crustAtomicLoadUSize(&bip->end[r], CRUST_MEMORY_ORDER_RELAXED);
	usize write_end = crustAtomicLoadUSize(&bip->end[w], CRUST_MEMORY_ORDER_RELAXED);

	if (r == w)
	{
		if (bip->capacity - write_end >= size)
		{
			bip->write_staged = size;
			return (u8 *)bip->memory + write_end;
		}

		usize available = (read_begin == read_end) ? bip->capacity : read_begin;

		if (available < size)
			return CRUST_NULL;

		usize next = (w + 1) % 2;

		usize next_begin = crustAtomicLoadUSize(&bip->begin[next], CRUST_MEMORY_ORDER_ACQUIRE);
		usize next_end = crustAtomicLoadUSize(&bip->end[next], CRUST_MEMORY_ORDER_RELAXED);

		CRUST_ASSERT(next_begin == next_end);
		CRUST_UNUSED(next_begin);
		CRUST_UNUSED(next_end);

		crustAtomicStoreUSize(&bip->end[next], 0, CRUST_MEMORY_ORDER_RELAXED);
		crustAtomicStoreUSize(&bip->writer, next, CRUST_MEMORY_ORDER_RELEASE);

		bip->write_staged = size;
		return bip->memory;
	}

	usize available = (read_begin == read_end) ? bip->capacity : read_begin;

	CRUST_ASSERT(available >= write_end);
	available -= write_end;

	if (available < size)
		return CRUST_NULL;

	bip->write_staged = size;
	return (u8 *)bip->memory + write_end;
}

CRUST_APIENTRY void crustSpscBipBufferCommitWrite(Crust_SpscBipBuffer *bip, usize size)
{
	CRUST_ASSERT(bip != CRUST_NULL);
	CRUST_ASSERT(bip->memory != CRUST_NULL);
	CRUST_ASSERT(bip->write_staged != 0);
	CRUST_ASSERT(bip->write_staged >= size);
	CRUST_ASSERT(size > 0);

	usize w = crustAtomicLoadUSize(&bip->writer, CRUST_MEMORY_ORDER_RELAXED);
	usize end = crustAtomicLoadUSize(&bip->end[w], CRUST_MEMORY_ORDER_RELAXED);

	crustAtomicStoreUSize(&bip->end[w], end + size, CRUST_MEMORY_ORDER_RELEASE);
	bip->write_staged = 0;
}
