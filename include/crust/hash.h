#pragma once

#include "allocator.h"

#ifdef __cplusplus
extern "C" {
#endif

//
typedef usize (*PFN_crustHashIndexMapHash)(void *context, const void *key, usize key_size);
typedef u8 (*PFN_crustHashIndexMapEqual)(void *context, const void *key1, const void *key2, usize key_size);
typedef void (*PFN_crustHashIndexMapCopy)(void *context, void *key_dst, const void *key_src, usize key_size);
typedef void (*PFN_crustHashIndexMapMove)(void *context, void *key_dst, void *key_src, usize key_size);
typedef void (*PFN_crustHashIndexMapDestroy)(void *context, void *key, usize key_size);

typedef struct Crust_HashIndexMapVtbl_t
{
	PFN_crustHashIndexMapHash hash;
	PFN_crustHashIndexMapEqual equal;
	PFN_crustHashIndexMapCopy copy;
	PFN_crustHashIndexMapMove move;
	PFN_crustHashIndexMapDestroy destroy;
} Crust_HashIndexMapVtbl;

typedef struct Crust_HashIndexMap_t
{
	void *context;
	const Crust_HashIndexMapVtbl *vtbl;

	u8 *keys;
	usize *indices;
	usize *hashes;
	u32 *masks;

	usize count;
	usize capacity;
	usize key_size;
	usize key_alignment;
	usize key_stride;
} Crust_HashIndexMap;

CRUST_APIENTRY Crust_HashIndexMap crustHashIndexMapInit(void *context, const Crust_HashIndexMapVtbl *vtbl, usize key_size, usize key_alignment);
CRUST_APIENTRY void crustHashIndexMapAlloc(Crust_Allocator allocator, Crust_HashIndexMap *hash_map, usize capacity);
CRUST_APIENTRY void crustHashIndexMapFree(Crust_Allocator allocator, Crust_HashIndexMap *hash_map);
CRUST_APIENTRY void crustHashIndexMapGrow(Crust_Allocator allocator, Crust_HashIndexMap *hash_map, usize new_capacity);
CRUST_APIENTRY usize crustHashIndexMapFind(const Crust_HashIndexMap *hash_map, const void *key, usize key_size);
CRUST_APIENTRY usize crustHashIndexMapAdd(Crust_HashIndexMap *hash_map, const void *key, usize key_size, usize index);
CRUST_APIENTRY usize crustHashIndexMapRemove(Crust_HashIndexMap *hash_map, const void *key, usize key_size);

#ifdef __cplusplus
}
#endif
