#include "crust_test.h"

#include <string.h>

enum
{
	CAPACITY = 16,
	FUZZ_CAPACITY = 1024,
	FUZZ_MAX_ENTRIES = 1024,
	FUZZ_STEPS = 200000,
};

static int testPushPop(void)
{
	static u8 memory[CAPACITY];
	Crust_Stack stack = crustStackAttach(memory, CAPACITY);

	u32 a = 0x11223344u;
	u64 b = 0x5566778899AABBCCull;
	u8 c[3] = { 'a', 'b', 'c' };

	crustStackPush(&stack, &a, sizeof(a));
	crustStackPush(&stack, &b, sizeof(b));
	crustStackPush(&stack, c, sizeof(c));
	crustStackPush(&stack, c, 0);
	CRUST_TEST_CHECK(stack.head == sizeof(a) + sizeof(b) + sizeof(c));

	u8 popped_c[3] = { 0 };
	crustStackPop(&stack, popped_c, sizeof(popped_c));
	CRUST_TEST_CHECK(memcmp(popped_c, c, sizeof(c)) == 0);

	u64 popped_b = 0;
	crustStackPop(&stack, &popped_b, sizeof(popped_b));
	CRUST_TEST_CHECK(popped_b == b);

	u32 popped_a = 0;
	crustStackPop(&stack, &popped_a, sizeof(popped_a));
	CRUST_TEST_CHECK(popped_a == a);

	CRUST_TEST_CHECK(stack.head == 0);
	return 0;
}

static int testPeek(void)
{
	static u8 memory[CAPACITY];
	Crust_Stack stack = crustStackAttach(memory, CAPACITY);

	u32 a = 1;
	u32 b = 2;

	crustStackPush(&stack, &a, sizeof(a));
	crustStackPush(&stack, &b, sizeof(b));

	for (u32 i = 0; i < 2; ++i)
	{
		u32 peeked = 0;
		crustStackPeek(&stack, &peeked, sizeof(peeked));
		CRUST_TEST_CHECK(peeked == b);
		CRUST_TEST_CHECK(stack.head == 2 * sizeof(u32));
	}

	u64 both = 0;
	crustStackPeek(&stack, &both, sizeof(both));
	CRUST_TEST_CHECK(memcmp(&both, memory, sizeof(both)) == 0);

	return 0;
}

static int testFullCapacityAndReset(void)
{
	static u8 memory[CAPACITY];
	Crust_Stack stack = crustStackAttach(memory, CAPACITY);

	u8 data[CAPACITY];
	for (u8 i = 0; i < CAPACITY; ++i)
		data[i] = i;

	crustStackPush(&stack, data, CAPACITY);
	CRUST_TEST_CHECK(stack.head == CAPACITY);

	u8 popped[CAPACITY] = { 0 };
	crustStackPop(&stack, popped, 6);
	CRUST_TEST_CHECK(memcmp(popped, data + 10, 6) == 0);

	crustStackReset(&stack);
	CRUST_TEST_CHECK(stack.head == 0);

	u32 value = 0xDEADBEEFu;
	crustStackPush(&stack, &value, sizeof(value));
	CRUST_TEST_CHECK(memcmp(memory, &value, sizeof(value)) == 0);

	return 0;
}

static int testFuzz(void)
{
	static u8 memory[FUZZ_CAPACITY];
	static u8 data[FUZZ_CAPACITY];
	static u32 entry_sizes[FUZZ_MAX_ENTRIES];
	static u8 entry_values[FUZZ_MAX_ENTRIES];

	Crust_Stack stack = crustStackAttach(memory, FUZZ_CAPACITY);
	u32 num_entries = 0;
	usize expected_head = 0;

	for (u32 step = 0; step < FUZZ_STEPS; ++step)
	{
		if ((crustTestRandom() & 1) && num_entries < FUZZ_MAX_ENTRIES)
		{
			u32 size = 1 + crustTestRandom() % 64;
			if (expected_head + size > FUZZ_CAPACITY)
				continue;

			u8 value = (u8)crustTestRandom();
			memset(data, value, size);
			crustStackPush(&stack, data, size);

			entry_sizes[num_entries] = size;
			entry_values[num_entries] = value;
			num_entries++;

			expected_head += size;
		}
		else if (num_entries > 0)
		{
			u32 top = num_entries - 1;
			u32 size = entry_sizes[top];

			if (crustTestRandom() & 1)
			{
				crustStackPeek(&stack, data, size);
			}
			else
			{
				crustStackPop(&stack, data, size);

				num_entries--;
				expected_head -= size;
			}

			for (u32 i = 0; i < size; ++i)
				CRUST_TEST_REQUIRE(data[i] == entry_values[top]);
		}

		CRUST_TEST_REQUIRE(stack.head == expected_head);
	}

	return 0;
}

int main(void)
{
	CRUST_TEST_RUN(testPushPop);
	CRUST_TEST_RUN(testPeek);
	CRUST_TEST_RUN(testFullCapacityAndReset);
	CRUST_TEST_RUN(testFuzz);

	return crustTestResult();
}
