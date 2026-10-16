#include <crust/heap.h>
#include <crust/scalar.h>

//
static u32 crustHeapNodePoolAcquire(Crust_HeapNodePool *pool)
{
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(pool->nodes != CRUST_NULL);
	CRUST_ASSERT(pool->count < pool->capacity);
	CRUST_ASSERT(pool->capacity > pool->free_head);

	u32 index = pool->free_head;
	pool->free_head = pool->nodes[index].next_bin;
	pool->count++;

	return index;
}

static void crustHeapNodePoolRelease(Crust_HeapNodePool *pool, u32 index)
{
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(pool->nodes != CRUST_NULL);
	CRUST_ASSERT(pool->capacity > index);
	CRUST_ASSERT(pool->count > 0);

	pool->nodes[index].next_bin = pool->free_head;
	pool->free_head = index;
	pool->count--;
}

static void crustHeapNodePoolPushFree(Crust_HeapNodePool *pool, u32 begin, u32 end)
{
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(pool->nodes != CRUST_NULL);
	CRUST_ASSERT(begin < end);

	for (u32 i = begin; i < end - 1; ++i)
		pool->nodes[i].next_bin = i + 1;

	pool->nodes[end - 1].next_bin = pool->free_head;
	pool->free_head = begin;
}

static void crustHeapNodePoolMarkUsed(Crust_HeapNodePool *pool, u32 index)
{
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(pool->nodes != CRUST_NULL);
	CRUST_ASSERT(pool->capacity > index);

	Crust_HeapNode *node = &pool->nodes[index];

	node->prev_bin = index;
	node->next_bin = index;
}

static u8 crustHeapNodePoolIsUsed(const Crust_HeapNodePool *pool, u32 index)
{
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(pool->nodes != CRUST_NULL);
	CRUST_ASSERT(pool->capacity > index);

	return pool->nodes[index].next_bin == index;
}

//
CRUST_APIENTRY Crust_HeapNodePool crustHeapNodePoolInit(void)
{
	Crust_HeapNodePool result;
	result.nodes = CRUST_NULL;
	result.free_head = CRUST_U32_MAX;
	result.count = 0;
	result.capacity = 0;

	return result;
}

CRUST_APIENTRY usize crustHeapNodePoolFootprint(u32 capacity)
{
	CRUST_ASSERT(capacity > 0);

	return crustAllocatorSizeAdd(crustAllocatorSizeMul(sizeof(Crust_HeapNode), capacity), CRUST_ALIGNOF(Crust_HeapNode) - 1);
}

CRUST_APIENTRY u8 crustHeapNodePoolAlloc(Crust_Allocator allocator, Crust_HeapNodePool *pool, u32 capacity)
{
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(capacity > 0);

	*pool = crustHeapNodePoolInit();
	pool->nodes = crustAllocArray(allocator, Crust_HeapNode, capacity);

	if (pool->nodes == CRUST_NULL)
		return 0;

	crustHeapNodePoolPushFree(pool, 0, capacity);
	pool->capacity = capacity;

	return 1;
}

CRUST_APIENTRY void crustHeapNodePoolFree(Crust_Allocator allocator, Crust_HeapNodePool *pool)
{
	CRUST_ASSERT(pool != CRUST_NULL);

	crustFreeArray(allocator, pool->nodes, Crust_HeapNode, pool->capacity);
	*pool = crustHeapNodePoolInit();
}

CRUST_APIENTRY u8 crustHeapNodePoolGrow(Crust_Allocator allocator, Crust_HeapNodePool *pool, u32 new_capacity)
{
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(new_capacity > pool->capacity);

	Crust_HeapNode *nodes = crustReallocArray(allocator, pool->nodes, Crust_HeapNode, pool->capacity, new_capacity);
	if (nodes == CRUST_NULL)
		return 0;

	pool->nodes = nodes;

	crustHeapNodePoolPushFree(pool, pool->capacity, new_capacity);
	pool->capacity = new_capacity;

	return 1;
}

//
static u32 crustHeapToBinSize(u8 bin)
{
	u32 mantissa = bin & CRUST_HEAP_PARAMETERS_MANTISSA_MASK;
	u32 exponent = bin >> CRUST_HEAP_PARAMETERS_MANTISSA_BITS;

	if (exponent == 0)
		return mantissa;

	return (mantissa | CRUST_HEAP_PARAMETERS_MANTISSA_MAX) << (exponent - 1);
}

static u8 crustHeapToBinIndex(u32 size)
{
	if (size < CRUST_HEAP_PARAMETERS_MANTISSA_MAX)
		return (u8)size;

	u32 leading_zeroes = crustLzcntU32(size);
	u32 highest_bit = 31 - leading_zeroes;

	u32 mantissa_bit = highest_bit - CRUST_HEAP_PARAMETERS_MANTISSA_BITS;

	u32 exponent = mantissa_bit + 1;
	u32 mantissa = (size >> mantissa_bit) & CRUST_HEAP_PARAMETERS_MANTISSA_MASK;

	return (u8)((exponent << CRUST_HEAP_PARAMETERS_MANTISSA_BITS) + mantissa);
}

static u8 crustHeapToBinIndexRoundUp(u32 size)
{
	if (size < CRUST_HEAP_PARAMETERS_MANTISSA_MAX)
		return (u8)size;

	u32 leading_zeroes = crustLzcntU32(size);
	u32 highest_bit = 31 - leading_zeroes;

	u32 mantissa_bit = highest_bit - CRUST_HEAP_PARAMETERS_MANTISSA_BITS;

	u32 exponent = mantissa_bit + 1;
	u32 mantissa = (size >> mantissa_bit) & CRUST_HEAP_PARAMETERS_MANTISSA_MASK;

	u32 lower_bits_mask = (1u << mantissa_bit) - 1;

	if ((size & lower_bits_mask) != 0)
		mantissa++;

	return (u8)((exponent << CRUST_HEAP_PARAMETERS_MANTISSA_BITS) + mantissa);
}

static u32 crustHeapCountRemainders(const Crust_HeapNode *node, u32 offset, u32 size)
{
	CRUST_ASSERT(node != CRUST_NULL);
	CRUST_ASSERT(offset >= node->offset);
	CRUST_ASSERT(node->size >= size);
	CRUST_ASSERT(offset - node->offset <= node->size - size);

	u32 count = 0;

	if (offset > node->offset)
		count++;

	if (offset - node->offset < node->size - size)
		count++;

	return count;
}

static u8 crustHeapFindBin(u32 bin_mask, u8 index)
{
	u32 mask = (1u << index) - 1;

	u32 masked_bins = bin_mask & ~mask;
	if (masked_bins == 0)
		return CRUST_U8_MAX;

	return (u8)crustTzcntU32(masked_bins);
}

static u8 crustHeapFindBinForSize(const Crust_Heap *heap, u32 size)
{
	CRUST_ASSERT(heap != CRUST_NULL);
	CRUST_ASSERT(heap->size >= size);

	Crust_HeapNodePool *pool = heap->pool;
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(pool->nodes != CRUST_NULL);
	CRUST_UNUSED(pool);

	u8 bin_index = crustHeapToBinIndexRoundUp(size);
	if (crustHeapToBinSize(bin_index) < size)
		return CRUST_U8_MAX;

	u8 sparse_bin_index = bin_index >> CRUST_HEAP_PARAMETERS_MANTISSA_BITS;
	u8 linear_bin_index = CRUST_U8_MAX;

	u8 used_linear_bins = heap->used_linear_bins[sparse_bin_index];

	if (used_linear_bins != 0)
	{
		u8 min_linear_bin = bin_index & CRUST_HEAP_PARAMETERS_MANTISSA_MASK;
		linear_bin_index = crustHeapFindBin(used_linear_bins, min_linear_bin);
	}

	if (linear_bin_index == CRUST_U8_MAX)
	{
		sparse_bin_index = crustHeapFindBin(heap->used_sparse_bins, sparse_bin_index + 1);
		if (sparse_bin_index == CRUST_U8_MAX)
			return CRUST_U8_MAX;

		CRUST_ASSERT(sparse_bin_index != 0);

		used_linear_bins = heap->used_linear_bins[sparse_bin_index];
		CRUST_ASSERT(used_linear_bins != 0);

		linear_bin_index = (u8)crustTzcntU32(used_linear_bins);
	}

	CRUST_ASSERT(linear_bin_index != CRUST_U8_MAX);
	return (u8)((sparse_bin_index << CRUST_HEAP_PARAMETERS_MANTISSA_BITS) | (linear_bin_index & CRUST_HEAP_PARAMETERS_MANTISSA_MASK));
}

static void crustHeapAddNodeToBin(Crust_Heap *heap, u32 index, u32 size, u32 offset)
{
	CRUST_ASSERT(heap != CRUST_NULL);
	CRUST_ASSERT(heap->size >= size + offset);
	CRUST_ASSERT(index != CRUST_U32_MAX);

	Crust_HeapNodePool *pool = heap->pool;
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(pool->nodes != CRUST_NULL);
	CRUST_ASSERT(index < pool->capacity);

	u8 bin_index = crustHeapToBinIndex(size);
	CRUST_ASSERT(crustHeapToBinSize(bin_index) <= size);

	u8 sparse_bin_index = bin_index >> CRUST_HEAP_PARAMETERS_MANTISSA_BITS;
	u8 linear_bin_index = bin_index & CRUST_HEAP_PARAMETERS_MANTISSA_MASK;

	heap->used_sparse_bins |= 1u << sparse_bin_index;
	heap->used_linear_bins[sparse_bin_index] |= 1u << linear_bin_index;

	Crust_HeapNode *node = &pool->nodes[index];

	node->offset = offset;
	node->size = size;
	node->prev_bin = CRUST_U32_MAX;
	node->next_bin = CRUST_U32_MAX;
	node->prev_neighbour = CRUST_U32_MAX;
	node->next_neighbour = CRUST_U32_MAX;

	u32 bin_head_index = heap->heads[bin_index];

	if (bin_head_index != CRUST_U32_MAX)
	{
		Crust_HeapNode *bin_head_node = &pool->nodes[bin_head_index];
		CRUST_ASSERT(crustHeapNodePoolIsUsed(pool, bin_head_index) == 0);

		bin_head_node->prev_bin = index;
		node->next_bin = bin_head_index;
	}

	heap->heads[bin_index] = index;
}

static void crustHeapRemoveNodeFromBin(Crust_Heap *heap, u32 index)
{
	CRUST_ASSERT(heap != CRUST_NULL);
	CRUST_ASSERT(index != CRUST_U32_MAX);

	Crust_HeapNodePool *pool = heap->pool;
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(pool->nodes != CRUST_NULL);
	CRUST_ASSERT(index < pool->capacity);

	Crust_HeapNode *node = &pool->nodes[index];
	CRUST_ASSERT(node != CRUST_NULL);
	CRUST_ASSERT(crustHeapNodePoolIsUsed(pool, index) == 0);

	u8 bin_index = crustHeapToBinIndex(node->size);
	CRUST_ASSERT(crustHeapToBinSize(bin_index) <= node->size);

	if (node->prev_bin != CRUST_U32_MAX)
	{
		Crust_HeapNode *prev_node = &pool->nodes[node->prev_bin];
		CRUST_ASSERT(prev_node != CRUST_NULL);

		prev_node->next_bin = node->next_bin;
	}

	if (node->next_bin != CRUST_U32_MAX)
	{
		Crust_HeapNode *next_node = &pool->nodes[node->next_bin];
		CRUST_ASSERT(next_node != CRUST_NULL);

		next_node->prev_bin = node->prev_bin;
	}

	u8 sparse_bin_index = bin_index >> CRUST_HEAP_PARAMETERS_MANTISSA_BITS;
	u8 linear_bin_index = bin_index & CRUST_HEAP_PARAMETERS_MANTISSA_MASK;

	u32 bin_head_index = heap->heads[bin_index];
	CRUST_ASSERT(bin_head_index != CRUST_U32_MAX);

	if (bin_head_index == index)
		bin_head_index = node->next_bin;

	if (bin_head_index == CRUST_U32_MAX)
	{
		u8 linear_bin_mask = heap->used_linear_bins[sparse_bin_index];
		u32 sparse_bin_mask = heap->used_sparse_bins;

		linear_bin_mask &= ~(u8)(1u << linear_bin_index);
		if (linear_bin_mask == 0)
			sparse_bin_mask &= ~(1u << sparse_bin_index);

		heap->used_linear_bins[sparse_bin_index] = linear_bin_mask;
		heap->used_sparse_bins = sparse_bin_mask;
	}

	heap->heads[bin_index] = bin_head_index;
}

//
CRUST_APIENTRY Crust_Heap crustHeapInit(Crust_HeapNodePool *pool, u32 size)
{
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(size > 0);

	Crust_Heap result;
	result.pool = pool;
	result.size = size;

	for (u32 i = 0; i < CRUST_HEAP_PARAMETERS_NUM_BINS; ++i)
		result.heads[i] = CRUST_U32_MAX;

	result.used_sparse_bins = 0;
	for (u32 i = 0; i < CRUST_HEAP_PARAMETERS_NUM_SPARSE_BINS; ++i)
		result.used_linear_bins[i] = 0;

	u32 index = crustHeapNodePoolAcquire(pool);
	crustHeapAddNodeToBin(&result, index, size, 0);

	return result;
}

CRUST_APIENTRY Crust_HeapAllocation crustHeapStageAlloc(const Crust_Heap *heap, u32 size)
{
	CRUST_ASSERT(heap != CRUST_NULL);
	CRUST_ASSERT(heap->size >= size);
	CRUST_ASSERT(size > 0);
	
	const Crust_HeapNodePool *pool = heap->pool;
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(pool->nodes != CRUST_NULL);

	Crust_HeapAllocation result;
	result.offset = CRUST_U32_MAX;
	result.index = CRUST_U32_MAX;

	u8 bin_index = crustHeapFindBinForSize(heap, size);
	if (bin_index == CRUST_U8_MAX)
		return result;

	CRUST_ASSERT(bin_index != 0);
	CRUST_ASSERT(crustHeapToBinSize(bin_index) >= size);

	u32 index = heap->heads[bin_index];
	CRUST_ASSERT(index != CRUST_U32_MAX);

	const Crust_HeapNode *node = &pool->nodes[index];
	CRUST_ASSERT(node != CRUST_NULL);
	CRUST_ASSERT(node->size >= size);

	if (pool->capacity - pool->count < crustHeapCountRemainders(node, node->offset, size))
		return result;

	result.offset = node->offset;
	result.index = index;

	return result;
}

CRUST_APIENTRY Crust_HeapAllocation crustHeapStageAllocAligned(const Crust_Heap *heap, u32 size, u32 alignment)
{
	CRUST_ASSERT(heap != CRUST_NULL);
	CRUST_ASSERT(heap->size >= size);
	CRUST_ASSERT(size > 0);
	CRUST_ASSERT(crustIsPow2U32(alignment) != 0);

	const Crust_HeapNodePool *pool = heap->pool;
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(pool->nodes != CRUST_NULL);

	Crust_HeapAllocation result;
	result.offset = CRUST_U32_MAX;
	result.index = CRUST_U32_MAX;

	u8 bin_index = crustHeapFindBinForSize(heap, size);
	if (bin_index == CRUST_U8_MAX)
		return result;

	CRUST_ASSERT(bin_index != 0);
	CRUST_ASSERT(crustHeapToBinSize(bin_index) >= size);

	u32 index = heap->heads[bin_index];
	CRUST_ASSERT(index != CRUST_U32_MAX);

	const Crust_HeapNode *node = &pool->nodes[index];
	CRUST_ASSERT(node != CRUST_NULL);
	CRUST_ASSERT(node->size >= size);

	u32 offset = crustAlignUpU32(node->offset, alignment);

	if (offset - node->offset > node->size - size)
	{
		if (alignment - 1 > heap->size - size)
			return result;

		bin_index = crustHeapFindBinForSize(heap, size + alignment - 1);
		if (bin_index == CRUST_U8_MAX)
			return result;

		CRUST_ASSERT(bin_index != 0);

		index = heap->heads[bin_index];
		CRUST_ASSERT(index != CRUST_U32_MAX);

		node = &pool->nodes[index];
		CRUST_ASSERT(node != CRUST_NULL);

		offset = crustAlignUpU32(node->offset, alignment);
	}

	if (pool->capacity - pool->count < crustHeapCountRemainders(node, offset, size))
		return result;

	result.offset = offset;
	result.index = index;

	return result;
}

CRUST_APIENTRY void crustHeapCommitAlloc(Crust_Heap *heap, Crust_HeapAllocation allocation, u32 size)
{
	CRUST_ASSERT(heap != CRUST_NULL);
	CRUST_ASSERT(heap->size >= size);
	CRUST_ASSERT(size > 0);

	CRUST_ASSERT(allocation.index != CRUST_U32_MAX);
	CRUST_ASSERT(allocation.offset != CRUST_U32_MAX);

	Crust_HeapNodePool *pool = heap->pool;
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(pool->nodes != CRUST_NULL);
	CRUST_ASSERT(pool->capacity > allocation.index);

	Crust_HeapNode *node = &pool->nodes[allocation.index];
	CRUST_ASSERT(node != CRUST_NULL);
	CRUST_ASSERT(crustHeapNodePoolIsUsed(pool, allocation.index) == 0);
	CRUST_ASSERT(allocation.offset >= node->offset);
	CRUST_ASSERT(pool->capacity - pool->count >= crustHeapCountRemainders(node, allocation.offset, size));

	u32 remainder_begin_size = allocation.offset - node->offset;
	u32 remainder_begin_offset = node->offset;
	u32 remainder_end_size = node->size - remainder_begin_size - size;
	u32 remainder_end_offset = allocation.offset + size;

	CRUST_ASSERT(remainder_begin_size + size <= node->size);

	u32 prev_index = node->prev_neighbour;
	u32 next_index = node->next_neighbour;

	CRUST_ASSERT(prev_index == CRUST_U32_MAX || crustHeapNodePoolIsUsed(pool, prev_index) != 0);
	CRUST_ASSERT(next_index == CRUST_U32_MAX || crustHeapNodePoolIsUsed(pool, next_index) != 0);

	crustHeapRemoveNodeFromBin(heap, allocation.index);

	node->offset = allocation.offset;
	node->size = size;

	crustHeapNodePoolMarkUsed(pool, allocation.index);

	if (remainder_begin_size > 0)
	{
		Crust_HeapNode *prev_node = (prev_index != CRUST_U32_MAX) ? &pool->nodes[prev_index] : CRUST_NULL;

		u32 new_index = crustHeapNodePoolAcquire(pool);
		crustHeapAddNodeToBin(heap, new_index, remainder_begin_size, remainder_begin_offset);

		Crust_HeapNode *new_node = &pool->nodes[new_index];
		CRUST_ASSERT(new_node != CRUST_NULL);

		node->prev_neighbour = new_index;
		new_node->next_neighbour = allocation.index;

		new_node->prev_neighbour = prev_index;
		if (prev_node)
			prev_node->next_neighbour = new_index;
	}

	if (remainder_end_size > 0)
	{
		Crust_HeapNode *next_node = (next_index != CRUST_U32_MAX) ? &pool->nodes[next_index] : CRUST_NULL;

		u32 new_index = crustHeapNodePoolAcquire(pool);
		crustHeapAddNodeToBin(heap, new_index, remainder_end_size, remainder_end_offset);

		Crust_HeapNode *new_node = &pool->nodes[new_index];
		CRUST_ASSERT(new_node != CRUST_NULL);

		node->next_neighbour = new_index;
		new_node->prev_neighbour = allocation.index;

		new_node->next_neighbour = next_index;
		if (next_node)
			next_node->prev_neighbour = new_index;
	}
}

CRUST_APIENTRY Crust_HeapAllocation crustHeapAlloc(Crust_Heap *heap, u32 size)
{
	CRUST_ASSERT(heap != CRUST_NULL);
	CRUST_ASSERT(heap->size >= size);
	CRUST_ASSERT(size > 0);

	Crust_HeapAllocation result = crustHeapStageAlloc(heap, size);
	if (result.index == CRUST_U32_MAX && result.offset == CRUST_U32_MAX)
		return result;

	crustHeapCommitAlloc(heap, result, size);
	return result;
}

CRUST_APIENTRY Crust_HeapAllocation crustHeapAllocAligned(Crust_Heap *heap, u32 size, u32 alignment)
{
	CRUST_ASSERT(heap != CRUST_NULL);
	CRUST_ASSERT(heap->size >= size);
	CRUST_ASSERT(size > 0);
	CRUST_ASSERT(crustIsPow2U32(alignment) != 0);

	Crust_HeapAllocation result = crustHeapStageAllocAligned(heap, size, alignment);
	if (result.index == CRUST_U32_MAX && result.offset == CRUST_U32_MAX)
		return result;

	crustHeapCommitAlloc(heap, result, size);
	return result;
}

CRUST_APIENTRY void crustHeapFree(Crust_Heap *heap, Crust_HeapAllocation allocation)
{
	CRUST_ASSERT(heap != CRUST_NULL);

	CRUST_ASSERT(allocation.index != CRUST_U32_MAX);
	CRUST_ASSERT(allocation.offset != CRUST_U32_MAX);

	Crust_HeapNodePool *pool = heap->pool;
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(pool->nodes != CRUST_NULL);
	CRUST_ASSERT(pool->capacity > allocation.index);

	Crust_HeapNode *node = &pool->nodes[allocation.index];
	CRUST_ASSERT(node != CRUST_NULL);
	CRUST_ASSERT(crustHeapNodePoolIsUsed(pool, allocation.index) != 0);
	CRUST_ASSERT(allocation.offset == node->offset);

	u32 prev_index = node->prev_neighbour;
	u32 next_index = node->next_neighbour;

	u32 size = node->size;
	u32 offset = node->offset;

	Crust_HeapNode *prev_node = (prev_index != CRUST_U32_MAX) ? &pool->nodes[prev_index] : CRUST_NULL;

	// try merge with previous free node
	if (prev_node != CRUST_NULL && crustHeapNodePoolIsUsed(pool, prev_index) == 0)
	{
		offset = prev_node->offset;
		size += prev_node->size;

		u32 prev_prev_index = prev_node->prev_neighbour;

		crustHeapRemoveNodeFromBin(heap, prev_index);
		crustHeapNodePoolRelease(pool, prev_index);

		prev_index = prev_prev_index;
	}

	Crust_HeapNode *next_node = (next_index != CRUST_U32_MAX) ? &pool->nodes[next_index] : CRUST_NULL;

	// try merge with next free node
	if (next_node != CRUST_NULL && crustHeapNodePoolIsUsed(pool, next_index) == 0)
	{
		size += next_node->size;

		u32 next_next_index = next_node->next_neighbour;

		crustHeapRemoveNodeFromBin(heap, next_index);
		crustHeapNodePoolRelease(pool, next_index);

		next_index = next_next_index;
	}

	crustHeapAddNodeToBin(heap, allocation.index, size, offset);

	node->prev_neighbour = prev_index;
	node->next_neighbour = next_index;

	if (prev_index != CRUST_U32_MAX)
	{
		Crust_HeapNode *prev_prev_node = &pool->nodes[prev_index];
		prev_prev_node->next_neighbour = allocation.index;
	}

	if (next_index != CRUST_U32_MAX)
	{
		Crust_HeapNode *next_next_node = &pool->nodes[next_index];
		next_next_node->prev_neighbour = allocation.index;
	}
}
