#include "crust_test.h"

enum
{
	UNIT_HEAP_SIZE = 64 * 1024 * 1024,
	UNIT_NUM_NODES = 64,
	FUZZ_NUM_HEAPS = 4,
	FUZZ_HEAP_SIZE = 1 << 16,
	FUZZ_MAX_LIVE = 256,
	FUZZ_NUM_NODES = FUZZ_NUM_HEAPS * (2 * FUZZ_MAX_LIVE + 1),
	FUZZ_STEPS = 300000,
};

typedef struct Live_t
{
	Crust_HeapAllocation allocation;
	u32 begin;
	u32 end;
} Live;

static u32 countFreeNodes(const Crust_HeapNodePool *pool)
{
	u32 count = 0;

	for (u32 i = pool->free_head; i != CRUST_U32_MAX && count <= pool->capacity; i = pool->nodes[i].next_bin)
		count++;

	return count;
}

static u8 checkNodeCount(const Crust_HeapNodePool *pool)
{
	return pool->count + countFreeNodes(pool) == pool->capacity;
}

static int testInterleavedReallocSameBinBiggerSize(void)
{
	Crust_Allocator allocator = crustSystemAllocatorInit();
	Crust_HeapNodePool pool_storage;
	crustHeapNodePoolAlloc(allocator, &pool_storage, UNIT_NUM_NODES);

	Crust_HeapNodePool *pool = &pool_storage;
	Crust_Heap heap = crustHeapInit(pool, UNIT_HEAP_SIZE);
	Crust_HeapAllocation allocations[2];

	allocations[0] = crustHeapAlloc(&heap, 5888);
	allocations[1] = crustHeapAlloc(&heap, 12);
	CRUST_TEST_REQUIRE(allocations[0].index != CRUST_U32_MAX);
	CRUST_TEST_REQUIRE(allocations[1].index != CRUST_U32_MAX);

	crustHeapFree(&heap, allocations[0]);
	allocations[0] = crustHeapAlloc(&heap, 6144);
	CRUST_TEST_REQUIRE(allocations[0].index != CRUST_U32_MAX);

	crustHeapFree(&heap, allocations[0]);
	crustHeapFree(&heap, allocations[1]);

	CRUST_TEST_CHECK(countFreeNodes(pool) == pool->capacity - 1);

	crustHeapNodePoolFree(allocator, pool);
	return 0;
}

static int testAlignedAllocsSameAlignment(void)
{
	Crust_Allocator allocator = crustSystemAllocatorInit();
	Crust_HeapNodePool pool_storage;
	crustHeapNodePoolAlloc(allocator, &pool_storage, UNIT_NUM_NODES);

	Crust_HeapNodePool *pool = &pool_storage;
	Crust_Heap heap = crustHeapInit(pool, UNIT_HEAP_SIZE);
	Crust_HeapAllocation allocations[2];

	allocations[0] = crustHeapAllocAligned(&heap, 9, 16);
	CRUST_TEST_CHECK(allocations[0].offset == 0);

	allocations[1] = crustHeapAllocAligned(&heap, 16, 16);
	CRUST_TEST_CHECK(allocations[1].offset == 16);

	crustHeapFree(&heap, allocations[0]);

	allocations[0] = crustHeapAllocAligned(&heap, 16, 16);
	CRUST_TEST_CHECK(allocations[0].offset == 0);

	crustHeapFree(&heap, allocations[0]);
	crustHeapFree(&heap, allocations[1]);

	CRUST_TEST_CHECK(countFreeNodes(pool) == pool->capacity - 1);

	crustHeapNodePoolFree(allocator, pool);
	return 0;
}

static int testAlignedAllocsDifferentAlignments(void)
{
	Crust_Allocator allocator = crustSystemAllocatorInit();
	Crust_HeapNodePool pool_storage;
	crustHeapNodePoolAlloc(allocator, &pool_storage, UNIT_NUM_NODES);

	Crust_HeapNodePool *pool = &pool_storage;
	Crust_Heap heap = crustHeapInit(pool, UNIT_HEAP_SIZE);
	Crust_HeapAllocation allocations[3];

	allocations[0] = crustHeapAllocAligned(&heap, 9, 16);
	CRUST_TEST_CHECK(allocations[0].offset == 0);

	allocations[1] = crustHeapAllocAligned(&heap, 16, 16);
	CRUST_TEST_CHECK(allocations[1].offset == 16);

	allocations[2] = crustHeapAllocAligned(&heap, 32, 32);
	CRUST_TEST_CHECK(allocations[2].offset == 32);

	crustHeapFree(&heap, allocations[0]);
	crustHeapFree(&heap, allocations[1]);

	allocations[1] = crustHeapAllocAligned(&heap, 32, 32);
	CRUST_TEST_CHECK(allocations[1].offset == 0);

	crustHeapFree(&heap, allocations[1]);
	crustHeapFree(&heap, allocations[2]);

	CRUST_TEST_CHECK(countFreeNodes(pool) == pool->capacity - 1);

	crustHeapNodePoolFree(allocator, pool);
	return 0;
}

static int testNonFitAlignedAlloc(void)
{
	Crust_Allocator allocator = crustSystemAllocatorInit();
	Crust_HeapNodePool pool_storage;
	crustHeapNodePoolAlloc(allocator, &pool_storage, UNIT_NUM_NODES);

	Crust_HeapNodePool *pool = &pool_storage;
	Crust_Heap heap = crustHeapInit(pool, UNIT_HEAP_SIZE);
	Crust_HeapAllocation allocations[3];

	allocations[0] = crustHeapAlloc(&heap, 3);
	CRUST_TEST_CHECK(allocations[0].offset == 0);

	allocations[1] = crustHeapAlloc(&heap, 16);
	CRUST_TEST_CHECK(allocations[1].offset == 3);

	allocations[2] = crustHeapAlloc(&heap, 16);
	CRUST_TEST_CHECK(allocations[2].offset == 19);

	crustHeapFree(&heap, allocations[1]);

	allocations[1] = crustHeapAllocAligned(&heap, 16, 16);
	CRUST_TEST_CHECK(allocations[1].offset == 48);

	crustHeapFree(&heap, allocations[0]);
	crustHeapFree(&heap, allocations[1]);
	crustHeapFree(&heap, allocations[2]);

	CRUST_TEST_CHECK(countFreeNodes(pool) == pool->capacity - 1);

	crustHeapNodePoolFree(allocator, pool);
	return 0;
}

static int testStageCommit(void)
{
	Crust_Allocator allocator = crustSystemAllocatorInit();
	Crust_HeapNodePool pool_storage;
	crustHeapNodePoolAlloc(allocator, &pool_storage, UNIT_NUM_NODES);

	Crust_HeapNodePool *pool = &pool_storage;
	Crust_Heap heap = crustHeapInit(pool, UNIT_HEAP_SIZE);

	Crust_HeapAllocation first = crustHeapAlloc(&heap, 100);

	Crust_HeapAllocation staged = crustHeapStageAlloc(&heap, 200);
	CRUST_TEST_REQUIRE(staged.index != CRUST_U32_MAX);
	CRUST_TEST_CHECK(staged.offset == 100);

	Crust_HeapAllocation restaged = crustHeapStageAlloc(&heap, 200);
	CRUST_TEST_CHECK(restaged.index == staged.index);
	CRUST_TEST_CHECK(restaged.offset == staged.offset);

	crustHeapCommitAlloc(&heap, staged, 200);

	Crust_HeapAllocation staged_aligned = crustHeapStageAllocAligned(&heap, 64, 256);
	CRUST_TEST_REQUIRE(staged_aligned.index != CRUST_U32_MAX);
	CRUST_TEST_CHECK(staged_aligned.offset == 512);

	crustHeapCommitAlloc(&heap, staged_aligned, 64);

	Crust_HeapAllocation next = crustHeapAlloc(&heap, 12);
	CRUST_TEST_CHECK(next.offset == 300);

	crustHeapFree(&heap, first);
	crustHeapFree(&heap, staged);
	crustHeapFree(&heap, staged_aligned);
	crustHeapFree(&heap, next);

	CRUST_TEST_CHECK(countFreeNodes(pool) == pool->capacity - 1);

	crustHeapNodePoolFree(allocator, pool);
	return 0;
}

static int testSharedNodePool(void)
{
	Crust_Allocator allocator = crustSystemAllocatorInit();

	Crust_HeapNodePool pool;
	crustHeapNodePoolAlloc(allocator, &pool, UNIT_NUM_NODES);

	Crust_Heap heaps[2];
	heaps[0] = crustHeapInit(&pool, UNIT_HEAP_SIZE);
	heaps[1] = crustHeapInit(&pool, UNIT_HEAP_SIZE);

	Crust_HeapAllocation allocations[4];
	allocations[0] = crustHeapAlloc(&heaps[0], 100);
	allocations[1] = crustHeapAlloc(&heaps[1], 200);
	allocations[2] = crustHeapAlloc(&heaps[0], 300);
	allocations[3] = crustHeapAlloc(&heaps[1], 400);

	CRUST_TEST_CHECK(allocations[0].offset == 0);
	CRUST_TEST_CHECK(allocations[1].offset == 0);
	CRUST_TEST_CHECK(allocations[2].offset == 100);
	CRUST_TEST_CHECK(allocations[3].offset == 200);

	for (u32 i = 0; i < 4; ++i)
		for (u32 j = i + 1; j < 4; ++j)
			CRUST_TEST_CHECK(allocations[i].index != allocations[j].index);

	CRUST_TEST_CHECK(countFreeNodes(&pool) == pool.capacity - 6);
	CRUST_TEST_CHECK(pool.count == 6);

	crustHeapFree(&heaps[0], allocations[0]);
	crustHeapFree(&heaps[0], allocations[2]);
	CRUST_TEST_CHECK(countFreeNodes(&pool) == pool.capacity - 4);
	CRUST_TEST_CHECK(pool.count == 4);

	Crust_HeapAllocation allocation = crustHeapAlloc(&heaps[1], 500);
	CRUST_TEST_CHECK(allocation.offset == 600);
	CRUST_TEST_CHECK(countFreeNodes(&pool) == pool.capacity - 5);
	CRUST_TEST_CHECK(pool.count == 5);

	crustHeapFree(&heaps[1], allocations[1]);
	crustHeapFree(&heaps[1], allocations[3]);
	crustHeapFree(&heaps[1], allocation);
	CRUST_TEST_CHECK(countFreeNodes(&pool) == pool.capacity - 2);
	CRUST_TEST_CHECK(pool.count == 2);

	for (u32 i = 0; i < 2; ++i)
	{
		Crust_HeapAllocation whole = crustHeapAlloc(&heaps[i], UNIT_HEAP_SIZE);
		CRUST_TEST_CHECK(whole.index != CRUST_U32_MAX);
		CRUST_TEST_CHECK(whole.offset == 0);
	}

	crustHeapNodePoolFree(allocator, &pool);
	return 0;
}

static int testAlignedAllocPaddingExceedsHeap(void)
{
	Crust_Allocator allocator = crustSystemAllocatorInit();

	Crust_HeapNodePool pool;
	CRUST_TEST_REQUIRE(crustHeapNodePoolAlloc(allocator, &pool, UNIT_NUM_NODES));

	Crust_Heap heap = crustHeapInit(&pool, 1024);

	Crust_HeapAllocation first = crustHeapAlloc(&heap, 8);
	CRUST_TEST_REQUIRE(first.index != CRUST_U32_MAX);

	Crust_HeapAllocation aligned = crustHeapAllocAligned(&heap, 1000, 256);
	CRUST_TEST_CHECK(aligned.index == CRUST_U32_MAX);
	CRUST_TEST_CHECK(aligned.offset == CRUST_U32_MAX);

	aligned = crustHeapAllocAligned(&heap, 512, 256);
	CRUST_TEST_REQUIRE(aligned.index != CRUST_U32_MAX);
	CRUST_TEST_CHECK(aligned.offset == 256);

	crustHeapFree(&heap, first);
	crustHeapFree(&heap, aligned);
	CRUST_TEST_CHECK(pool.count == 1);

	crustHeapNodePoolFree(allocator, &pool);
	return 0;
}

static int testSizeLimits(void)
{
	Crust_Allocator allocator = crustSystemAllocatorInit();

	Crust_HeapNodePool pool;
	CRUST_TEST_REQUIRE(crustHeapNodePoolAlloc(allocator, &pool, UNIT_NUM_NODES));

	Crust_Heap heap = crustHeapInit(&pool, CRUST_U32_MAX);

	Crust_HeapAllocation largest = crustHeapAlloc(&heap, 0xF0000000u);
	CRUST_TEST_REQUIRE(largest.index != CRUST_U32_MAX);
	CRUST_TEST_CHECK(largest.offset == 0);
	crustHeapFree(&heap, largest);

	CRUST_TEST_CHECK(crustHeapStageAlloc(&heap, 0xF0000001u).index == CRUST_U32_MAX);
	CRUST_TEST_CHECK(crustHeapStageAlloc(&heap, CRUST_U32_MAX).index == CRUST_U32_MAX);
	CRUST_TEST_CHECK(crustHeapStageAllocAligned(&heap, 0xF0000001u, 16).index == CRUST_U32_MAX);
	CRUST_TEST_CHECK(crustHeapStageAllocAligned(&heap, CRUST_U32_MAX, 1).index == CRUST_U32_MAX);

	Crust_HeapAllocation first = crustHeapAlloc(&heap, 1);
	CRUST_TEST_REQUIRE(first.index != CRUST_U32_MAX);

	Crust_HeapAllocation aligned = crustHeapStageAllocAligned(&heap, 0xF0000000u, 0x10000000u);
	CRUST_TEST_CHECK(aligned.index == CRUST_U32_MAX);
	CRUST_TEST_CHECK(aligned.offset == CRUST_U32_MAX);

	aligned = crustHeapAllocAligned(&heap, 0xE0000000u, 0x10000000u);
	CRUST_TEST_REQUIRE(aligned.index != CRUST_U32_MAX);
	CRUST_TEST_CHECK(aligned.offset == 0x10000000u);

	crustHeapFree(&heap, first);
	crustHeapFree(&heap, aligned);
	CRUST_TEST_CHECK(pool.count == 1);

	crustHeapNodePoolFree(allocator, &pool);
	return 0;
}

static int testNodePoolExhaustion(void)
{
	Crust_Allocator allocator = crustSystemAllocatorInit();

	Crust_HeapNodePool pool;
	CRUST_TEST_REQUIRE(crustHeapNodePoolAlloc(allocator, &pool, 3));

	Crust_Heap heap = crustHeapInit(&pool, 1050);
	CRUST_TEST_CHECK(pool.count == 1);

	Crust_HeapAllocation a = crustHeapAlloc(&heap, 10);
	CRUST_TEST_REQUIRE(a.index != CRUST_U32_MAX);
	CRUST_TEST_CHECK(pool.count == 2);

	Crust_HeapAllocation b = crustHeapStageAllocAligned(&heap, 16, 256);
	CRUST_TEST_CHECK(b.index == CRUST_U32_MAX);
	CRUST_TEST_CHECK(b.offset == CRUST_U32_MAX);

	b = crustHeapAllocAligned(&heap, 16, 256);
	CRUST_TEST_CHECK(b.index == CRUST_U32_MAX);
	CRUST_TEST_CHECK(pool.count == 2);

	b = crustHeapAllocAligned(&heap, 16, 2);
	CRUST_TEST_REQUIRE(b.index != CRUST_U32_MAX);
	CRUST_TEST_CHECK(b.offset == 10);
	CRUST_TEST_CHECK(pool.count == 3);

	Crust_HeapAllocation c = crustHeapStageAlloc(&heap, 5);
	CRUST_TEST_CHECK(c.index == CRUST_U32_MAX);

	c = crustHeapAlloc(&heap, 5);
	CRUST_TEST_CHECK(c.index == CRUST_U32_MAX);
	CRUST_TEST_CHECK(pool.count == 3);

	c = crustHeapAlloc(&heap, 1024);
	CRUST_TEST_REQUIRE(c.index != CRUST_U32_MAX);
	CRUST_TEST_CHECK(c.offset == 26);
	CRUST_TEST_CHECK(pool.count == 3);
	CRUST_TEST_CHECK(checkNodeCount(&pool));

	crustHeapFree(&heap, b);
	crustHeapFree(&heap, a);
	crustHeapFree(&heap, c);
	CRUST_TEST_CHECK(pool.count == 1);
	CRUST_TEST_CHECK(checkNodeCount(&pool));

	Crust_HeapAllocation whole = crustHeapAlloc(&heap, 1024);
	CRUST_TEST_CHECK(whole.offset == 0);

	crustHeapNodePoolFree(allocator, &pool);
	return 0;
}

static int testNodePoolAllocationFailure(void)
{
	CrustTest_Allocator test_allocator;
	Crust_Allocator allocator = crustTestAllocatorInit(&test_allocator, 0);

	Crust_HeapNodePool pool;
	CRUST_TEST_CHECK(crustHeapNodePoolAlloc(allocator, &pool, 2) == 0);
	CRUST_TEST_CHECK(pool.nodes == CRUST_NULL);
	CRUST_TEST_CHECK(pool.count == 0);
	CRUST_TEST_CHECK(pool.capacity == 0);

	crustHeapNodePoolFree(allocator, &pool);
	CRUST_TEST_CHECK(test_allocator.blocks == 0);

	test_allocator.budget = 1;
	CRUST_TEST_REQUIRE(crustHeapNodePoolAlloc(allocator, &pool, 2));

	Crust_Heap heap = crustHeapInit(&pool, 1024);

	Crust_HeapAllocation a = crustHeapAlloc(&heap, 100);
	CRUST_TEST_REQUIRE(a.index != CRUST_U32_MAX);
	CRUST_TEST_CHECK(crustHeapAlloc(&heap, 100).index == CRUST_U32_MAX);

	Crust_HeapNode *nodes = pool.nodes;
	CRUST_TEST_CHECK(crustHeapNodePoolGrow(allocator, &pool, 8) == 0);
	CRUST_TEST_CHECK(pool.nodes == nodes);
	CRUST_TEST_CHECK(pool.count == 2);
	CRUST_TEST_CHECK(pool.capacity == 2);

	test_allocator.budget = CRUST_USIZE_MAX;
	CRUST_TEST_REQUIRE(crustHeapNodePoolGrow(allocator, &pool, 8));
	CRUST_TEST_CHECK(pool.count == 2);
	CRUST_TEST_CHECK(pool.capacity == 8);
	CRUST_TEST_CHECK(checkNodeCount(&pool));

	Crust_HeapAllocation b = crustHeapAlloc(&heap, 100);
	CRUST_TEST_REQUIRE(b.index != CRUST_U32_MAX);
	CRUST_TEST_CHECK(b.offset == 100);
	CRUST_TEST_CHECK(pool.count == 3);

	crustHeapFree(&heap, a);
	crustHeapFree(&heap, b);
	CRUST_TEST_CHECK(pool.count == 1);
	CRUST_TEST_CHECK(checkNodeCount(&pool));

	Crust_HeapAllocation whole = crustHeapAlloc(&heap, 1024);
	CRUST_TEST_CHECK(whole.offset == 0);

	crustHeapNodePoolFree(allocator, &pool);
	CRUST_TEST_CHECK(test_allocator.blocks == 0);
	CRUST_TEST_CHECK(test_allocator.bytes == 0);

	return 0;
}

static int fuzz(u32 initial_nodes, u8 grow)
{
	Crust_Allocator allocator = crustSystemAllocatorInit();

	Crust_HeapNodePool pool;
	CRUST_TEST_REQUIRE(crustHeapNodePoolAlloc(allocator, &pool, initial_nodes));

	Crust_Heap heaps[FUZZ_NUM_HEAPS];
	for (u32 h = 0; h < FUZZ_NUM_HEAPS; ++h)
		heaps[h] = crustHeapInit(&pool, FUZZ_HEAP_SIZE);

	static Live live[FUZZ_NUM_HEAPS][FUZZ_MAX_LIVE];
	static u8 node_used[FUZZ_NUM_NODES];
	u32 num_live[FUZZ_NUM_HEAPS] = { 0 };

	for (u32 i = 0; i < FUZZ_NUM_NODES; ++i)
		node_used[i] = 0;

	for (u32 step = 0; step < FUZZ_STEPS; ++step)
	{
		u32 h = crustTestRandom() % FUZZ_NUM_HEAPS;

		if (step % 1024 == 0)
			CRUST_TEST_REQUIRE(checkNodeCount(&pool));

		if ((crustTestRandom() & 1) && num_live[h] < FUZZ_MAX_LIVE)
		{
			if (grow && pool.capacity - pool.count < 2)
				CRUST_TEST_REQUIRE(crustHeapNodePoolGrow(allocator, &pool, crustMinU32(pool.capacity * 2, FUZZ_NUM_NODES)));

			u32 size = 1 + crustTestRandom() % 1500;
			u32 alignment = 1u << (crustTestRandom() % 8);
			u8 aligned = (crustTestRandom() & 1) != 0;

			Crust_HeapAllocation allocation;
			if (aligned)
				allocation = crustHeapAllocAligned(&heaps[h], size, alignment);
			else
				allocation = crustHeapAlloc(&heaps[h], size);

			if (allocation.index == CRUST_U32_MAX)
				continue;

			CRUST_TEST_REQUIRE(allocation.index < pool.capacity);
			CRUST_TEST_REQUIRE(node_used[allocation.index] == 0);
			node_used[allocation.index] = 1;

			const Crust_HeapNode *node = &pool.nodes[allocation.index];
			CRUST_TEST_REQUIRE(node->offset == allocation.offset);
			CRUST_TEST_REQUIRE(node->size == size);
			CRUST_TEST_REQUIRE(!aligned || allocation.offset % alignment == 0);
			CRUST_TEST_REQUIRE(allocation.offset + size <= FUZZ_HEAP_SIZE);

			u32 begin = allocation.offset;
			u32 end = allocation.offset + size;

			for (u32 i = 0; i < num_live[h]; ++i)
				CRUST_TEST_REQUIRE(end <= live[h][i].begin || live[h][i].end <= begin);

			live[h][num_live[h]].allocation = allocation;
			live[h][num_live[h]].begin = begin;
			live[h][num_live[h]].end = end;
			num_live[h]++;
		}
		else if (num_live[h] > 0)
		{
			u32 i = crustTestRandom() % num_live[h];
			node_used[live[h][i].allocation.index] = 0;

			crustHeapFree(&heaps[h], live[h][i].allocation);
			live[h][i] = live[h][--num_live[h]];
		}
	}

	for (u32 h = 0; h < FUZZ_NUM_HEAPS; ++h)
		while (num_live[h] > 0)
			crustHeapFree(&heaps[h], live[h][--num_live[h]].allocation);

	CRUST_TEST_CHECK(countFreeNodes(&pool) == pool.capacity - FUZZ_NUM_HEAPS);
	CRUST_TEST_CHECK(pool.count == FUZZ_NUM_HEAPS);

	for (u32 h = 0; h < FUZZ_NUM_HEAPS; ++h)
	{
		Crust_HeapAllocation whole = crustHeapAlloc(&heaps[h], FUZZ_HEAP_SIZE);
		CRUST_TEST_CHECK(whole.index != CRUST_U32_MAX);
		CRUST_TEST_CHECK(whole.offset == 0);
	}

	crustHeapNodePoolFree(allocator, &pool);
	return 0;
}

static int testFuzz(void)
{
	return fuzz(FUZZ_NUM_NODES, 0);
}

static int testFuzzSmallNodePool(void)
{
	return fuzz(FUZZ_NUM_HEAPS + 12, 0);
}

static int testFuzzGrowingNodePool(void)
{
	return fuzz(FUZZ_NUM_HEAPS + 1, 1);
}

int main(void)
{
	CRUST_TEST_RUN(testInterleavedReallocSameBinBiggerSize);
	CRUST_TEST_RUN(testAlignedAllocsSameAlignment);
	CRUST_TEST_RUN(testAlignedAllocsDifferentAlignments);
	CRUST_TEST_RUN(testNonFitAlignedAlloc);
	CRUST_TEST_RUN(testStageCommit);
	CRUST_TEST_RUN(testSharedNodePool);
	CRUST_TEST_RUN(testAlignedAllocPaddingExceedsHeap);
	CRUST_TEST_RUN(testSizeLimits);
	CRUST_TEST_RUN(testNodePoolExhaustion);
	CRUST_TEST_RUN(testNodePoolAllocationFailure);
	CRUST_TEST_RUN(testFuzz);
	CRUST_TEST_RUN(testFuzzSmallNodePool);
	CRUST_TEST_RUN(testFuzzGrowingNodePool);

	return crustTestResult();
}
