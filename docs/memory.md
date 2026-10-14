# Memory and allocators

Crust never allocates behind your back. Data structures either work on memory you hand them (`*Attach` functions) or take a `Crust_Allocator` on every call that allocates (`*Alloc`, `*Grow`, `*Free`).

Headers: `<crust/allocator.h>` for allocators, `<crust/arena.h>` for the arena, `<crust/memory.h>` for `crustMemcpy` and `crustMemset`.

## Crust_Allocator

An allocator is a small value: a context pointer and a table of three functions.

```c
typedef void *(*PFN_crustAllocatorAlloc)(void *context, usize size, usize alignment);
typedef void *(*PFN_crustAllocatorRealloc)(void *context, void *ptr, usize old_size, usize new_size, usize alignment);
typedef void (*PFN_crustAllocatorFree)(void *context, void *ptr, usize size, usize alignment);

typedef struct Crust_AllocatorVtbl_t
{
	PFN_crustAllocatorAlloc alloc;
	PFN_crustAllocatorRealloc realloc;
	PFN_crustAllocatorFree free;
} Crust_AllocatorVtbl;

typedef struct Crust_Allocator_t
{
	void *context;
	const Crust_AllocatorVtbl *vtbl;
} Crust_Allocator;
```

It is passed by value. Copying it doesn't copy the allocator's state, only the pointer to it.

**Every call carries the size and the alignment**, including `free`. The caller has to remember both, and in return an allocator doesn't have to store headers next to each block. Pass exactly the size and alignment that were used to allocate.

### Calling an allocator

Use the wrappers rather than the vtable:

| Function | Behavior |
|---|---|
| `crustAllocatorAlloc(allocator, size, alignment)` | Returns the block or `CRUST_NULL`. `size > 0`, `alignment` a power of two (asserted) |
| `crustAllocatorRealloc(allocator, ptr, old_size, new_size, alignment)` | With `ptr == CRUST_NULL` it allocates; with `new_size == 0` it frees and returns `CRUST_NULL`; otherwise resizes. On failure it returns `CRUST_NULL` and **the old block stays valid** |
| `crustAllocatorFree(allocator, ptr, size, alignment)` | Frees; `CRUST_NULL` is ignored |

Typed macros compute the size and alignment from the type. An array size that overflows fails like any other allocation:

```c
Particle *p = crustAlloc(allocator, Particle);
Particle *ps = crustAllocArray(allocator, Particle, count);

ps = crustReallocArray(allocator, ps, Particle, count, count * 2);

crustFreeArray(allocator, ps, Particle, count * 2);
crustFree(allocator, p, Particle);
```

Because a failed realloc keeps the old block, assigning the result straight back loses it on failure. When failure is possible, keep the old pointer until the call succeeds:

```c
Particle *grown = crustReallocArray(allocator, ps, Particle, count, new_count);
if (grown == CRUST_NULL)
	return 0;

ps = grown;
count = new_count;
```

Container `Alloc` and `Grow` functions return `0` when the allocator fails. A failed `Alloc` leaves the container empty, a failed `Grow` leaves it untouched.

## System allocator

```c
Crust_Allocator allocator = crustSystemAllocatorInit();
```

Aligned allocations from the C library: `_aligned_malloc` on Windows, `posix_memalign` elsewhere. The allocator is thread-safe and has no state. Memory from it must go back through it, not through `free`.

## Arena

`Crust_Arena` hands out memory from a fixed buffer by bumping an offset. Individual allocations are never freed; the whole arena is reset at once.

```c
static u8 memory[64 * 1024];

Crust_Arena arena = crustArenaAttach(memory, sizeof(memory));

f32 *weights = (f32 *)crustArenaAlloc(&arena, 256 * sizeof(f32), CRUST_ALIGNOF(f32));
if (weights == CRUST_NULL)
	return; // out of space

crustArenaReset(&arena);
```

| Function | Behavior |
|---|---|
| `crustArenaAttach(memory, capacity)` | Wraps memory you own. Crust never frees it |
| `crustArenaAlloc(arena, size, alignment)` | Aligns the current position to `alignment` (the real address, not the offset), reserves `size` bytes, or returns `CRUST_NULL` when they don't fit |
| `crustArenaReset(arena)` | Makes all memory available again. Every pointer handed out before becomes invalid |

`arena.size` is the number of bytes used, including alignment padding, and `arena.capacity` the total.

### The arena as an allocator

```c
Crust_Allocator allocator = crustArenaAllocatorInit(&arena);
```

The allocator keeps a pointer to the arena, so the arena must outlive it and must not move. As an allocator, the arena behaves like this:

- **free** does nothing. Memory comes back only with `crustArenaReset`.
- **realloc** of the most recent allocation grows or shrinks it in place when the new size fits. For any other block it allocates a new one and copies, and the old space stays used until the next reset.
- **alloc** returns `CRUST_NULL` when the arena is full.

This makes an arena a good fit for temporary per-frame or per-task data, and for containers whose final size is known: allocate them from an arena and reset everything at once.

The arena is not thread-safe.

## Memory functions

`crustMemcpy` and `crustMemset` are `memcpy` and `memset`. The rest of Crust calls them instead of the C library, see [running without the C standard library](building.md#running-without-the-c-standard-library).

## Writing an allocator

Any struct can back an allocator. This one counts the bytes in use and forwards to another allocator:

```c
typedef struct CountingAllocator_t
{
	Crust_Allocator parent;
	usize bytes;
} CountingAllocator;

static void *countingAlloc(void *context, usize size, usize alignment)
{
	CountingAllocator *self = (CountingAllocator *)context;
	void *ptr = crustAllocatorAlloc(self->parent, size, alignment);

	if (ptr != CRUST_NULL)
		self->bytes += size;

	return ptr;
}

static void *countingRealloc(void *context, void *ptr, usize old_size, usize new_size, usize alignment)
{
	CountingAllocator *self = (CountingAllocator *)context;
	void *new_ptr = crustAllocatorRealloc(self->parent, ptr, old_size, new_size, alignment);

	if (new_ptr != CRUST_NULL)
		self->bytes = self->bytes - old_size + new_size;

	return new_ptr;
}

static void countingFree(void *context, void *ptr, usize size, usize alignment)
{
	CountingAllocator *self = (CountingAllocator *)context;
	crustAllocatorFree(self->parent, ptr, size, alignment);
	self->bytes -= size;
}

static const Crust_AllocatorVtbl counting_vtbl = { countingAlloc, countingRealloc, countingFree };
```

The wrappers already guarantee that `alloc` gets `size > 0` and a power-of-two `alignment`, `realloc` a non-null `ptr` and non-zero sizes, and `free` a non-null `ptr`. In return, `alloc` and `realloc` must return `CRUST_NULL` for any request they can't satisfy, including a size of `CRUST_USIZE_MAX`, which the typed macros pass when the size overflows.
