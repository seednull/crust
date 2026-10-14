#include "crust_test.h"

static const Crust_MemoryOrder orders[] =
{
	CRUST_MEMORY_ORDER_RELAXED,
	CRUST_MEMORY_ORDER_CONSUME,
	CRUST_MEMORY_ORDER_ACQUIRE,
	CRUST_MEMORY_ORDER_RELEASE,
	CRUST_MEMORY_ORDER_ACQ_REL,
	CRUST_MEMORY_ORDER_SEQ_CST,
};

enum
{
	NUM_ORDERS = sizeof(orders) / sizeof(orders[0]),
};

static u8 isLoadOrder(Crust_MemoryOrder order)
{
	return order != CRUST_MEMORY_ORDER_RELEASE && order != CRUST_MEMORY_ORDER_ACQ_REL;
}

static u8 isStoreOrder(Crust_MemoryOrder order)
{
	return order == CRUST_MEMORY_ORDER_RELAXED || order == CRUST_MEMORY_ORDER_RELEASE || order == CRUST_MEMORY_ORDER_SEQ_CST;
}

static int testU32(void)
{
	for (u32 i = 0; i < NUM_ORDERS; ++i)
	{
		Crust_MemoryOrder order = orders[i];
		volatile u32 value = 0xFFFFFFFFu;

		CRUST_TEST_CHECK(crustAtomicIncrementU32(&value, order) == 0);
		CRUST_TEST_CHECK(crustAtomicDecrementU32(&value, order) == 0xFFFFFFFFu);
		CRUST_TEST_CHECK(crustAtomicSwapU32(&value, 7, order) == 0xFFFFFFFFu);
		CRUST_TEST_CHECK(value == 7);

		if (isLoadOrder(order))
			CRUST_TEST_CHECK(crustAtomicLoadU32(&value, order) == 7);

		if (isStoreOrder(order))
		{
			crustAtomicStoreU32(&value, 11, order);
			CRUST_TEST_CHECK(value == 11);
		}

		for (u32 j = 0; j < NUM_ORDERS; ++j)
		{
			if (crustIsValidFailureOrder(order, orders[j]) == 0)
				continue;

			value = 7;

			CRUST_TEST_CHECK(crustAtomicCompareAndSwapU32(&value, 9, 8, order, orders[j]) == 7);
			CRUST_TEST_CHECK(value == 7);

			CRUST_TEST_CHECK(crustAtomicCompareAndSwapU32(&value, 9, 7, order, orders[j]) == 7);
			CRUST_TEST_CHECK(value == 9);
		}
	}

	return 0;
}

static int testU64(void)
{
	for (u32 i = 0; i < NUM_ORDERS; ++i)
	{
		Crust_MemoryOrder order = orders[i];
		CRUST_ALIGNAS(8) volatile u64 value = 0xFFFFFFFFull;

		CRUST_TEST_CHECK(crustAtomicIncrementU64(&value, order) == 0x100000000ull);
		CRUST_TEST_CHECK(crustAtomicDecrementU64(&value, order) == 0xFFFFFFFFull);

		value = 0;
		CRUST_TEST_CHECK(crustAtomicDecrementU64(&value, order) == 0xFFFFFFFFFFFFFFFFull);
		CRUST_TEST_CHECK(crustAtomicIncrementU64(&value, order) == 0);

		CRUST_TEST_CHECK(crustAtomicSwapU64(&value, 0x123456789ABCDEF0ull, order) == 0);
		CRUST_TEST_CHECK(value == 0x123456789ABCDEF0ull);

		if (isLoadOrder(order))
			CRUST_TEST_CHECK(crustAtomicLoadU64(&value, order) == 0x123456789ABCDEF0ull);

		if (isStoreOrder(order))
		{
			crustAtomicStoreU64(&value, 0xAAAABBBBCCCCDDDDull, order);
			CRUST_TEST_CHECK(value == 0xAAAABBBBCCCCDDDDull);
		}

		for (u32 j = 0; j < NUM_ORDERS; ++j)
		{
			if (crustIsValidFailureOrder(order, orders[j]) == 0)
				continue;

			value = 0x100000007ull;

			CRUST_TEST_CHECK(crustAtomicCompareAndSwapU64(&value, 0x200000009ull, 0x000000007ull, order, orders[j]) == 0x100000007ull);
			CRUST_TEST_CHECK(value == 0x100000007ull);

			CRUST_TEST_CHECK(crustAtomicCompareAndSwapU64(&value, 0x200000009ull, 0x100000007ull, order, orders[j]) == 0x100000007ull);
			CRUST_TEST_CHECK(value == 0x200000009ull);
		}
	}

	return 0;
}

static int testUSize(void)
{
	volatile usize value = 0;

	CRUST_TEST_CHECK(crustAtomicIncrementUSize(&value, CRUST_MEMORY_ORDER_SEQ_CST) == 1);
	CRUST_TEST_CHECK(crustAtomicDecrementUSize(&value, CRUST_MEMORY_ORDER_SEQ_CST) == 0);
	CRUST_TEST_CHECK(crustAtomicDecrementUSize(&value, CRUST_MEMORY_ORDER_SEQ_CST) == CRUST_USIZE_MAX);
	CRUST_TEST_CHECK(crustAtomicSwapUSize(&value, 5, CRUST_MEMORY_ORDER_SEQ_CST) == CRUST_USIZE_MAX);
	CRUST_TEST_CHECK(crustAtomicCompareAndSwapUSize(&value, 6, 5, CRUST_MEMORY_ORDER_SEQ_CST, CRUST_MEMORY_ORDER_SEQ_CST) == 5);

	crustAtomicStoreUSize(&value, 8, CRUST_MEMORY_ORDER_RELEASE);
	CRUST_TEST_CHECK(crustAtomicLoadUSize(&value, CRUST_MEMORY_ORDER_ACQUIRE) == 8);

	return 0;
}

#if defined(CRUST_TEST_THREADS)
enum
{
	NUM_THREADS = 4,
	ITERATIONS = 500000,
};

static volatile u32 counter32;
static volatile u64 counter64;
static volatile u64 slot64;
static volatile u32 torn_values;

static u8 isTorn(u64 value)
{
	return (u32)(value >> 32) != (u32)value;
}

static void worker(void *arg)
{
	u32 id = (u32)(usize)arg;

	for (u32 i = 0; i < ITERATIONS; ++i)
	{
		crustAtomicIncrementU32(&counter32, CRUST_MEMORY_ORDER_SEQ_CST);

		crustAtomicIncrementU64(&counter64, CRUST_MEMORY_ORDER_RELAXED);
		crustAtomicIncrementU64(&counter64, CRUST_MEMORY_ORDER_SEQ_CST);
		crustAtomicDecrementU64(&counter64, CRUST_MEMORY_ORDER_ACQ_REL);

		u32 half = id * ITERATIONS + i;
		u64 value = ((u64)half << 32) | half;

		u64 previous = crustAtomicSwapU64(&slot64, value, CRUST_MEMORY_ORDER_SEQ_CST);
		if (isTorn(previous))
			crustAtomicIncrementU32(&torn_values, CRUST_MEMORY_ORDER_RELAXED);

		Crust_MemoryOrder store_order = CRUST_MEMORY_ORDER_RELEASE;
		if (i & 1)
			store_order = CRUST_MEMORY_ORDER_SEQ_CST;

		crustAtomicStoreU64(&slot64, value, store_order);

		u64 loaded = crustAtomicLoadU64(&slot64, CRUST_MEMORY_ORDER_ACQUIRE);
		if (isTorn(loaded))
			crustAtomicIncrementU32(&torn_values, CRUST_MEMORY_ORDER_RELAXED);

		u64 exchanged = crustAtomicCompareAndSwapU64(&slot64, value, loaded, CRUST_MEMORY_ORDER_ACQ_REL, CRUST_MEMORY_ORDER_ACQUIRE);
		if (isTorn(exchanged))
			crustAtomicIncrementU32(&torn_values, CRUST_MEMORY_ORDER_RELAXED);
	}
}

static int testConcurrentUpdates(void)
{
	CrustTest_Thread threads[NUM_THREADS];

	for (u32 i = 0; i < NUM_THREADS; ++i)
		threads[i] = crustTestThreadCreate(worker, (void *)(usize)i);

	for (u32 i = 0; i < NUM_THREADS; ++i)
		crustTestThreadJoin(threads[i]);

	CRUST_TEST_CHECK(counter32 == (u32)NUM_THREADS * ITERATIONS);
	CRUST_TEST_CHECK(counter64 == (u64)NUM_THREADS * ITERATIONS);
	CRUST_TEST_CHECK(torn_values == 0);

	return 0;
}
#endif

int main(void)
{
	CRUST_TEST_RUN(testU32);
	CRUST_TEST_RUN(testU64);
	CRUST_TEST_RUN(testUSize);

#if defined(CRUST_TEST_THREADS)
	CRUST_TEST_RUN(testConcurrentUpdates);
#endif

	return crustTestResult();
}
