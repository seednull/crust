#pragma once

#include "core.h"

#ifdef __cplusplus
extern "C" {
#endif

//
typedef struct Crust_RingBuffer_t
{
	void *memory;
	usize capacity;
	usize size;
	usize read;
	usize write;
} Crust_RingBuffer;

CRUST_APIENTRY Crust_RingBuffer crustRingBufferAttach(void *memory, usize capacity);
CRUST_APIENTRY void crustRingBufferReset(Crust_RingBuffer *ring);
CRUST_APIENTRY void crustRingBufferRead(Crust_RingBuffer *ring, void *data, usize size);
CRUST_APIENTRY void crustRingBufferWrite(Crust_RingBuffer *ring, const void *data, usize size);

//
typedef struct Crust_SpscRingBuffer_t
{
	void *memory;
	usize capacity;
	usize mask;
	u8 padding0[CRUST_CACHE_LINE_SIZE - sizeof(void *) - 2 * sizeof(usize)];

	CRUST_ALIGNAS(CRUST_CACHE_LINE_SIZE) volatile usize read;
	u8 padding1[CRUST_CACHE_LINE_SIZE - sizeof(usize)];

	CRUST_ALIGNAS(CRUST_CACHE_LINE_SIZE) volatile usize write;
	u8 padding2[CRUST_CACHE_LINE_SIZE - sizeof(usize)];
} Crust_SpscRingBuffer;

CRUST_APIENTRY Crust_SpscRingBuffer crustSpscRingBufferAttach(void *memory, usize capacity);
CRUST_APIENTRY void crustSpscRingBufferReset(Crust_SpscRingBuffer *ring);
CRUST_APIENTRY usize crustSpscRingBufferRead(Crust_SpscRingBuffer *ring, void *data, usize size);
CRUST_APIENTRY usize crustSpscRingBufferWrite(Crust_SpscRingBuffer *ring, const void *data, usize size);

#ifdef __cplusplus
}
#endif
