#include "crust_test.h"

enum
{
	ITERATIONS = 10000,
};

static const u32 special_f32_bits[] =
{
	0x00000000u,
	0x80000000u,
	0x3f800000u,
	0xbf800000u,
	0x00000001u,
	0x807fffffu,
	0x00800000u,
	0x7f7fffffu,
	0xff7fffffu,
	0x7f800000u,
	0xff800000u,
	0x7fc00000u,
	0xffc00001u,
};

#define SPECIAL_F32_COUNT ((u32)(sizeof(special_f32_bits) / sizeof(special_f32_bits[0])))

typedef void (*PFN_checkF32)(const f32 *a, const f32 *b);

//
static u32 bitsF32(f32 v)
{
	u32 result;
	crustMemcpy(&result, &v, sizeof(result));
	return result;
}

static f32 fromBitsF32(u32 v)
{
	f32 result;
	crustMemcpy(&result, &v, sizeof(result));
	return result;
}

static u8 isNanBits(u32 bits)
{
	return (bits & 0x7f800000u) == 0x7f800000u && (bits & 0x007fffffu) != 0;
}

static u8 sameF32(f32 expected, f32 actual)
{
	u32 expected_bits = bitsF32(expected);
	u32 actual_bits = bitsF32(actual);

	if (isNanBits(expected_bits))
		return isNanBits(actual_bits);

	return expected_bits == actual_bits;
}

static f32 randomF32(void)
{
	switch (crustTestRandom() % 3)
	{
		case 0:
			return fromBitsF32(special_f32_bits[crustTestRandom() % SPECIAL_F32_COUNT]);

		case 1:
			return (f32)(crustTestRandomF64() * 200.0 - 100.0);

		default:
		{
			u32 bits = crustTestRandom();
			if (isNanBits(bits))
				bits |= 0x00400000u;

			return fromBitsF32(bits);
		}
	}
}

static void runF32(PFN_checkF32 check)
{
	f32 a[4];
	f32 b[4];

	for (u32 i = 0; i < SPECIAL_F32_COUNT; ++i)
	{
		for (u32 j = 0; j < SPECIAL_F32_COUNT; ++j)
		{
			for (u32 lane = 0; lane < 4; ++lane)
			{
				a[lane] = fromBitsF32(special_f32_bits[i]);
				b[lane] = fromBitsF32(special_f32_bits[j]);
			}

			check(a, b);
		}
	}

	for (u32 i = 0; i < ITERATIONS; ++i)
	{
		for (u32 lane = 0; lane < 4; ++lane)
		{
			a[lane] = randomF32();
			b[lane] = randomF32();
		}

		check(a, b);
	}
}

//
static void checkArithmeticF32(const f32 *a, const f32 *b)
{
	Crust_Simd128 va = crustSimd128LoadU(a);
	Crust_Simd128 vb = crustSimd128LoadU(b);

	f32 add[4];
	f32 mul[4];
	crustSimd128StoreU(add, crustSimd128AddF32(va, vb));
	crustSimd128StoreU(mul, crustSimd128MulF32(va, vb));

	for (u32 i = 0; i < 4; ++i)
	{
		CRUST_TEST_CHECK(sameF32(a[i] + b[i], add[i]));
		CRUST_TEST_CHECK(sameF32(a[i] * b[i], mul[i]));
	}
}

static void checkCompareF32(const f32 *a, const f32 *b)
{
	Crust_Simd128 va = crustSimd128LoadU(a);
	Crust_Simd128 vb = crustSimd128LoadU(b);

	u32 lt[4];
	u32 gt[4];
	crustSimd128StoreU(lt, crustSimd128LtF32(va, vb));
	crustSimd128StoreU(gt, crustSimd128GtF32(va, vb));

	for (u32 i = 0; i < 4; ++i)
	{
		CRUST_TEST_CHECK(lt[i] == ((a[i] < b[i]) ? CRUST_U32_MAX : 0u));
		CRUST_TEST_CHECK(gt[i] == ((a[i] > b[i]) ? CRUST_U32_MAX : 0u));
	}
}

static void checkMinMaxF32(const f32 *a, const f32 *b)
{
	Crust_Simd128 va = crustSimd128LoadU(a);
	Crust_Simd128 vb = crustSimd128LoadU(b);

	f32 min[4];
	f32 max[4];
	crustSimd128StoreU(min, crustSimd128MinF32(va, vb));
	crustSimd128StoreU(max, crustSimd128MaxF32(va, vb));

	for (u32 i = 0; i < 4; ++i)
	{
		CRUST_TEST_CHECK(bitsF32(crustMinF32(a[i], b[i])) == bitsF32(min[i]));
		CRUST_TEST_CHECK(bitsF32(crustMaxF32(a[i], b[i])) == bitsF32(max[i]));
	}
}

//
static int testLayout(void)
{
	usize size = sizeof(Crust_Simd128);
	usize alignment = CRUST_ALIGNOF(Crust_Simd128);
	u32 backends = CRUST_SIMD_SSE + CRUST_SIMD_NEON + CRUST_SIMD_WASM + CRUST_SIMD_SCALAR;
	u32 use_simd = CRUST_USE_SIMD;

	CRUST_TEST_CHECK(size == 16);
	CRUST_TEST_CHECK(alignment == 16);
	CRUST_TEST_CHECK(backends == 1);
	CRUST_TEST_CHECK(use_simd == 1 || CRUST_SIMD_SCALAR == 1);

	return 0;
}

static int testMemory(void)
{
	u8 src[32];
	u8 dst[48];

	for (u32 i = 0; i < 32; ++i)
		src[i] = (u8)(i * 7 + 1);

	for (u32 offset = 0; offset < 16; ++offset)
	{
		crustMemset(dst, 0, sizeof(dst));
		crustSimd128StoreU(dst + offset + 1, crustSimd128LoadU(src + offset));

		for (u32 i = 0; i < 48; ++i)
		{
			u8 inside = i >= offset + 1 && i < offset + 17;
			CRUST_TEST_CHECK(dst[i] == (inside ? src[i - 1] : 0));
		}
	}

	u32 lanes[4];
	crustSimd128StoreU(lanes, crustSimd128Zero());
	for (u32 i = 0; i < 4; ++i)
		CRUST_TEST_CHECK(lanes[i] == 0);

	crustSimd128StoreU(lanes, crustSimd128SetU32(1, 2, 3, 4));
	for (u32 i = 0; i < 4; ++i)
		CRUST_TEST_CHECK(lanes[i] == i + 1);

	crustSimd128StoreU(lanes, crustSimd128SplatU32(0xdeadbeefu));
	for (u32 i = 0; i < 4; ++i)
		CRUST_TEST_CHECK(lanes[i] == 0xdeadbeefu);

	f32 floats[4];
	crustSimd128StoreU(floats, crustSimd128SetF32(1.0f, 2.0f, 3.0f, 4.0f));
	for (u32 i = 0; i < 4; ++i)
		CRUST_TEST_CHECK(floats[i] == (f32)(i + 1));

	crustSimd128StoreU(floats, crustSimd128SplatF32(-0.0f));
	for (u32 i = 0; i < 4; ++i)
		CRUST_TEST_CHECK(bitsF32(floats[i]) == 0x80000000u);

	return 0;
}

static int testSelect(void)
{
	for (u32 i = 0; i < ITERATIONS; ++i)
	{
		u32 mask[4];
		u32 a[4];
		u32 b[4];

		for (u32 lane = 0; lane < 4; ++lane)
		{
			mask[lane] = crustTestRandom();
			a[lane] = crustTestRandom();
			b[lane] = crustTestRandom();
		}

		u32 result[4];
		crustSimd128StoreU(result, crustSimd128Select(crustSimd128LoadU(mask), crustSimd128LoadU(a), crustSimd128LoadU(b)));

		for (u32 lane = 0; lane < 4; ++lane)
			CRUST_TEST_CHECK(result[lane] == ((mask[lane] & a[lane]) | (~mask[lane] & b[lane])));
	}

	return 0;
}

static int testArithmeticU32(void)
{
	u32 lanes[4];
	crustSimd128StoreU(lanes, crustSimd128AddU32(crustSimd128SetU32(0xffffffffu, 0x7fffffffu, 0, 1), crustSimd128SetU32(2, 1, 0, 0xffffffffu)));

	CRUST_TEST_CHECK(lanes[0] == 1);
	CRUST_TEST_CHECK(lanes[1] == 0x80000000u);
	CRUST_TEST_CHECK(lanes[2] == 0);
	CRUST_TEST_CHECK(lanes[3] == 0);

	for (u32 i = 0; i < ITERATIONS; ++i)
	{
		u32 a[4];
		u32 b[4];

		for (u32 lane = 0; lane < 4; ++lane)
		{
			a[lane] = crustTestRandom();
			b[lane] = crustTestRandom();
		}

		u32 add_u32[4];
		u32 add_i32[4];
		crustSimd128StoreU(add_u32, crustSimd128AddU32(crustSimd128LoadU(a), crustSimd128LoadU(b)));
		crustSimd128StoreU(add_i32, crustSimd128AddI32(crustSimd128LoadU(a), crustSimd128LoadU(b)));

		for (u32 lane = 0; lane < 4; ++lane)
		{
			CRUST_TEST_CHECK(add_u32[lane] == a[lane] + b[lane]);
			CRUST_TEST_CHECK(add_i32[lane] == a[lane] + b[lane]);
		}
	}

	return 0;
}

static int testArithmeticF32(void)
{
	runF32(checkArithmeticF32);
	return 0;
}

static int testCompareF32(void)
{
	runF32(checkCompareF32);
	return 0;
}

static int testMinMaxF32(void)
{
	runF32(checkMinMaxF32);
	return 0;
}

int main(void)
{
	CRUST_TEST_RUN(testLayout);
	CRUST_TEST_RUN(testMemory);
	CRUST_TEST_RUN(testSelect);
	CRUST_TEST_RUN(testArithmeticU32);
	CRUST_TEST_RUN(testArithmeticF32);
	CRUST_TEST_RUN(testCompareF32);
	CRUST_TEST_RUN(testMinMaxF32);

	return crustTestResult();
}
