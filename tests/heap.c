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

	crustHeapFree(&heaps[0], allocations[0]);
	crustHeapFree(&heaps[0], allocations[2]);
	CRUST_TEST_CHECK(countFreeNodes(&pool) == pool.capacity - 4);

	Crust_HeapAllocation allocation = crustHeapAlloc(&heaps[1], 500);
	CRUST_TEST_CHECK(allocation.offset == 600);
	CRUST_TEST_CHECK(countFreeNodes(&pool) == pool.capacity - 5);

	crustHeapFree(&heaps[1], allocations[1]);
	crustHeapFree(&heaps[1], allocations[3]);
	crustHeapFree(&heaps[1], allocation);
	CRUST_TEST_CHECK(countFreeNodes(&pool) == pool.capacity - 2);

	for (u32 i = 0; i < 2; ++i)
	{
		Crust_HeapAllocation whole = crustHeapAlloc(&heaps[i], UNIT_HEAP_SIZE);
		CRUST_TEST_CHECK(whole.index != CRUST_U32_MAX);
		CRUST_TEST_CHECK(whole.offset == 0);
	}

	crustHeapNodePoolFree(allocator, &pool);
	return 0;
}

static int testFuzz(void)
{
	Crust_Allocator allocator = crustSystemAllocatorInit();

	Crust_HeapNodePool pool;
	crustHeapNodePoolAlloc(allocator, &pool, FUZZ_NUM_NODES);

	Crust_Heap heaps[FUZZ_NUM_HEAPS];
	for (u32 h = 0; h < FUZZ_NUM_HEAPS; ++h)
		heaps[h] = crustHeapInit(&pool, FUZZ_HEAP_SIZE);

	static Live live[FUZZ_NUM_HEAPS][FUZZ_MAX_LIVE];
	static u8 node_used[FUZZ_NUM_NODES];
	u32 num_live[FUZZ_NUM_HEAPS] = { 0 };

	for (u32 step = 0; step < FUZZ_STEPS; ++step)
	{
		u32 h = crustTestRandom() % FUZZ_NUM_HEAPS;

		if ((crustTestRandom() & 1) && num_live[h] < FUZZ_MAX_LIVE)
		{
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

			CRUST_TEST_REQUIRE(allocation.index < FUZZ_NUM_NODES);
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

	for (u32 h = 0; h < FUZZ_NUM_HEAPS; ++h)
	{
		Crust_HeapAllocation whole = crustHeapAlloc(&heaps[h], FUZZ_HEAP_SIZE);
		CRUST_TEST_CHECK(whole.index != CRUST_U32_MAX);
		CRUST_TEST_CHECK(whole.offset == 0);
	}

	crustHeapNodePoolFree(allocator, &pool);
	return 0;
}

int main(void)
{
	CRUST_TEST_RUN(testInterleavedReallocSameBinBiggerSize);
	CRUST_TEST_RUN(testAlignedAllocsSameAlignment);
	CRUST_TEST_RUN(testAlignedAllocsDifferentAlignments);
	CRUST_TEST_RUN(testNonFitAlignedAlloc);
	CRUST_TEST_RUN(testStageCommit);
	CRUST_TEST_RUN(testSharedNodePool);
	CRUST_TEST_RUN(testFuzz);

	return crustTestResult();
}
