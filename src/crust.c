#include <malloc.h>
#include <string.h>

#define CRUST_IMPLEMENTATION
#include <crust.h>
#undef CRUST_IMPLEMENTATION

typedef enum Crust_MemoryOrder_t
{
	CRUST_MEMORY_ORDER_RELAXED = 0,
	CRUST_MEMORY_ORDER_CONSUME = 1,
	CRUST_MEMORY_ORDER_ACQUIRE = 2,
	CRUST_MEMORY_ORDER_RELEASE = 3,
	CRUST_MEMORY_ORDER_ACQ_REL = 4,
	CRUST_MEMORY_ORDER_SEQ_CST = 5,
} Crust_MemoryOrder;

//
#if defined(_MSC_VER)
	CRUST_APIENTRY void crustMemcpy(void *dst, const void *src, usize size)
	{
		memcpy(dst, src, size);
	}

	CRUST_APIENTRY void crustMemset(void *dst, u8 value, usize size)
	{
		memset(dst, value, size);
	}

	CRUST_APIENTRY void *crustAlignedMalloc(usize size, usize alignment)
	{
		return _aligned_malloc(size, alignment);
	}

	CRUST_APIENTRY void *crustAlignedRealloc(void *ptr, usize size, usize alignment)
	{
		return _aligned_realloc(ptr, size, alignment);
	}

	CRUST_APIENTRY void crustAlignedFree(void *ptr)
	{
		_aligned_free(ptr);
	}

	static CRUST_INLINE u32 crustAtomicSwapU32(volatile u32 *p, u32 v, Crust_MemoryOrder order)
	{
		CRUST_UNUSED(order);
		return (u32)_InterlockedExchange((volatile long *)p, (long)v);
	}

	static CRUST_INLINE u32 crustAtomicCompareAndSwapU32(volatile u32 *p, u32 desired, u32 expected, Crust_MemoryOrder order)
	{
		CRUST_UNUSED(order);
		return (u32)_InterlockedCompareExchange((volatile long *)p, (long)desired, (long)expected);
	}

	static CRUST_INLINE u32 crustAtomicLoadU32(volatile u32 *p, Crust_MemoryOrder order)
	{
		CRUST_ASSERT(order != CRUST_MEMORY_ORDER_RELEASE);
		CRUST_ASSERT(order != CRUST_MEMORY_ORDER_ACQ_REL);

		u32 result = (u32)__iso_volatile_load32((const volatile int *)p);
		if (order != CRUST_MEMORY_ORDER_RELAXED)
			_ReadWriteBarrier();

		return result;
	}

	static CRUST_INLINE void crustAtomicStoreU32(volatile u32 *p, u32 v, Crust_MemoryOrder order)
	{
		CRUST_ASSERT(order != CRUST_MEMORY_ORDER_ACQUIRE);
		CRUST_ASSERT(order != CRUST_MEMORY_ORDER_ACQ_REL);
		CRUST_ASSERT(order != CRUST_MEMORY_ORDER_CONSUME);

		if (order == CRUST_MEMORY_ORDER_SEQ_CST)
		{
			_InterlockedExchange((volatile long *)p, (long)v);
			return;
		}

		if (order != CRUST_MEMORY_ORDER_RELAXED)
			_ReadWriteBarrier();

		__iso_volatile_store32((volatile int *)p, (int)v);
	}

	static CRUST_INLINE u32 crustAtomicIncrementU32(volatile u32 *p, Crust_MemoryOrder order)
	{
		CRUST_UNUSED(order);
		return (u32)_InterlockedIncrement((volatile long *)p);
	}

	static CRUST_INLINE u32 crustAtomicDecrementU32(volatile u32 *p, Crust_MemoryOrder order)
	{
		CRUST_UNUSED(order);
		return (u32)_InterlockedDecrement((volatile long *)p);
	}

	#if defined(_WIN64)
		static CRUST_INLINE u64 crustAtomicSwapU64(volatile u64 *p, u64 v, Crust_MemoryOrder order)
		{
			CRUST_UNUSED(order);
			return (u64)_InterlockedExchange64((volatile long long *)p, (long long)v);
		}

		static CRUST_INLINE u64 crustAtomicCompareAndSwapU64(volatile u64 *p, u64 desired, u64 expected, Crust_MemoryOrder order)
		{
			CRUST_UNUSED(order);
			return (u64)_InterlockedCompareExchange64((volatile long long *)p, (long long)desired, (long long)expected);
		}

		static CRUST_INLINE u64 crustAtomicLoadU64(volatile u64 *p, Crust_MemoryOrder order)
		{
			CRUST_ASSERT(order != CRUST_MEMORY_ORDER_RELEASE);
			CRUST_ASSERT(order != CRUST_MEMORY_ORDER_ACQ_REL);

			u64 result = (u64)__iso_volatile_load64((const volatile long long *)p);
			if (order != CRUST_MEMORY_ORDER_RELAXED)
				_ReadWriteBarrier();

			return result;
		}

		static CRUST_INLINE void crustAtomicStoreU64(volatile u64 *p, u64 v, Crust_MemoryOrder order)
		{
			CRUST_ASSERT(order != CRUST_MEMORY_ORDER_ACQUIRE);
			CRUST_ASSERT(order != CRUST_MEMORY_ORDER_ACQ_REL);
			CRUST_ASSERT(order != CRUST_MEMORY_ORDER_CONSUME);

			if (order == CRUST_MEMORY_ORDER_SEQ_CST)
			{
				_InterlockedExchange64((volatile long long *)p, (long long)v);
				return;
			}

			if (order != CRUST_MEMORY_ORDER_RELAXED)
				_ReadWriteBarrier();

			__iso_volatile_store64((volatile long long *)p, (long long)v);
		}

		static CRUST_INLINE u64 crustAtomicIncrementU64(volatile u64 *p, Crust_MemoryOrder order)
		{
			CRUST_UNUSED(order);
			return (u64)_InterlockedIncrement64((volatile long long *)p);
		}

		static CRUST_INLINE u64 crustAtomicDecrementU64(volatile u64 *p, Crust_MemoryOrder order)
		{
			CRUST_UNUSED(order);
			return (u64)_InterlockedDecrement64((volatile long long *)p);
		}


		#define crustAtomicSwapUSize crustAtomicSwapU64
		#define crustAtomicCompareAndSwapUSize crustAtomicCompareAndSwapU64
		#define crustAtomicLoadUSize crustAtomicLoadU64
		#define crustAtomicStoreUSize crustAtomicStoreU64
		#define crustAtomicIncrementUSize crustAtomicIncrementU64
		#define crustAtomicDecrementUSize crustAtomicDecrementU64
	#else
		#define crustAtomicSwapUSize crustAtomicSwapU32
		#define crustAtomicCompareAndSwapUSize crustAtomicCompareAndSwapU32
		#define crustAtomicLoadUSize crustAtomicLoadU32
		#define crustAtomicStoreUSize crustAtomicStoreU32
		#define crustAtomicIncrementUSize crustAtomicIncrementU32
		#define crustAtomicDecrementUSize crustAtomicDecrementU32
	#endif

	static CRUST_INLINE void crustCpuRelax(void)
	{
		_mm_pause();
	}
#endif

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

		if (crustAtomicCompareAndSwapU32(&lock->counter, 1, 0, CRUST_MEMORY_ORDER_ACQUIRE) == 0)
			break;

		if (backoff < 1024)
			backoff *= 2;
	}
}

CRUST_APIENTRY u8 crustSpinLockTryAcquire(Crust_SpinLock *lock)
{
	CRUST_ASSERT(lock != CRUST_NULL);
	return (crustAtomicCompareAndSwapU32(&lock->counter, 1, 0, CRUST_MEMORY_ORDER_ACQUIRE) == 0);
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

			u32 result = crustAtomicCompareAndSwapU32(&lock->counter, value + 1, value, CRUST_MEMORY_ORDER_ACQUIRE);

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
			u32 result = crustAtomicCompareAndSwapU32(&lock->counter, new_value, value, CRUST_MEMORY_ORDER_RELAXED);

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

	u32 result = crustAtomicCompareAndSwapU32(&lock->counter, CRUST_RWLOCK_WRITE_BIT, CRUST_RWLOCK_PENDING_BIT, CRUST_MEMORY_ORDER_ACQUIRE);
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

	u32 result = crustAtomicCompareAndSwapU32(&lock->counter, value + 1, value, CRUST_MEMORY_ORDER_ACQUIRE);
	return (result == value);
}

CRUST_APIENTRY u8 crustRwLockTryAcquireWrite(Crust_RwLock *lock)
{
	CRUST_ASSERT(lock != CRUST_NULL);

	u32 result = crustAtomicCompareAndSwapU32(&lock->counter, CRUST_RWLOCK_WRITE_BIT, 0, CRUST_MEMORY_ORDER_ACQUIRE);
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

//
CRUST_APIENTRY Crust_RingBuffer crustRingBufferAttach(void *memory, usize capacity)
{
	CRUST_ASSERT(memory != CRUST_NULL);
	CRUST_ASSERT(capacity > 0);

	Crust_RingBuffer result;
	result.memory = memory;
	result.capacity = capacity;
	result.size = 0;
	result.read = 0;
	result.write = 0;

	return result;
}

CRUST_APIENTRY void crustRingBufferReset(Crust_RingBuffer *ring)
{
	CRUST_ASSERT(ring != CRUST_NULL);
	CRUST_ASSERT(ring->memory != CRUST_NULL);

	ring->size = 0;
	ring->read = 0;
	ring->write = 0;
}

CRUST_APIENTRY void crustRingBufferRead(Crust_RingBuffer *ring, void *data, usize size)
{
	CRUST_ASSERT(ring != CRUST_NULL);
	CRUST_ASSERT(ring->memory != CRUST_NULL);
	CRUST_ASSERT(ring->size >= size);
	CRUST_ASSERT(data != CRUST_NULL);

	if (size == 0)
		return;

	usize begin = ring->read;
	usize end = (begin + size) % ring->capacity;

	ring->read = end;
	ring->size -= size;

	u8 *ptr = (u8 *)ring->memory + begin;
	if (begin < end)
	{
		crustMemcpy(data, ptr, size);
	}
	else
	{
		usize remainder = end;
		usize base = size - remainder;
		crustMemcpy(data, ptr, base);
		crustMemcpy((u8 *)data + base, ring->memory, remainder);
	}

}

CRUST_APIENTRY void crustRingBufferWrite(Crust_RingBuffer *ring, const void *data, usize size)
{
	CRUST_ASSERT(ring != CRUST_NULL);
	CRUST_ASSERT(ring->memory != CRUST_NULL);
	CRUST_ASSERT(ring->size + size <= ring->capacity);
	CRUST_ASSERT(data != CRUST_NULL);

	if (size == 0)
		return;

	usize begin = ring->write;
	usize end = (begin + size) % ring->capacity;

	ring->write = end;
	ring->size += size;

	u8 *ptr = (u8 *)ring->memory + begin;
	if (begin < end)
	{
		crustMemcpy(ptr, data, size);
	}
	else
	{
		usize remainder = end;
		usize base = size - remainder;
		crustMemcpy(ptr, data, base);
		crustMemcpy(ring->memory, (const u8 *)data + base, remainder);
	}
}

//
CRUST_APIENTRY Crust_BipBuffer crustBipBufferAttach(void *memory, usize capacity)
{
	CRUST_ASSERT(memory != CRUST_NULL);
	CRUST_ASSERT(capacity > 0);

	Crust_BipBuffer result;
	result.memory = memory;
	result.capacity = capacity;

	for (usize i = 0; i < 2; ++i)
	{
		result.begin[i] = 0;
		result.end[i] = 0;
	}

	result.read_staged = 0;
	result.write_staged = 0;
	result.reader = 0;
	result.writer = 0;

	return result;
}

CRUST_APIENTRY void crustBipBufferReset(Crust_BipBuffer *bip)
{
	CRUST_ASSERT(bip != CRUST_NULL);
	CRUST_ASSERT(bip->memory != CRUST_NULL);

	for (usize i = 0; i < 2; ++i)
	{
		bip->begin[i] = 0;
		bip->end[i] = 0;
	}

	bip->read_staged = 0;
	bip->write_staged = 0;
	bip->reader = 0;
	bip->writer = 0;
}

CRUST_APIENTRY const void *crustBipBufferStageRead(Crust_BipBuffer *bip, usize size)
{
	CRUST_ASSERT(bip != CRUST_NULL);
	CRUST_ASSERT(bip->memory != CRUST_NULL);
	CRUST_ASSERT(bip->read_staged == 0);
	CRUST_ASSERT(size > 0);

	usize r = bip->reader;

	usize begin = bip->begin[r];
	usize end = bip->end[r];

	if (begin != end)
	{
		if (end - begin < size)
			return CRUST_NULL;

		bip->read_staged = size;
		return (const u8 *)bip->memory + begin;
	}

	usize w = bip->writer;

	if (r == w)
		return CRUST_NULL;

	usize next = (r + 1) % 2;
	CRUST_ASSERT(next == w);

	end = bip->end[next];

	if (end < size)
		return CRUST_NULL;

	bip->begin[next] = 0;
	bip->reader = next;

	bip->read_staged = size;
	return bip->memory;
}

CRUST_APIENTRY void crustBipBufferCommitRead(Crust_BipBuffer *bip, usize size)
{
	CRUST_ASSERT(bip != CRUST_NULL);
	CRUST_ASSERT(bip->memory != CRUST_NULL);
	CRUST_ASSERT(bip->read_staged != 0);
	CRUST_ASSERT(bip->read_staged >= size);
	CRUST_ASSERT(size > 0);

	usize r = bip->reader;
	usize begin = bip->begin[r];

	bip->begin[r] = begin + size;
	bip->read_staged = 0;
}

CRUST_APIENTRY void *crustBipBufferStageWrite(Crust_BipBuffer *bip, usize size)
{
	CRUST_ASSERT(bip != CRUST_NULL);
	CRUST_ASSERT(bip->memory != CRUST_NULL);
	CRUST_ASSERT(bip->write_staged == 0);
	CRUST_ASSERT(size > 0);

	usize r = bip->reader;
	usize w = bip->writer;

	usize read_begin = bip->begin[r];
	usize read_end = bip->end[r];
	usize write_end = bip->end[w];

	if (r == w)
	{
		if (bip->capacity - write_end >= size)
		{
			bip->write_staged = size;
			return (u8 *)bip->memory + write_end;
		}

		usize available = (read_begin == read_end) ? bip->capacity : read_begin;

		if (available < size)
			return CRUST_NULL;

		usize next = (w + 1) % 2;

		usize next_begin = bip->begin[next];
		usize next_end = bip->end[next];

		CRUST_ASSERT(next_begin == next_end);
		CRUST_UNUSED(next_begin);
		CRUST_UNUSED(next_end);

		bip->end[next] = 0;
		bip->writer = next;

		bip->write_staged = size;
		return bip->memory;
	}

	usize available = (read_begin == read_end) ? bip->capacity : read_begin;
	
	CRUST_ASSERT(available >= write_end);
	available -= write_end;

	if (available < size)
		return CRUST_NULL;

	bip->write_staged = size;
	return (u8 *)bip->memory + write_end;
}

CRUST_APIENTRY void crustBipBufferCommitWrite(Crust_BipBuffer *bip, usize size)
{
	CRUST_ASSERT(bip != CRUST_NULL);
	CRUST_ASSERT(bip->memory != CRUST_NULL);
	CRUST_ASSERT(bip->write_staged != 0);
	CRUST_ASSERT(bip->write_staged >= size);
	CRUST_ASSERT(size > 0);

	usize w = bip->writer;
	usize end = bip->end[w];

	bip->end[w] = end + size;
	bip->write_staged = 0;
}

//
CRUST_APIENTRY Crust_Stack crustStackAttach(void *memory, usize capacity)
{
	CRUST_ASSERT(memory != CRUST_NULL);
	CRUST_ASSERT(capacity > 0);

	Crust_Stack result;
	result.memory = memory;
	result.capacity = capacity;
	result.head = 0;

	return result;
}

CRUST_APIENTRY void crustStackReset(Crust_Stack *stack)
{
	CRUST_ASSERT(stack != CRUST_NULL);
	CRUST_ASSERT(stack->memory != CRUST_NULL);

	stack->head = 0;
}

CRUST_APIENTRY void crustStackPush(Crust_Stack *stack, const void *data, usize size)
{
	CRUST_ASSERT(stack != CRUST_NULL);
	CRUST_ASSERT(stack->memory != CRUST_NULL);
	CRUST_ASSERT(stack->head + size <= stack->capacity);
	CRUST_ASSERT(data != CRUST_NULL);

	if (size == 0)
		return;

	u8 *ptr = (u8 *)stack->memory + stack->head;
	crustMemcpy(ptr, data, size);

	stack->head += size;
}

CRUST_APIENTRY void crustStackPeek(const Crust_Stack *stack, void *data, usize size)
{
	CRUST_ASSERT(stack != CRUST_NULL);
	CRUST_ASSERT(stack->memory != CRUST_NULL);
	CRUST_ASSERT(stack->head >= size);
	CRUST_ASSERT(data != CRUST_NULL);

	usize offset = stack->head - size;

	const u8 *ptr = (const u8 *)stack->memory + offset;
	crustMemcpy(data, ptr, size);
}

CRUST_APIENTRY void crustStackPop(Crust_Stack *stack, void *data, usize size)
{
	CRUST_ASSERT(stack != CRUST_NULL);
	CRUST_ASSERT(stack->memory != CRUST_NULL);
	CRUST_ASSERT(stack->head >= size);
	CRUST_ASSERT(data != CRUST_NULL);

	stack->head -= size;

	u8 *ptr = (u8 *)stack->memory + stack->head;
	crustMemcpy(data, ptr, size);
}

//
CRUST_APIENTRY Crust_Arena crustArenaAttach(void *memory, usize capacity)
{
	CRUST_ASSERT(memory != CRUST_NULL);
	CRUST_ASSERT(capacity > 0);

	Crust_Arena result;
	result.memory = memory;
	result.capacity = capacity;
	result.size = 0;

	return result;
}

CRUST_APIENTRY void crustArenaReset(Crust_Arena *arena)
{
	CRUST_ASSERT(arena != CRUST_NULL);
	CRUST_ASSERT(arena->memory != CRUST_NULL);

	arena->size = 0;
}

CRUST_APIENTRY void *crustArenaAlloc(Crust_Arena *arena, usize size, usize alignment)
{
	CRUST_ASSERT(arena != CRUST_NULL);
	CRUST_ASSERT(arena->memory != CRUST_NULL);
	CRUST_ASSERT(alignment > 0);
	CRUST_ASSERT(crustIsPow2USize(alignment));

	u8 *base = (u8 *)arena->memory;
	u8 *current = base + arena->size;
	u8 *aligned = (u8 *)crustAlignUpUSize((usize)current, alignment);

	usize offset = (usize)(aligned - base);

	if (offset > arena->capacity)
		return CRUST_NULL;

	if (size > arena->capacity - offset)
		return CRUST_NULL;

	arena->size = offset + size;
	return (void *)aligned;
}

//
CRUST_APIENTRY Crust_SpscRingBuffer crustSpscRingBufferAttach(void *memory, usize capacity)
{
	CRUST_ASSERT(memory != CRUST_NULL);
	CRUST_ASSERT(capacity > 0);
	CRUST_ASSERT(crustIsPow2USize(capacity) != 0);

	Crust_SpscRingBuffer result;
	result.memory = memory;
	result.capacity = capacity;
	result.mask = capacity - 1;
	result.read = 0;
	result.write = 0;

	return result;
}

CRUST_APIENTRY void crustSpscRingBufferReset(Crust_SpscRingBuffer *ring)
{
	CRUST_ASSERT(ring != CRUST_NULL);
	CRUST_ASSERT(ring->memory != CRUST_NULL);

	crustAtomicStoreUSize(&ring->read, 0, CRUST_MEMORY_ORDER_RELAXED);
	crustAtomicStoreUSize(&ring->write, 0, CRUST_MEMORY_ORDER_RELAXED);
}

CRUST_APIENTRY usize crustSpscRingBufferRead(Crust_SpscRingBuffer *ring, void *data, usize size)
{
	CRUST_ASSERT(ring != CRUST_NULL);
	CRUST_ASSERT(ring->memory != CRUST_NULL);
	CRUST_ASSERT(data != CRUST_NULL);

	if (size == 0)
		return 0;

	usize r = crustAtomicLoadUSize(&ring->read, CRUST_MEMORY_ORDER_RELAXED);
	usize w = crustAtomicLoadUSize(&ring->write, CRUST_MEMORY_ORDER_ACQUIRE);

	usize used = w - r;
	CRUST_ASSERT(used <= ring->capacity);

	if (used == 0)
		return 0;

	size = crustMinUSize(size, used);

	usize begin = r & ring->mask;
	usize end = (begin + size) & ring->mask;

	const u8 *ptr = (const u8 *)ring->memory + begin;
	if (begin < end)
	{
		crustMemcpy(data, ptr, size);
	}
	else
	{
		usize remainder = end;
		usize base = size - remainder;
		crustMemcpy(data, ptr, base);
		crustMemcpy((u8 *)data + base, ring->memory, remainder);
	}

	crustAtomicStoreUSize(&ring->read, r + size, CRUST_MEMORY_ORDER_RELEASE);
	return size;
}

CRUST_APIENTRY usize crustSpscRingBufferWrite(Crust_SpscRingBuffer *ring, const void *data, usize size)
{
	CRUST_ASSERT(ring != CRUST_NULL);
	CRUST_ASSERT(ring->memory != CRUST_NULL);
	CRUST_ASSERT(data != CRUST_NULL);

	if (size == 0)
		return 0;

	usize r = crustAtomicLoadUSize(&ring->read, CRUST_MEMORY_ORDER_ACQUIRE);
	usize w = crustAtomicLoadUSize(&ring->write, CRUST_MEMORY_ORDER_RELAXED);

	usize used = w - r;
	CRUST_ASSERT(used <= ring->capacity);

	usize free = ring->capacity - used;

	if (free == 0)
		return 0;

	size = crustMinUSize(size, free);

	usize begin = w & ring->mask;
	usize end = (begin + size) & ring->mask;

	u8 *ptr = (u8 *)ring->memory + begin;
	if (begin < end)
	{
		crustMemcpy(ptr, data, size);
	}
	else
	{
		usize remainder = end;
		usize base = size - remainder;
		crustMemcpy(ptr, data, base);
		crustMemcpy(ring->memory, (const u8 *)data + base, remainder);
	}

	crustAtomicStoreUSize(&ring->write, w + size, CRUST_MEMORY_ORDER_RELEASE);
	return size;
}

//
CRUST_APIENTRY Crust_SpscBipBuffer crustSpscBipBufferAttach(void *memory, usize capacity)
{
	CRUST_ASSERT(memory != CRUST_NULL);
	CRUST_ASSERT(capacity > 0);

	Crust_SpscBipBuffer result;
	result.memory = memory;
	result.capacity = capacity;

	for (usize i = 0; i < 2; ++i)
	{
		result.begin[i] = 0;
		result.end[i] = 0;
	}

	result.read_staged = 0;
	result.write_staged = 0;
	result.reader = 0;
	result.writer = 0;

	return result;
}

CRUST_APIENTRY void crustSpscBipBufferReset(Crust_SpscBipBuffer *bip)
{
	CRUST_ASSERT(bip != CRUST_NULL);
	CRUST_ASSERT(bip->memory != CRUST_NULL);

	for (usize i = 0; i < 2; ++i)
	{
		crustAtomicStoreUSize(&bip->begin[i], 0, CRUST_MEMORY_ORDER_RELAXED);
		crustAtomicStoreUSize(&bip->end[i], 0, CRUST_MEMORY_ORDER_RELAXED);
	}

	crustAtomicStoreUSize(&bip->reader, 0, CRUST_MEMORY_ORDER_RELAXED);
	crustAtomicStoreUSize(&bip->writer, 0, CRUST_MEMORY_ORDER_RELAXED);

	bip->read_staged = 0;
	bip->write_staged = 0;
}

CRUST_APIENTRY const void *crustSpscBipBufferStageRead(Crust_SpscBipBuffer *bip, usize size)
{
	CRUST_ASSERT(bip != CRUST_NULL);
	CRUST_ASSERT(bip->memory != CRUST_NULL);
	CRUST_ASSERT(bip->read_staged == 0);
	CRUST_ASSERT(size > 0);

	usize r = crustAtomicLoadUSize(&bip->reader, CRUST_MEMORY_ORDER_RELAXED);

	usize begin = crustAtomicLoadUSize(&bip->begin[r], CRUST_MEMORY_ORDER_RELAXED);
	usize end = crustAtomicLoadUSize(&bip->end[r], CRUST_MEMORY_ORDER_ACQUIRE);

	if (begin != end)
	{
		if (end - begin < size)
			return CRUST_NULL;

		bip->read_staged = size;
		return (const u8 *)bip->memory + begin;
	}

	usize w = crustAtomicLoadUSize(&bip->writer, CRUST_MEMORY_ORDER_ACQUIRE);

	if (r == w)
		return CRUST_NULL;

	usize next = (r + 1) % 2;
	CRUST_ASSERT(next == w);

	end = crustAtomicLoadUSize(&bip->end[next], CRUST_MEMORY_ORDER_ACQUIRE);

	if (end < size)
		return CRUST_NULL;

	crustAtomicStoreUSize(&bip->begin[next], 0, CRUST_MEMORY_ORDER_RELAXED);
	crustAtomicStoreUSize(&bip->reader, next, CRUST_MEMORY_ORDER_RELEASE);

	bip->read_staged = size;
	return bip->memory;
}

CRUST_APIENTRY void crustSpscBipBufferCommitRead(Crust_SpscBipBuffer *bip, usize size)
{
	CRUST_ASSERT(bip != CRUST_NULL);
	CRUST_ASSERT(bip->memory != CRUST_NULL);
	CRUST_ASSERT(bip->read_staged != 0);
	CRUST_ASSERT(bip->read_staged >= size);
	CRUST_ASSERT(size > 0);

	usize r = crustAtomicLoadUSize(&bip->reader, CRUST_MEMORY_ORDER_RELAXED);
	usize begin = crustAtomicLoadUSize(&bip->begin[r], CRUST_MEMORY_ORDER_RELAXED);

	crustAtomicStoreUSize(&bip->begin[r], begin + size, CRUST_MEMORY_ORDER_RELEASE);
	bip->read_staged = 0;
}

CRUST_APIENTRY void *crustSpscBipBufferStageWrite(Crust_SpscBipBuffer *bip, usize size)
{
	CRUST_ASSERT(bip != CRUST_NULL);
	CRUST_ASSERT(bip->memory != CRUST_NULL);
	CRUST_ASSERT(bip->write_staged == 0);
	CRUST_ASSERT(size > 0);

	usize r = crustAtomicLoadUSize(&bip->reader, CRUST_MEMORY_ORDER_ACQUIRE);
	usize w = crustAtomicLoadUSize(&bip->writer, CRUST_MEMORY_ORDER_RELAXED);

	usize read_begin = crustAtomicLoadUSize(&bip->begin[r], CRUST_MEMORY_ORDER_ACQUIRE);
	usize read_end = crustAtomicLoadUSize(&bip->end[r], CRUST_MEMORY_ORDER_RELAXED);
	usize write_end = crustAtomicLoadUSize(&bip->end[w], CRUST_MEMORY_ORDER_RELAXED);

	if (r == w)
	{
		if (bip->capacity - write_end >= size)
		{
			bip->write_staged = size;
			return (u8 *)bip->memory + write_end;
		}

		usize available = (read_begin == read_end) ? bip->capacity : read_begin;

		if (available < size)
			return CRUST_NULL;

		usize next = (w + 1) % 2;

		usize next_begin = crustAtomicLoadUSize(&bip->begin[next], CRUST_MEMORY_ORDER_ACQUIRE);
		usize next_end = crustAtomicLoadUSize(&bip->end[next], CRUST_MEMORY_ORDER_RELAXED);

		CRUST_ASSERT(next_begin == next_end);
		CRUST_UNUSED(next_begin);
		CRUST_UNUSED(next_end);

		crustAtomicStoreUSize(&bip->end[next], 0, CRUST_MEMORY_ORDER_RELAXED);
		crustAtomicStoreUSize(&bip->writer, next, CRUST_MEMORY_ORDER_RELEASE);

		bip->write_staged = size;
		return bip->memory;
	}

	usize available = (read_begin == read_end) ? bip->capacity : read_begin;

	CRUST_ASSERT(available >= write_end);
	available -= write_end;

	if (available < size)
		return CRUST_NULL;

	bip->write_staged = size;
	return (u8 *)bip->memory + write_end;
}

CRUST_APIENTRY void crustSpscBipBufferCommitWrite(Crust_SpscBipBuffer *bip, usize size)
{
	CRUST_ASSERT(bip != CRUST_NULL);
	CRUST_ASSERT(bip->memory != CRUST_NULL);
	CRUST_ASSERT(bip->write_staged != 0);
	CRUST_ASSERT(bip->write_staged >= size);
	CRUST_ASSERT(size > 0);

	usize w = crustAtomicLoadUSize(&bip->writer, CRUST_MEMORY_ORDER_RELAXED);
	usize end = crustAtomicLoadUSize(&bip->end[w], CRUST_MEMORY_ORDER_RELAXED);

	crustAtomicStoreUSize(&bip->end[w], end + size, CRUST_MEMORY_ORDER_RELEASE);
	bip->write_staged = 0;
}

//
CRUST_APIENTRY void *crustAllocatorAlloc(Crust_Allocator allocator, usize size, usize alignment)
{
	CRUST_ASSERT(allocator.vtbl != CRUST_NULL);
	CRUST_ASSERT(allocator.vtbl->alloc != CRUST_NULL);

	CRUST_ASSERT(size > 0);
	CRUST_ASSERT(alignment > 0);
	CRUST_ASSERT(crustIsPow2USize(alignment));

	return allocator.vtbl->alloc(allocator.context, size, alignment);
}

CRUST_APIENTRY void *crustAllocatorRealloc(Crust_Allocator allocator, void *ptr, usize old_size, usize new_size, usize alignment)
{
	CRUST_ASSERT(allocator.vtbl != CRUST_NULL);
	CRUST_ASSERT(allocator.vtbl->alloc != CRUST_NULL);
	CRUST_ASSERT(allocator.vtbl->realloc != CRUST_NULL);
	CRUST_ASSERT(allocator.vtbl->free != CRUST_NULL);

	CRUST_ASSERT(alignment > 0);
	CRUST_ASSERT(crustIsPow2USize(alignment));
	
	if (ptr == CRUST_NULL)
	{
		CRUST_ASSERT(old_size == 0);

		if (new_size == 0)
			return CRUST_NULL;

		return allocator.vtbl->alloc(allocator.context, new_size, alignment);
	}

	CRUST_ASSERT(old_size > 0);

	if (new_size == 0)
	{
		allocator.vtbl->free(allocator.context, ptr, old_size, alignment);
		return CRUST_NULL;
	}

	return allocator.vtbl->realloc(allocator.context, ptr, old_size, new_size, alignment);
}

CRUST_APIENTRY void crustAllocatorFree(Crust_Allocator allocator, void *ptr, usize size, usize alignment)
{
	CRUST_ASSERT(allocator.vtbl != CRUST_NULL);
	CRUST_ASSERT(allocator.vtbl->free != CRUST_NULL);

	if (ptr == CRUST_NULL)
		return;

	CRUST_ASSERT(size > 0);
	CRUST_ASSERT(alignment > 0);
	CRUST_ASSERT(crustIsPow2USize(alignment));

	allocator.vtbl->free(allocator.context, ptr, size, alignment);
}

//
//
static void *crustSystemAllocatorAlloc(void *context, usize size, usize alignment)
{
	CRUST_UNUSED(context);

	return crustAlignedMalloc(size, alignment);
}

static void *crustSystemAllocatorRealloc(void *context, void *ptr, usize old_size, usize new_size, usize alignment)
{
	CRUST_UNUSED(context);
	CRUST_UNUSED(old_size);

	return crustAlignedRealloc(ptr, new_size, alignment);
}

static void crustSystemAllocatorFree(void *context, void *ptr, usize size, usize alignment)
{
	CRUST_UNUSED(context);
	CRUST_UNUSED(size);
	CRUST_UNUSED(alignment);

	crustAlignedFree(ptr);
}

static const Crust_AllocatorVtbl crustSystemAllocatorVtbl =
{
	crustSystemAllocatorAlloc,
	crustSystemAllocatorRealloc,
	crustSystemAllocatorFree,
};

CRUST_APIENTRY Crust_Allocator crustSystemAllocatorInit(void)
{
	Crust_Allocator result;
	result.context = CRUST_NULL;
	result.vtbl = &crustSystemAllocatorVtbl;

	return result;
}

//
static void *crustArenaAllocatorAlloc(void *context, usize size, usize alignment)
{
	return crustArenaAlloc((Crust_Arena *)context, size, alignment);
}

static void *crustArenaAllocatorRealloc(void *context, void *ptr, usize old_size, usize new_size, usize alignment)
{
	Crust_Arena *arena = (Crust_Arena *)context;
	CRUST_ASSERT(arena != CRUST_NULL);
	CRUST_ASSERT(arena->memory != CRUST_NULL);

	u8 *base = (u8 *)arena->memory;

	u8 *end = base + arena->size;
	u8 *allocation_end = (u8 *)ptr + old_size;

	if (allocation_end == end)
	{
		usize offset = (usize)((u8 *)ptr - base);

		if (new_size <= arena->capacity - offset)
		{
			arena->size = offset + new_size;
			return ptr;
		}

		return CRUST_NULL;
	}

	void *new_ptr = crustArenaAlloc(arena, new_size, alignment);
	if (new_ptr == CRUST_NULL)
		return CRUST_NULL;

	crustMemcpy(new_ptr, ptr, crustMinUSize(old_size, new_size));
	return new_ptr;
}

static void crustArenaAllocatorFree(void *context, void *ptr, usize size, usize alignment)
{
	CRUST_UNUSED(context);
	CRUST_UNUSED(ptr);
	CRUST_UNUSED(size);
	CRUST_UNUSED(alignment);
}

static const Crust_AllocatorVtbl crustArenaAllocatorVtbl =
{
	crustArenaAllocatorAlloc,
	crustArenaAllocatorRealloc,
	crustArenaAllocatorFree,
};

CRUST_APIENTRY Crust_Allocator crustArenaAllocatorInit(Crust_Arena *arena)
{
	CRUST_ASSERT(arena != CRUST_NULL);
	CRUST_ASSERT(arena->memory != CRUST_NULL);

	Crust_Allocator result;
	result.context = arena;
	result.vtbl = &crustArenaAllocatorVtbl;

	return result;
}

//
typedef struct Crust_HashIndexMapProbe_t
{
	usize bucket;
	u8 found;
} Crust_HashIndexMapProbe;

static u8 crustHashIndexMapCheckMask(const Crust_HashIndexMap *hash_map, usize bucket)
{
	CRUST_ASSERT(hash_map != CRUST_NULL);
	CRUST_ASSERT(bucket < hash_map->capacity);

	usize mask_index = bucket / 32;
	u32 bit_mask = (1u << (bucket % 32));

	return (hash_map->masks[mask_index] & bit_mask) != 0;
}

static void crustHashIndexMapAddMask(Crust_HashIndexMap *hash_map, usize bucket)
{
	CRUST_ASSERT(hash_map != CRUST_NULL);
	CRUST_ASSERT(bucket < hash_map->capacity);

	usize mask_index = bucket / 32;
	u32 bit_mask = (1u << (bucket % 32));

	hash_map->masks[mask_index] |= bit_mask;
}

static void crustHashIndexMapRemoveMask(Crust_HashIndexMap *hash_map, usize bucket)
{
	CRUST_ASSERT(hash_map != CRUST_NULL);
	CRUST_ASSERT(bucket < hash_map->capacity);

	usize mask_index = bucket / 32;
	u32 bit_mask = (1u << (bucket % 32));

	hash_map->masks[mask_index] &= ~bit_mask;
}

static u8 *crustHashIndexMapKey(Crust_HashIndexMap *hash_map, usize bucket)
{
	CRUST_ASSERT(hash_map != CRUST_NULL);
	CRUST_ASSERT(bucket < hash_map->capacity);

	return hash_map->keys + hash_map->key_stride * bucket;
}

static const u8 *crustHashIndexMapConstKey(const Crust_HashIndexMap *hash_map, usize bucket)
{
	CRUST_ASSERT(hash_map != CRUST_NULL);
	CRUST_ASSERT(bucket < hash_map->capacity);

	return hash_map->keys + hash_map->key_stride * bucket;
}

static void crustHashIndexMapFreeStorage(Crust_Allocator allocator, Crust_HashIndexMap *hash_map)
{
	CRUST_ASSERT(hash_map != CRUST_NULL);

	usize masks_capacity = crustAlignUpUSize(hash_map->capacity, 32) / 32;

	crustAllocatorFree(allocator, hash_map->keys, hash_map->key_stride * hash_map->capacity, hash_map->key_alignment);
	crustFreeArray(allocator, hash_map->hashes, usize, hash_map->capacity);
	crustFreeArray(allocator, hash_map->indices, usize, hash_map->capacity);
	crustFreeArray(allocator, hash_map->masks, u32, masks_capacity);

	hash_map->keys = CRUST_NULL;
	hash_map->hashes = CRUST_NULL;
	hash_map->indices = CRUST_NULL;
	hash_map->masks = CRUST_NULL;

	hash_map->capacity = 0;
	hash_map->count = 0;
}

static Crust_HashIndexMapProbe crustHashIndexMapProbeKey(const Crust_HashIndexMap *hash_map, const void *key, usize key_size, usize hash)
{
	CRUST_ASSERT(hash_map != CRUST_NULL);
	CRUST_ASSERT(hash_map->capacity > 0);

	CRUST_ASSERT(key != CRUST_NULL);
	CRUST_ASSERT(key_size == hash_map->key_size);

	usize bucket = hash % hash_map->capacity;

	for (usize probe = 0; probe < hash_map->capacity; ++probe)
	{
		if (crustHashIndexMapCheckMask(hash_map, bucket) == 0)
		{
			Crust_HashIndexMapProbe result = { bucket, 0 };
			return result;
		}

		if (hash_map->hashes[bucket] == hash)
		{
			const u8 *stored_key = crustHashIndexMapConstKey(hash_map, bucket);

			if (hash_map->vtbl->equal(hash_map->context, stored_key, key, key_size))
			{
				Crust_HashIndexMapProbe result = { bucket, 1 };
				return result;
			}
		}

		bucket = (bucket + 1) % hash_map->capacity;
	}

	Crust_HashIndexMapProbe result = { USIZE_MAX, 0 };
	return result;
}

static usize crustHashIndexMapFindFreeBucket(const Crust_HashIndexMap *hash_map, usize hash)
{
	CRUST_ASSERT(hash_map != CRUST_NULL);
	CRUST_ASSERT(hash_map->capacity > 0);
	CRUST_ASSERT(hash_map->count < hash_map->capacity);

	usize bucket = hash % hash_map->capacity;

	for (usize probe = 0; probe < hash_map->capacity; ++probe)
	{
		if (crustHashIndexMapCheckMask(hash_map, bucket) == 0)
			return bucket;

		bucket = (bucket + 1) % hash_map->capacity;
	}

	CRUST_ASSERT(0);
	return USIZE_MAX;
}

CRUST_APIENTRY Crust_HashIndexMap crustHashIndexMapInit(void *context, const Crust_HashIndexMapVtbl *vtbl, usize key_size, usize key_alignment)
{
	CRUST_ASSERT(vtbl != CRUST_NULL);
	CRUST_ASSERT(vtbl->hash != CRUST_NULL);
	CRUST_ASSERT(vtbl->equal != CRUST_NULL);
	CRUST_ASSERT(vtbl->copy != CRUST_NULL);
	CRUST_ASSERT(vtbl->move != CRUST_NULL);
	CRUST_ASSERT(vtbl->destroy != CRUST_NULL);

	CRUST_ASSERT(key_size > 0);
	CRUST_ASSERT(key_alignment > 0);
	CRUST_ASSERT(crustIsPow2USize(key_alignment) != 0);

	Crust_HashIndexMap result;
	result.context = context;
	result.vtbl = vtbl;

	result.keys = CRUST_NULL;
	result.indices = CRUST_NULL;
	result.hashes = CRUST_NULL;
	result.masks = CRUST_NULL;

	result.count = 0;
	result.capacity = 0;
	result.key_size = key_size;
	result.key_alignment = key_alignment;
	result.key_stride = crustAlignUpUSize(key_size, key_alignment);

	return result;
}

CRUST_APIENTRY void crustHashIndexMapAlloc(Crust_Allocator allocator, Crust_HashIndexMap *hash_map, usize capacity)
{
	CRUST_ASSERT(hash_map != CRUST_NULL);
	CRUST_ASSERT(hash_map->capacity == 0);

	CRUST_ASSERT(capacity > 0);

	usize masks_capacity = crustAlignUpUSize(capacity, 32) / 32;

	hash_map->keys = (u8 *)crustAllocatorAlloc(allocator, hash_map->key_stride * capacity, hash_map->key_alignment);
	hash_map->hashes = crustAllocArray(allocator, usize, capacity);
	hash_map->indices = crustAllocArray(allocator, usize, capacity);
	hash_map->masks = crustAllocArray(allocator, u32, masks_capacity);
	hash_map->capacity = capacity;
	hash_map->count = 0;

	for (usize i = 0; i < masks_capacity; ++i)
		hash_map->masks[i] = 0;
}

CRUST_APIENTRY void crustHashIndexMapFree(Crust_Allocator allocator, Crust_HashIndexMap *hash_map)
{
	CRUST_ASSERT(hash_map != CRUST_NULL);

	for (usize i = 0; i < hash_map->capacity; ++i)
	{
		if (crustHashIndexMapCheckMask(hash_map, i) == 0)
			continue;

		u8 *key = crustHashIndexMapKey(hash_map, i);
		hash_map->vtbl->destroy(hash_map->context, key, hash_map->key_size);
	}

	crustHashIndexMapFreeStorage(allocator, hash_map);
}

CRUST_APIENTRY void crustHashIndexMapGrow(Crust_Allocator allocator, Crust_HashIndexMap *hash_map, usize new_capacity)
{
	CRUST_ASSERT(hash_map != CRUST_NULL);

	CRUST_ASSERT(new_capacity > hash_map->capacity);

	Crust_HashIndexMap new_hash_map = crustHashIndexMapInit(hash_map->context, hash_map->vtbl, hash_map->key_size, hash_map->key_alignment);
	crustHashIndexMapAlloc(allocator, &new_hash_map, new_capacity);
	
	for (usize i = 0; i < hash_map->capacity; ++i)
	{
		if (crustHashIndexMapCheckMask(hash_map, i) == 0)
			continue;

		usize hash = hash_map->hashes[i];
		usize index = hash_map->indices[i];

		usize new_bucket = crustHashIndexMapFindFreeBucket(&new_hash_map, hash);

		u8 *dst_key = crustHashIndexMapKey(&new_hash_map, new_bucket);
		u8 *src_key = crustHashIndexMapKey(hash_map, i);

		hash_map->vtbl->move(hash_map->context, dst_key, src_key, hash_map->key_size);

		new_hash_map.hashes[new_bucket] = hash;
		new_hash_map.indices[new_bucket] = index;
		
		crustHashIndexMapAddMask(&new_hash_map, new_bucket);
		new_hash_map.count++;
	}

	crustHashIndexMapFreeStorage(allocator, hash_map);
	*hash_map = new_hash_map;
}

CRUST_APIENTRY usize crustHashIndexMapFind(const Crust_HashIndexMap *hash_map, const void *key, usize key_size)
{
	CRUST_ASSERT(hash_map != CRUST_NULL);

	CRUST_ASSERT(key != CRUST_NULL);
	CRUST_ASSERT(key_size == hash_map->key_size);

	if (hash_map->count == 0)
		return USIZE_MAX;

	usize hash = hash_map->vtbl->hash(hash_map->context, key, key_size);
	Crust_HashIndexMapProbe probe = crustHashIndexMapProbeKey(hash_map, key, key_size, hash);

	if (probe.found == 0)
		return USIZE_MAX;

	return hash_map->indices[probe.bucket];
}

CRUST_APIENTRY usize crustHashIndexMapAdd(Crust_HashIndexMap *hash_map, const void *key, usize key_size, usize index)
{
	CRUST_ASSERT(hash_map != CRUST_NULL);
	CRUST_ASSERT(hash_map->capacity > 0);

	CRUST_ASSERT(key != CRUST_NULL);
	CRUST_ASSERT(key_size == hash_map->key_size);

	CRUST_ASSERT(index != USIZE_MAX);

	usize hash = hash_map->vtbl->hash(hash_map->context, key, key_size);

	Crust_HashIndexMapProbe probe = crustHashIndexMapProbeKey(hash_map, key, key_size, hash);

	if (probe.found)
	{
		usize stored_index = hash_map->indices[probe.bucket];
		CRUST_ASSERT(stored_index == index);

		return stored_index;
	}

	if (probe.bucket == USIZE_MAX)
		return USIZE_MAX;

	u8 *stored_key = crustHashIndexMapKey(hash_map, probe.bucket);

	hash_map->vtbl->copy(hash_map->context, stored_key, key, key_size);

	hash_map->hashes[probe.bucket] = hash;
	hash_map->indices[probe.bucket] = index;

	crustHashIndexMapAddMask(hash_map, probe.bucket);
	hash_map->count++;

	return index;
}

CRUST_APIENTRY usize crustHashIndexMapRemove(Crust_HashIndexMap *hash_map, const void *key, usize key_size)
{
	CRUST_ASSERT(hash_map != CRUST_NULL);

	CRUST_ASSERT(key != CRUST_NULL);
	CRUST_ASSERT(key_size == hash_map->key_size);

	if (hash_map->count == 0)
		return USIZE_MAX;

	usize hash = hash_map->vtbl->hash(hash_map->context, key, key_size);
	Crust_HashIndexMapProbe probe = crustHashIndexMapProbeKey(hash_map, key, key_size, hash);

	if (probe.found == 0)
		return USIZE_MAX;

	usize removed_index = hash_map->indices[probe.bucket];
	u8 *stored_key = crustHashIndexMapKey(hash_map, probe.bucket);

	hash_map->vtbl->destroy(hash_map->context, stored_key, hash_map->key_size);

	usize hole = probe.bucket;
	usize scan = (hole + 1) % hash_map->capacity;

	for (usize shift_probe = 0; shift_probe < hash_map->capacity - 1; ++shift_probe)
	{
		if (crustHashIndexMapCheckMask(hash_map, scan) == 0)
			break;

		usize scan_hash = hash_map->hashes[scan];
		usize home = scan_hash % hash_map->capacity;

		usize scan_distance = (scan + hash_map->capacity - home) % hash_map->capacity;
		usize hole_distance = (hole + hash_map->capacity - home) % hash_map->capacity;

		if (hole_distance < scan_distance)
		{
			u8 *dst_key = crustHashIndexMapKey(hash_map, hole);
			u8 *src_key = crustHashIndexMapKey(hash_map, scan);

			hash_map->vtbl->move(hash_map->context, dst_key, src_key, hash_map->key_size);

			hash_map->hashes[hole] = hash_map->hashes[scan];
			hash_map->indices[hole] = hash_map->indices[scan];

			hole = scan;
		}

		scan = (scan + 1) % hash_map->capacity;
	}

	crustHashIndexMapRemoveMask(hash_map, hole);
	hash_map->count--;

	return removed_index;
}

//
static void crustGenerationalHandlePoolAddMask(Crust_GenerationalHandlePool *pool, usize index)
{
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(pool->masks != CRUST_NULL);
	CRUST_ASSERT(pool->capacity > index);

	usize mask_index = index / 32;
	usize bit_index = index % 32;

	pool->masks[mask_index] |= (1u << bit_index);
}

static void crustGenerationalHandlePoolRemoveMask(Crust_GenerationalHandlePool *pool, usize index)
{
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(pool->masks != CRUST_NULL);
	CRUST_ASSERT(pool->capacity > index);

	usize mask_index = index / 32;
	usize bit_index = index % 32;

	pool->masks[mask_index] &= ~(1u << bit_index);
}

static u8 crustGenerationalHandlePoolCheckMask(const Crust_GenerationalHandlePool *pool, usize index)
{
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(pool->masks != CRUST_NULL);
	CRUST_ASSERT(index < pool->capacity);

	usize mask_index = index / 32;
	usize bit_index = index % 32;

	return (pool->masks[mask_index] & (1u << bit_index)) != 0;
}

CRUST_APIENTRY void crustGenerationalHandlePoolAlloc(Crust_Allocator allocator, Crust_GenerationalHandlePool *pool, usize capacity)
{
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(capacity > 0);

	usize masks_capacity = crustAlignUpUSize(capacity, 32) / 32;

	pool->generations = crustAllocArray(allocator, usize, capacity);
	pool->prevs = crustAllocArray(allocator, usize, capacity);
	pool->nexts = crustAllocArray(allocator, usize, capacity);
	pool->masks = crustAllocArray(allocator, u32, masks_capacity);
	pool->capacity = capacity;

	for (usize i = 0; i < capacity; ++i)
	{
		usize next = (i + 1 < capacity) ? i + 1 : USIZE_MAX;
		usize prev = (i > 0) ? i - 1 : USIZE_MAX;

		pool->nexts[i] = next;
		pool->prevs[i] = prev;
		pool->generations[i] = 0;
	}

	for (usize i = 0; i < masks_capacity; ++i)
		pool->masks[i] = 0;

	pool->free_head = 0;
	pool->free_tail = capacity - 1;

	pool->used_head = USIZE_MAX;
	pool->used_tail = USIZE_MAX;
}

CRUST_APIENTRY void crustGenerationalHandlePoolFree(Crust_Allocator allocator, Crust_GenerationalHandlePool *pool)
{
	CRUST_ASSERT(pool != CRUST_NULL);

	usize masks_capacity = crustAlignUpUSize(pool->capacity, 32) / 32;

	crustFreeArray(allocator, pool->generations, usize, pool->capacity);
	crustFreeArray(allocator, pool->prevs, usize, pool->capacity);
	crustFreeArray(allocator, pool->nexts, usize, pool->capacity);
	crustFreeArray(allocator, pool->masks, u32, masks_capacity);

	pool->generations = CRUST_NULL;
	pool->prevs = CRUST_NULL;
	pool->nexts = CRUST_NULL;
	pool->masks = CRUST_NULL;

	pool->capacity = 0;

	pool->free_head = USIZE_MAX;
	pool->free_tail = USIZE_MAX;

	pool->used_head = USIZE_MAX;
	pool->used_tail = USIZE_MAX;
}

CRUST_APIENTRY void crustGenerationalHandlePoolGrow(Crust_Allocator allocator, Crust_GenerationalHandlePool *pool, usize new_capacity)
{
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(pool->capacity < new_capacity);

	usize masks_capacity = crustAlignUpUSize(pool->capacity, 32) / 32;
	usize new_masks_capacity = crustAlignUpUSize(new_capacity, 32) / 32;

	pool->generations = crustReallocArray(allocator, pool->generations, usize, pool->capacity, new_capacity);
	pool->prevs = crustReallocArray(allocator, pool->prevs, usize, pool->capacity, new_capacity);
	pool->nexts = crustReallocArray(allocator, pool->nexts, usize, pool->capacity, new_capacity);

	if (masks_capacity != new_masks_capacity)
		pool->masks = crustReallocArray(allocator, pool->masks, u32, masks_capacity, new_masks_capacity);

	for (usize i = masks_capacity; i < new_masks_capacity; ++i)
		pool->masks[i] = 0;

	for (usize i = pool->capacity; i < new_capacity; ++i)
	{
		usize next = (i + 1 < new_capacity) ? i + 1 : USIZE_MAX;
		usize prev = (i > pool->capacity) ? i - 1 : pool->free_tail;

		pool->nexts[i] = next;
		pool->prevs[i] = prev;
		pool->generations[i] = 0;
	}

	if (pool->free_tail != USIZE_MAX)
		pool->nexts[pool->free_tail] = pool->capacity;

	if (pool->free_head == USIZE_MAX)
		pool->free_head = pool->capacity;

	pool->free_tail = new_capacity - 1;
	pool->capacity = new_capacity;
}

CRUST_APIENTRY u32 crustGenerationalHandlePoolCheck(const Crust_GenerationalHandlePool *pool, Crust_GenerationalHandle handle)
{
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(handle.index < pool->capacity);

	usize generation = pool->generations[handle.index];

	if (generation == 0 || generation != handle.generation)
		return 0;

	if (crustGenerationalHandlePoolCheckMask(pool, handle.index) == 0)
		return 0;

	return 1;
}

CRUST_APIENTRY Crust_GenerationalHandle crustGenerationalHandlePoolAdd(Crust_GenerationalHandlePool *pool, usize max_generation)
{
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(pool->prevs != CRUST_NULL);
	CRUST_ASSERT(pool->nexts != CRUST_NULL);
	CRUST_ASSERT(pool->generations != CRUST_NULL);
	CRUST_ASSERT(pool->free_head != USIZE_MAX);

	usize index = pool->free_head;
	usize next = pool->nexts[index];
	usize prev = pool->prevs[index];

	pool->prevs[index] = USIZE_MAX;
	pool->nexts[index] = USIZE_MAX;

	if (prev != USIZE_MAX)
		pool->nexts[prev] = next;

	if (next != USIZE_MAX)
		pool->prevs[next] = prev;

	if (pool->free_head == index)
		pool->free_head = next;

	if (pool->free_tail == index)
		pool->free_tail = prev;

	crustGenerationalHandlePoolAddMask(pool, index);

	usize generation = pool->generations[index];
	generation = crustMaxUSize(1, (generation + 1) % max_generation);

	pool->generations[index] = generation;

	if (pool->used_tail == USIZE_MAX)
	{
		CRUST_ASSERT(pool->used_head == USIZE_MAX);

		pool->used_head = index;
		pool->used_tail = index;
	}
	else
	{
		usize used_tail = pool->used_tail;

		pool->nexts[used_tail] = index;
		pool->prevs[index] = used_tail;

		pool->used_tail = index;
	}

	Crust_GenerationalHandle result;
	result.index = index;
	result.generation = generation;

	return result;
}

CRUST_APIENTRY void crustGenerationalHandlePoolRemove(Crust_GenerationalHandlePool *pool, Crust_GenerationalHandle handle)
{
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(pool->prevs != CRUST_NULL);
	CRUST_ASSERT(pool->nexts != CRUST_NULL);
	CRUST_ASSERT(pool->generations != CRUST_NULL);
	CRUST_ASSERT(pool->used_tail != USIZE_MAX);
	CRUST_ASSERT(crustGenerationalHandlePoolCheck(pool, handle) != 0);

	usize index = handle.index;
	usize next = pool->nexts[index];
	usize prev = pool->prevs[index];

	pool->nexts[index] = USIZE_MAX;
	pool->prevs[index] = USIZE_MAX;

	if (prev != USIZE_MAX)
		pool->nexts[prev] = next;

	if (next != USIZE_MAX)
		pool->prevs[next] = prev;

	if (pool->used_head == index)
		pool->used_head = next;

	if (pool->used_tail == index)
		pool->used_tail = prev;

	if (pool->free_head == USIZE_MAX)
	{
		CRUST_ASSERT(pool->free_tail == USIZE_MAX);

		pool->free_head = index;
		pool->free_tail = index;
	}
	else
	{
		usize free_head = pool->free_head;

		pool->prevs[free_head] = index;
		pool->nexts[index] = free_head;

		pool->free_head = index;
	}

	crustGenerationalHandlePoolRemoveMask(pool, index);
}

CRUST_APIENTRY Crust_GenerationalHandle crustGenerationalHandlePoolHead(const Crust_GenerationalHandlePool *pool)
{
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(pool->generations != CRUST_NULL);

	Crust_GenerationalHandle result;
	result.index = pool->used_head;
	result.generation = 0;

	if (result.index != USIZE_MAX)
		result.generation = pool->generations[result.index];

	return result;
}

CRUST_APIENTRY Crust_GenerationalHandle crustGenerationalHandlePoolTail(const Crust_GenerationalHandlePool *pool)
{
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(pool->generations != CRUST_NULL);

	Crust_GenerationalHandle result;
	result.index = pool->used_tail;
	result.generation = 0;

	if (result.index != USIZE_MAX)
		result.generation = pool->generations[result.index];

	return result;
}

CRUST_APIENTRY Crust_GenerationalHandle crustGenerationalHandlePoolNext(const Crust_GenerationalHandlePool *pool, Crust_GenerationalHandle handle)
{
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(pool->nexts != CRUST_NULL);
	CRUST_ASSERT(pool->generations != CRUST_NULL);
	CRUST_ASSERT(crustGenerationalHandlePoolCheck(pool, handle) != 0);

	Crust_GenerationalHandle result;
	result.index = pool->nexts[handle.index];
	result.generation = 0;

	if (result.index != USIZE_MAX)
		result.generation = pool->generations[result.index];

	return result;
}

CRUST_APIENTRY Crust_GenerationalHandle crustGenerationalHandlePoolPrev(const Crust_GenerationalHandlePool *pool, Crust_GenerationalHandle handle)
{
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(pool->prevs != CRUST_NULL);
	CRUST_ASSERT(pool->generations != CRUST_NULL);
	CRUST_ASSERT(crustGenerationalHandlePoolCheck(pool, handle) != 0);

	Crust_GenerationalHandle result;
	result.index = pool->prevs[handle.index];
	result.generation = 0;

	if (result.index != USIZE_MAX)
		result.generation = pool->generations[result.index];

	return result;
}

//
static u32 crustHeapNodePoolAcquire(Crust_HeapNodePool *pool)
{
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(pool->free_indices != CRUST_NULL);
	CRUST_ASSERT(pool->num_free_indices > 0);

	return pool->free_indices[--pool->num_free_indices];
}

static void crustHeapNodePoolRelease(Crust_HeapNodePool *pool, u32 index)
{
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(pool->free_indices != CRUST_NULL);
	CRUST_ASSERT(pool->num_free_indices < pool->capacity);
	CRUST_ASSERT(pool->capacity > index);

	pool->free_indices[pool->num_free_indices++] = index;
}

static void crustHeapNodePoolAddMask(Crust_HeapNodePool *pool, u32 index)
{
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(pool->node_masks != CRUST_NULL);
	CRUST_ASSERT(pool->capacity > index);

	u32 mask_index = index / 32;
	u32 bit_index = index % 32;

	pool->node_masks[mask_index] |= (1u << bit_index);
}

static void crustHeapNodePoolRemoveMask(Crust_HeapNodePool *pool, u32 index)
{
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(pool->node_masks != CRUST_NULL);
	CRUST_ASSERT(pool->capacity > index);

	u32 mask_index = index / 32;
	u32 bit_index = index % 32;

	pool->node_masks[mask_index] &= ~(1u << bit_index);
}

static u8 crustHeapNodePoolCheckMask(const Crust_HeapNodePool *pool, u32 index)
{
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(pool->node_masks != CRUST_NULL);
	CRUST_ASSERT(pool->capacity > index);

	u32 mask_index = index / 32;
	u32 bit_index = index % 32;

	return (pool->node_masks[mask_index] & (1u << bit_index)) != 0;
}

CRUST_APIENTRY void crustHeapNodePoolAlloc(Crust_Allocator allocator, Crust_HeapNodePool *pool, u32 capacity)
{
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(capacity > 0);

	usize masks_capacity = crustAlignUpUSize(capacity, 32) / 32;

	pool->nodes = crustAllocArray(allocator, Crust_HeapNode, capacity);
	pool->node_masks = crustAllocArray(allocator, u32, masks_capacity);
	pool->free_indices = crustAllocArray(allocator, u32, capacity);
	pool->num_free_indices = capacity;
	pool->capacity = capacity;

	for (u32 i = 0; i < capacity; ++i)
		pool->free_indices[i] = capacity - i - 1;

	for (u32 i = 0; i < masks_capacity; ++i)
		pool->node_masks[i] = 0;
}

CRUST_APIENTRY void crustHeapNodePoolFree(Crust_Allocator allocator, Crust_HeapNodePool *pool)
{
	CRUST_ASSERT(pool != CRUST_NULL);

	usize masks_capacity = crustAlignUpUSize(pool->capacity, 32) / 32;

	crustFreeArray(allocator, pool->nodes, Crust_HeapNode, pool->capacity);
	crustFreeArray(allocator, pool->node_masks, u32, masks_capacity);
	crustFreeArray(allocator, pool->free_indices, u32, pool->capacity);

	pool->nodes = CRUST_NULL;
	pool->node_masks = CRUST_NULL;
	pool->free_indices = CRUST_NULL;

	pool->num_free_indices = 0;
	pool->capacity = 0;
}

//
static u32 crustHeapToBinSize(u8 bin)
{
	u32 mantissa = bin & CRUST_HEAP_PARAMETERS_MANTISSA_MASK;
	u32 exponent = bin >> CRUST_HEAP_PARAMETERS_MANTISSA_BITS;

	if (exponent == 0)
		return mantissa;

	return (mantissa | CRUST_HEAP_PARAMETERS_MANTISSA_MAX) << (exponent - 1);
}

static u8 crustHeapToBinIndex(u32 size)
{
	if (size < CRUST_HEAP_PARAMETERS_MANTISSA_MAX)
		return (u8)size;

	u32 leading_zeroes = crustLzcntU32(size);
	u32 highest_bit = 31 - leading_zeroes;

	u32 mantissa_bit = highest_bit - CRUST_HEAP_PARAMETERS_MANTISSA_BITS;

	u32 exponent = mantissa_bit + 1;
	u32 mantissa = (size >> mantissa_bit) & CRUST_HEAP_PARAMETERS_MANTISSA_MASK;

	return (u8)((exponent << CRUST_HEAP_PARAMETERS_MANTISSA_BITS) + mantissa);
}

static u8 crustHeapToBinIndexRoundUp(u32 size)
{
	if (size < CRUST_HEAP_PARAMETERS_MANTISSA_MAX)
		return (u8)size;

	u32 leading_zeroes = crustLzcntU32(size);
	u32 highest_bit = 31 - leading_zeroes;

	u32 mantissa_bit = highest_bit - CRUST_HEAP_PARAMETERS_MANTISSA_BITS;

	u32 exponent = mantissa_bit + 1;
	u32 mantissa = (size >> mantissa_bit) & CRUST_HEAP_PARAMETERS_MANTISSA_MASK;

	u32 lower_bits_mask = (1u << mantissa_bit) - 1;

	if ((size & lower_bits_mask) != 0)
		mantissa++;

	return (u8)((exponent << CRUST_HEAP_PARAMETERS_MANTISSA_BITS) + mantissa);
}

static u8 crustHeapFindBin(u32 bin_mask, u8 index)
{
	u32 mask = (1u << index) - 1;

	u32 masked_bins = bin_mask & ~mask;
	if (masked_bins == 0)
		return U8_MAX;

	return (u8)crustTzcntU32(masked_bins);
}

static u8 crustHeapFindBinForSize(const Crust_Heap *heap, u32 size)
{
	CRUST_ASSERT(heap != CRUST_NULL);
	CRUST_ASSERT(heap->size >= size);

	Crust_HeapNodePool *pool = heap->pool;
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(pool->nodes != CRUST_NULL);

	u8 bin_index = crustHeapToBinIndexRoundUp(size);
	CRUST_ASSERT(crustHeapToBinSize(bin_index) >= size);

	u8 sparse_bin_index = bin_index >> CRUST_HEAP_PARAMETERS_MANTISSA_BITS;
	u8 linear_bin_index = U8_MAX;

	u8 used_linear_bins = heap->used_linear_bins[sparse_bin_index];

	if (used_linear_bins != 0)
	{
		u8 min_linear_bin = bin_index & CRUST_HEAP_PARAMETERS_MANTISSA_MASK;
		linear_bin_index = crustHeapFindBin(used_linear_bins, min_linear_bin);
	}

	if (linear_bin_index == U8_MAX)
	{
		sparse_bin_index = crustHeapFindBin(heap->used_sparse_bins, sparse_bin_index + 1);
		if (sparse_bin_index == U8_MAX)
			return U8_MAX;

		CRUST_ASSERT(sparse_bin_index != 0);

		used_linear_bins = heap->used_linear_bins[sparse_bin_index];
		CRUST_ASSERT(used_linear_bins != 0);

		linear_bin_index = (u8)crustTzcntU32(used_linear_bins);
	}

	CRUST_ASSERT(linear_bin_index != U8_MAX);
	return (sparse_bin_index << CRUST_HEAP_PARAMETERS_MANTISSA_BITS) | (linear_bin_index & CRUST_HEAP_PARAMETERS_MANTISSA_MASK);
}

static void crustHeapAddNodeToBin(Crust_Heap *heap, u32 index, u32 size, u32 offset)
{
	CRUST_ASSERT(heap != CRUST_NULL);
	CRUST_ASSERT(heap->size >= size + offset);
	CRUST_ASSERT(index != U32_MAX);

	Crust_HeapNodePool *pool = heap->pool;
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(pool->nodes != CRUST_NULL);
	CRUST_ASSERT(index < pool->capacity);

	u8 bin_index = crustHeapToBinIndex(size);
	CRUST_ASSERT(crustHeapToBinSize(bin_index) <= size);

	u8 sparse_bin_index = bin_index >> CRUST_HEAP_PARAMETERS_MANTISSA_BITS;
	u8 linear_bin_index = bin_index & CRUST_HEAP_PARAMETERS_MANTISSA_MASK;

	heap->used_sparse_bins |= 1u << sparse_bin_index;
	heap->used_linear_bins[sparse_bin_index] |= 1u << linear_bin_index;

	Crust_HeapNode *node = &pool->nodes[index];

	node->offset = offset;
	node->size = size;
	node->prev_bin = U32_MAX;
	node->next_bin = U32_MAX;
	node->prev_neighbour = U32_MAX;
	node->next_neighbour = U32_MAX;
	crustHeapNodePoolRemoveMask(pool, index);

	u32 bin_head_index = heap->heads[bin_index];

	if (bin_head_index != U32_MAX)
	{
		Crust_HeapNode *bin_head_node = &pool->nodes[bin_head_index];
		CRUST_ASSERT(crustHeapNodePoolCheckMask(pool, bin_head_index) == 0);

		bin_head_node->prev_bin = index;
		node->next_bin = bin_head_index;
	}

	heap->heads[bin_index] = index;
}

static void crustHeapRemoveNodeFromBin(Crust_Heap *heap, u32 index)
{
	CRUST_ASSERT(heap != CRUST_NULL);
	CRUST_ASSERT(index != U32_MAX);

	Crust_HeapNodePool *pool = heap->pool;
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(pool->nodes != CRUST_NULL);
	CRUST_ASSERT(index < pool->capacity);

	Crust_HeapNode *node = &pool->nodes[index];
	CRUST_ASSERT(node != CRUST_NULL);
	CRUST_ASSERT(crustHeapNodePoolCheckMask(pool, index) == 0);

	u8 bin_index = crustHeapToBinIndex(node->size);
	CRUST_ASSERT(crustHeapToBinSize(bin_index) <= node->size);

	if (node->prev_bin != U32_MAX)
	{
		Crust_HeapNode *prev_node = &pool->nodes[node->prev_bin];
		CRUST_ASSERT(prev_node != CRUST_NULL);

		prev_node->next_bin = node->next_bin;
	}

	if (node->next_bin != U32_MAX)
	{
		Crust_HeapNode *next_node = &pool->nodes[node->next_bin];
		CRUST_ASSERT(next_node != CRUST_NULL);

		next_node->prev_bin = node->prev_bin;
	}

	u8 sparse_bin_index = bin_index >> CRUST_HEAP_PARAMETERS_MANTISSA_BITS;
	u8 linear_bin_index = bin_index & CRUST_HEAP_PARAMETERS_MANTISSA_MASK;

	u32 bin_head_index = heap->heads[bin_index];
	CRUST_ASSERT(bin_head_index != U32_MAX);

	if (bin_head_index == index)
		bin_head_index = node->next_bin;

	if (bin_head_index == U32_MAX)
	{
		u8 linear_bin_mask = heap->used_linear_bins[sparse_bin_index];
		u32 sparse_bin_mask = heap->used_sparse_bins;

		linear_bin_mask &= ~(1u << linear_bin_index);
		if (linear_bin_mask == 0)
			sparse_bin_mask &= ~(1u << sparse_bin_index);

		heap->used_linear_bins[sparse_bin_index] = linear_bin_mask;
		heap->used_sparse_bins = sparse_bin_mask;
	}

	heap->heads[bin_index] = bin_head_index;
}

CRUST_APIENTRY Crust_Heap crustHeapInit(Crust_HeapNodePool *pool, u32 size)
{
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(size > 0);

	Crust_Heap result;
	result.pool = pool;
	result.size = size;

	for (u32 i = 0; i < CRUST_HEAP_PARAMETERS_NUM_BINS; ++i)
		result.heads[i] = U32_MAX;

	result.used_sparse_bins = 0;
	for (u32 i = 0; i < CRUST_HEAP_PARAMETERS_NUM_SPARSE_BINS; ++i)
		result.used_linear_bins[i] = 0;

	u32 index = crustHeapNodePoolAcquire(pool);
	crustHeapAddNodeToBin(&result, index, size, 0);

	return result;
}

CRUST_APIENTRY Crust_HeapAllocation crustHeapStageAlloc(const Crust_Heap *heap, u32 size)
{
	CRUST_ASSERT(heap != CRUST_NULL);
	CRUST_ASSERT(heap->size >= size);
	CRUST_ASSERT(size > 0);
	
	const Crust_HeapNodePool *pool = heap->pool;
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(pool->nodes != CRUST_NULL);

	Crust_HeapAllocation result;
	result.offset = U32_MAX;
	result.index = U32_MAX;

	u8 bin_index = crustHeapFindBinForSize(heap, size);
	if (bin_index == U8_MAX)
		return result;

	CRUST_ASSERT(bin_index != 0);
	CRUST_ASSERT(crustHeapToBinSize(bin_index) >= size);

	result.index = heap->heads[bin_index];
	CRUST_ASSERT(result.index != U32_MAX);

	const Crust_HeapNode *node = &pool->nodes[result.index];
	CRUST_ASSERT(node != CRUST_NULL);
	CRUST_ASSERT(node->size >= size);

	result.offset = node->offset;

	return result;
}

CRUST_APIENTRY Crust_HeapAllocation crustHeapStageAllocAligned(const Crust_Heap *heap, u32 size, u32 alignment)
{
	CRUST_ASSERT(heap != CRUST_NULL);
	CRUST_ASSERT(heap->size >= size);
	CRUST_ASSERT(size > 0);
	CRUST_ASSERT(crustIsPow2U32(alignment) != 0);

	const Crust_HeapNodePool *pool = heap->pool;
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(pool->nodes != CRUST_NULL);

	Crust_HeapAllocation result;
	result.offset = U32_MAX;
	result.index = U32_MAX;

	u8 bin_index = crustHeapFindBinForSize(heap, size);
	if (bin_index == U8_MAX)
		return result;

	CRUST_ASSERT(bin_index != 0);
	CRUST_ASSERT(crustHeapToBinSize(bin_index) >= size);

	result.index = heap->heads[bin_index];
	CRUST_ASSERT(result.index != U32_MAX);

	const Crust_HeapNode *node = &pool->nodes[result.index];
	CRUST_ASSERT(node != CRUST_NULL);
	CRUST_ASSERT(node->size >= size);

	result.offset = crustAlignUpU32(node->offset, alignment);

	u32 remainder = result.offset - node->offset;
	if (remainder + size > node->size)
	{
		u32 max_size = size + alignment - 1;
		bin_index = crustHeapFindBinForSize(heap, max_size);
		if (bin_index == U8_MAX)
		{
			result.offset = U32_MAX;
			result.index = U32_MAX;

			return result;
		}

		CRUST_ASSERT(bin_index != 0);
		result.index = heap->heads[bin_index];

		CRUST_ASSERT(result.index != U32_MAX);

		node = &pool->nodes[result.index];
		CRUST_ASSERT(node != CRUST_NULL);

		result.offset = crustAlignUpU32(node->offset, alignment);
	}

	return result;
}

CRUST_APIENTRY void crustHeapCommitAlloc(Crust_Heap *heap, Crust_HeapAllocation allocation, u32 size)
{
	CRUST_ASSERT(heap != CRUST_NULL);
	CRUST_ASSERT(heap->size >= size);
	CRUST_ASSERT(size > 0);

	CRUST_ASSERT(allocation.index != U32_MAX);
	CRUST_ASSERT(allocation.offset != U32_MAX);

	Crust_HeapNodePool *pool = heap->pool;
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(pool->nodes != CRUST_NULL);
	CRUST_ASSERT(pool->capacity > allocation.index);

	Crust_HeapNode *node = &pool->nodes[allocation.index];
	CRUST_ASSERT(node != CRUST_NULL);
	CRUST_ASSERT(crustHeapNodePoolCheckMask(pool, allocation.index) == 0);
	CRUST_ASSERT(allocation.offset >= node->offset);

	u32 remainder_begin_size = allocation.offset - node->offset;
	u32 remainder_begin_offset = node->offset;
	u32 remainder_end_size = node->size - remainder_begin_size - size;
	u32 remainder_end_offset = allocation.offset + size;

	CRUST_ASSERT(remainder_begin_size + size <= node->size);

	u32 prev_index = node->prev_neighbour;
	u32 next_index = node->next_neighbour;

	crustHeapRemoveNodeFromBin(heap, allocation.index);

	node->offset = allocation.offset;
	node->size = size;

	crustHeapNodePoolAddMask(pool, allocation.index);

	if (remainder_begin_size > 0)
	{
		Crust_HeapNode *prev_node = (prev_index != U32_MAX) ? &pool->nodes[prev_index] : CRUST_NULL;

		// try merge with previous free node
		if (prev_node != CRUST_NULL && crustHeapNodePoolCheckMask(pool, prev_index) == 0)
		{
			CRUST_ASSERT(prev_node->next_neighbour == allocation.index);

			remainder_begin_offset = prev_node->offset;
			remainder_begin_size += prev_node->size;

			u32 prev_prev_index = prev_node->prev_neighbour;

			crustHeapRemoveNodeFromBin(heap, prev_index);
			crustHeapNodePoolRelease(pool, prev_index);

			prev_index = prev_prev_index;
			prev_node = (prev_index != U32_MAX) ? &pool->nodes[prev_index] : CRUST_NULL;
		}

		u32 new_index = crustHeapNodePoolAcquire(pool);
		crustHeapAddNodeToBin(heap, new_index, remainder_begin_size, remainder_begin_offset);

		Crust_HeapNode *new_node = &pool->nodes[new_index];
		CRUST_ASSERT(new_node != CRUST_NULL);

		node->prev_neighbour = new_index;
		new_node->next_neighbour = allocation.index;

		new_node->prev_neighbour = prev_index;
		if (prev_node)
			prev_node->next_neighbour = new_index;
	}

	if (remainder_end_size > 0)
	{
		Crust_HeapNode *next_node = (next_index != U32_MAX) ? &pool->nodes[next_index] : CRUST_NULL;

		// try merge with next free node
		if (next_node != CRUST_NULL && crustHeapNodePoolCheckMask(pool, next_index) == 0)
		{
			CRUST_ASSERT(next_node->prev_neighbour == allocation.index);

			remainder_end_size += next_node->size;

			u32 next_next_index = next_node->next_neighbour;

			crustHeapRemoveNodeFromBin(heap, next_index);
			crustHeapNodePoolRelease(pool, next_index);

			next_index = next_next_index;
			next_node = (next_index != U32_MAX) ? &pool->nodes[next_index] : CRUST_NULL;
		}

		u32 new_index = crustHeapNodePoolAcquire(pool);
		crustHeapAddNodeToBin(heap, new_index, remainder_end_size, remainder_end_offset);

		Crust_HeapNode *new_node = &pool->nodes[new_index];
		CRUST_ASSERT(new_node != CRUST_NULL);

		node->next_neighbour = new_index;
		new_node->prev_neighbour = allocation.index;

		new_node->next_neighbour = next_index;
		if (next_node)
			next_node->prev_neighbour = new_index;
	}
}

CRUST_APIENTRY Crust_HeapAllocation crustHeapAlloc(Crust_Heap *heap, u32 size)
{
	CRUST_ASSERT(heap != CRUST_NULL);
	CRUST_ASSERT(heap->size >= size);
	CRUST_ASSERT(size > 0);

	Crust_HeapAllocation result = crustHeapStageAlloc(heap, size);
	if (result.index == U32_MAX && result.offset == U32_MAX)
		return result;

	crustHeapCommitAlloc(heap, result, size);
	return result;
}

CRUST_APIENTRY Crust_HeapAllocation crustHeapAllocAligned(Crust_Heap *heap, u32 size, u32 alignment)
{
	CRUST_ASSERT(heap != CRUST_NULL);
	CRUST_ASSERT(heap->size >= size);
	CRUST_ASSERT(size > 0);
	CRUST_ASSERT(crustIsPow2U32(alignment) != 0);

	Crust_HeapAllocation result = crustHeapStageAllocAligned(heap, size, alignment);
	if (result.index == U32_MAX && result.offset == U32_MAX)
		return result;

	crustHeapCommitAlloc(heap, result, size);
	return result;
}

CRUST_APIENTRY void crustHeapFree(Crust_Heap *heap, Crust_HeapAllocation allocation)
{
	CRUST_ASSERT(heap != CRUST_NULL);

	CRUST_ASSERT(allocation.index != U32_MAX);
	CRUST_ASSERT(allocation.offset != U32_MAX);

	Crust_HeapNodePool *pool = heap->pool;
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(pool->nodes != CRUST_NULL);
	CRUST_ASSERT(pool->capacity > allocation.index);

	Crust_HeapNode *node = &pool->nodes[allocation.index];
	CRUST_ASSERT(node != CRUST_NULL);
	CRUST_ASSERT(crustHeapNodePoolCheckMask(pool, allocation.index) != 0);
	CRUST_ASSERT(allocation.offset >= node->offset);

	u32 prev_index = node->prev_neighbour;
	u32 next_index = node->next_neighbour;

	u32 size = node->size;
	u32 offset = node->offset;

	Crust_HeapNode *prev_node = (prev_index != U32_MAX) ? &pool->nodes[prev_index] : CRUST_NULL;

	// try merge with previous free node
	if (prev_node != CRUST_NULL && crustHeapNodePoolCheckMask(pool, prev_index) == 0)
	{
		offset = prev_node->offset;
		size += prev_node->size;

		crustHeapRemoveNodeFromBin(heap, prev_index);
		crustHeapNodePoolRelease(pool, prev_index);

		prev_index = prev_node->prev_neighbour;
	}

	Crust_HeapNode *next_node = (next_index != U32_MAX) ? &pool->nodes[next_index] : CRUST_NULL;

	// try merge with next free node
	if (next_node != CRUST_NULL && crustHeapNodePoolCheckMask(pool, next_index) == 0)
	{
		size += next_node->size;

		crustHeapRemoveNodeFromBin(heap, next_index);
		crustHeapNodePoolRelease(pool, next_index);

		next_index = next_node->next_neighbour;
	}

	crustHeapAddNodeToBin(heap, allocation.index, size, offset);

	node->prev_neighbour = prev_index;
	node->next_neighbour = next_index;

	if (prev_index != U32_MAX)
	{
		Crust_HeapNode *prev_prev_node = &pool->nodes[prev_index];
		prev_prev_node->next_neighbour = allocation.index;
	}

	if (next_index != U32_MAX)
	{
		Crust_HeapNode *next_next_node = &pool->nodes[next_index];
		next_next_node->prev_neighbour = allocation.index;
	}
}
