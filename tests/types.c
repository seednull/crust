#include "crust_test.h"

#include <stddef.h>
#include <stdint.h>

extern int8_t crust_types_i8;
extern i8 crust_types_i8;
extern int16_t crust_types_i16;
extern i16 crust_types_i16;
extern int32_t crust_types_i32;
extern i32 crust_types_i32;
extern int64_t crust_types_i64;
extern i64 crust_types_i64;

extern uint8_t crust_types_u8;
extern u8 crust_types_u8;
extern uint16_t crust_types_u16;
extern u16 crust_types_u16;
extern uint32_t crust_types_u32;
extern u32 crust_types_u32;
extern uint64_t crust_types_u64;
extern u64 crust_types_u64;

extern ptrdiff_t crust_types_isize;
extern isize crust_types_isize;
extern size_t crust_types_usize;
extern usize crust_types_usize;

//
static int testSizes(void)
{
	usize sizes[] =
	{
		sizeof(i8), sizeof(i16), sizeof(i32), sizeof(i64),
		sizeof(u8), sizeof(u16), sizeof(u32), sizeof(u64),
	};

	usize expected[] = { 1, 2, 4, 8, 1, 2, 4, 8 };

	for (u32 i = 0; i < 8; ++i)
		CRUST_TEST_CHECK(sizes[i] == expected[i]);

	usize pointer_size = sizeof(void *);
	CRUST_TEST_CHECK(sizeof(usize) == pointer_size);
	CRUST_TEST_CHECK(sizeof(isize) == pointer_size);

	return 0;
}

static int testConstants(void)
{
	f32 pi32 = CRUST_F32_PI;
	f32 tau32 = CRUST_F32_TAU;
	f64 pi64 = CRUST_F64_PI;
	f64 tau64 = CRUST_F64_TAU;
	f64 pi = CRUST_PI;
	f64 tau = CRUST_TAU;

	CRUST_TEST_CHECK(pi32 == (f32)pi64);
	CRUST_TEST_CHECK(tau32 == (f32)tau64);
	CRUST_TEST_CHECK(tau32 == 2.0f * pi32);
	CRUST_TEST_CHECK(tau64 == 2.0 * pi64);
	CRUST_TEST_CHECK(pi == pi64);
	CRUST_TEST_CHECK(tau == tau64);

	return 0;
}

int main(void)
{
	CRUST_TEST_RUN(testSizes);
	CRUST_TEST_RUN(testConstants);

	return crustTestResult();
}
