# Memory and allocators

Crust never allocates behind your back. Data structures either work on memory you hand them (`*Attach` functions) or take a `Crust_Allocator` on every call that allocates (`*Alloc`, `*Grow`, `*Free`).

| Header | Contents |
|---|---|
| `<crust/allocator.h>` | `Crust_Allocator`, the `crustAlloc*` macros, the system allocator |
| `<crust/arena.h>` | `Crust_Arena` and the arena allocator |
| `<crust/memory.h>` | `crustMemcpy`, `crustMemset` |

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

Typed macros compute the size and alignment from the type and check the multiplication for overflow (asserted):

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

**Containers assume success.** `crustHashIndexMapAlloc`, `crustGenerationalHandlePoolAlloc`, the `Grow` functions and `crustHeapNodePoolAlloc` don't check for allocation failure. Give them an allocator that can satisfy the request, for example an arena sized for the worst case.

## System allocator

```c
Crust_Allocator allocator = crustSystemAllocatorInit();
```

Aligned allocations from the C library:

| Platform | alloc | realloc | free |
|---|---|---|---|
| MSVC, MinGW | `_aligned_malloc` | `_aligned_realloc` | `_aligned_free` |
| POSIX, Emscripten, WASI | `posix_memalign`, with the alignment raised to at least `sizeof(void *)` | allocate, copy, free | `free` |

It is thread-safe and has no state: every call to `crustSystemAllocatorInit` returns the same allocator. Memory from it must go back through it: it is not interchangeable with `malloc`/`free` on Windows.

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

| Function | Meaning |
|---|---|
| `crustMemcpy(dst, src, size)` | `memcpy` |
| `crustMemset(dst, value, size)` | `memset` |

Both are exported functions in `src/libc`. The rest of Crust uses them instead of calling the C library directly, see [running without the C standard library](building.md#running-without-the-c-standard-library).

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

Use it like any other allocator:

```c
CountingAllocator counter = { crustSystemAllocatorInit(), 0 };
Crust_Allocator allocator = { &counter, &counting_vtbl };

u32 *indices = crustAllocArray(allocator, u32, 256);
// counter.bytes == 1024
crustFreeArray(allocator, indices, u32, 256);
```

What the wrappers already guarantee to an implementation:

- `alloc` gets `size > 0` and a power-of-two `alignment`;
- `realloc` gets a non-null `ptr`, `old_size > 0` and `new_size > 0`, because the wrapper turns the other cases into `alloc` and `free`;
- `free` gets a non-null `ptr`.
