#include "crust_test.h"

#include <string.h>

enum
{
	CAPACITY = 128,
	FUZZ_CAPACITY = 4096,
	FUZZ_ROUNDS = 1000,
};

static int testAlloc(void)
{
	static CRUST_ALIGNAS(64) u8 memory[CAPACITY];
	Crust_Arena arena = crustArenaAttach(memory, CAPACITY);

	CRUST_TEST_CHECK(crustArenaAlloc(&arena, 10, 1) == memory);
	CRUST_TEST_CHECK(crustArenaAlloc(&arena, 4, 4) == memory + 12);
	CRUST_TEST_CHECK(crustArenaAlloc(&arena, 1, 16) == memory + 16);
	CRUST_TEST_CHECK(arena.size == 17);

	CRUST_TEST_CHECK(crustArenaAlloc(&arena, 112, 1) == CRUST_NULL);
	CRUST_TEST_CHECK(arena.size == 17);

	CRUST_TEST_CHECK(crustArenaAlloc(&arena, 96, 32) == memory + 32);
	CRUST_TEST_CHECK(arena.size == CAPACITY);

	CRUST_TEST_CHECK(crustArenaAlloc(&arena, 1, 1) == CRUST_NULL);
	return 0;
}

static int testReset(void)
{
	static CRUST_ALIGNAS(64) u8 memory[CAPACITY];
	Crust_Arena arena = crustArenaAttach(memory, CAPACITY);

	CRUST_TEST_CHECK(crustArenaAlloc(&arena, CAPACITY, 1) == memory);
	CRUST_TEST_CHECK(crustArenaAlloc(&arena, 1, 1) == CRUST_NULL);

	crustArenaReset(&arena);
	CRUST_TEST_CHECK(arena.size == 0);
	CRUST_TEST_CHECK(crustArenaAlloc(&arena, 8, 8) == memory);

	return 0;
}

static int testAllocator(void)
{
	static CRUST_ALIGNAS(64) u8 memory[CAPACITY];
	Crust_Arena arena = crustArenaAttach(memory, CAPACITY);
	Crust_Allocator allocator = crustArenaAllocatorInit(&arena);

	u8 *a = (u8 *)crustAllocatorAlloc(allocator, 16, 8);
	CRUST_TEST_REQUIRE(a == memory);

	for (u8 i = 0; i < 16; ++i)
		a[i] = i;

	a = (u8 *)crustAllocatorRealloc(allocator, a, 16, 32, 8);
	CRUST_TEST_CHECK(a == memory);
	CRUST_TEST_CHECK(arena.size == 32);

	u8 *b = (u8 *)crustAllocatorAlloc(allocator, 8, 8);
	CRUST_TEST_CHECK(b == memory + 32);

	a = (u8 *)crustAllocatorRealloc(allocator, a, 32, 48, 8);
	CRUST_TEST_REQUIRE(a == memory + 40);
	CRUST_TEST_CHECK(arena.size == 88);

	for (u8 i = 0; i < 16; ++i)
		CRUST_TEST_CHECK(a[i] == i);

	a = (u8 *)crustAllocatorRealloc(allocator, a, 48, 24, 8);
	CRUST_TEST_CHECK(a == memory + 40);
	CRUST_TEST_CHECK(arena.size == 64);

	crustAllocatorFree(allocator, b, 8, 8);
	CRUST_TEST_CHECK(arena.size == 64);

	CRUST_TEST_CHECK(crustAllocatorRealloc(allocator, a, 24, 100, 8) == CRUST_NULL);
	CRUST_TEST_CHECK(arena.size == 64);

	u32 *array = crustAllocArray(allocator, u32, 4);
	CRUST_TEST_CHECK((u8 *)array == memory + 64);
	CRUST_TEST_CHECK(arena.size == 80);

	return 0;
}

static int testAlignmentBeyondCapacity(void)
{
	static CRUST_ALIGNAS(64) u8 memory[2];
	Crust_Arena arena = crustArenaAttach(memory + 1, 1);

	CRUST_TEST_CHECK(crustArenaAlloc(&arena, 1, 64) == CRUST_NULL);
	CRUST_TEST_CHECK(arena.size == 0);

	CRUST_TEST_CHECK(crustArenaAlloc(&arena, 1, 1) == memory + 1);
	CRUST_TEST_CHECK(arena.size == 1);

	CRUST_TEST_CHECK(crustArenaAlloc(&arena, 0, 64) == CRUST_NULL);
	CRUST_TEST_CHECK(arena.size == 1);

	return 0;
}

static int testSizeMul(void)
{
	static CRUST_ALIGNAS(64) u8 memory[CAPACITY];
	Crust_Arena arena = crustArenaAttach(memory, CAPACITY);
	Crust_Allocator allocator = crustArenaAllocatorInit(&arena);

	CRUST_TEST_CHECK(crustAllocatorSizeMul(0, CRUST_USIZE_MAX) == 0);
	CRUST_TEST_CHECK(crustAllocatorSizeMul(CRUST_USIZE_MAX, 1) == CRUST_USIZE_MAX);
	CRUST_TEST_CHECK(crustAllocatorSizeMul(12, 10) == 120);

	CRUST_TEST_CHECK(crustAllocatorAlloc(allocator, CRUST_USIZE_MAX, 8) == CRUST_NULL);
	CRUST_TEST_CHECK(arena.size == 0);

#if defined(NDEBUG)
	CRUST_TEST_CHECK(crustAllocatorSizeMul(2, CRUST_USIZE_MAX / 2 + 1) == CRUST_USIZE_MAX);
	CRUST_TEST_CHECK(crustAllocArray(allocator, u64, CRUST_USIZE_MAX / 4) == CRUST_NULL);
	CRUST_TEST_CHECK(arena.size == 0);
#endif

	return 0;
}

static int testFuzz(void)
{
	static CRUST_ALIGNAS(64) u8 memory[FUZZ_CAPACITY];
	Crust_Arena arena = crustArenaAttach(memory, FUZZ_CAPACITY);

	for (u32 round = 0; round < FUZZ_ROUNDS; ++round)
	{
		crustArenaReset(&arena);
		u8 *end = memory;

		for (;;)
		{
			usize size = crustTestRandom() % 100;
			usize alignment = (usize)1 << (crustTestRandom() % 7);

			u8 *ptr = (u8 *)crustArenaAlloc(&arena, size, alignment);

			if (ptr == CRUST_NULL)
			{
				usize aligned_offset = crustAlignUpUSize((usize)end, alignment) - (usize)memory;
				CRUST_TEST_REQUIRE(aligned_offset + size > FUZZ_CAPACITY);
				CRUST_TEST_REQUIRE(memory + arena.size == end);
				break;
			}

			CRUST_TEST_REQUIRE((usize)ptr % alignment == 0);
			CRUST_TEST_REQUIRE(ptr >= end);
			CRUST_TEST_REQUIRE(ptr < end + alignment);
			CRUST_TEST_REQUIRE(ptr + size <= memory + FUZZ_CAPACITY);

			memset(ptr, (u8)round, size);

			end = ptr + size;
			CRUST_TEST_REQUIRE(memory + arena.size == end);
		}
	}

	return 0;
}

int main(void)
{
	CRUST_TEST_RUN(testAlloc);
	CRUST_TEST_RUN(testReset);
	CRUST_TEST_RUN(testAllocator);
	CRUST_TEST_RUN(testAlignmentBeyondCapacity);
	CRUST_TEST_RUN(testSizeMul);
	CRUST_TEST_RUN(testFuzz);

	return crustTestResult();
}
