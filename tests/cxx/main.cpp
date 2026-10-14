#include "crust_test.h"

#include <string.h>

extern "C"
{
	static usize hashU32(void *context, const void *key, usize key_size)
	{
		CRUST_UNUSED(context);
		CRUST_UNUSED(key_size);

		return *static_cast<const u32 *>(key);
	}

	static u8 equalU32(void *context, const void *key1, const void *key2, usize key_size)
	{
		CRUST_UNUSED(context);
		CRUST_UNUSED(key_size);

		return *static_cast<const u32 *>(key1) == *static_cast<const u32 *>(key2);
	}

	static void copyU32(void *context, void *key_dst, const void *key_src, usize key_size)
	{
		CRUST_UNUSED(context);
		memcpy(key_dst, key_src, key_size);
	}

	static void moveU32(void *context, void *key_dst, void *key_src, usize key_size)
	{
		CRUST_UNUSED(context);
		memcpy(key_dst, key_src, key_size);
	}

	static void destroyU32(void *context, void *key, usize key_size)
	{
		CRUST_UNUSED(context);
		CRUST_UNUSED(key);
		CRUST_UNUSED(key_size);
	}
}

static int testBuffers()
{
	static CRUST_ALIGNAS(16) u8 storage[256];
	u32 value = 7;
	u32 read = 0;

	Crust_RingBuffer ring = crustRingBufferAttach(storage, 64);
	crustRingBufferWrite(&ring, &value, sizeof(value));
	crustRingBufferRead(&ring, &read, sizeof(read));
	CRUST_TEST_CHECK(read == 7);

	Crust_Stack stack = crustStackAttach(storage + 64, 64);
	crustStackPush(&stack, &value, sizeof(value));
	crustStackPop(&stack, &read, sizeof(read));
	CRUST_TEST_CHECK(read == 7);

	Crust_BipBuffer bip = crustBipBufferAttach(storage + 128, 64);
	void *write = crustBipBufferStageWrite(&bip, sizeof(value));
	CRUST_TEST_REQUIRE(write != CRUST_NULL);
	crustMemcpy(write, &value, sizeof(value));
	crustBipBufferCommitWrite(&bip, sizeof(value));

	const void *staged = crustBipBufferStageRead(&bip, sizeof(value));
	CRUST_TEST_REQUIRE(staged != CRUST_NULL);
	CRUST_TEST_CHECK(*static_cast<const u32 *>(staged) == 7);
	crustBipBufferCommitRead(&bip, sizeof(value));

	Crust_Arena arena = crustArenaAttach(storage + 192, 64);
	Crust_Allocator allocator = crustArenaAllocatorInit(&arena);
	u64 *number = crustAlloc(allocator, u64);
	CRUST_TEST_REQUIRE(number != CRUST_NULL);
	crustMemset(number, 0, sizeof(*number));
	CRUST_TEST_CHECK(*number == 0);

	return 0;
}

static int testContainers()
{
	static const Crust_HashIndexMapVtbl vtbl = { hashU32, equalU32, copyU32, moveU32, destroyU32 };
	Crust_Allocator allocator = crustSystemAllocatorInit();

	Crust_HashIndexMap map = crustHashIndexMapInit(CRUST_NULL, &vtbl, sizeof(u32), CRUST_ALIGNOF(u32));
	CRUST_TEST_REQUIRE(crustHashIndexMapAlloc(allocator, &map, 8));

	u32 key = 42;
	CRUST_TEST_CHECK(crustHashIndexMapAdd(&map, &key, sizeof(key), 3) == 3);
	CRUST_TEST_CHECK(crustHashIndexMapFind(&map, &key, sizeof(key)) == 3);
	crustHashIndexMapFree(allocator, &map);

	Crust_GenerationalHandlePool pool;
	CRUST_TEST_REQUIRE(crustGenerationalHandlePoolAlloc(allocator, &pool, 4));

	Crust_GenerationalHandle handle = crustGenerationalHandlePoolAdd(&pool, CRUST_USIZE_MAX);
	CRUST_TEST_CHECK(crustGenerationalHandlePoolCheck(&pool, handle) != 0);
	crustGenerationalHandlePoolRemove(&pool, handle);
	crustGenerationalHandlePoolFree(allocator, &pool);

	Crust_HeapNodePool nodes;
	CRUST_TEST_REQUIRE(crustHeapNodePoolAlloc(allocator, &nodes, 8));

	Crust_Heap heap = crustHeapInit(&nodes, 1024);
	Crust_HeapAllocation allocation = crustHeapAllocAligned(&heap, 100, 64);
	CRUST_TEST_CHECK(allocation.offset == 0);
	crustHeapFree(&heap, allocation);
	crustHeapNodePoolFree(allocator, &nodes);

	return 0;
}

static int testInline()
{
	Crust_SpinLock spin = crustSpinLockInit();
	crustSpinLockAcquire(&spin);
	crustSpinLockRelease(&spin);

	Crust_RwLock rw = crustRwLockInit();
	CRUST_TEST_CHECK(crustRwLockTryAcquireRead(&rw) != 0);
	crustRwLockReleaseRead(&rw);

	volatile u32 counter = 0;
	CRUST_TEST_CHECK(crustAtomicIncrementU32(&counter, CRUST_MEMORY_ORDER_SEQ_CST) == 1);

	CRUST_TEST_CHECK(crustSinF32(0.0f) == 0.0f);
	CRUST_TEST_CHECK(crustSqrtF32(16.0f) == 4.0f);
	CRUST_TEST_CHECK(crustMinU32(3, 5) == 3);

	Crust_Simd128 v = crustSimd128SplatU32(7);
	CRUST_TEST_CHECK(crustSimd128GetLaneU32(crustSimd128AddU32(v, v), 3) == 14);

	Crust_Vec3 a = { 1.0f, 2.0f, 2.0f };
	CRUST_TEST_CHECK(crustVec3Length(a) == 3.0f);

	Crust_Quat identity = { 0.0f, 0.0f, 0.0f, 1.0f };
	Crust_Vec3 rotated = crustQuatRotate(identity, a);
	CRUST_TEST_CHECK(rotated.x == 1.0f && rotated.y == 2.0f && rotated.z == 2.0f);

	return 0;
}

int main()
{
	CRUST_TEST_RUN(testBuffers);
	CRUST_TEST_RUN(testContainers);
	CRUST_TEST_RUN(testInline);

	return crustTestResult();
}
