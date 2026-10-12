#include "crust_test.h"

#include <string.h>

enum
{
	CAPACITY = 8,
	FUZZ_CAPACITY = 1024,
	FUZZ_STEPS = 200000,
};

static void fillSequence(u8 *data, usize size, u8 first)
{
	for (usize i = 0; i < size; ++i)
		data[i] = (u8)(first + i);
}

static u8 isSequence(const u8 *data, usize size, u8 first)
{
	for (usize i = 0; i < size; ++i)
		if (data[i] != (u8)(first + i))
			return 0;

	return 1;
}

static int testWrapAround(void)
{
	static u8 memory[CAPACITY];
	Crust_RingBuffer ring = crustRingBufferAttach(memory, CAPACITY);

	u8 data[CAPACITY];

	fillSequence(data, 6, 0);
	crustRingBufferWrite(&ring, data, 6);

	crustRingBufferRead(&ring, data, 4);
	CRUST_TEST_CHECK(isSequence(data, 4, 0));

	fillSequence(data, 5, 6);
	crustRingBufferWrite(&ring, data, 5);
	CRUST_TEST_CHECK(ring.size == 7);

	crustRingBufferRead(&ring, data, 7);
	CRUST_TEST_CHECK(isSequence(data, 7, 4));
	CRUST_TEST_CHECK(ring.size == 0);

	return 0;
}

static int testFullCapacity(void)
{
	static u8 memory[CAPACITY];
	Crust_RingBuffer ring = crustRingBufferAttach(memory, CAPACITY);

	u8 data[CAPACITY];

	fillSequence(data, CAPACITY, 0);
	crustRingBufferWrite(&ring, data, CAPACITY);
	CRUST_TEST_CHECK(ring.size == CAPACITY);

	crustRingBufferRead(&ring, data, CAPACITY);
	CRUST_TEST_CHECK(isSequence(data, CAPACITY, 0));

	fillSequence(data, CAPACITY, CAPACITY);
	crustRingBufferWrite(&ring, data, CAPACITY);

	crustRingBufferRead(&ring, data, 3);
	CRUST_TEST_CHECK(isSequence(data, 3, CAPACITY));

	fillSequence(data, 3, 2 * CAPACITY);
	crustRingBufferWrite(&ring, data, 3);

	crustRingBufferRead(&ring, data, CAPACITY);
	CRUST_TEST_CHECK(isSequence(data, CAPACITY, CAPACITY + 3));
	CRUST_TEST_CHECK(ring.size == 0);

	return 0;
}

static int testSpscPartialTransfers(void)
{
	static u8 memory[CAPACITY];
	Crust_SpscRingBuffer ring = crustSpscRingBufferAttach(memory, CAPACITY);

	u8 data[2 * CAPACITY];

	fillSequence(data, 10, 0);
	CRUST_TEST_CHECK(crustSpscRingBufferWrite(&ring, data, 10) == CAPACITY);
	CRUST_TEST_CHECK(crustSpscRingBufferWrite(&ring, data, 1) == 0);

	CRUST_TEST_CHECK(crustSpscRingBufferRead(&ring, data, 3) == 3);
	CRUST_TEST_CHECK(isSequence(data, 3, 0));

	fillSequence(data, 5, CAPACITY);
	CRUST_TEST_CHECK(crustSpscRingBufferWrite(&ring, data, 5) == 3);

	CRUST_TEST_CHECK(crustSpscRingBufferRead(&ring, data, sizeof(data)) == CAPACITY);
	CRUST_TEST_CHECK(isSequence(data, CAPACITY, 3));

	CRUST_TEST_CHECK(crustSpscRingBufferRead(&ring, data, 1) == 0);
	return 0;
}

static int testZeroSize(void)
{
	static u8 memory[CAPACITY];
	static u8 spsc_memory[CAPACITY];

	Crust_RingBuffer ring = crustRingBufferAttach(memory, CAPACITY);
	Crust_SpscRingBuffer spsc_ring = crustSpscRingBufferAttach(spsc_memory, CAPACITY);

	u8 data[CAPACITY] = { 0 };

	crustRingBufferWrite(&ring, data, 0);
	crustRingBufferRead(&ring, data, 0);
	CRUST_TEST_CHECK(ring.size == 0);

	CRUST_TEST_CHECK(crustSpscRingBufferWrite(&spsc_ring, data, 0) == 0);
	CRUST_TEST_CHECK(crustSpscRingBufferRead(&spsc_ring, data, 0) == 0);

	return 0;
}

static int testFuzz(void)
{
	static u8 memory[FUZZ_CAPACITY];
	static u8 spsc_memory[FUZZ_CAPACITY];
	static u8 data[FUZZ_CAPACITY];
	static u8 spsc_data[FUZZ_CAPACITY];

	Crust_RingBuffer ring = crustRingBufferAttach(memory, FUZZ_CAPACITY);
	Crust_SpscRingBuffer spsc_ring = crustSpscRingBufferAttach(spsc_memory, FUZZ_CAPACITY);

	u8 write_value = 0;
	u8 read_value = 0;

	for (u32 step = 0; step < FUZZ_STEPS; ++step)
	{
		if (crustTestRandom() & 1)
		{
			usize size = crustTestRandom() % (FUZZ_CAPACITY - ring.size + 1);
			fillSequence(data, size, write_value);

			crustRingBufferWrite(&ring, data, size);
			CRUST_TEST_REQUIRE(crustSpscRingBufferWrite(&spsc_ring, data, size) == size);

			write_value = (u8)(write_value + size);
		}
		else
		{
			usize size = crustTestRandom() % (ring.size + 1);

			crustRingBufferRead(&ring, data, size);
			CRUST_TEST_REQUIRE(crustSpscRingBufferRead(&spsc_ring, spsc_data, size) == size);

			CRUST_TEST_REQUIRE(isSequence(data, size, read_value));
			CRUST_TEST_REQUIRE(memcmp(data, spsc_data, size) == 0);

			read_value = (u8)(read_value + size);
		}
	}

	return 0;
}

static int testSpscFuzz(void)
{
	static u8 memory[FUZZ_CAPACITY];
	static u8 data[2 * FUZZ_CAPACITY];

	Crust_SpscRingBuffer ring = crustSpscRingBufferAttach(memory, FUZZ_CAPACITY);

	usize used = 0;
	u8 write_value = 0;
	u8 read_value = 0;

	for (u32 step = 0; step < FUZZ_STEPS; ++step)
	{
		usize size = crustTestRandom() % (sizeof(data) + 1);

		if (crustTestRandom() & 1)
		{
			usize expected = crustMinUSize(size, FUZZ_CAPACITY - used);
			fillSequence(data, size, write_value);

			CRUST_TEST_REQUIRE(crustSpscRingBufferWrite(&ring, data, size) == expected);

			used += expected;
			write_value = (u8)(write_value + expected);
		}
		else
		{
			usize expected = crustMinUSize(size, used);

			CRUST_TEST_REQUIRE(crustSpscRingBufferRead(&ring, data, size) == expected);
			CRUST_TEST_REQUIRE(isSequence(data, expected, read_value));

			used -= expected;
			read_value = (u8)(read_value + expected);
		}
	}

	return 0;
}

#if defined(CRUST_TEST_THREADS)
enum
{
	THREADED_MESSAGES = 500000,
	THREADED_MAX_MESSAGE_SIZE = 64,
};

static u8 threaded_memory[FUZZ_CAPACITY];
static Crust_SpscRingBuffer threaded_ring;
static volatile u32 threaded_failed;

static void threadedWriter(void *arg)
{
	CRUST_UNUSED(arg);
	u8 data[THREADED_MAX_MESSAGE_SIZE];

	for (u32 seq = 0; seq < THREADED_MESSAGES; ++seq)
	{
		usize size = 1 + seq % THREADED_MAX_MESSAGE_SIZE;
		fillSequence(data, size, (u8)seq);

		usize offset = 0;
		while (offset < size)
		{
			if (crustAtomicLoadU32(&threaded_failed, CRUST_MEMORY_ORDER_RELAXED) != 0)
				return;

			offset += crustSpscRingBufferWrite(&threaded_ring, data + offset, size - offset);
		}
	}
}

static void threadedReader(void *arg)
{
	CRUST_UNUSED(arg);
	u8 data[THREADED_MAX_MESSAGE_SIZE];

	for (u32 seq = 0; seq < THREADED_MESSAGES; ++seq)
	{
		usize size = 1 + seq % THREADED_MAX_MESSAGE_SIZE;

		usize offset = 0;
		while (offset < size)
			offset += crustSpscRingBufferRead(&threaded_ring, data + offset, size - offset);

		if (isSequence(data, size, (u8)seq) == 0)
		{
			crustAtomicStoreU32(&threaded_failed, 1, CRUST_MEMORY_ORDER_RELAXED);
			return;
		}
	}
}

static int testSpscThreaded(void)
{
	threaded_ring = crustSpscRingBufferAttach(threaded_memory, sizeof(threaded_memory));

	CrustTest_Thread writer = crustTestThreadCreate(threadedWriter, CRUST_NULL);
	CrustTest_Thread reader = crustTestThreadCreate(threadedReader, CRUST_NULL);

	crustTestThreadJoin(writer);
	crustTestThreadJoin(reader);

	CRUST_TEST_CHECK(threaded_failed == 0);
	return 0;
}
#endif

int main(void)
{
	CRUST_TEST_RUN(testWrapAround);
	CRUST_TEST_RUN(testFullCapacity);
	CRUST_TEST_RUN(testSpscPartialTransfers);
	CRUST_TEST_RUN(testZeroSize);
	CRUST_TEST_RUN(testFuzz);
	CRUST_TEST_RUN(testSpscFuzz);

#if defined(CRUST_TEST_THREADS)
	CRUST_TEST_RUN(testSpscThreaded);
#endif

	return crustTestResult();
}
