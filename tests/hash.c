#include "crust_test.h"

#include <string.h>

#define MOVED_FROM_KEY 0xCDCDCDCDu

enum
{
	NUM_KEYS = 300,
	INITIAL_CAPACITY = 16,
	STEPS = 30000,
};

static i32 live_keys = 0;
static i32 destroyed_moved_from = 0;

static usize hashKey(void *context, const void *key, usize key_size)
{
	CRUST_UNUSED(context);
	CRUST_UNUSED(key_size);

	return (*(const u32 *)key) % 13;
}

static u8 equalKeys(void *context, const void *key1, const void *key2, usize key_size)
{
	CRUST_UNUSED(context);
	return memcmp(key1, key2, key_size) == 0;
}

static void copyKey(void *context, void *key_dst, const void *key_src, usize key_size)
{
	CRUST_UNUSED(context);

	memcpy(key_dst, key_src, key_size);
	live_keys++;
}

static void moveKey(void *context, void *key_dst, void *key_src, usize key_size)
{
	CRUST_UNUSED(context);

	memcpy(key_dst, key_src, key_size);
	memset(key_src, 0xCD, key_size);
}

static void destroyKey(void *context, void *key, usize key_size)
{
	CRUST_UNUSED(context);
	CRUST_UNUSED(key_size);

	if (*(const u32 *)key == MOVED_FROM_KEY)
		destroyed_moved_from++;

	live_keys--;
}

static int testFuzz(void)
{
	static const Crust_HashIndexMapVtbl vtbl = { hashKey, equalKeys, copyKey, moveKey, destroyKey };
	Crust_Allocator allocator = crustSystemAllocatorInit();

	Crust_HashIndexMap map = crustHashIndexMapInit(CRUST_NULL, &vtbl, sizeof(u32), sizeof(u32));
	crustHashIndexMapAlloc(allocator, &map, INITIAL_CAPACITY);

	static usize model[NUM_KEYS];
	usize count = 0;

	for (u32 i = 0; i < NUM_KEYS; ++i)
		model[i] = CRUST_USIZE_MAX;

	for (u32 step = 0; step < STEPS; ++step)
	{
		u32 key = crustTestRandom() % NUM_KEYS;
		u32 op = crustTestRandom() % 3;

		if (op == 0)
		{
			if (count + 1 > map.capacity * 3 / 4)
				crustHashIndexMapGrow(allocator, &map, map.capacity * 2);

			usize index = step;
			if (model[key] != CRUST_USIZE_MAX)
				index = model[key];

			CRUST_TEST_REQUIRE(crustHashIndexMapAdd(&map, &key, sizeof(key), index) == index);

			if (model[key] == CRUST_USIZE_MAX)
			{
				model[key] = index;
				count++;
			}
		}
		else if (op == 1)
		{
			CRUST_TEST_REQUIRE(crustHashIndexMapRemove(&map, &key, sizeof(key)) == model[key]);

			if (model[key] != CRUST_USIZE_MAX)
			{
				model[key] = CRUST_USIZE_MAX;
				count--;
			}
		}

		CRUST_TEST_REQUIRE(map.count == count);
		CRUST_TEST_REQUIRE(crustHashIndexMapFind(&map, &key, sizeof(key)) == model[key]);

		if (step % 8 != 0)
			continue;

		for (u32 k = 0; k < NUM_KEYS; ++k)
			CRUST_TEST_REQUIRE(crustHashIndexMapFind(&map, &k, sizeof(k)) == model[k]);
	}

	crustHashIndexMapFree(allocator, &map);

	CRUST_TEST_CHECK(live_keys == 0);
	CRUST_TEST_CHECK(destroyed_moved_from == 0);

	return 0;
}

int main(void)
{
	CRUST_TEST_RUN(testFuzz);

	return crustTestResult();
}
