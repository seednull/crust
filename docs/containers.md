# Containers

| Header | Contents |
|---|---|
| `<crust/hash.h>` | `Crust_HashIndexMap` |
| `<crust/pool.h>` | `Crust_GenerationalHandlePool` |

## Crust_HashIndexMap

A hash table that maps keys to `usize` indices. It doesn't store values: keep your items in your own arrays and use the map to find their index by key.

```c
typedef struct Mesh_t { u64 id; /* ... */ } Mesh;

Mesh meshes[1024];
usize mesh_count = 0;

Crust_HashIndexMap mesh_by_id; // u64 id -> index into meshes
```

- **Open addressing with linear probing.** Collisions go to the next free bucket.
- **Backward-shift deletion.** Removing a key moves the following keys back, so there are no tombstones and lookups don't slow down after many removals.
- **Hashes are cached** per bucket and compared before calling `equal`.
- **Any capacity.** Buckets are chosen as `hash % capacity`, so the capacity doesn't have to be a power of two. A good hash function matters more than the capacity.

### Key operations

Keys are fixed-size blobs of `key_size` bytes. What a key is and how it is hashed, compared, copied and destroyed is up to you, through a table of callbacks. Every callback gets the `context` pointer passed to `crustHashIndexMapInit`:

| Callback | Called by | Purpose |
|---|---|---|
| `hash(context, key, key_size)` | `Find`, `Add`, `Remove` | Returns a `usize` hash |
| `equal(context, key1, key2, key_size)` | `Find`, `Add`, `Remove` | Returns `1` if the keys are equal |
| `copy(context, dst, src, key_size)` | `Add` | Copies the caller's key into the table |
| `move(context, dst, src, key_size)` | `Grow`, `Remove` | Moves a key between buckets. `src` is not used afterwards |
| `destroy(context, key, key_size)` | `Remove`, `Free` | Releases what the key owns |

For plain keys, copy and move are `crustMemcpy` and destroy does nothing. For keys that own memory, such as strings, `copy` can duplicate the data with an allocator kept in `context`, and `destroy` can free it.

```c
static usize hashU64(void *context, const void *key, usize key_size)
{
	CRUST_UNUSED(context);
	CRUST_UNUSED(key_size);

	u64 x = *(const u64 *)key;
	x ^= x >> 33;
	x *= 0xff51afd7ed558ccdULL;
	x ^= x >> 33;
	x *= 0xc4ceb9fe1a85ec53ULL;
	x ^= x >> 33;

	return (usize)x;
}

static u8 equalU64(void *context, const void *key1, const void *key2, usize key_size)
{
	CRUST_UNUSED(context);
	CRUST_UNUSED(key_size);

	return *(const u64 *)key1 == *(const u64 *)key2;
}

static void copyKey(void *context, void *dst, const void *src, usize key_size)
{
	CRUST_UNUSED(context);
	crustMemcpy(dst, src, key_size);
}

static void moveKey(void *context, void *dst, void *src, usize key_size)
{
	CRUST_UNUSED(context);
	crustMemcpy(dst, src, key_size);
}

static void destroyKey(void *context, void *key, usize key_size)
{
	CRUST_UNUSED(context);
	CRUST_UNUSED(key);
	CRUST_UNUSED(key_size);
}

static const Crust_HashIndexMapVtbl u64_keys = { hashU64, equalU64, copyKey, moveKey, destroyKey };
```

### Usage

```c
Crust_Allocator allocator = crustSystemAllocatorInit();

Crust_HashIndexMap map = crustHashIndexMapInit(CRUST_NULL, &u64_keys, sizeof(u64), CRUST_ALIGNOF(u64));
crustHashIndexMapAlloc(allocator, &map, 64);

u64 id = 0x1234;
crustHashIndexMapAdd(&map, &id, sizeof(id), mesh_count++);

usize index = crustHashIndexMapFind(&map, &id, sizeof(id));
if (index != CRUST_USIZE_MAX)
{
	// meshes[index]
}

crustHashIndexMapRemove(&map, &id, sizeof(id));
crustHashIndexMapFree(allocator, &map);
```

| Function | Behavior |
|---|---|
| `crustHashIndexMapInit(context, vtbl, key_size, key_alignment)` | Sets up an empty map without allocating |
| `crustHashIndexMapAlloc(allocator, map, capacity)` | Allocates `capacity` buckets. The map must be empty and unallocated. Returns `0` if the allocator fails, leaving the map unallocated |
| `crustHashIndexMapGrow(allocator, map, new_capacity)` | Rehashes into a larger table, moving keys with `move`. `new_capacity` must be larger. Returns `0` if the allocator fails, leaving the map untouched |
| `crustHashIndexMapFree(allocator, map)` | Destroys every key and frees the storage |
| `crustHashIndexMapAdd(map, key, key_size, index)` | Inserts `key → index` and returns `index`. Returns `CRUST_USIZE_MAX` when every bucket is taken |
| `crustHashIndexMapFind(map, key, key_size)` | Returns the stored index or `CRUST_USIZE_MAX` |
| `crustHashIndexMapRemove(map, key, key_size)` | Removes the key and returns its index, or `CRUST_USIZE_MAX` if it wasn't there |

Things to know:

- **No automatic growth.** `map.count` and `map.capacity` are public; call `Grow` yourself. Linear probing degrades quickly as the table fills, so grow at a load factor of about 0.7–0.8, not when `Add` starts failing:

  ```c
  if ((map.count + 1) * 4 > map.capacity * 3)
  	crustHashIndexMapGrow(allocator, &map, map.capacity * 2);
  ```

- **`Add` doesn't overwrite.** Adding a key that is already present returns the index stored for it. In debug builds it also asserts that the new index is the same; to change an index, `Remove` and `Add` again.
- **`CRUST_USIZE_MAX` is reserved** as the "not found" marker and can't be stored as an index (asserted).
- **`key_size` is checked.** Every call passes the key size, which must equal the map's `key_size` (asserted). It is forwarded to the callbacks.
- **Keys are stored aligned:** each bucket's key starts at a multiple of `key_alignment`.
- **Not thread-safe.** Concurrent reads are fine as long as nothing writes.

### Keeping indices in sync

The map stores indices into your arrays, so moving an item means updating its entry. With swap-remove:

```c
void removeMesh(u64 id)
{
	usize index = crustHashIndexMapRemove(&mesh_by_id, &id, sizeof(id));
	if (index == CRUST_USIZE_MAX)
		return;

	usize last = --mesh_count;

	if (index != last)
	{
		meshes[index] = meshes[last];

		u64 moved_id = meshes[index].id;
		crustHashIndexMapRemove(&mesh_by_id, &moved_id, sizeof(moved_id));
		crustHashIndexMapAdd(&mesh_by_id, &moved_id, sizeof(moved_id), index);
	}
}
```

## Crust_GenerationalHandlePool

Hands out handles that detect use after free. A handle is an index plus a generation:

```c
typedef struct Crust_GenerationalHandle_t
{
	usize generation;
	usize index;
} Crust_GenerationalHandle;
```

The pool tracks which slots are in use and the current generation of each slot. When a slot is freed and reused, its generation changes, so old handles to it stop validating. The pool stores no data: keep it in your own arrays indexed by `handle.index`.

```c
Crust_Allocator allocator = crustSystemAllocatorInit();

Crust_GenerationalHandlePool pool;
crustGenerationalHandlePoolAlloc(allocator, &pool, 64);

Texture textures[64];

Crust_GenerationalHandle handle = crustGenerationalHandlePoolAdd(&pool, CRUST_USIZE_MAX);
textures[handle.index] = loadTexture("albedo.png");

if (crustGenerationalHandlePoolCheck(&pool, handle))
	useTexture(&textures[handle.index]);

crustGenerationalHandlePoolRemove(&pool, handle);
crustGenerationalHandlePoolCheck(&pool, handle); // 0 from now on

crustGenerationalHandlePoolFree(allocator, &pool);
```

| Function | Behavior |
|---|---|
| `crustGenerationalHandlePoolAlloc(allocator, pool, capacity)` | Allocates `capacity` free slots. Returns `0` if the allocator fails, leaving the pool empty |
| `crustGenerationalHandlePoolGrow(allocator, pool, new_capacity)` | Adds slots. Existing handles stay valid. Returns `0` if the allocator fails, leaving the pool untouched |
| `crustGenerationalHandlePoolFree(allocator, pool)` | Frees the pool |
| `crustGenerationalHandlePoolAdd(pool, max_generation)` | Takes a free slot and returns its new handle, or a zeroed handle when the pool is full |
| `crustGenerationalHandlePoolRemove(pool, handle)` | Frees the slot. The handle must be valid (asserted) |
| `crustGenerationalHandlePoolCheck(pool, handle)` | `1` if the handle refers to a live slot with the same generation |

### Rules

- **`Add` fails on a full pool.** It returns a zeroed handle, which never passes `Check`; test `handle.generation != 0`. The pool is full when `pool.free_head == CRUST_USIZE_MAX`, so you can grow it before adding:

  ```c
  if (pool.free_head == CRUST_USIZE_MAX)
  	crustGenerationalHandlePoolGrow(allocator, &pool, pool.capacity * 2);

  Crust_GenerationalHandle handle = crustGenerationalHandlePoolAdd(&pool, CRUST_USIZE_MAX);
  if (handle.generation == 0)
  	return;
  ```

- **Generations wrap.** A slot's generation goes 1, 2, …, `max_generation − 1`, then back to 1. A stale handle is detected as long as its slot hasn't been reused `max_generation − 1` times since. Pass a large value, such as `CRUST_USIZE_MAX`, or the largest value your packed handle format can hold. With `max_generation` of 1 or 2 every generation is 1 and stale handles are never detected.
- **Generation 0 is never valid.** A zeroed `Crust_GenerationalHandle` works as a null handle for `Check`, provided the pool has at least one slot.
- **`Check` asserts that the index is in range.** Handles from another, larger pool are a contract violation, not just a failed check.
- **Freed slots are reused first**, most recently freed first, which keeps the used part of your arrays compact.

### Iterating live handles

Live handles form a list in the order they were added:

```c
Crust_GenerationalHandle it = crustGenerationalHandlePoolHead(&pool);

while (it.index != CRUST_USIZE_MAX)
{
	Crust_GenerationalHandle next = crustGenerationalHandlePoolNext(&pool, it);

	if (shouldDestroy(&textures[it.index]))
		crustGenerationalHandlePoolRemove(&pool, it);

	it = next;
}
```

`Tail` and `Prev` walk the list backwards. `Next` and `Prev` assert that their argument is still valid, so fetch the next handle before removing the current one, as above.
