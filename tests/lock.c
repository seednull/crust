#include "crust_test.h"

enum
{
	NUM_WRITERS = 2,
	NUM_READERS = 4,
	WRITER_ITERATIONS = 200000,
	NUM_SPINNERS = 4,
	SPINNER_ITERATIONS = 500000,
};

static Crust_RwLock rw_lock;
static u64 guarded_a;
static u64 guarded_b;
static volatile u32 writers_done;
static volatile u32 torn_reads;

static Crust_SpinLock spin_lock;
static u64 spin_counter;

static void rwWriter(void *arg)
{
	CRUST_UNUSED(arg);

	for (u32 i = 0; i < WRITER_ITERATIONS; ++i)
	{
		u8 acquired = 0;
		if (i & 1)
			acquired = crustRwLockTryAcquireWrite(&rw_lock);

		if (acquired == 0)
			crustRwLockAcquireWrite(&rw_lock);

		guarded_a++;
		guarded_b++;

		crustRwLockReleaseWrite(&rw_lock);
	}

	crustAtomicIncrementU32(&writers_done, CRUST_MEMORY_ORDER_RELEASE);
}

static void rwReader(void *arg)
{
	CRUST_UNUSED(arg);

	for (u32 i = 0; crustAtomicLoadU32(&writers_done, CRUST_MEMORY_ORDER_ACQUIRE) < NUM_WRITERS; ++i)
	{
		u8 acquired = 0;
		if (i & 1)
			acquired = crustRwLockTryAcquireRead(&rw_lock);

		if (acquired == 0)
			crustRwLockAcquireRead(&rw_lock);

		if (guarded_a != guarded_b)
			crustAtomicIncrementU32(&torn_reads, CRUST_MEMORY_ORDER_RELAXED);

		crustRwLockReleaseRead(&rw_lock);
	}
}

static void spinner(void *arg)
{
	CRUST_UNUSED(arg);

	for (u32 i = 0; i < SPINNER_ITERATIONS; ++i)
	{
		u8 acquired = 0;
		if (i & 1)
			acquired = crustSpinLockTryAcquire(&spin_lock);

		if (acquired == 0)
			crustSpinLockAcquire(&spin_lock);

		spin_counter++;

		crustSpinLockRelease(&spin_lock);
	}
}

static int testRwLock(void)
{
	rw_lock = crustRwLockInit();

	CrustTest_Thread threads[NUM_WRITERS + NUM_READERS];

	for (u32 i = 0; i < NUM_WRITERS; ++i)
		threads[i] = crustTestThreadCreate(rwWriter, CRUST_NULL);

	for (u32 i = 0; i < NUM_READERS; ++i)
		threads[NUM_WRITERS + i] = crustTestThreadCreate(rwReader, CRUST_NULL);

	for (u32 i = 0; i < NUM_WRITERS + NUM_READERS; ++i)
		crustTestThreadJoin(threads[i]);

	CRUST_TEST_CHECK(torn_reads == 0);
	CRUST_TEST_CHECK(guarded_a == (u64)NUM_WRITERS * WRITER_ITERATIONS);
	CRUST_TEST_CHECK(guarded_b == (u64)NUM_WRITERS * WRITER_ITERATIONS);

	return 0;
}

static int testSpinLock(void)
{
	spin_lock = crustSpinLockInit();

	CrustTest_Thread spinners[NUM_SPINNERS];

	for (u32 i = 0; i < NUM_SPINNERS; ++i)
		spinners[i] = crustTestThreadCreate(spinner, CRUST_NULL);

	for (u32 i = 0; i < NUM_SPINNERS; ++i)
		crustTestThreadJoin(spinners[i]);

	CRUST_TEST_CHECK(spin_counter == (u64)NUM_SPINNERS * SPINNER_ITERATIONS);

	return 0;
}

int main(void)
{
	CRUST_TEST_RUN(testRwLock);
	CRUST_TEST_RUN(testSpinLock);

	return crustTestResult();
}
