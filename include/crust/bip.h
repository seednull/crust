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

#if CRUST_COMPILER_MSVC
	#pragma warning(push)
	#pragma warning(disable: 4324)
#endif

//
typedef struct Crust_SpscBipBuffer_t
{
	void *memory;
	usize capacity;

	CRUST_ALIGNAS(128) volatile usize begin[2];
	volatile usize reader;
	usize read_staged;

	CRUST_ALIGNAS(128) volatile usize end[2];
	volatile usize writer;
	usize write_staged;
} Crust_SpscBipBuffer;

CRUST_APIENTRY Crust_SpscBipBuffer crustSpscBipBufferAttach(void *memory, usize capacity);
CRUST_APIENTRY void crustSpscBipBufferReset(Crust_SpscBipBuffer *bip);
CRUST_APIENTRY const void *crustSpscBipBufferStageRead(Crust_SpscBipBuffer *bip, usize size);
CRUST_APIENTRY void crustSpscBipBufferCommitRead(Crust_SpscBipBuffer *bip, usize size);
CRUST_APIENTRY void *crustSpscBipBufferStageWrite(Crust_SpscBipBuffer *bip, usize size);
CRUST_APIENTRY void crustSpscBipBufferCommitWrite(Crust_SpscBipBuffer *bip, usize size);

#if CRUST_COMPILER_MSVC
	#pragma warning(pop)
#endif

#ifdef __cplusplus
}
#endif
