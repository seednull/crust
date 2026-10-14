#include "crust_test.h"

enum
{
	NUM_WRITERS = 2,
	NUM_READERS = 4,
	WRITER_ITERATIONS = 200000,
	NUM_SPINNERS = 4,
	SPINNER_ITERATIONS = 500000,
	MAX_WAIT_ITERATIONS = 100000000,
};

static int testSpinLockTryAcquire(void)
{
	Crust_SpinLock lock = crustSpinLockInit();

	CRUST_TEST_CHECK(crustSpinLockTryAcquire(&lock) != 0);
	CRUST_TEST_CHECK(crustSpinLockTryAcquire(&lock) == 0);

	crustSpinLockRelease(&lock);
	CRUST_TEST_CHECK(crustSpinLockTryAcquire(&lock) != 0);
	crustSpinLockRelease(&lock);

	crustSpinLockAcquire(&lock);
	CRUST_TEST_CHECK(crustSpinLockTryAcquire(&lock) == 0);
	crustSpinLockRelease(&lock);

	return 0;
}

static int testRwLockTryAcquire(void)
{
	Crust_RwLock lock = crustRwLockInit();

	CRUST_TEST_CHECK(crustRwLockTryAcquireRead(&lock) != 0);
	CRUST_TEST_CHECK(crustRwLockTryAcquireRead(&lock) != 0);
	crustRwLockAcquireRead(&lock);
	CRUST_TEST_CHECK(crustRwLockTryAcquireWrite(&lock) == 0);

	crustRwLockReleaseRead(&lock);
	crustRwLockReleaseRead(&lock);
	CRUST_TEST_CHECK(crustRwLockTryAcquireWrite(&lock) == 0);

	crustRwLockReleaseRead(&lock);
	CRUST_TEST_CHECK(crustRwLockTryAcquireWrite(&lock) != 0);
	CRUST_TEST_CHECK(crustRwLockTryAcquireRead(&lock) == 0);
	CRUST_TEST_CHECK(crustRwLockTryAcquireWrite(&lock) == 0);
	crustRwLockReleaseWrite(&lock);

	crustRwLockAcquireWrite(&lock);
	CRUST_TEST_CHECK(crustRwLockTryAcquireRead(&lock) == 0);
	CRUST_TEST_CHECK(crustRwLockTryAcquireWrite(&lock) == 0);
	crustRwLockReleaseWrite(&lock);

	CRUST_TEST_CHECK(crustRwLockTryAcquireRead(&lock) != 0);
	crustRwLockReleaseRead(&lock);

	return 0;
}

#if defined(CRUST_TEST_THREADS)
static Crust_RwLock rw_lock;
static u64 guarded_a;
static u64 guarded_b;
static volatile u32 active_readers;
static volatile u32 active_writers;
static volatile u32 writers_done;
static volatile u32 rw_overlaps;

static Crust_SpinLock spin_lock;
static u64 spin_counter;
static volatile u32 spin_holders;
static volatile u32 spin_overlaps;

static Crust_RwLock preference_lock;
static volatile u32 pending_writer_done;

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

		u32 writers = crustAtomicIncrementU32(&active_writers, CRUST_MEMORY_ORDER_RELAXED);
		u32 readers = crustAtomicLoadU32(&active_readers, CRUST_MEMORY_ORDER_RELAXED);

		if (writers != 1 || readers != 0)
			crustAtomicIncrementU32(&rw_overlaps, CRUST_MEMORY_ORDER_RELAXED);

		guarded_a++;
		guarded_b++;

		crustAtomicDecrementU32(&active_writers, CRUST_MEMORY_ORDER_RELAXED);
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

		crustAtomicIncrementU32(&active_readers, CRUST_MEMORY_ORDER_RELAXED);

		if (crustAtomicLoadU32(&active_writers, CRUST_MEMORY_ORDER_RELAXED) != 0 || guarded_a != guarded_b)
			crustAtomicIncrementU32(&rw_overlaps, CRUST_MEMORY_ORDER_RELAXED);

		crustAtomicDecrementU32(&active_readers, CRUST_MEMORY_ORDER_RELAXED);
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

		if (crustAtomicIncrementU32(&spin_holders, CRUST_MEMORY_ORDER_RELAXED) != 1)
			crustAtomicIncrementU32(&spin_overlaps, CRUST_MEMORY_ORDER_RELAXED);

		spin_counter++;

		crustAtomicDecrementU32(&spin_holders, CRUST_MEMORY_ORDER_RELAXED);
		crustSpinLockRelease(&spin_lock);
	}
}

static void pendingWriter(void *arg)
{
	CRUST_UNUSED(arg);

	crustRwLockAcquireWrite(&preference_lock);
	crustAtomicStoreU32(&pending_writer_done, 1, CRUST_MEMORY_ORDER_RELAXED);
	crustRwLockReleaseWrite(&preference_lock);
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

	CRUST_TEST_CHECK(rw_overlaps == 0);
	CRUST_TEST_CHECK(guarded_a == (u64)NUM_WRITERS * WRITER_ITERATIONS);
	CRUST_TEST_CHECK(guarded_b == (u64)NUM_WRITERS * WRITER_ITERATIONS);
	CRUST_TEST_CHECK(crustRwLockTryAcquireWrite(&rw_lock) != 0);

	return 0;
}

static int testRwLockWriterPreference(void)
{
	preference_lock = crustRwLockInit();
	crustRwLockAcquireRead(&preference_lock);

	CrustTest_Thread writer = crustTestThreadCreate(pendingWriter, CRUST_NULL);

	u8 refused = 0;
	for (u32 i = 0; i < MAX_WAIT_ITERATIONS && refused == 0; ++i)
	{
		if (crustRwLockTryAcquireRead(&preference_lock) == 0)
		{
			refused = 1;
			continue;
		}

		crustRwLockReleaseRead(&preference_lock);
		crustCpuRelax();
	}

	CRUST_TEST_CHECK(refused != 0);
	CRUST_TEST_CHECK(crustRwLockTryAcquireWrite(&preference_lock) == 0);
	CRUST_TEST_CHECK(crustAtomicLoadU32(&pending_writer_done, CRUST_MEMORY_ORDER_RELAXED) == 0);

	crustRwLockReleaseRead(&preference_lock);
	crustTestThreadJoin(writer);

	CRUST_TEST_CHECK(pending_writer_done == 1);
	CRUST_TEST_CHECK(crustRwLockTryAcquireRead(&preference_lock) != 0);
	crustRwLockReleaseRead(&preference_lock);

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

	CRUST_TEST_CHECK(spin_overlaps == 0);
	CRUST_TEST_CHECK(spin_counter == (u64)NUM_SPINNERS * SPINNER_ITERATIONS);
	CRUST_TEST_CHECK(crustSpinLockTryAcquire(&spin_lock) != 0);

	return 0;
}
#endif

int main(void)
{
	CRUST_TEST_RUN(testSpinLockTryAcquire);
	CRUST_TEST_RUN(testRwLockTryAcquire);

#if defined(CRUST_TEST_THREADS)
	CRUST_TEST_RUN(testRwLock);
	CRUST_TEST_RUN(testRwLockWriterPreference);
	CRUST_TEST_RUN(testSpinLock);
#endif

	return crustTestResult();
}
