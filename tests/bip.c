#include "crust_test.h"

#include <string.h>

enum
{
	CAPACITY = 100,
	FUZZ_CAPACITY = 1000,
	FUZZ_MAX_PENDING = 4096,
	FUZZ_STEPS = 500000,
};

static int testWrapAround(void)
{
	static u8 memory[CAPACITY];
	Crust_BipBuffer bip = crustBipBufferAttach(memory, CAPACITY);

	CRUST_TEST_REQUIRE(crustBipBufferStageWrite(&bip, 40) == memory);
	crustBipBufferCommitWrite(&bip, 40);

	CRUST_TEST_REQUIRE(crustBipBufferStageWrite(&bip, 40) == memory + 40);
	crustBipBufferCommitWrite(&bip, 40);

	CRUST_TEST_REQUIRE(crustBipBufferStageRead(&bip, 40) == memory);
	crustBipBufferCommitRead(&bip, 40);

	CRUST_TEST_REQUIRE(crustBipBufferStageWrite(&bip, 40) == memory);
	crustBipBufferCommitWrite(&bip, 40);

	CRUST_TEST_CHECK(crustBipBufferStageWrite(&bip, 1) == CRUST_NULL);

	CRUST_TEST_REQUIRE(crustBipBufferStageRead(&bip, 40) == memory + 40);
	crustBipBufferCommitRead(&bip, 40);

	CRUST_TEST_REQUIRE(crustBipBufferStageRead(&bip, 40) == memory);
	crustBipBufferCommitRead(&bip, 40);

	CRUST_TEST_CHECK(crustBipBufferStageRead(&bip, 1) == CRUST_NULL);
	return 0;
}

static int testFullBufferRejectsWrite(void)
{
	static u8 memory[CAPACITY];
	Crust_BipBuffer bip = crustBipBufferAttach(memory, CAPACITY);

	CRUST_TEST_REQUIRE(crustBipBufferStageWrite(&bip, 60) == memory);
	crustBipBufferCommitWrite(&bip, 60);

	CRUST_TEST_CHECK(crustBipBufferStageWrite(&bip, 50) == CRUST_NULL);

	CRUST_TEST_REQUIRE(crustBipBufferStageWrite(&bip, 40) == memory + 60);
	crustBipBufferCommitWrite(&bip, 40);

	CRUST_TEST_CHECK(crustBipBufferStageWrite(&bip, 1) == CRUST_NULL);
	return 0;
}

static int testPartialCommit(void)
{
	static u8 memory[CAPACITY];
	Crust_BipBuffer bip = crustBipBufferAttach(memory, CAPACITY);

	CRUST_TEST_REQUIRE(crustBipBufferStageWrite(&bip, 50) == memory);
	crustBipBufferCommitWrite(&bip, 20);

	CRUST_TEST_CHECK(crustBipBufferStageRead(&bip, 21) == CRUST_NULL);

	CRUST_TEST_REQUIRE(crustBipBufferStageRead(&bip, 20) == memory);
	crustBipBufferCommitRead(&bip, 10);

	CRUST_TEST_REQUIRE(crustBipBufferStageRead(&bip, 10) == memory + 10);
	crustBipBufferCommitRead(&bip, 10);

	CRUST_TEST_REQUIRE(crustBipBufferStageWrite(&bip, 30) == memory + 20);
	crustBipBufferCommitWrite(&bip, 30);

	return 0;
}

static int testCancel(void)
{
	static u8 memory[CAPACITY];
	Crust_BipBuffer bip = crustBipBufferAttach(memory, CAPACITY);

	CRUST_TEST_REQUIRE(crustBipBufferStageWrite(&bip, 40) == memory);
	crustBipBufferCommitWrite(&bip, 0);

	CRUST_TEST_CHECK(crustBipBufferStageRead(&bip, 1) == CRUST_NULL);

	CRUST_TEST_REQUIRE(crustBipBufferStageWrite(&bip, 60) == memory);
	crustBipBufferCommitWrite(&bip, 60);

	CRUST_TEST_REQUIRE(crustBipBufferStageWrite(&bip, 30) == memory + 60);
	crustBipBufferCommitWrite(&bip, 30);

	CRUST_TEST_REQUIRE(crustBipBufferStageRead(&bip, 60) == memory);
	crustBipBufferCommitRead(&bip, 0);

	CRUST_TEST_REQUIRE(crustBipBufferStageRead(&bip, 60) == memory);
	crustBipBufferCommitRead(&bip, 60);

	CRUST_TEST_REQUIRE(crustBipBufferStageWrite(&bip, 50) == memory);
	crustBipBufferCommitWrite(&bip, 0);

	CRUST_TEST_REQUIRE(crustBipBufferStageRead(&bip, 30) == memory + 60);
	crustBipBufferCommitRead(&bip, 30);

	CRUST_TEST_CHECK(crustBipBufferStageRead(&bip, 1) == CRUST_NULL);

	CRUST_TEST_REQUIRE(crustBipBufferStageWrite(&bip, 50) == memory);
	crustBipBufferCommitWrite(&bip, 50);

	CRUST_TEST_REQUIRE(crustBipBufferStageRead(&bip, 50) == memory);
	crustBipBufferCommitRead(&bip, 0);

	CRUST_TEST_REQUIRE(crustBipBufferStageRead(&bip, 50) == memory);
	crustBipBufferCommitRead(&bip, 50);

	CRUST_TEST_CHECK(crustBipBufferStageRead(&bip, 1) == CRUST_NULL);
	return 0;
}

static int testFuzz(void)
{
	static u8 memory[FUZZ_CAPACITY];
	static u8 spsc_memory[FUZZ_CAPACITY];

	Crust_BipBuffer bip = crustBipBufferAttach(memory, FUZZ_CAPACITY);
	Crust_SpscBipBuffer spsc_bip = crustSpscBipBufferAttach(spsc_memory, FUZZ_CAPACITY);

	static u32 pending_sizes[FUZZ_MAX_PENDING];
	u32 pending_head = 0;
	u32 pending_tail = 0;

	u32 write_seq = 0;
	u32 read_seq = 0;

	for (u32 step = 0; step < FUZZ_STEPS; ++step)
	{
		if (crustTestRandom() & 1)
		{
			u32 size = 4 + crustTestRandom() % 300;

			u8 *data = (u8 *)crustBipBufferStageWrite(&bip, size);
			u8 *spsc_data = (u8 *)crustSpscBipBufferStageWrite(&spsc_bip, size);

			CRUST_TEST_REQUIRE((data == CRUST_NULL) == (spsc_data == CRUST_NULL));
			if (data == CRUST_NULL)
				continue;

			CRUST_TEST_REQUIRE(data - memory == spsc_data - spsc_memory);

			if (crustTestRandom() % 4 == 0)
			{
				crustBipBufferCommitWrite(&bip, 0);
				crustSpscBipBufferCommitWrite(&spsc_bip, 0);
				continue;
			}

			memcpy(data, &write_seq, sizeof(write_seq));
			memset(data + sizeof(write_seq), (u8)write_seq, size - sizeof(write_seq));
			memcpy(spsc_data, data, size);

			crustBipBufferCommitWrite(&bip, size);
			crustSpscBipBufferCommitWrite(&spsc_bip, size);

			pending_sizes[pending_tail++ % FUZZ_MAX_PENDING] = size;
			write_seq++;
		}
		else if (pending_head != pending_tail)
		{
			u32 size = pending_sizes[pending_head % FUZZ_MAX_PENDING];

			const u8 *data = (const u8 *)crustBipBufferStageRead(&bip, size);
			const u8 *spsc_data = (const u8 *)crustSpscBipBufferStageRead(&spsc_bip, size);

			CRUST_TEST_REQUIRE(data != CRUST_NULL);
			CRUST_TEST_REQUIRE(spsc_data != CRUST_NULL);
			CRUST_TEST_REQUIRE(data - memory == spsc_data - spsc_memory);

			u32 seq;
			memcpy(&seq, data, sizeof(seq));
			CRUST_TEST_REQUIRE(seq == read_seq);

			for (u32 i = sizeof(seq); i < size; ++i)
				CRUST_TEST_REQUIRE(data[i] == (u8)read_seq);

			CRUST_TEST_REQUIRE(memcmp(data, spsc_data, size) == 0);

			if (crustTestRandom() % 4 == 0)
			{
				crustBipBufferCommitRead(&bip, 0);
				crustSpscBipBufferCommitRead(&spsc_bip, 0);
				continue;
			}

			crustBipBufferCommitRead(&bip, size);
			crustSpscBipBufferCommitRead(&spsc_bip, size);

			pending_head++;
			read_seq++;
		}
	}

	CRUST_TEST_CHECK(write_seq > FUZZ_STEPS / 4);
	return 0;
}

#if defined(CRUST_TEST_THREADS)
enum
{
	THREADED_MESSAGES = 2000000,
	THREADED_MESSAGE_SIZE = 24,
};

static u8 threaded_memory[FUZZ_CAPACITY];
static Crust_SpscBipBuffer threaded_bip;
static volatile u32 threaded_failed;

static void threadedWriter(void *arg)
{
	CRUST_UNUSED(arg);

	for (u32 seq = 0; seq < THREADED_MESSAGES; ++seq)
	{
		u8 *data = CRUST_NULL;
		while (data == CRUST_NULL)
		{
			if (crustAtomicLoadU32(&threaded_failed, CRUST_MEMORY_ORDER_RELAXED) != 0)
				return;

			data = (u8 *)crustSpscBipBufferStageWrite(&threaded_bip, THREADED_MESSAGE_SIZE);
		}

		memcpy(data, &seq, sizeof(seq));
		crustSpscBipBufferCommitWrite(&threaded_bip, THREADED_MESSAGE_SIZE);
	}
}

static void threadedReader(void *arg)
{
	CRUST_UNUSED(arg);

	for (u32 seq = 0; seq < THREADED_MESSAGES; ++seq)
	{
		const u8 *data = CRUST_NULL;
		while (data == CRUST_NULL)
			data = (const u8 *)crustSpscBipBufferStageRead(&threaded_bip, THREADED_MESSAGE_SIZE);

		u32 value;
		memcpy(&value, data, sizeof(value));

		if (value != seq)
		{
			crustAtomicStoreU32(&threaded_failed, 1, CRUST_MEMORY_ORDER_RELAXED);
			return;
		}

		crustSpscBipBufferCommitRead(&threaded_bip, THREADED_MESSAGE_SIZE);
	}
}

static int testSpscThreaded(void)
{
	threaded_bip = crustSpscBipBufferAttach(threaded_memory, sizeof(threaded_memory));

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
	CRUST_TEST_RUN(testFullBufferRejectsWrite);
	CRUST_TEST_RUN(testPartialCommit);
	CRUST_TEST_RUN(testCancel);
	CRUST_TEST_RUN(testFuzz);

#if defined(CRUST_TEST_THREADS)
	CRUST_TEST_RUN(testSpscThreaded);
#endif

	return crustTestResult();
}
