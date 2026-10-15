# Offset heap

Header: `<crust/heap.h>`.

`Crust_Heap` is a TLSF-style (two-level segregated fit) allocator for an abstract range `[0, size)`. It hands out **offsets** and never touches the memory it manages, which may not even be addressable by the CPU.

Typical uses:

- sub-allocating GPU memory: one heap per `VkDeviceMemory`, `ID3D12Heap` or `MTLHeap` block;
- packing ranges inside one large buffer: vertex and index data, uniform ring regions;
- managing space in a file, an atlas or any other linear resource.

Allocation and freeing take constant time, adjacent free blocks are merged immediately, and the bookkeeping lives outside the managed memory.

## Node pools

Blocks are tracked by `Crust_HeapNode` records that come from a `Crust_HeapNodePool`. One pool can serve many heaps, so a single number bounds the bookkeeping for all of them:

```c
Crust_Allocator allocator = crustSystemAllocatorInit();

Crust_HeapNodePool pool;
crustHeapNodePoolAlloc(allocator, &pool, 4096);

Crust_Heap heap_a = crustHeapInit(&pool, 256 * 1024 * 1024);
Crust_Heap heap_b = crustHeapInit(&pool, 64 * 1024 * 1024);

// ...

crustHeapNodePoolFree(allocator, &pool);
```

Every block, used or free, takes one node: `pool.count` is the number of nodes in use, `pool.capacity` the total. Free blocks are always merged with free neighbors, so a heap with `n` live allocations has at most `2n + 1` blocks. A pool sized for

```
capacity ≥ 2 × (live allocations across all heaps) + (number of heaps)
```

never runs out.

**Running out of nodes fails the allocation** like a full heap does. An allocation needs at most two free nodes, so growing the pool before that guarantees that a failure means the heap is full:

```c
if (pool.capacity - pool.count < 2)
	crustHeapNodePoolGrow(allocator, &pool, crustMaxU32(pool.capacity * 2, 64));
```

| Function | Behavior |
|---|---|
| `crustHeapNodePoolInit()` | Returns an empty pool without allocating, the same state that `Free` leaves. `Grow` on it works like `Alloc`. A zeroed struct is not an empty pool |
| `crustHeapNodePoolFootprint(capacity)` | Upper bound of the arena bytes that `Alloc` takes for `capacity`, wherever the arena's free space starts |
| `crustHeapNodePoolAlloc(allocator, pool, capacity)` | Allocates `capacity` nodes. Returns `0` if the allocator fails, leaving the pool empty |
| `crustHeapNodePoolGrow(allocator, pool, new_capacity)` | Adds nodes; `pool.nodes` may move. Returns `0` if the allocator fails, leaving the pool untouched |
| `crustHeapNodePoolFree(allocator, pool)` | Frees the nodes of every heap that uses the pool |

- **A heap holds at least one node for as long as it exists**, taken by `crustHeapInit`, which asserts that the pool has one. Discard heaps by freeing the whole pool.
- **Heaps point to the pool**, so keep the `Crust_HeapNodePool` itself at a fixed address.

## Allocating

```c
Crust_HeapAllocation allocation = crustHeapAllocAligned(&heap_a, 64 * 1024, 256);

if (allocation.index != CRUST_U32_MAX)
{
	// the block is [allocation.offset, allocation.offset + 64 KiB)
	crustHeapFree(&heap_a, allocation);
}
```

| Function | Behavior |
|---|---|
| `crustHeapInit(pool, size)` | Creates a heap for `[0, size)`. `size` is a `u32`, so one heap covers up to 4 GiB − 1 |
| `crustHeapAlloc(heap, size)` | Allocates exactly `size` bytes |
| `crustHeapAllocAligned(heap, size, alignment)` | Same, with `offset` a multiple of `alignment` (a power of two) |
| `crustHeapFree(heap, allocation)` | Frees the block and merges it with free neighbors |

A `Crust_HeapAllocation` is `{ offset, index }`, where `index` identifies the node. On failure both are `CRUST_U32_MAX`. Keep the whole struct: `Free` needs the `index`.

Rules:

- **Free into the same heap.** Heaps that share a pool can't tell each other's allocations apart, and freeing into the wrong heap corrupts both. Double frees are asserted.
- **Sizes are exact.** A block occupies exactly the requested size, so the next block may start at an odd offset. Use `AllocAligned` or round sizes up yourself when offsets must stay aligned.
- **Alignment padding is not lost.** When `AllocAligned` has to skip bytes to reach an aligned offset, those bytes become a free block of their own.
- **Not thread-safe.** Lock around a heap, or give each thread its own heaps; the shared pool is modified by every heap that uses it.

## Size classes

Free blocks are sorted into 256 bins, 8 for each power of two: block sizes are rounded to 3 significant bits, like a tiny floating-point number. Sizes below 8 get exact bins. A request is rounded **up** to the next bin boundary, and the search takes any block from the first non-empty bin at or above it, using bit masks to find it in constant time. Any block found that way is large enough.

The price of that guarantee: a free block whose size lies between the request and the next bin boundary is not considered. **An allocation can fail although a free block slightly larger than the request exists.** Bins are 1/8 of a power of two wide, so in the worst case the heap needs a free block up to 12.5% larger than the request. Leave some headroom when sizing heaps.

## Staging

Allocation can be split into finding a block and taking it:

| Function | Behavior |
|---|---|
| `crustHeapStageAlloc(heap, size)` | Finds where an allocation would go without changing the heap |
| `crustHeapStageAllocAligned(heap, size, alignment)` | Same, aligned |
| `crustHeapCommitAlloc(heap, allocation, size)` | Takes the staged block |

`crustHeapAlloc` is exactly `Stage` followed by `Commit`. Staging takes a `const Crust_Heap *`, so it is safe to try several heaps and commit only one of them, for example the one where the block lands closest to the start:

```c
Crust_HeapAllocation a = crustHeapStageAllocAligned(&heap_a, size, 256);
Crust_HeapAllocation b = crustHeapStageAllocAligned(&heap_b, size, 256);

if (a.index != CRUST_U32_MAX && (b.index == CRUST_U32_MAX || a.offset <= b.offset))
	crustHeapCommitAlloc(&heap_a, a, size);
else if (b.index != CRUST_U32_MAX)
	crustHeapCommitAlloc(&heap_b, b, size);
```

Commit with the same size you staged, and don't modify the heap, or another heap that shares its pool, between staging and committing: the staged node may be gone otherwise, or the nodes the commit needs may have been taken. Growing the pool in between is fine.
