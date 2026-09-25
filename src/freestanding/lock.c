#include <crust.h>

//
CRUST_APIENTRY Crust_SpinLock crustSpinLockInit(void)
{
	Crust_SpinLock result;
	result.counter = 0;

	return result;
}

CRUST_APIENTRY void crustSpinLockAcquire(Crust_SpinLock *lock)
{
	CRUST_ASSERT(lock != CRUST_NULL);

	u32 backoff = 1;

	for (;;)
	{
		while (crustAtomicLoadU32(&lock->counter, CRUST_MEMORY_ORDER_RELAXED) != 0)
		{
			u32 n = backoff;
			while (n--)
				crustCpuRelax();

			if (backoff < 1024)
				backoff *= 2;
		}

		if (crustAtomicCompareAndSwapU32(&lock->counter, 1, 0, CRUST_MEMORY_ORDER_ACQUIRE, CRUST_MEMORY_ORDER_RELAXED) == 0)
			break;

		if (backoff < 1024)
			backoff *= 2;
	}
}

CRUST_APIENTRY u8 crustSpinLockTryAcquire(Crust_SpinLock *lock)
{
	CRUST_ASSERT(lock != CRUST_NULL);
	return (crustAtomicCompareAndSwapU32(&lock->counter, 1, 0, CRUST_MEMORY_ORDER_ACQUIRE, CRUST_MEMORY_ORDER_RELAXED) == 0);
}

CRUST_APIENTRY void crustSpinLockRelease(Crust_SpinLock *lock)
{
	CRUST_ASSERT(lock != CRUST_NULL);
	crustAtomicStoreU32(&lock->counter, 0, CRUST_MEMORY_ORDER_RELEASE);
}

//
#define CRUST_RWLOCK_WRITE_BIT 0x80000000u
#define CRUST_RWLOCK_PENDING_BIT 0x40000000u
#define CRUST_RWLOCK_READER_MASK 0x3FFFFFFFu
#define CRUST_RWLOCK_WRITER_MASK 0xC0000000u

CRUST_APIENTRY Crust_RwLock crustRwLockInit(void)
{
	Crust_RwLock result;
	result.counter = 0;

	return result;
}

CRUST_APIENTRY void crustRwLockAcquireRead(Crust_RwLock *lock)
{
	CRUST_ASSERT(lock != CRUST_NULL);

	u32 backoff = 1;

	for (;;)
	{
		u32 value = crustAtomicLoadU32(&lock->counter, CRUST_MEMORY_ORDER_RELAXED);

		if ((value & CRUST_RWLOCK_WRITER_MASK) == 0)
		{
			CRUST_ASSERT((value & CRUST_RWLOCK_READER_MASK) != CRUST_RWLOCK_READER_MASK);

			u32 result = crustAtomicCompareAndSwapU32(&lock->counter, value + 1, value, CRUST_MEMORY_ORDER_ACQUIRE, CRUST_MEMORY_ORDER_RELAXED);

			if (result == value)
				break;
		}

		u32 n = backoff;
		while (n--)
			crustCpuRelax();

		if (backoff < 1024)
			backoff *= 2;
	}
}

CRUST_APIENTRY void crustRwLockAcquireWrite(Crust_RwLock *lock)
{
	CRUST_ASSERT(lock != CRUST_NULL);

	u32 backoff = 1;

	for (;;)
	{
		u32 value = crustAtomicLoadU32(&lock->counter, CRUST_MEMORY_ORDER_RELAXED);

		if ((value & CRUST_RWLOCK_WRITER_MASK) == 0)
		{
			u32 new_value = value | CRUST_RWLOCK_PENDING_BIT;
			u32 result = crustAtomicCompareAndSwapU32(&lock->counter, new_value, value, CRUST_MEMORY_ORDER_RELAXED, CRUST_MEMORY_ORDER_RELAXED);

			if (result == value)
				break;
		}

		u32 n = backoff;
		while (n--)
			crustCpuRelax();

		if (backoff < 1024)
			backoff *= 2;
	}

	backoff = 1;

	while (crustAtomicLoadU32(&lock->counter, CRUST_MEMORY_ORDER_RELAXED) != CRUST_RWLOCK_PENDING_BIT)
	{
		u32 n = backoff;
		while (n--)
			crustCpuRelax();

		if (backoff < 1024)
			backoff *= 2;
	}

	u32 result = crustAtomicCompareAndSwapU32(&lock->counter, CRUST_RWLOCK_WRITE_BIT, CRUST_RWLOCK_PENDING_BIT, CRUST_MEMORY_ORDER_ACQUIRE, CRUST_MEMORY_ORDER_RELAXED);
	CRUST_ASSERT(result == CRUST_RWLOCK_PENDING_BIT);
	CRUST_UNUSED(result);
}

CRUST_APIENTRY u8 crustRwLockTryAcquireRead(Crust_RwLock *lock)
{
	CRUST_ASSERT(lock != CRUST_NULL);

	u32 value = crustAtomicLoadU32(&lock->counter, CRUST_MEMORY_ORDER_RELAXED);

	if ((value & CRUST_RWLOCK_WRITER_MASK) != 0)
		return 0;

	if ((value & CRUST_RWLOCK_READER_MASK) == CRUST_RWLOCK_READER_MASK)
		return 0;

	u32 result = crustAtomicCompareAndSwapU32(&lock->counter, value + 1, value, CRUST_MEMORY_ORDER_ACQUIRE, CRUST_MEMORY_ORDER_RELAXED);
	return (result == value);
}

CRUST_APIENTRY u8 crustRwLockTryAcquireWrite(Crust_RwLock *lock)
{
	CRUST_ASSERT(lock != CRUST_NULL);

	u32 result = crustAtomicCompareAndSwapU32(&lock->counter, CRUST_RWLOCK_WRITE_BIT, 0, CRUST_MEMORY_ORDER_ACQUIRE, CRUST_MEMORY_ORDER_RELAXED);
	return (result == 0);
}

CRUST_APIENTRY void crustRwLockReleaseRead(Crust_RwLock *lock)
{
	CRUST_ASSERT(lock != CRUST_NULL);

	u32 value = crustAtomicLoadU32(&lock->counter, CRUST_MEMORY_ORDER_RELAXED);
	CRUST_ASSERT((value & CRUST_RWLOCK_WRITE_BIT) == 0);
	CRUST_ASSERT((value & CRUST_RWLOCK_READER_MASK) != 0);

	crustAtomicDecrementU32(&lock->counter, CRUST_MEMORY_ORDER_RELEASE);
}

CRUST_APIENTRY void crustRwLockReleaseWrite(Crust_RwLock *lock)
{
	CRUST_ASSERT(lock != CRUST_NULL);

	u32 value = crustAtomicLoadU32(&lock->counter, CRUST_MEMORY_ORDER_RELAXED);
	CRUST_ASSERT(value == CRUST_RWLOCK_WRITE_BIT);
	CRUST_UNUSED(value);

	crustAtomicStoreU32(&lock->counter, 0, CRUST_MEMORY_ORDER_RELEASE);
}
