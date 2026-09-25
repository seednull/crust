#include <crust.h>

//
typedef struct Crust_HashIndexMapProbe_t
{
	usize bucket;
	u8 found;
} Crust_HashIndexMapProbe;

//
static u8 crustHashIndexMapCheckMask(const Crust_HashIndexMap *hash_map, usize bucket)
{
	CRUST_ASSERT(hash_map != CRUST_NULL);
	CRUST_ASSERT(bucket < hash_map->capacity);

	usize mask_index = bucket / 32;
	u32 bit_mask = (1u << (bucket % 32));

	return (hash_map->masks[mask_index] & bit_mask) != 0;
}

static void crustHashIndexMapAddMask(Crust_HashIndexMap *hash_map, usize bucket)
{
	CRUST_ASSERT(hash_map != CRUST_NULL);
	CRUST_ASSERT(bucket < hash_map->capacity);

	usize mask_index = bucket / 32;
	u32 bit_mask = (1u << (bucket % 32));

	hash_map->masks[mask_index] |= bit_mask;
}

static void crustHashIndexMapRemoveMask(Crust_HashIndexMap *hash_map, usize bucket)
{
	CRUST_ASSERT(hash_map != CRUST_NULL);
	CRUST_ASSERT(bucket < hash_map->capacity);

	usize mask_index = bucket / 32;
	u32 bit_mask = (1u << (bucket % 32));

	hash_map->masks[mask_index] &= ~bit_mask;
}

static u8 *crustHashIndexMapKey(Crust_HashIndexMap *hash_map, usize bucket)
{
	CRUST_ASSERT(hash_map != CRUST_NULL);
	CRUST_ASSERT(bucket < hash_map->capacity);

	return hash_map->keys + hash_map->key_stride * bucket;
}

static const u8 *crustHashIndexMapConstKey(const Crust_HashIndexMap *hash_map, usize bucket)
{
	CRUST_ASSERT(hash_map != CRUST_NULL);
	CRUST_ASSERT(bucket < hash_map->capacity);

	return hash_map->keys + hash_map->key_stride * bucket;
}

static void crustHashIndexMapFreeStorage(Crust_Allocator allocator, Crust_HashIndexMap *hash_map)
{
	CRUST_ASSERT(hash_map != CRUST_NULL);

	usize masks_capacity = crustAlignUpUSize(hash_map->capacity, 32) / 32;

	crustAllocatorFree(allocator, hash_map->keys, hash_map->key_stride * hash_map->capacity, hash_map->key_alignment);
	crustFreeArray(allocator, hash_map->hashes, usize, hash_map->capacity);
	crustFreeArray(allocator, hash_map->indices, usize, hash_map->capacity);
	crustFreeArray(allocator, hash_map->masks, u32, masks_capacity);

	hash_map->keys = CRUST_NULL;
	hash_map->hashes = CRUST_NULL;
	hash_map->indices = CRUST_NULL;
	hash_map->masks = CRUST_NULL;

	hash_map->capacity = 0;
	hash_map->count = 0;
}

static Crust_HashIndexMapProbe crustHashIndexMapProbeKey(const Crust_HashIndexMap *hash_map, const void *key, usize key_size, usize hash)
{
	CRUST_ASSERT(hash_map != CRUST_NULL);
	CRUST_ASSERT(hash_map->capacity > 0);

	CRUST_ASSERT(key != CRUST_NULL);
	CRUST_ASSERT(key_size == hash_map->key_size);

	usize bucket = hash % hash_map->capacity;

	for (usize probe = 0; probe < hash_map->capacity; ++probe)
	{
		if (crustHashIndexMapCheckMask(hash_map, bucket) == 0)
		{
			Crust_HashIndexMapProbe result = { bucket, 0 };
			return result;
		}

		if (hash_map->hashes[bucket] == hash)
		{
			const u8 *stored_key = crustHashIndexMapConstKey(hash_map, bucket);

			if (hash_map->vtbl->equal(hash_map->context, stored_key, key, key_size))
			{
				Crust_HashIndexMapProbe result = { bucket, 1 };
				return result;
			}
		}

		bucket = (bucket + 1) % hash_map->capacity;
	}

	Crust_HashIndexMapProbe result = { CRUST_USIZE_MAX, 0 };
	return result;
}

static usize crustHashIndexMapFindFreeBucket(const Crust_HashIndexMap *hash_map, usize hash)
{
	CRUST_ASSERT(hash_map != CRUST_NULL);
	CRUST_ASSERT(hash_map->capacity > 0);
	CRUST_ASSERT(hash_map->count < hash_map->capacity);

	usize bucket = hash % hash_map->capacity;

	for (usize probe = 0; probe < hash_map->capacity; ++probe)
	{
		if (crustHashIndexMapCheckMask(hash_map, bucket) == 0)
			return bucket;

		bucket = (bucket + 1) % hash_map->capacity;
	}

	CRUST_ASSERT(0);
	return CRUST_USIZE_MAX;
}

//
CRUST_APIENTRY Crust_HashIndexMap crustHashIndexMapInit(void *context, const Crust_HashIndexMapVtbl *vtbl, usize key_size, usize key_alignment)
{
	CRUST_ASSERT(vtbl != CRUST_NULL);
	CRUST_ASSERT(vtbl->hash != CRUST_NULL);
	CRUST_ASSERT(vtbl->equal != CRUST_NULL);
	CRUST_ASSERT(vtbl->copy != CRUST_NULL);
	CRUST_ASSERT(vtbl->move != CRUST_NULL);
	CRUST_ASSERT(vtbl->destroy != CRUST_NULL);

	CRUST_ASSERT(key_size > 0);
	CRUST_ASSERT(key_alignment > 0);
	CRUST_ASSERT(crustIsPow2USize(key_alignment) != 0);

	Crust_HashIndexMap result;
	result.context = context;
	result.vtbl = vtbl;

	result.keys = CRUST_NULL;
	result.indices = CRUST_NULL;
	result.hashes = CRUST_NULL;
	result.masks = CRUST_NULL;

	result.count = 0;
	result.capacity = 0;
	result.key_size = key_size;
	result.key_alignment = key_alignment;
	result.key_stride = crustAlignUpUSize(key_size, key_alignment);

	return result;
}

CRUST_APIENTRY void crustHashIndexMapAlloc(Crust_Allocator allocator, Crust_HashIndexMap *hash_map, usize capacity)
{
	CRUST_ASSERT(hash_map != CRUST_NULL);
	CRUST_ASSERT(hash_map->capacity == 0);

	CRUST_ASSERT(capacity > 0);

	usize masks_capacity = crustAlignUpUSize(capacity, 32) / 32;

	hash_map->keys = (u8 *)crustAllocatorAlloc(allocator, hash_map->key_stride * capacity, hash_map->key_alignment);
	hash_map->hashes = crustAllocArray(allocator, usize, capacity);
	hash_map->indices = crustAllocArray(allocator, usize, capacity);
	hash_map->masks = crustAllocArray(allocator, u32, masks_capacity);
	hash_map->capacity = capacity;
	hash_map->count = 0;

	for (usize i = 0; i < masks_capacity; ++i)
		hash_map->masks[i] = 0;
}

CRUST_APIENTRY void crustHashIndexMapFree(Crust_Allocator allocator, Crust_HashIndexMap *hash_map)
{
	CRUST_ASSERT(hash_map != CRUST_NULL);

	for (usize i = 0; i < hash_map->capacity; ++i)
	{
		if (crustHashIndexMapCheckMask(hash_map, i) == 0)
			continue;

		u8 *key = crustHashIndexMapKey(hash_map, i);
		hash_map->vtbl->destroy(hash_map->context, key, hash_map->key_size);
	}

	crustHashIndexMapFreeStorage(allocator, hash_map);
}

CRUST_APIENTRY void crustHashIndexMapGrow(Crust_Allocator allocator, Crust_HashIndexMap *hash_map, usize new_capacity)
{
	CRUST_ASSERT(hash_map != CRUST_NULL);

	CRUST_ASSERT(new_capacity > hash_map->capacity);

	Crust_HashIndexMap new_hash_map = crustHashIndexMapInit(hash_map->context, hash_map->vtbl, hash_map->key_size, hash_map->key_alignment);
	crustHashIndexMapAlloc(allocator, &new_hash_map, new_capacity);
	
	for (usize i = 0; i < hash_map->capacity; ++i)
	{
		if (crustHashIndexMapCheckMask(hash_map, i) == 0)
			continue;

		usize hash = hash_map->hashes[i];
		usize index = hash_map->indices[i];

		usize new_bucket = crustHashIndexMapFindFreeBucket(&new_hash_map, hash);

		u8 *dst_key = crustHashIndexMapKey(&new_hash_map, new_bucket);
		u8 *src_key = crustHashIndexMapKey(hash_map, i);

		hash_map->vtbl->move(hash_map->context, dst_key, src_key, hash_map->key_size);

		new_hash_map.hashes[new_bucket] = hash;
		new_hash_map.indices[new_bucket] = index;
		
		crustHashIndexMapAddMask(&new_hash_map, new_bucket);
		new_hash_map.count++;
	}

	crustHashIndexMapFreeStorage(allocator, hash_map);
	*hash_map = new_hash_map;
}

CRUST_APIENTRY usize crustHashIndexMapFind(const Crust_HashIndexMap *hash_map, const void *key, usize key_size)
{
	CRUST_ASSERT(hash_map != CRUST_NULL);

	CRUST_ASSERT(key != CRUST_NULL);
	CRUST_ASSERT(key_size == hash_map->key_size);

	if (hash_map->count == 0)
		return CRUST_USIZE_MAX;

	usize hash = hash_map->vtbl->hash(hash_map->context, key, key_size);
	Crust_HashIndexMapProbe probe = crustHashIndexMapProbeKey(hash_map, key, key_size, hash);

	if (probe.found == 0)
		return CRUST_USIZE_MAX;

	return hash_map->indices[probe.bucket];
}

CRUST_APIENTRY usize crustHashIndexMapAdd(Crust_HashIndexMap *hash_map, const void *key, usize key_size, usize index)
{
	CRUST_ASSERT(hash_map != CRUST_NULL);
	CRUST_ASSERT(hash_map->capacity > 0);

	CRUST_ASSERT(key != CRUST_NULL);
	CRUST_ASSERT(key_size == hash_map->key_size);

	CRUST_ASSERT(index != CRUST_USIZE_MAX);

	usize hash = hash_map->vtbl->hash(hash_map->context, key, key_size);

	Crust_HashIndexMapProbe probe = crustHashIndexMapProbeKey(hash_map, key, key_size, hash);

	if (probe.found)
	{
		usize stored_index = hash_map->indices[probe.bucket];
		CRUST_ASSERT(stored_index == index);

		return stored_index;
	}

	if (probe.bucket == CRUST_USIZE_MAX)
		return CRUST_USIZE_MAX;

	u8 *stored_key = crustHashIndexMapKey(hash_map, probe.bucket);

	hash_map->vtbl->copy(hash_map->context, stored_key, key, key_size);

	hash_map->hashes[probe.bucket] = hash;
	hash_map->indices[probe.bucket] = index;

	crustHashIndexMapAddMask(hash_map, probe.bucket);
	hash_map->count++;

	return index;
}

CRUST_APIENTRY usize crustHashIndexMapRemove(Crust_HashIndexMap *hash_map, const void *key, usize key_size)
{
	CRUST_ASSERT(hash_map != CRUST_NULL);

	CRUST_ASSERT(key != CRUST_NULL);
	CRUST_ASSERT(key_size == hash_map->key_size);

	if (hash_map->count == 0)
		return CRUST_USIZE_MAX;

	usize hash = hash_map->vtbl->hash(hash_map->context, key, key_size);
	Crust_HashIndexMapProbe probe = crustHashIndexMapProbeKey(hash_map, key, key_size, hash);

	if (probe.found == 0)
		return CRUST_USIZE_MAX;

	usize removed_index = hash_map->indices[probe.bucket];
	u8 *stored_key = crustHashIndexMapKey(hash_map, probe.bucket);

	hash_map->vtbl->destroy(hash_map->context, stored_key, hash_map->key_size);

	usize hole = probe.bucket;
	usize scan = (hole + 1) % hash_map->capacity;

	for (usize shift_probe = 0; shift_probe < hash_map->capacity - 1; ++shift_probe)
	{
		if (crustHashIndexMapCheckMask(hash_map, scan) == 0)
			break;

		usize scan_hash = hash_map->hashes[scan];
		usize home = scan_hash % hash_map->capacity;

		usize scan_distance = (scan + hash_map->capacity - home) % hash_map->capacity;
		usize hole_distance = (hole + hash_map->capacity - home) % hash_map->capacity;

		if (hole_distance < scan_distance)
		{
			u8 *dst_key = crustHashIndexMapKey(hash_map, hole);
			u8 *src_key = crustHashIndexMapKey(hash_map, scan);

			hash_map->vtbl->move(hash_map->context, dst_key, src_key, hash_map->key_size);

			hash_map->hashes[hole] = hash_map->hashes[scan];
			hash_map->indices[hole] = hash_map->indices[scan];

			hole = scan;
		}

		scan = (scan + 1) % hash_map->capacity;
	}

	crustHashIndexMapRemoveMask(hash_map, hole);
	hash_map->count--;

	return removed_index;
}
