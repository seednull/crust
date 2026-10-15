#include "crust_test.h"

enum
{
	NUM_HANDLES = 16,
	MAX_GENERATION = 1024,
	STORAGE_BLOCKS = 4,
	FUZZ_INITIAL_CAPACITY = 8,
	FUZZ_MAX_CAPACITY = 256,
	FUZZ_STEPS = 100000,
};

static usize countUsed(const Crust_GenerationalHandlePool *pool)
{
	usize count = 0;
	Crust_GenerationalHandle handle = crustGenerationalHandlePoolHead(pool);

	while (handle.index != CRUST_USIZE_MAX && count <= pool->capacity)
	{
		count++;
		handle = crustGenerationalHandlePoolNext(pool, handle);
	}

	return count;
}

static int testSequentialAddCheckRemove(void)
{
	Crust_Allocator allocator = crustSystemAllocatorInit();
	Crust_GenerationalHandlePool pool;
	crustGenerationalHandlePoolAlloc(allocator, &pool, NUM_HANDLES);

	Crust_GenerationalHandle handles[NUM_HANDLES];

	for (u32 i = 0; i < NUM_HANDLES; ++i)
	{
		handles[i] = crustGenerationalHandlePoolAdd(&pool, MAX_GENERATION);
		CRUST_TEST_CHECK(handles[i].index == i);
		CRUST_TEST_CHECK(handles[i].generation != 0);
	}

	for (u32 i = 0; i < NUM_HANDLES; ++i)
		CRUST_TEST_CHECK(crustGenerationalHandlePoolCheck(&pool, handles[i]) != 0);

	CRUST_TEST_CHECK(countUsed(&pool) == NUM_HANDLES);

	for (u32 i = 0; i < NUM_HANDLES; ++i)
	{
		crustGenerationalHandlePoolRemove(&pool, handles[i]);
		CRUST_TEST_CHECK(crustGenerationalHandlePoolCheck(&pool, handles[i]) == 0);
	}

	CRUST_TEST_CHECK(countUsed(&pool) == 0);

	crustGenerationalHandlePoolFree(allocator, &pool);
	return 0;
}

static int testReuseFreeSlots(void)
{
	Crust_Allocator allocator = crustSystemAllocatorInit();
	Crust_GenerationalHandlePool pool;
	crustGenerationalHandlePoolAlloc(allocator, &pool, NUM_HANDLES);

	Crust_GenerationalHandle handles[NUM_HANDLES];

	for (u32 i = 0; i < NUM_HANDLES; ++i)
		handles[i] = crustGenerationalHandlePoolAdd(&pool, MAX_GENERATION);

	crustGenerationalHandlePoolRemove(&pool, handles[4]);
	crustGenerationalHandlePoolRemove(&pool, handles[7]);
	CRUST_TEST_CHECK(countUsed(&pool) == NUM_HANDLES - 2);

	Crust_GenerationalHandle first = crustGenerationalHandlePoolAdd(&pool, MAX_GENERATION);
	Crust_GenerationalHandle second = crustGenerationalHandlePoolAdd(&pool, MAX_GENERATION);

	CRUST_TEST_CHECK((first.index == 4 && second.index == 7) || (first.index == 7 && second.index == 4));
	CRUST_TEST_CHECK(countUsed(&pool) == NUM_HANDLES);
	CRUST_TEST_CHECK(pool.capacity == NUM_HANDLES);
	CRUST_TEST_CHECK(pool.free_head == CRUST_USIZE_MAX);

	crustGenerationalHandlePoolFree(allocator, &pool);
	return 0;
}

static int testOldHandlesAreInvalid(void)
{
	Crust_Allocator allocator = crustSystemAllocatorInit();
	Crust_GenerationalHandlePool pool;
	crustGenerationalHandlePoolAlloc(allocator, &pool, NUM_HANDLES);

	Crust_GenerationalHandle handles[NUM_HANDLES];

	for (u32 i = 0; i < NUM_HANDLES; ++i)
		handles[i] = crustGenerationalHandlePoolAdd(&pool, MAX_GENERATION);

	Crust_GenerationalHandle old_handles[2] = { handles[3], handles[5] };

	crustGenerationalHandlePoolRemove(&pool, handles[3]);
	crustGenerationalHandlePoolRemove(&pool, handles[5]);

	handles[3] = crustGenerationalHandlePoolAdd(&pool, MAX_GENERATION);
	handles[5] = crustGenerationalHandlePoolAdd(&pool, MAX_GENERATION);

	CRUST_TEST_CHECK(crustGenerationalHandlePoolCheck(&pool, old_handles[0]) == 0);
	CRUST_TEST_CHECK(crustGenerationalHandlePoolCheck(&pool, old_handles[1]) == 0);

	CRUST_TEST_CHECK(crustGenerationalHandlePoolCheck(&pool, handles[3]) != 0);
	CRUST_TEST_CHECK(crustGenerationalHandlePoolCheck(&pool, handles[5]) != 0);

	CRUST_TEST_CHECK(handles[3].index == old_handles[1].index);
	CRUST_TEST_CHECK(handles[5].index == old_handles[0].index);

	crustGenerationalHandlePoolFree(allocator, &pool);
	return 0;
}

static int testIterationOrder(void)
{
	Crust_Allocator allocator = crustSystemAllocatorInit();
	Crust_GenerationalHandlePool pool;
	crustGenerationalHandlePoolAlloc(allocator, &pool, NUM_HANDLES);

	Crust_GenerationalHandle handles[NUM_HANDLES];

	for (u32 i = 0; i < NUM_HANDLES; ++i)
		handles[i] = crustGenerationalHandlePoolAdd(&pool, MAX_GENERATION);

	for (u32 i = 0; i < NUM_HANDLES; i += 3)
		crustGenerationalHandlePoolRemove(&pool, handles[i]);

	Crust_GenerationalHandle extra = crustGenerationalHandlePoolAdd(&pool, MAX_GENERATION);

	usize expected[NUM_HANDLES];
	u32 num_expected = 0;

	for (u32 i = 0; i < NUM_HANDLES; ++i)
		if (i % 3 != 0)
			expected[num_expected++] = handles[i].index;

	expected[num_expected++] = extra.index;

	Crust_GenerationalHandle handle = crustGenerationalHandlePoolHead(&pool);
	for (u32 i = 0; i < num_expected; ++i)
	{
		CRUST_TEST_REQUIRE(handle.index == expected[i]);
		handle = crustGenerationalHandlePoolNext(&pool, handle);
	}

	CRUST_TEST_CHECK(handle.index == CRUST_USIZE_MAX);

	handle = crustGenerationalHandlePoolTail(&pool);
	for (u32 i = num_expected; i > 0; --i)
	{
		CRUST_TEST_REQUIRE(handle.index == expected[i - 1]);
		handle = crustGenerationalHandlePoolPrev(&pool, handle);
	}

	CRUST_TEST_CHECK(handle.index == CRUST_USIZE_MAX);

	crustGenerationalHandlePoolFree(allocator, &pool);
	return 0;
}

static int testGenerationWrap(void)
{
	Crust_Allocator allocator = crustSystemAllocatorInit();
	Crust_GenerationalHandlePool pool;
	crustGenerationalHandlePoolAlloc(allocator, &pool, 1);

	for (u32 i = 0; i < 12; ++i)
	{
		Crust_GenerationalHandle handle = crustGenerationalHandlePoolAdd(&pool, 4);

		CRUST_TEST_CHECK(handle.index == 0);
		CRUST_TEST_CHECK(handle.generation == 1 + i % 3);

		crustGenerationalHandlePoolRemove(&pool, handle);
	}

	crustGenerationalHandlePoolFree(allocator, &pool);
	return 0;
}

static int testGrowKeepsHandles(void)
{
	Crust_Allocator allocator = crustSystemAllocatorInit();
	Crust_GenerationalHandlePool pool;
	crustGenerationalHandlePoolAlloc(allocator, &pool, 4);

	Crust_GenerationalHandle handles[NUM_HANDLES];

	for (u32 i = 0; i < 4; ++i)
		handles[i] = crustGenerationalHandlePoolAdd(&pool, MAX_GENERATION);

	crustGenerationalHandlePoolRemove(&pool, handles[1]);
	crustGenerationalHandlePoolGrow(allocator, &pool, NUM_HANDLES);

	CRUST_TEST_CHECK(crustGenerationalHandlePoolCheck(&pool, handles[0]) != 0);
	CRUST_TEST_CHECK(crustGenerationalHandlePoolCheck(&pool, handles[1]) == 0);
	CRUST_TEST_CHECK(crustGenerationalHandlePoolCheck(&pool, handles[2]) != 0);
	CRUST_TEST_CHECK(crustGenerationalHandlePoolCheck(&pool, handles[3]) != 0);

	for (u32 i = 0; i < NUM_HANDLES - 3; ++i)
		crustGenerationalHandlePoolAdd(&pool, MAX_GENERATION);

	CRUST_TEST_CHECK(countUsed(&pool) == NUM_HANDLES);
	CRUST_TEST_CHECK(pool.free_head == CRUST_USIZE_MAX);

	crustGenerationalHandlePoolFree(allocator, &pool);
	return 0;
}

static int testAddToFullPool(void)
{
	Crust_Allocator allocator = crustSystemAllocatorInit();
	Crust_GenerationalHandlePool pool;
	CRUST_TEST_REQUIRE(crustGenerationalHandlePoolAlloc(allocator, &pool, 2));

	Crust_GenerationalHandle handles[2];
	handles[0] = crustGenerationalHandlePoolAdd(&pool, MAX_GENERATION);
	handles[1] = crustGenerationalHandlePoolAdd(&pool, MAX_GENERATION);

	Crust_GenerationalHandle handle = crustGenerationalHandlePoolAdd(&pool, MAX_GENERATION);
	CRUST_TEST_CHECK(handle.index == 0);
	CRUST_TEST_CHECK(handle.generation == 0);
	CRUST_TEST_CHECK(crustGenerationalHandlePoolCheck(&pool, handle) == 0);

	CRUST_TEST_CHECK(crustGenerationalHandlePoolCheck(&pool, handles[0]) != 0);
	CRUST_TEST_CHECK(crustGenerationalHandlePoolCheck(&pool, handles[1]) != 0);
	CRUST_TEST_CHECK(countUsed(&pool) == 2);

	crustGenerationalHandlePoolRemove(&pool, handles[0]);

	handle = crustGenerationalHandlePoolAdd(&pool, MAX_GENERATION);
	CRUST_TEST_CHECK(handle.index == 0);
	CRUST_TEST_CHECK(handle.generation != 0);

	crustGenerationalHandlePoolFree(allocator, &pool);
	return 0;
}

static int testAllocationFailure(void)
{
	CrustTest_Allocator test_allocator;
	Crust_Allocator allocator = crustTestAllocatorInit(&test_allocator, 0);

	Crust_GenerationalHandlePool pool;

	for (usize budget = 0; budget < STORAGE_BLOCKS; ++budget)
	{
		test_allocator.budget = budget;
		CRUST_TEST_CHECK(crustGenerationalHandlePoolAlloc(allocator, &pool, 4) == 0);
		CRUST_TEST_CHECK(pool.capacity == 0);
		CRUST_TEST_CHECK(pool.generations == CRUST_NULL);
		CRUST_TEST_CHECK(pool.masks == CRUST_NULL);
		CRUST_TEST_CHECK(test_allocator.blocks == 0);

		crustGenerationalHandlePoolFree(allocator, &pool);
	}

	test_allocator.budget = STORAGE_BLOCKS;
	CRUST_TEST_REQUIRE(crustGenerationalHandlePoolAlloc(allocator, &pool, 4));

	Crust_GenerationalHandle handles[4];
	for (u32 i = 0; i < 4; ++i)
		handles[i] = crustGenerationalHandlePoolAdd(&pool, MAX_GENERATION);

	crustGenerationalHandlePoolRemove(&pool, handles[1]);

	for (usize budget = 0; budget < STORAGE_BLOCKS; ++budget)
	{
		usize *generations = pool.generations;

		test_allocator.budget = budget;
		CRUST_TEST_CHECK(crustGenerationalHandlePoolGrow(allocator, &pool, NUM_HANDLES) == 0);
		CRUST_TEST_CHECK(pool.generations == generations);
		CRUST_TEST_CHECK(pool.capacity == 4);
		CRUST_TEST_CHECK(test_allocator.blocks == STORAGE_BLOCKS);
		CRUST_TEST_CHECK(countUsed(&pool) == 3);
		CRUST_TEST_CHECK(crustGenerationalHandlePoolCheck(&pool, handles[0]) != 0);
		CRUST_TEST_CHECK(crustGenerationalHandlePoolCheck(&pool, handles[2]) != 0);
		CRUST_TEST_CHECK(crustGenerationalHandlePoolCheck(&pool, handles[3]) != 0);
	}

	test_allocator.budget = CRUST_USIZE_MAX;
	CRUST_TEST_REQUIRE(crustGenerationalHandlePoolGrow(allocator, &pool, NUM_HANDLES));
	CRUST_TEST_CHECK(pool.capacity == NUM_HANDLES);
	CRUST_TEST_CHECK(countUsed(&pool) == 3);
	CRUST_TEST_CHECK(crustGenerationalHandlePoolCheck(&pool, handles[0]) != 0);
	CRUST_TEST_CHECK(crustGenerationalHandlePoolCheck(&pool, handles[1]) == 0);
	CRUST_TEST_CHECK(crustGenerationalHandlePoolCheck(&pool, handles[2]) != 0);
	CRUST_TEST_CHECK(crustGenerationalHandlePoolCheck(&pool, handles[3]) != 0);

	for (u32 i = 0; i < NUM_HANDLES - 3; ++i)
		CRUST_TEST_CHECK(crustGenerationalHandlePoolAdd(&pool, MAX_GENERATION).generation != 0);

	CRUST_TEST_CHECK(crustGenerationalHandlePoolAdd(&pool, MAX_GENERATION).generation == 0);
	CRUST_TEST_CHECK(countUsed(&pool) == NUM_HANDLES);

	crustGenerationalHandlePoolFree(allocator, &pool);
	CRUST_TEST_CHECK(test_allocator.blocks == 0);
	CRUST_TEST_CHECK(test_allocator.bytes == 0);

	return 0;
}

static int testCheckAnyHandle(void)
{
	Crust_Allocator allocator = crustSystemAllocatorInit();
	Crust_GenerationalHandlePool pool;
	CRUST_TEST_REQUIRE(crustGenerationalHandlePoolAlloc(allocator, &pool, 4));

	Crust_GenerationalHandle live = crustGenerationalHandlePoolAdd(&pool, MAX_GENERATION);
	Crust_GenerationalHandle handle = live;

	handle.index = pool.capacity;
	CRUST_TEST_CHECK(crustGenerationalHandlePoolCheck(&pool, handle) == 0);

	handle.index = CRUST_USIZE_MAX;
	CRUST_TEST_CHECK(crustGenerationalHandlePoolCheck(&pool, handle) == 0);

	crustGenerationalHandlePoolFree(allocator, &pool);
	CRUST_TEST_CHECK(crustGenerationalHandlePoolCheck(&pool, live) == 0);

	return 0;
}

static int testFuzz(void)
{
	Crust_Allocator allocator = crustSystemAllocatorInit();
	Crust_GenerationalHandlePool pool;
	crustGenerationalHandlePoolAlloc(allocator, &pool, FUZZ_INITIAL_CAPACITY);

	static Crust_GenerationalHandle live[FUZZ_MAX_CAPACITY];
	usize num_live = 0;

	for (u32 step = 0; step < FUZZ_STEPS; ++step)
	{
		if (crustTestRandom() % 10 < 5)
		{
			if (num_live == pool.capacity)
			{
				if (pool.capacity >= FUZZ_MAX_CAPACITY)
					continue;

				crustGenerationalHandlePoolGrow(allocator, &pool, pool.capacity * 2);
			}

			live[num_live++] = crustGenerationalHandlePoolAdd(&pool, 7);
		}
		else if (num_live > 0)
		{
			usize i = crustTestRandom() % num_live;
			Crust_GenerationalHandle handle = live[i];

			crustGenerationalHandlePoolRemove(&pool, handle);
			CRUST_TEST_REQUIRE(crustGenerationalHandlePoolCheck(&pool, handle) == 0);

			live[i] = live[--num_live];
		}

		for (usize i = 0; i < num_live; ++i)
			CRUST_TEST_REQUIRE(crustGenerationalHandlePoolCheck(&pool, live[i]) != 0);

		CRUST_TEST_REQUIRE(countUsed(&pool) == num_live);
	}

	crustGenerationalHandlePoolFree(allocator, &pool);
	return 0;
}

int main(void)
{
	CRUST_TEST_RUN(testSequentialAddCheckRemove);
	CRUST_TEST_RUN(testReuseFreeSlots);
	CRUST_TEST_RUN(testOldHandlesAreInvalid);
	CRUST_TEST_RUN(testIterationOrder);
	CRUST_TEST_RUN(testGenerationWrap);
	CRUST_TEST_RUN(testGrowKeepsHandles);
	CRUST_TEST_RUN(testAddToFullPool);
	CRUST_TEST_RUN(testAllocationFailure);
	CRUST_TEST_RUN(testCheckAnyHandle);
	CRUST_TEST_RUN(testFuzz);

	return crustTestResult();
}
