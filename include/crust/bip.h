#pragma once

#include "core.h"

#ifdef __cplusplus
extern "C" {
#endif

//
typedef struct Crust_BipBuffer_t
{
	void *memory;
	usize capacity;
	usize begin[2];
	usize end[2];
	usize read_staged;
	usize write_staged;
	usize reader;
	usize writer;
} Crust_BipBuffer;

CRUST_APIENTRY Crust_BipBuffer crustBipBufferAttach(void *memory, usize capacity);
CRUST_APIENTRY void crustBipBufferReset(Crust_BipBuffer *bip);
CRUST_APIENTRY const void *crustBipBufferStageRead(Crust_BipBuffer *bip, usize size);
CRUST_APIENTRY void crustBipBufferCommitRead(Crust_BipBuffer *bip, usize size);
CRUST_APIENTRY void *crustBipBufferStageWrite(Crust_BipBuffer *bip, usize size);
CRUST_APIENTRY void crustBipBufferCommitWrite(Crust_BipBuffer *bip, usize size);

//
typedef struct Crust_SpscBipBuffer_t
{
	void *memory;
	usize capacity;
	u8 padding0[128 - sizeof(void *) - sizeof(usize)];

	CRUST_ALIGNAS(128) volatile usize begin[2];
	volatile usize reader;
	usize read_staged;
	u8 padding1[128 - 4 * sizeof(usize)];

	CRUST_ALIGNAS(128) volatile usize end[2];
	volatile usize writer;
	usize write_staged;
	u8 padding2[128 - 4 * sizeof(usize)];
} Crust_SpscBipBuffer;

CRUST_APIENTRY Crust_SpscBipBuffer crustSpscBipBufferAttach(void *memory, usize capacity);
CRUST_APIENTRY void crustSpscBipBufferReset(Crust_SpscBipBuffer *bip);
CRUST_APIENTRY const void *crustSpscBipBufferStageRead(Crust_SpscBipBuffer *bip, usize size);
CRUST_APIENTRY void crustSpscBipBufferCommitRead(Crust_SpscBipBuffer *bip, usize size);
CRUST_APIENTRY void *crustSpscBipBufferStageWrite(Crust_SpscBipBuffer *bip, usize size);
CRUST_APIENTRY void crustSpscBipBufferCommitWrite(Crust_SpscBipBuffer *bip, usize size);

#ifdef __cplusplus
}
#endif
