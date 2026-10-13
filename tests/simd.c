#include "crust_test.h"

#include <math.h>
#include <string.h>

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
	0x3f000000u,
	0xbf000000u,
	0x3fc00000u,
	0x40200000u,
	0xc0200000u,
	0x4affffffu,
	0x4b000000u,
	0x4effffffu,
	0x4f000000u,
	0xcf000000u,
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

static const u64 special_f64_bits[] =
{
	0x0000000000000000ULL,
	0x8000000000000000ULL,
	0x3ff0000000000000ULL,
	0xbff0000000000000ULL,
	0x3fe0000000000000ULL,
	0xbfe0000000000000ULL,
	0x3ff8000000000000ULL,
	0x4004000000000000ULL,
	0xc004000000000000ULL,
	0x432fffffffffffffULL,
	0x4330000000000000ULL,
	0x47efffffe0000000ULL,
	0x47f0000000000000ULL,
	0x36a0000000000000ULL,
	0x0000000000000001ULL,
	0x800fffffffffffffULL,
	0x0010000000000000ULL,
	0x7fefffffffffffffULL,
	0xffefffffffffffffULL,
	0x7ff0000000000000ULL,
	0xfff0000000000000ULL,
	0x7ff8000000000000ULL,
	0xfff8000000000001ULL,
};

#define SPECIAL_F32_COUNT ((u32)(sizeof(special_f32_bits) / sizeof(special_f32_bits[0])))
#define SPECIAL_F64_COUNT ((u32)(sizeof(special_f64_bits) / sizeof(special_f64_bits[0])))

#define INPUTS_F32 (SPECIAL_F32_COUNT * SPECIAL_F32_COUNT + ITERATIONS)
#define INPUTS_F64 (SPECIAL_F64_COUNT * SPECIAL_F64_COUNT + ITERATIONS)

typedef Crust_Simd128 (*PFN_simdUnary)(Crust_Simd128 v);
typedef Crust_Simd128 (*PFN_simdBinary)(Crust_Simd128 a, Crust_Simd128 b);

typedef void (*PFN_unaryF32)(f32 v, f32 *result);
typedef void (*PFN_binaryF32)(f32 a, f32 b, f32 *result);
typedef u8 (*PFN_compareF32)(f32 a, f32 b);

typedef void (*PFN_unaryF64)(f64 v, f64 *result);
typedef void (*PFN_binaryF64)(f64 a, f64 b, f64 *result);
typedef u8 (*PFN_compareF64)(f64 a, f64 b);

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

static u64 bitsF64(f64 v)
{
	u64 result;
	crustMemcpy(&result, &v, sizeof(result));
	return result;
}

static f64 fromBitsF64(u64 v)
{
	f64 result;
	crustMemcpy(&result, &v, sizeof(result));
	return result;
}

static u8 isNanBitsF32(u32 bits)
{
	return (bits & 0x7f800000u) == 0x7f800000u && (bits & 0x007fffffu) != 0;
}

static u8 isNanBitsF64(u64 bits)
{
	return (bits & 0x7ff0000000000000ULL) == 0x7ff0000000000000ULL && (bits & 0x000fffffffffffffULL) != 0;
}

static u8 sameF32(f32 expected, f32 actual, u8 exact)
{
	u32 expected_bits = bitsF32(expected);
	u32 actual_bits = bitsF32(actual);

	if (!exact && isNanBitsF32(expected_bits))
		return isNanBitsF32(actual_bits);

	return expected_bits == actual_bits;
}

static u8 sameF64(f64 expected, f64 actual, u8 exact)
{
	u64 expected_bits = bitsF64(expected);
	u64 actual_bits = bitsF64(actual);

	if (!exact && isNanBitsF64(expected_bits))
		return isNanBitsF64(actual_bits);

	return expected_bits == actual_bits;
}

static u64 randomU64(void)
{
	return ((u64)crustTestRandom() << 32) | crustTestRandom();
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
			if (isNanBitsF32(bits))
				bits |= 0x00400000u;

			return fromBitsF32(bits);
		}
	}
}

static f64 randomF64(void)
{
	switch (crustTestRandom() % 3)
	{
		case 0:
			return fromBitsF64(special_f64_bits[crustTestRandom() % SPECIAL_F64_COUNT]);

		case 1:
			return crustTestRandomF64() * 200.0 - 100.0;

		default:
		{
			u64 bits = randomU64();
			if (isNanBitsF64(bits))
				bits |= 0x0008000000000000ULL;

			return fromBitsF64(bits);
		}
	}
}

static void inputF32(u32 index, f32 *a, f32 *b)
{
	u32 count = SPECIAL_F32_COUNT;

	for (u32 lane = 0; lane < 4; ++lane)
	{
		a[lane] = (index < count * count) ? fromBitsF32(special_f32_bits[index / count]) : randomF32();
		b[lane] = (index < count * count) ? fromBitsF32(special_f32_bits[index % count]) : randomF32();
	}
}

static void inputF64(u32 index, f64 *a, f64 *b)
{
	u32 count = SPECIAL_F64_COUNT;

	for (u32 lane = 0; lane < 2; ++lane)
	{
		a[lane] = (index < count * count) ? fromBitsF64(special_f64_bits[index / count]) : randomF64();
		b[lane] = (index < count * count) ? fromBitsF64(special_f64_bits[index % count]) : randomF64();
	}
}

//
static void addF32(f32 a, f32 b, f32 *result) { *result = a + b; }
static void subF32(f32 a, f32 b, f32 *result) { *result = a - b; }
static void mulF32(f32 a, f32 b, f32 *result) { *result = a * b; }
static void divF32(f32 a, f32 b, f32 *result) { *result = a / b; }
static void minF32(f32 a, f32 b, f32 *result) { *result = (a < b) ? a : b; }
static void maxF32(f32 a, f32 b, f32 *result) { *result = (a < b) ? b : a; }

static void sqrtF32(f32 v, f32 *result) { *result = sqrtf(v); }
static void absF32(f32 v, f32 *result) { *result = fromBitsF32(bitsF32(v) & 0x7fffffffu); }
static void negateF32(f32 v, f32 *result) { *result = fromBitsF32(bitsF32(v) ^ 0x80000000u); }
static void floorF32(f32 v, f32 *result) { *result = floorf(v); }
static void ceilF32(f32 v, f32 *result) { *result = ceilf(v); }
static void roundEvenF32(f32 v, f32 *result) { *result = nearbyintf(v); }
static void truncateF32(f32 v, f32 *result) { *result = truncf(v); }

static u8 equalF32(f32 a, f32 b) { return a == b; }
static u8 notEqualF32(f32 a, f32 b) { return a != b; }
static u8 lessF32(f32 a, f32 b) { return a < b; }
static u8 lessEqualF32(f32 a, f32 b) { return a <= b; }
static u8 greaterF32(f32 a, f32 b) { return a > b; }
static u8 greaterEqualF32(f32 a, f32 b) { return a >= b; }

static void addF64(f64 a, f64 b, f64 *result) { *result = a + b; }
static void subF64(f64 a, f64 b, f64 *result) { *result = a - b; }
static void mulF64(f64 a, f64 b, f64 *result) { *result = a * b; }
static void divF64(f64 a, f64 b, f64 *result) { *result = a / b; }
static void minF64(f64 a, f64 b, f64 *result) { *result = (a < b) ? a : b; }
static void maxF64(f64 a, f64 b, f64 *result) { *result = (a < b) ? b : a; }

static void sqrtF64(f64 v, f64 *result) { *result = sqrt(v); }
static void absF64(f64 v, f64 *result) { *result = fromBitsF64(bitsF64(v) & 0x7fffffffffffffffULL); }
static void negateF64(f64 v, f64 *result) { *result = fromBitsF64(bitsF64(v) ^ 0x8000000000000000ULL); }
static void floorF64(f64 v, f64 *result) { *result = floor(v); }
static void ceilF64(f64 v, f64 *result) { *result = ceil(v); }
static void roundEvenF64(f64 v, f64 *result) { *result = nearbyint(v); }
static void truncateF64(f64 v, f64 *result) { *result = trunc(v); }

static u8 equalF64(f64 a, f64 b) { return a == b; }
static u8 notEqualF64(f64 a, f64 b) { return a != b; }
static u8 lessF64(f64 a, f64 b) { return a < b; }
static u8 lessEqualF64(f64 a, f64 b) { return a <= b; }
static u8 greaterF64(f64 a, f64 b) { return a > b; }
static u8 greaterEqualF64(f64 a, f64 b) { return a >= b; }

static i32 saturateI32(f32 v)
{
	if (v != v)
		return 0;

	if (v >= 2147483648.0f)
		return CRUST_I32_MAX;

	if (v <= -2147483648.0f)
		return CRUST_I32_MIN;

	return (i32)v;
}

//
static u8 checkUnaryF32(PFN_simdUnary simd, PFN_unaryF32 reference, u8 exact)
{
	for (u32 i = 0; i < INPUTS_F32; ++i)
	{
		f32 a[4];
		f32 b[4];
		inputF32(i, a, b);

		f32 result[4];
		crustSimd128StoreF32(result, simd(crustSimd128LoadF32(a)));

		for (u32 lane = 0; lane < 4; ++lane)
		{
			f32 expected;
			reference(a[lane], &expected);

			if (!sameF32(expected, result[lane], exact))
				return 0;
		}
	}

	return 1;
}

static u8 checkBinaryF32(PFN_simdBinary simd, PFN_binaryF32 reference, u8 exact)
{
	for (u32 i = 0; i < INPUTS_F32; ++i)
	{
		f32 a[4];
		f32 b[4];
		inputF32(i, a, b);

		f32 result[4];
		crustSimd128StoreF32(result, simd(crustSimd128LoadF32(a), crustSimd128LoadF32(b)));

		for (u32 lane = 0; lane < 4; ++lane)
		{
			f32 expected;
			reference(a[lane], b[lane], &expected);

			if (!sameF32(expected, result[lane], exact))
				return 0;
		}
	}

	return 1;
}

static u8 checkCompareF32(PFN_simdBinary simd, PFN_compareF32 reference)
{
	for (u32 i = 0; i < INPUTS_F32; ++i)
	{
		f32 a[4];
		f32 b[4];
		inputF32(i, a, b);

		u32 result[4];
		crustSimd128StoreU32(result, simd(crustSimd128LoadF32(a), crustSimd128LoadF32(b)));

		for (u32 lane = 0; lane < 4; ++lane)
		{
			if (result[lane] != (reference(a[lane], b[lane]) ? CRUST_U32_MAX : 0u))
				return 0;
		}
	}

	return 1;
}

static u8 checkUnaryF64(PFN_simdUnary simd, PFN_unaryF64 reference, u8 exact)
{
	for (u32 i = 0; i < INPUTS_F64; ++i)
	{
		f64 a[2];
		f64 b[2];
		inputF64(i, a, b);

		f64 result[2];
		crustSimd128StoreF64(result, simd(crustSimd128LoadF64(a)));

		for (u32 lane = 0; lane < 2; ++lane)
		{
			f64 expected;
			reference(a[lane], &expected);

			if (!sameF64(expected, result[lane], exact))
				return 0;
		}
	}

	return 1;
}

static u8 checkBinaryF64(PFN_simdBinary simd, PFN_binaryF64 reference, u8 exact)
{
	for (u32 i = 0; i < INPUTS_F64; ++i)
	{
		f64 a[2];
		f64 b[2];
		inputF64(i, a, b);

		f64 result[2];
		crustSimd128StoreF64(result, simd(crustSimd128LoadF64(a), crustSimd128LoadF64(b)));

		for (u32 lane = 0; lane < 2; ++lane)
		{
			f64 expected;
			reference(a[lane], b[lane], &expected);

			if (!sameF64(expected, result[lane], exact))
				return 0;
		}
	}

	return 1;
}

static u8 checkCompareF64(PFN_simdBinary simd, PFN_compareF64 reference)
{
	for (u32 i = 0; i < INPUTS_F64; ++i)
	{
		f64 a[2];
		f64 b[2];
		inputF64(i, a, b);

		u64 result[2];
		crustSimd128StoreU64(result, simd(crustSimd128LoadF64(a), crustSimd128LoadF64(b)));

		for (u32 lane = 0; lane < 2; ++lane)
		{
			if (result[lane] != (reference(a[lane], b[lane]) ? CRUST_U64_MAX : 0u))
				return 0;
		}
	}

	return 1;
}

//
static u8 checkConvertF32FromI32(void)
{
	static const i32 special[] = { 0, 1, -1, 16777217, -16777217, CRUST_I32_MAX, CRUST_I32_MIN };

	for (u32 i = 0; i < ITERATIONS; ++i)
	{
		i32 x[4];
		for (u32 lane = 0; lane < 4; ++lane)
			x[lane] = (i < 7) ? special[i] : (i32)crustTestRandom();

		f32 result[4];
		crustSimd128StoreF32(result, crustSimd128ConvertF32FromI32(crustSimd128LoadI32(x)));

		for (u32 lane = 0; lane < 4; ++lane)
		{
			if (!sameF32((f32)x[lane], result[lane], 1))
				return 0;
		}
	}

	return 1;
}

static u8 checkConvertI32FromF32(void)
{
	for (u32 i = 0; i < INPUTS_F32; ++i)
	{
		f32 a[4];
		f32 b[4];
		inputF32(i, a, b);

		for (u32 lane = 0; lane < 4; ++lane)
		{
			if (!(crustAbsF32(a[lane]) < 2147483648.0f))
				a[lane] = (f32)(crustTestRandomF64() * 4.0e9 - 2.0e9);
		}

		i32 result[4];
		crustSimd128StoreI32(result, crustSimd128ConvertI32FromF32(crustSimd128LoadF32(a)));

		for (u32 lane = 0; lane < 4; ++lane)
		{
			if (result[lane] != (i32)a[lane])
				return 0;
		}
	}

	return 1;
}

static u8 checkConvertI32FromF32Saturate(void)
{
	for (u32 i = 0; i < INPUTS_F32; ++i)
	{
		f32 a[4];
		f32 b[4];
		inputF32(i, a, b);

		i32 result[4];
		crustSimd128StoreI32(result, crustSimd128ConvertI32FromF32Saturate(crustSimd128LoadF32(a)));

		for (u32 lane = 0; lane < 4; ++lane)
		{
			if (result[lane] != saturateI32(a[lane]))
				return 0;
		}
	}

	return 1;
}

static u8 checkConvertF64FromF32Low(void)
{
	for (u32 i = 0; i < INPUTS_F32; ++i)
	{
		f32 a[4];
		f32 b[4];
		inputF32(i, a, b);

		a[1] = b[0];

		f64 result[2];
		crustSimd128StoreF64(result, crustSimd128ConvertF64FromF32Low(crustSimd128LoadF32(a)));

		for (u32 lane = 0; lane < 2; ++lane)
		{
			if (!sameF64((f64)a[lane], result[lane], 0))
				return 0;
		}
	}

	return 1;
}

static u8 checkConvertF64FromF32High(void)
{
	for (u32 i = 0; i < INPUTS_F32; ++i)
	{
		f32 a[4];
		f32 b[4];
		inputF32(i, a, b);

		a[3] = b[0];

		f64 result[2];
		crustSimd128StoreF64(result, crustSimd128ConvertF64FromF32High(crustSimd128LoadF32(a)));

		for (u32 lane = 0; lane < 2; ++lane)
		{
			if (!sameF64((f64)a[lane + 2], result[lane], 0))
				return 0;
		}
	}

	return 1;
}

static u8 checkConvertF32FromF64(void)
{
	for (u32 i = 0; i < INPUTS_F64; ++i)
	{
		f64 a[2];
		f64 b[2];
		inputF64(i, a, b);

		a[1] = b[0];

		f32 result[4];
		crustSimd128StoreF32(result, crustSimd128ConvertF32FromF64(crustSimd128LoadF64(a), crustSimd128LoadF64(b)));

		for (u32 lane = 0; lane < 2; ++lane)
		{
			if (!sameF32((f32)a[lane], result[lane], 0))
				return 0;

			if (!sameF32((f32)b[lane], result[lane + 2], 0))
				return 0;
		}
	}

	return 1;
}

typedef Crust_Simd128 (*PFN_simdShift)(Crust_Simd128 v, u32 count);
typedef u64 (*PFN_referenceInteger)(u64 a, u64 b, u32 bits);
typedef u64 (*PFN_referenceShift)(u64 v, u32 count, u32 bits);

//
static u64 maskOf(u32 bits)
{
	return (bits == 64) ? CRUST_U64_MAX : ((u64)1 << bits) - 1;
}

static u64 signOf(u32 bits)
{
	return (u64)1 << (bits - 1);
}

static i64 signedOf(u64 v, u32 bits)
{
	CRUST_ASSERT(bits < 64);
	return (i64)(v ^ signOf(bits)) - (i64)signOf(bits);
}

static u8 lessSigned(u64 a, u64 b, u32 bits)
{
	return (a ^ signOf(bits)) < (b ^ signOf(bits));
}

static u64 maskIf(u8 condition, u32 bits)
{
	return condition ? maskOf(bits) : 0;
}

static u64 saturateSigned(i64 v, u32 bits)
{
	return (u64)crustClampI64(v, -(i64)signOf(bits), (i64)signOf(bits) - 1) & maskOf(bits);
}

static void lanesOf(Crust_Simd128 v, u32 bits, u64 *lanes)
{
	switch (bits)
	{
		case 8:
		{
			u8 x[16];
			crustSimd128StoreU8(x, v);

			for (u32 i = 0; i < 16; ++i)
				lanes[i] = x[i];

			break;
		}

		case 16:
		{
			u16 x[8];
			crustSimd128StoreU16(x, v);

			for (u32 i = 0; i < 8; ++i)
				lanes[i] = x[i];

			break;
		}

		case 32:
		{
			u32 x[4];
			crustSimd128StoreU32(x, v);

			for (u32 i = 0; i < 4; ++i)
				lanes[i] = x[i];

			break;
		}

		default:
		{
			u64 x[2];
			crustSimd128StoreU64(x, v);

			for (u32 i = 0; i < 2; ++i)
				lanes[i] = x[i];

			break;
		}
	}
}

static Crust_Simd128 vectorOf(const u64 *lanes, u32 bits)
{
	switch (bits)
	{
		case 8:
		{
			u8 x[16];
			for (u32 i = 0; i < 16; ++i)
				x[i] = (u8)lanes[i];

			return crustSimd128LoadU8(x);
		}

		case 16:
		{
			u16 x[8];
			for (u32 i = 0; i < 8; ++i)
				x[i] = (u16)lanes[i];

			return crustSimd128LoadU16(x);
		}

		case 32:
		{
			u32 x[4];
			for (u32 i = 0; i < 4; ++i)
				x[i] = (u32)lanes[i];

			return crustSimd128LoadU32(x);
		}

		default:
			return crustSimd128LoadU64(lanes);
	}
}

static void randomLanes(u32 bits, u64 *lanes)
{
	u64 mask = maskOf(bits);
	u64 sign = signOf(bits);
	u64 special[8] = { 0, 1, 2, mask, mask - 1, sign, sign - 1, sign + 1 };

	for (u32 i = 0; i < 128 / bits; ++i)
		lanes[i] = ((crustTestRandom() % 2) ? special[crustTestRandom() % 8] : randomU64()) & mask;
}

//
static u64 referenceAdd(u64 a, u64 b, u32 bits) { return (a + b) & maskOf(bits); }
static u64 referenceSub(u64 a, u64 b, u32 bits) { return (a - b) & maskOf(bits); }
static u64 referenceMul(u64 a, u64 b, u32 bits) { return (a * b) & maskOf(bits); }

static u64 referenceAddSaturateUnsigned(u64 a, u64 b, u32 bits) { return crustMinU64(a + b, maskOf(bits)); }
static u64 referenceSubSaturateUnsigned(u64 a, u64 b, u32 bits) { CRUST_UNUSED(bits); return (a > b) ? a - b : 0; }
static u64 referenceAddSaturateSigned(u64 a, u64 b, u32 bits) { return saturateSigned(signedOf(a, bits) + signedOf(b, bits), bits); }
static u64 referenceSubSaturateSigned(u64 a, u64 b, u32 bits) { return saturateSigned(signedOf(a, bits) - signedOf(b, bits), bits); }

static u64 referenceMinUnsigned(u64 a, u64 b, u32 bits) { CRUST_UNUSED(bits); return crustMinU64(a, b); }
static u64 referenceMaxUnsigned(u64 a, u64 b, u32 bits) { CRUST_UNUSED(bits); return crustMaxU64(a, b); }
static u64 referenceMinSigned(u64 a, u64 b, u32 bits) { return lessSigned(a, b, bits) ? a : b; }
static u64 referenceMaxSigned(u64 a, u64 b, u32 bits) { return lessSigned(a, b, bits) ? b : a; }

static u64 referenceEqual(u64 a, u64 b, u32 bits) { return maskIf(a == b, bits); }
static u64 referenceNotEqual(u64 a, u64 b, u32 bits) { return maskIf(a != b, bits); }
static u64 referenceLessUnsigned(u64 a, u64 b, u32 bits) { return maskIf(a < b, bits); }
static u64 referenceLessSigned(u64 a, u64 b, u32 bits) { return maskIf(lessSigned(a, b, bits), bits); }
static u64 referenceLessEqualUnsigned(u64 a, u64 b, u32 bits) { return maskIf(a <= b, bits); }
static u64 referenceLessEqualSigned(u64 a, u64 b, u32 bits) { return maskIf(!lessSigned(b, a, bits), bits); }
static u64 referenceGreaterUnsigned(u64 a, u64 b, u32 bits) { return maskIf(a > b, bits); }
static u64 referenceGreaterSigned(u64 a, u64 b, u32 bits) { return maskIf(lessSigned(b, a, bits), bits); }
static u64 referenceGreaterEqualUnsigned(u64 a, u64 b, u32 bits) { return maskIf(a >= b, bits); }
static u64 referenceGreaterEqualSigned(u64 a, u64 b, u32 bits) { return maskIf(!lessSigned(a, b, bits), bits); }

static u64 referenceShiftLeft(u64 v, u32 count, u32 bits)
{
	return (v << count) & maskOf(bits);
}

static u64 referenceShiftRightLogical(u64 v, u32 count, u32 bits)
{
	CRUST_UNUSED(bits);
	return v >> count;
}

static u64 referenceShiftRightArithmetic(u64 v, u32 count, u32 bits)
{
	u64 sign = (v & signOf(bits)) ? maskOf(bits) : 0;
	return (((v ^ sign) >> count) ^ sign) & maskOf(bits);
}

//
static u8 checkBinaryInteger(PFN_simdBinary simd, PFN_referenceInteger reference, u32 bits)
{
	for (u32 i = 0; i < ITERATIONS; ++i)
	{
		u64 a[16];
		u64 b[16];
		u64 result[16];

		randomLanes(bits, a);
		randomLanes(bits, b);
		lanesOf(simd(vectorOf(a, bits), vectorOf(b, bits)), bits, result);

		for (u32 lane = 0; lane < 128 / bits; ++lane)
		{
			if (result[lane] != reference(a[lane], b[lane], bits))
				return 0;
		}
	}

	return 1;
}

static u8 checkShiftInteger(PFN_simdShift simd, PFN_referenceShift reference, u32 bits)
{
	for (u32 i = 0; i < ITERATIONS; ++i)
	{
		u32 count = (i < bits) ? i : crustTestRandom() % bits;

		u64 v[16];
		u64 result[16];

		randomLanes(bits, v);
		lanesOf(simd(vectorOf(v, bits), count), bits, result);

		for (u32 lane = 0; lane < 128 / bits; ++lane)
		{
			if (result[lane] != reference(v[lane], count, bits))
				return 0;
		}
	}

	return 1;
}

static u8 checkWidenInteger(PFN_simdUnary simd, u32 source_bits, u8 high, u8 is_signed)
{
	u32 count = 64 / source_bits;
	u64 extension = maskOf(2 * source_bits) & ~maskOf(source_bits);

	for (u32 i = 0; i < ITERATIONS; ++i)
	{
		u64 v[16];
		u64 result[8];

		randomLanes(source_bits, v);
		lanesOf(simd(vectorOf(v, source_bits)), 2 * source_bits, result);

		for (u32 lane = 0; lane < count; ++lane)
		{
			u64 value = v[lane + (high ? count : 0)];
			u64 expected = (is_signed && (value & signOf(source_bits))) ? value | extension : value;

			if (result[lane] != expected)
				return 0;
		}
	}

	return 1;
}

static u8 checkNarrowInteger(PFN_simdBinary simd, u32 source_bits, u8 signed_output)
{
	u32 count = 128 / source_bits;
	u32 result_bits = source_bits / 2;
	i64 minimum = signed_output ? -(i64)signOf(result_bits) : 0;
	i64 maximum = signed_output ? (i64)signOf(result_bits) - 1 : (i64)maskOf(result_bits);

	for (u32 i = 0; i < ITERATIONS; ++i)
	{
		u64 low[8];
		u64 high[8];
		u64 result[16];

		randomLanes(source_bits, low);
		randomLanes(source_bits, high);
		lanesOf(simd(vectorOf(low, source_bits), vectorOf(high, source_bits)), result_bits, result);

		for (u32 lane = 0; lane < count; ++lane)
		{
			u64 expected_low = (u64)crustClampI64(signedOf(low[lane], source_bits), minimum, maximum) & maskOf(result_bits);
			u64 expected_high = (u64)crustClampI64(signedOf(high[lane], source_bits), minimum, maximum) & maskOf(result_bits);

			if (result[lane] != expected_low || result[lane + count] != expected_high)
				return 0;
		}
	}

	return 1;
}

//
static u8 checkZip(PFN_simdBinary simd, u32 bits, u8 high)
{
	u32 half = 64 / bits;
	u32 offset = high ? half : 0;

	for (u32 i = 0; i < ITERATIONS; ++i)
	{
		u64 a[16];
		u64 b[16];
		u64 result[16];

		randomLanes(bits, a);
		randomLanes(bits, b);
		lanesOf(simd(vectorOf(a, bits), vectorOf(b, bits)), bits, result);

		for (u32 lane = 0; lane < half; ++lane)
		{
			if (result[2 * lane] != a[offset + lane] || result[2 * lane + 1] != b[offset + lane])
				return 0;
		}
	}

	return 1;
}

static u8 checkSwizzle32(PFN_simdUnary simd, u32 lane0, u32 lane1, u32 lane2, u32 lane3)
{
	u32 pattern[4] = { lane0, lane1, lane2, lane3 };

	for (u32 i = 0; i < ITERATIONS; ++i)
	{
		u64 v[16];
		u64 result[16];

		randomLanes(32, v);
		lanesOf(simd(vectorOf(v, 32)), 32, result);

		for (u32 lane = 0; lane < 4; ++lane)
		{
			if (result[lane] != v[pattern[lane]])
				return 0;
		}
	}

	return 1;
}

static u8 checkSwizzle64(PFN_simdUnary simd, u32 lane0, u32 lane1)
{
	u32 pattern[2] = { lane0, lane1 };

	for (u32 i = 0; i < ITERATIONS; ++i)
	{
		u64 v[16];
		u64 result[16];

		randomLanes(64, v);
		lanesOf(simd(vectorOf(v, 64)), 64, result);

		for (u32 lane = 0; lane < 2; ++lane)
		{
			if (result[lane] != v[pattern[lane]])
				return 0;
		}
	}

	return 1;
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
		crustSimd128StoreU8(dst + offset + 1, crustSimd128LoadU8(src + offset));

		for (u32 i = 0; i < 48; ++i)
		{
			u8 inside = i >= offset + 1 && i < offset + 17;
			CRUST_TEST_CHECK(dst[i] == (inside ? src[i - 1] : 0));
		}
	}

	u8 u8_src[16];
	i8 i8_src[16];
	u16 u16_src[8];
	i16 i16_src[8];
	u32 u32_src[4];
	i32 i32_src[4];
	u64 u64_src[2];
	i64 i64_src[2];
	f32 f32_src[4];
	f64 f64_src[2];

	for (u32 i = 0; i < 16; ++i)
	{
		u8_src[i] = (u8)crustTestRandom();
		i8_src[i] = (i8)(i * 13 - 100);
	}

	for (u32 i = 0; i < 8; ++i)
	{
		u16_src[i] = (u16)crustTestRandom();
		i16_src[i] = (i16)(i * 1000 - 30000);
	}

	for (u32 i = 0; i < 4; ++i)
	{
		u32_src[i] = crustTestRandom();
		i32_src[i] = (i32)i * 100000 - 2000000000;
		f32_src[i] = (f32)i * 1.5f - 2.0f;
	}

	for (u32 i = 0; i < 2; ++i)
	{
		u64_src[i] = randomU64();
		i64_src[i] = (i64)i * 7 - 9000000000000000000LL;
		f64_src[i] = (f64)i * 2.5 - 1.0;
	}

	u8 u8_dst[16];
	i8 i8_dst[16];
	u16 u16_dst[8];
	i16 i16_dst[8];
	u32 u32_dst[4];
	i32 i32_dst[4];
	u64 u64_dst[2];
	i64 i64_dst[2];
	f32 f32_dst[4];
	f64 f64_dst[2];

	crustSimd128StoreU8(u8_dst, crustSimd128LoadU8(u8_src));
	crustSimd128StoreI8(i8_dst, crustSimd128LoadI8(i8_src));
	crustSimd128StoreU16(u16_dst, crustSimd128LoadU16(u16_src));
	crustSimd128StoreI16(i16_dst, crustSimd128LoadI16(i16_src));
	crustSimd128StoreU32(u32_dst, crustSimd128LoadU32(u32_src));
	crustSimd128StoreI32(i32_dst, crustSimd128LoadI32(i32_src));
	crustSimd128StoreU64(u64_dst, crustSimd128LoadU64(u64_src));
	crustSimd128StoreI64(i64_dst, crustSimd128LoadI64(i64_src));
	crustSimd128StoreF32(f32_dst, crustSimd128LoadF32(f32_src));
	crustSimd128StoreF64(f64_dst, crustSimd128LoadF64(f64_src));

	CRUST_TEST_CHECK(memcmp(u8_dst, u8_src, 16) == 0);
	CRUST_TEST_CHECK(memcmp(i8_dst, i8_src, 16) == 0);
	CRUST_TEST_CHECK(memcmp(u16_dst, u16_src, 16) == 0);
	CRUST_TEST_CHECK(memcmp(i16_dst, i16_src, 16) == 0);
	CRUST_TEST_CHECK(memcmp(u32_dst, u32_src, 16) == 0);
	CRUST_TEST_CHECK(memcmp(i32_dst, i32_src, 16) == 0);
	CRUST_TEST_CHECK(memcmp(u64_dst, u64_src, 16) == 0);
	CRUST_TEST_CHECK(memcmp(i64_dst, i64_src, 16) == 0);
	CRUST_TEST_CHECK(memcmp(f32_dst, f32_src, 16) == 0);
	CRUST_TEST_CHECK(memcmp(f64_dst, f64_src, 16) == 0);

	CRUST_ALIGNAS(16) u8 aligned_u8[16];
	CRUST_ALIGNAS(16) i8 aligned_i8[16];
	CRUST_ALIGNAS(16) u16 aligned_u16[8];
	CRUST_ALIGNAS(16) i16 aligned_i16[8];
	CRUST_ALIGNAS(16) u32 aligned_u32[4];
	CRUST_ALIGNAS(16) i32 aligned_i32[4];
	CRUST_ALIGNAS(16) u64 aligned_u64[2];
	CRUST_ALIGNAS(16) i64 aligned_i64[2];
	CRUST_ALIGNAS(16) f32 aligned_f32[4];
	CRUST_ALIGNAS(16) f64 aligned_f64[2];

	crustSimd128StoreAlignedU8(aligned_u8, crustSimd128LoadU8(u8_src));
	crustSimd128StoreAlignedI8(aligned_i8, crustSimd128LoadI8(i8_src));
	crustSimd128StoreAlignedU16(aligned_u16, crustSimd128LoadU16(u16_src));
	crustSimd128StoreAlignedI16(aligned_i16, crustSimd128LoadI16(i16_src));
	crustSimd128StoreAlignedU32(aligned_u32, crustSimd128LoadU32(u32_src));
	crustSimd128StoreAlignedI32(aligned_i32, crustSimd128LoadI32(i32_src));
	crustSimd128StoreAlignedU64(aligned_u64, crustSimd128LoadU64(u64_src));
	crustSimd128StoreAlignedI64(aligned_i64, crustSimd128LoadI64(i64_src));
	crustSimd128StoreAlignedF32(aligned_f32, crustSimd128LoadF32(f32_src));
	crustSimd128StoreAlignedF64(aligned_f64, crustSimd128LoadF64(f64_src));

	crustSimd128StoreU8(u8_dst, crustSimd128LoadAlignedU8(aligned_u8));
	crustSimd128StoreI8(i8_dst, crustSimd128LoadAlignedI8(aligned_i8));
	crustSimd128StoreU16(u16_dst, crustSimd128LoadAlignedU16(aligned_u16));
	crustSimd128StoreI16(i16_dst, crustSimd128LoadAlignedI16(aligned_i16));
	crustSimd128StoreU32(u32_dst, crustSimd128LoadAlignedU32(aligned_u32));
	crustSimd128StoreI32(i32_dst, crustSimd128LoadAlignedI32(aligned_i32));
	crustSimd128StoreU64(u64_dst, crustSimd128LoadAlignedU64(aligned_u64));
	crustSimd128StoreI64(i64_dst, crustSimd128LoadAlignedI64(aligned_i64));
	crustSimd128StoreF32(f32_dst, crustSimd128LoadAlignedF32(aligned_f32));
	crustSimd128StoreF64(f64_dst, crustSimd128LoadAlignedF64(aligned_f64));

	CRUST_TEST_CHECK(memcmp(u8_dst, u8_src, 16) == 0);
	CRUST_TEST_CHECK(memcmp(i8_dst, i8_src, 16) == 0);
	CRUST_TEST_CHECK(memcmp(u16_dst, u16_src, 16) == 0);
	CRUST_TEST_CHECK(memcmp(i16_dst, i16_src, 16) == 0);
	CRUST_TEST_CHECK(memcmp(u32_dst, u32_src, 16) == 0);
	CRUST_TEST_CHECK(memcmp(i32_dst, i32_src, 16) == 0);
	CRUST_TEST_CHECK(memcmp(u64_dst, u64_src, 16) == 0);
	CRUST_TEST_CHECK(memcmp(i64_dst, i64_src, 16) == 0);
	CRUST_TEST_CHECK(memcmp(f32_dst, f32_src, 16) == 0);
	CRUST_TEST_CHECK(memcmp(f64_dst, f64_src, 16) == 0);

	u8 bytes[16];
	crustSimd128StoreU8(bytes, crustSimd128AllZeros());
	for (u32 i = 0; i < 16; ++i)
		CRUST_TEST_CHECK(bytes[i] == 0);

	crustSimd128StoreU8(bytes, crustSimd128AllOnes());
	for (u32 i = 0; i < 16; ++i)
		CRUST_TEST_CHECK(bytes[i] == CRUST_U8_MAX);

	return 0;
}

static int testConstructors(void)
{
	u8 u8_lanes[16];
	i8 i8_lanes[16];
	u16 u16_lanes[8];
	i16 i16_lanes[8];
	u32 u32_lanes[4];
	i32 i32_lanes[4];
	u64 u64_lanes[2];
	i64 i64_lanes[2];
	f32 f32_lanes[4];
	f64 f64_lanes[2];

	crustSimd128StoreU8(u8_lanes, crustSimd128SplatU8(0xab));
	crustSimd128StoreI8(i8_lanes, crustSimd128SplatI8(-2));
	crustSimd128StoreU16(u16_lanes, crustSimd128SplatU16(0xabcd));
	crustSimd128StoreI16(i16_lanes, crustSimd128SplatI16(-2));

	for (u32 i = 0; i < 16; ++i)
	{
		CRUST_TEST_CHECK(u8_lanes[i] == 0xab);
		CRUST_TEST_CHECK(i8_lanes[i] == -2);
	}

	for (u32 i = 0; i < 8; ++i)
	{
		CRUST_TEST_CHECK(u16_lanes[i] == 0xabcd);
		CRUST_TEST_CHECK(i16_lanes[i] == -2);
	}

	crustSimd128StoreU32(u32_lanes, crustSimd128SplatU32(0xdeadbeefu));
	crustSimd128StoreI32(i32_lanes, crustSimd128SplatI32(-2));
	crustSimd128StoreF32(f32_lanes, crustSimd128SplatF32(-0.0f));

	for (u32 i = 0; i < 4; ++i)
	{
		CRUST_TEST_CHECK(u32_lanes[i] == 0xdeadbeefu);
		CRUST_TEST_CHECK(i32_lanes[i] == -2);
		CRUST_TEST_CHECK(bitsF32(f32_lanes[i]) == 0x80000000u);
	}

	crustSimd128StoreU64(u64_lanes, crustSimd128SplatU64(0x0123456789abcdefULL));
	crustSimd128StoreI64(i64_lanes, crustSimd128SplatI64(-2));
	crustSimd128StoreF64(f64_lanes, crustSimd128SplatF64(-1.5));

	for (u32 i = 0; i < 2; ++i)
	{
		CRUST_TEST_CHECK(u64_lanes[i] == 0x0123456789abcdefULL);
		CRUST_TEST_CHECK(i64_lanes[i] == -2);
		CRUST_TEST_CHECK(f64_lanes[i] == -1.5);
	}

	crustSimd128StoreU32(u32_lanes, crustSimd128MakeU32(1, 2, 3, 0xffffffffu));
	CRUST_TEST_CHECK(u32_lanes[0] == 1 && u32_lanes[1] == 2 && u32_lanes[2] == 3 && u32_lanes[3] == 0xffffffffu);

	crustSimd128StoreI32(i32_lanes, crustSimd128MakeI32(-1, 2, -3, CRUST_I32_MIN));
	CRUST_TEST_CHECK(i32_lanes[0] == -1 && i32_lanes[1] == 2 && i32_lanes[2] == -3 && i32_lanes[3] == CRUST_I32_MIN);

	crustSimd128StoreF32(f32_lanes, crustSimd128MakeF32(1.0f, 2.0f, 3.0f, 4.0f));
	CRUST_TEST_CHECK(f32_lanes[0] == 1.0f && f32_lanes[1] == 2.0f && f32_lanes[2] == 3.0f && f32_lanes[3] == 4.0f);

	crustSimd128StoreU64(u64_lanes, crustSimd128MakeU64(1, CRUST_U64_MAX));
	CRUST_TEST_CHECK(u64_lanes[0] == 1 && u64_lanes[1] == CRUST_U64_MAX);

	crustSimd128StoreI64(i64_lanes, crustSimd128MakeI64(-1, CRUST_I64_MIN));
	CRUST_TEST_CHECK(i64_lanes[0] == -1 && i64_lanes[1] == CRUST_I64_MIN);

	crustSimd128StoreF64(f64_lanes, crustSimd128MakeF64(1.5, -2.5));
	CRUST_TEST_CHECK(f64_lanes[0] == 1.5 && f64_lanes[1] == -2.5);

	return 0;
}

static int testLanes(void)
{
	for (u32 i = 0; i < ITERATIONS; ++i)
	{
		f32 floats[4] = { randomF32(), randomF32(), randomF32(), randomF32() };
		Crust_Simd128 v = (i % 2) ? crustSimd128MakeU64(randomU64(), randomU64()) : crustSimd128LoadF32(floats);

		u8 u8_lanes[16];
		i8 i8_lanes[16];
		u16 u16_lanes[8];
		i16 i16_lanes[8];
		u32 u32_lanes[4];
		i32 i32_lanes[4];
		u64 u64_lanes[2];
		i64 i64_lanes[2];
		f32 f32_lanes[4];
		f64 f64_lanes[2];

		crustSimd128StoreU8(u8_lanes, v);
		crustSimd128StoreI8(i8_lanes, v);
		crustSimd128StoreU16(u16_lanes, v);
		crustSimd128StoreI16(i16_lanes, v);
		crustSimd128StoreU32(u32_lanes, v);
		crustSimd128StoreI32(i32_lanes, v);
		crustSimd128StoreU64(u64_lanes, v);
		crustSimd128StoreI64(i64_lanes, v);
		crustSimd128StoreF32(f32_lanes, v);
		crustSimd128StoreF64(f64_lanes, v);

		for (u32 lane = 0; lane < 16; ++lane)
		{
			CRUST_TEST_CHECK(crustSimd128GetLaneU8(v, lane) == u8_lanes[lane]);
			CRUST_TEST_CHECK(crustSimd128GetLaneI8(v, lane) == i8_lanes[lane]);
		}

		for (u32 lane = 0; lane < 8; ++lane)
		{
			CRUST_TEST_CHECK(crustSimd128GetLaneU16(v, lane) == u16_lanes[lane]);
			CRUST_TEST_CHECK(crustSimd128GetLaneI16(v, lane) == i16_lanes[lane]);
		}

		for (u32 lane = 0; lane < 4; ++lane)
		{
			CRUST_TEST_CHECK(crustSimd128GetLaneU32(v, lane) == u32_lanes[lane]);
			CRUST_TEST_CHECK(crustSimd128GetLaneI32(v, lane) == i32_lanes[lane]);
			CRUST_TEST_CHECK(sameF32(f32_lanes[lane], crustSimd128GetLaneF32(v, lane), 0));
		}

		for (u32 lane = 0; lane < 2; ++lane)
		{
			CRUST_TEST_CHECK(crustSimd128GetLaneU64(v, lane) == u64_lanes[lane]);
			CRUST_TEST_CHECK(crustSimd128GetLaneI64(v, lane) == i64_lanes[lane]);
			CRUST_TEST_CHECK(sameF64(f64_lanes[lane], crustSimd128GetLaneF64(v, lane), 0));
		}

		u32 lane = crustTestRandom();

		u8_lanes[lane % 16] = (u8)crustTestRandom();
		i8_lanes[lane % 16] = (i8)crustTestRandom();
		u16_lanes[lane % 8] = (u16)crustTestRandom();
		i16_lanes[lane % 8] = (i16)crustTestRandom();
		u32_lanes[lane % 4] = crustTestRandom();
		i32_lanes[lane % 4] = (i32)crustTestRandom();
		u64_lanes[lane % 2] = randomU64();
		i64_lanes[lane % 2] = (i64)randomU64();
		f32_lanes[lane % 4] = (f32)crustTestRandomF64();
		f64_lanes[lane % 2] = crustTestRandomF64();

		u8 u8_result[16];
		i8 i8_result[16];
		u16 u16_result[8];
		i16 i16_result[8];
		u32 u32_result[4];
		i32 i32_result[4];
		u64 u64_result[2];
		i64 i64_result[2];

		crustSimd128StoreU8(u8_result, crustSimd128SetLaneU8(v, lane % 16, u8_lanes[lane % 16]));
		crustSimd128StoreI8(i8_result, crustSimd128SetLaneI8(v, lane % 16, i8_lanes[lane % 16]));
		crustSimd128StoreU16(u16_result, crustSimd128SetLaneU16(v, lane % 8, u16_lanes[lane % 8]));
		crustSimd128StoreI16(i16_result, crustSimd128SetLaneI16(v, lane % 8, i16_lanes[lane % 8]));
		crustSimd128StoreU32(u32_result, crustSimd128SetLaneU32(v, lane % 4, u32_lanes[lane % 4]));
		crustSimd128StoreI32(i32_result, crustSimd128SetLaneI32(v, lane % 4, i32_lanes[lane % 4]));
		crustSimd128StoreU64(u64_result, crustSimd128SetLaneU64(v, lane % 2, u64_lanes[lane % 2]));
		crustSimd128StoreI64(i64_result, crustSimd128SetLaneI64(v, lane % 2, i64_lanes[lane % 2]));

		CRUST_TEST_CHECK(memcmp(u8_result, u8_lanes, 16) == 0);
		CRUST_TEST_CHECK(memcmp(i8_result, i8_lanes, 16) == 0);
		CRUST_TEST_CHECK(memcmp(u16_result, u16_lanes, 16) == 0);
		CRUST_TEST_CHECK(memcmp(i16_result, i16_lanes, 16) == 0);
		CRUST_TEST_CHECK(memcmp(u32_result, u32_lanes, 16) == 0);
		CRUST_TEST_CHECK(memcmp(i32_result, i32_lanes, 16) == 0);
		CRUST_TEST_CHECK(memcmp(u64_result, u64_lanes, 16) == 0);
		CRUST_TEST_CHECK(memcmp(i64_result, i64_lanes, 16) == 0);

		f32 f32_result[4];
		f64 f64_result[2];

		crustSimd128StoreF32(f32_result, crustSimd128SetLaneF32(v, lane % 4, f32_lanes[lane % 4]));
		crustSimd128StoreF64(f64_result, crustSimd128SetLaneF64(v, lane % 2, f64_lanes[lane % 2]));

		for (u32 j = 0; j < 4; ++j)
			CRUST_TEST_CHECK(sameF32(f32_lanes[j], f32_result[j], j != lane % 4));

		for (u32 j = 0; j < 2; ++j)
			CRUST_TEST_CHECK(sameF64(f64_lanes[j], f64_result[j], j != lane % 2));
	}

	return 0;
}

static int testZip(void)
{
	CRUST_TEST_CHECK(checkZip(crustSimd128ZipLowU8, 8, 0));
	CRUST_TEST_CHECK(checkZip(crustSimd128ZipLowI8, 8, 0));
	CRUST_TEST_CHECK(checkZip(crustSimd128ZipLowU16, 16, 0));
	CRUST_TEST_CHECK(checkZip(crustSimd128ZipLowI16, 16, 0));
	CRUST_TEST_CHECK(checkZip(crustSimd128ZipLowU32, 32, 0));
	CRUST_TEST_CHECK(checkZip(crustSimd128ZipLowI32, 32, 0));
	CRUST_TEST_CHECK(checkZip(crustSimd128ZipLowU64, 64, 0));
	CRUST_TEST_CHECK(checkZip(crustSimd128ZipLowI64, 64, 0));
	CRUST_TEST_CHECK(checkZip(crustSimd128ZipLowF32, 32, 0));
	CRUST_TEST_CHECK(checkZip(crustSimd128ZipLowF64, 64, 0));

	CRUST_TEST_CHECK(checkZip(crustSimd128ZipHighU8, 8, 1));
	CRUST_TEST_CHECK(checkZip(crustSimd128ZipHighI8, 8, 1));
	CRUST_TEST_CHECK(checkZip(crustSimd128ZipHighU16, 16, 1));
	CRUST_TEST_CHECK(checkZip(crustSimd128ZipHighI16, 16, 1));
	CRUST_TEST_CHECK(checkZip(crustSimd128ZipHighU32, 32, 1));
	CRUST_TEST_CHECK(checkZip(crustSimd128ZipHighI32, 32, 1));
	CRUST_TEST_CHECK(checkZip(crustSimd128ZipHighU64, 64, 1));
	CRUST_TEST_CHECK(checkZip(crustSimd128ZipHighI64, 64, 1));
	CRUST_TEST_CHECK(checkZip(crustSimd128ZipHighF32, 32, 1));
	CRUST_TEST_CHECK(checkZip(crustSimd128ZipHighF64, 64, 1));

	return 0;
}

static int testSwizzle(void)
{
	CRUST_TEST_CHECK(checkSwizzle32(crustSimd128Swizzle32XYWZ, 0, 1, 3, 2));
	CRUST_TEST_CHECK(checkSwizzle32(crustSimd128Swizzle32XZYW, 0, 2, 1, 3));
	CRUST_TEST_CHECK(checkSwizzle32(crustSimd128Swizzle32XZWY, 0, 2, 3, 1));
	CRUST_TEST_CHECK(checkSwizzle32(crustSimd128Swizzle32XWYZ, 0, 3, 1, 2));
	CRUST_TEST_CHECK(checkSwizzle32(crustSimd128Swizzle32XWZY, 0, 3, 2, 1));
	CRUST_TEST_CHECK(checkSwizzle32(crustSimd128Swizzle32YXZW, 1, 0, 2, 3));
	CRUST_TEST_CHECK(checkSwizzle32(crustSimd128Swizzle32YXWZ, 1, 0, 3, 2));
	CRUST_TEST_CHECK(checkSwizzle32(crustSimd128Swizzle32YZXW, 1, 2, 0, 3));
	CRUST_TEST_CHECK(checkSwizzle32(crustSimd128Swizzle32YZWX, 1, 2, 3, 0));
	CRUST_TEST_CHECK(checkSwizzle32(crustSimd128Swizzle32YWXZ, 1, 3, 0, 2));
	CRUST_TEST_CHECK(checkSwizzle32(crustSimd128Swizzle32YWZX, 1, 3, 2, 0));
	CRUST_TEST_CHECK(checkSwizzle32(crustSimd128Swizzle32ZXYW, 2, 0, 1, 3));
	CRUST_TEST_CHECK(checkSwizzle32(crustSimd128Swizzle32ZXWY, 2, 0, 3, 1));
	CRUST_TEST_CHECK(checkSwizzle32(crustSimd128Swizzle32ZYXW, 2, 1, 0, 3));
	CRUST_TEST_CHECK(checkSwizzle32(crustSimd128Swizzle32ZYWX, 2, 1, 3, 0));
	CRUST_TEST_CHECK(checkSwizzle32(crustSimd128Swizzle32ZWXY, 2, 3, 0, 1));
	CRUST_TEST_CHECK(checkSwizzle32(crustSimd128Swizzle32ZWYX, 2, 3, 1, 0));
	CRUST_TEST_CHECK(checkSwizzle32(crustSimd128Swizzle32WXYZ, 3, 0, 1, 2));
	CRUST_TEST_CHECK(checkSwizzle32(crustSimd128Swizzle32WXZY, 3, 0, 2, 1));
	CRUST_TEST_CHECK(checkSwizzle32(crustSimd128Swizzle32WYXZ, 3, 1, 0, 2));
	CRUST_TEST_CHECK(checkSwizzle32(crustSimd128Swizzle32WYZX, 3, 1, 2, 0));
	CRUST_TEST_CHECK(checkSwizzle32(crustSimd128Swizzle32WZXY, 3, 2, 0, 1));
	CRUST_TEST_CHECK(checkSwizzle32(crustSimd128Swizzle32WZYX, 3, 2, 1, 0));
	CRUST_TEST_CHECK(checkSwizzle32(crustSimd128Swizzle32XXXX, 0, 0, 0, 0));
	CRUST_TEST_CHECK(checkSwizzle32(crustSimd128Swizzle32YYYY, 1, 1, 1, 1));
	CRUST_TEST_CHECK(checkSwizzle32(crustSimd128Swizzle32ZZZZ, 2, 2, 2, 2));
	CRUST_TEST_CHECK(checkSwizzle32(crustSimd128Swizzle32WWWW, 3, 3, 3, 3));

	CRUST_TEST_CHECK(checkSwizzle64(crustSimd128Swizzle64YX, 1, 0));
	CRUST_TEST_CHECK(checkSwizzle64(crustSimd128Swizzle64XX, 0, 0));
	CRUST_TEST_CHECK(checkSwizzle64(crustSimd128Swizzle64YY, 1, 1));

	return 0;
}

static int testBitwise(void)
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

		Crust_Simd128 vmask = crustSimd128LoadU32(mask);
		Crust_Simd128 va = crustSimd128LoadU32(a);
		Crust_Simd128 vb = crustSimd128LoadU32(b);

		u32 and_result[4];
		u32 or_result[4];
		u32 xor_result[4];
		u32 and_not_result[4];
		u32 not_result[4];
		u32 select_result[4];

		crustSimd128StoreU32(and_result, crustSimd128And(va, vb));
		crustSimd128StoreU32(or_result, crustSimd128Or(va, vb));
		crustSimd128StoreU32(xor_result, crustSimd128Xor(va, vb));
		crustSimd128StoreU32(and_not_result, crustSimd128AndNot(va, vb));
		crustSimd128StoreU32(not_result, crustSimd128Not(va));
		crustSimd128StoreU32(select_result, crustSimd128Select(vmask, va, vb));

		for (u32 lane = 0; lane < 4; ++lane)
		{
			CRUST_TEST_CHECK(and_result[lane] == (a[lane] & b[lane]));
			CRUST_TEST_CHECK(or_result[lane] == (a[lane] | b[lane]));
			CRUST_TEST_CHECK(xor_result[lane] == (a[lane] ^ b[lane]));
			CRUST_TEST_CHECK(and_not_result[lane] == (a[lane] & ~b[lane]));
			CRUST_TEST_CHECK(not_result[lane] == ~a[lane]);
			CRUST_TEST_CHECK(select_result[lane] == ((mask[lane] & a[lane]) | (~mask[lane] & b[lane])));
		}
	}

	return 0;
}

static int testMasks(void)
{
	CRUST_TEST_CHECK(crustSimd128Any(crustSimd128AllZeros()) == 0);
	CRUST_TEST_CHECK(crustSimd128All(crustSimd128AllZeros()) == 0);
	CRUST_TEST_CHECK(crustSimd128Any(crustSimd128AllOnes()) == 1);
	CRUST_TEST_CHECK(crustSimd128All(crustSimd128AllOnes()) == 1);

	for (u32 bit = 0; bit < 128; ++bit)
	{
		u8 bytes[16];
		crustMemset(bytes, 0, sizeof(bytes));
		bytes[bit / 8] = (u8)(1u << (bit % 8));

		Crust_Simd128 single = crustSimd128LoadU8(bytes);
		Crust_Simd128 hole = crustSimd128Not(single);

		CRUST_TEST_CHECK(crustSimd128Any(single) == 1);
		CRUST_TEST_CHECK(crustSimd128All(single) == 0);
		CRUST_TEST_CHECK(crustSimd128Any(hole) == 1);
		CRUST_TEST_CHECK(crustSimd128All(hole) == 0);
	}

	for (u32 i = 0; i < ITERATIONS; ++i)
	{
		u32 random[4];
		for (u32 lane = 0; lane < 4; ++lane)
			random[lane] = crustTestRandom();

		Crust_Simd128 v = crustSimd128LoadU32(random);

		u8 u8_lanes[16];
		u16 u16_lanes[8];
		u32 u32_lanes[4];
		u64 u64_lanes[2];

		crustSimd128StoreU8(u8_lanes, v);
		crustSimd128StoreU16(u16_lanes, v);
		crustSimd128StoreU32(u32_lanes, v);
		crustSimd128StoreU64(u64_lanes, v);

		u32 expected_u8 = 0;
		u32 expected_u16 = 0;
		u32 expected_u32 = 0;
		u32 expected_u64 = 0;

		for (u32 lane = 0; lane < 16; ++lane)
			expected_u8 |= (u32)(u8_lanes[lane] >> 7) << lane;

		for (u32 lane = 0; lane < 8; ++lane)
			expected_u16 |= (u32)(u16_lanes[lane] >> 15) << lane;

		for (u32 lane = 0; lane < 4; ++lane)
			expected_u32 |= (u32_lanes[lane] >> 31) << lane;

		for (u32 lane = 0; lane < 2; ++lane)
			expected_u64 |= (u32)(u64_lanes[lane] >> 63) << lane;

		CRUST_TEST_CHECK(crustSimd128BitmaskU8(v) == expected_u8);
		CRUST_TEST_CHECK(crustSimd128BitmaskU16(v) == expected_u16);
		CRUST_TEST_CHECK(crustSimd128BitmaskU32(v) == expected_u32);
		CRUST_TEST_CHECK(crustSimd128BitmaskU64(v) == expected_u64);
	}

	return 0;
}

static int testArithmeticInteger(void)
{
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128AddU8, referenceAdd, 8));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128AddI8, referenceAdd, 8));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128AddU16, referenceAdd, 16));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128AddI16, referenceAdd, 16));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128AddU32, referenceAdd, 32));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128AddI32, referenceAdd, 32));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128AddU64, referenceAdd, 64));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128AddI64, referenceAdd, 64));

	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128SubU8, referenceSub, 8));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128SubI8, referenceSub, 8));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128SubU16, referenceSub, 16));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128SubI16, referenceSub, 16));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128SubU32, referenceSub, 32));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128SubI32, referenceSub, 32));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128SubU64, referenceSub, 64));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128SubI64, referenceSub, 64));

	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128AddSaturateU8, referenceAddSaturateUnsigned, 8));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128AddSaturateI8, referenceAddSaturateSigned, 8));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128AddSaturateU16, referenceAddSaturateUnsigned, 16));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128AddSaturateI16, referenceAddSaturateSigned, 16));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128SubSaturateU8, referenceSubSaturateUnsigned, 8));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128SubSaturateI8, referenceSubSaturateSigned, 8));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128SubSaturateU16, referenceSubSaturateUnsigned, 16));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128SubSaturateI16, referenceSubSaturateSigned, 16));

	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128MulU16, referenceMul, 16));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128MulI16, referenceMul, 16));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128MulU32, referenceMul, 32));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128MulI32, referenceMul, 32));

	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128MinU8, referenceMinUnsigned, 8));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128MinI8, referenceMinSigned, 8));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128MinU16, referenceMinUnsigned, 16));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128MinI16, referenceMinSigned, 16));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128MinU32, referenceMinUnsigned, 32));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128MinI32, referenceMinSigned, 32));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128MinU64, referenceMinUnsigned, 64));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128MinI64, referenceMinSigned, 64));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128MaxU8, referenceMaxUnsigned, 8));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128MaxI8, referenceMaxSigned, 8));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128MaxU16, referenceMaxUnsigned, 16));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128MaxI16, referenceMaxSigned, 16));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128MaxU32, referenceMaxUnsigned, 32));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128MaxI32, referenceMaxSigned, 32));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128MaxU64, referenceMaxUnsigned, 64));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128MaxI64, referenceMaxSigned, 64));

	return 0;
}

static int testShiftInteger(void)
{
	CRUST_TEST_CHECK(checkShiftInteger(crustSimd128ShiftLeftU8, referenceShiftLeft, 8));
	CRUST_TEST_CHECK(checkShiftInteger(crustSimd128ShiftLeftI8, referenceShiftLeft, 8));
	CRUST_TEST_CHECK(checkShiftInteger(crustSimd128ShiftLeftU16, referenceShiftLeft, 16));
	CRUST_TEST_CHECK(checkShiftInteger(crustSimd128ShiftLeftI16, referenceShiftLeft, 16));
	CRUST_TEST_CHECK(checkShiftInteger(crustSimd128ShiftLeftU32, referenceShiftLeft, 32));
	CRUST_TEST_CHECK(checkShiftInteger(crustSimd128ShiftLeftI32, referenceShiftLeft, 32));
	CRUST_TEST_CHECK(checkShiftInteger(crustSimd128ShiftLeftU64, referenceShiftLeft, 64));
	CRUST_TEST_CHECK(checkShiftInteger(crustSimd128ShiftLeftI64, referenceShiftLeft, 64));

	CRUST_TEST_CHECK(checkShiftInteger(crustSimd128ShiftRightU8, referenceShiftRightLogical, 8));
	CRUST_TEST_CHECK(checkShiftInteger(crustSimd128ShiftRightI8, referenceShiftRightArithmetic, 8));
	CRUST_TEST_CHECK(checkShiftInteger(crustSimd128ShiftRightU16, referenceShiftRightLogical, 16));
	CRUST_TEST_CHECK(checkShiftInteger(crustSimd128ShiftRightI16, referenceShiftRightArithmetic, 16));
	CRUST_TEST_CHECK(checkShiftInteger(crustSimd128ShiftRightU32, referenceShiftRightLogical, 32));
	CRUST_TEST_CHECK(checkShiftInteger(crustSimd128ShiftRightI32, referenceShiftRightArithmetic, 32));
	CRUST_TEST_CHECK(checkShiftInteger(crustSimd128ShiftRightU64, referenceShiftRightLogical, 64));
	CRUST_TEST_CHECK(checkShiftInteger(crustSimd128ShiftRightI64, referenceShiftRightArithmetic, 64));

	return 0;
}

static int testCompareInteger(void)
{
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128EqualU8, referenceEqual, 8));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128EqualI8, referenceEqual, 8));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128EqualU16, referenceEqual, 16));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128EqualI16, referenceEqual, 16));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128EqualU32, referenceEqual, 32));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128EqualI32, referenceEqual, 32));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128EqualU64, referenceEqual, 64));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128EqualI64, referenceEqual, 64));

	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128NotEqualU8, referenceNotEqual, 8));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128NotEqualI8, referenceNotEqual, 8));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128NotEqualU16, referenceNotEqual, 16));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128NotEqualI16, referenceNotEqual, 16));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128NotEqualU32, referenceNotEqual, 32));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128NotEqualI32, referenceNotEqual, 32));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128NotEqualU64, referenceNotEqual, 64));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128NotEqualI64, referenceNotEqual, 64));

	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128LessU8, referenceLessUnsigned, 8));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128LessI8, referenceLessSigned, 8));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128LessU16, referenceLessUnsigned, 16));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128LessI16, referenceLessSigned, 16));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128LessU32, referenceLessUnsigned, 32));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128LessI32, referenceLessSigned, 32));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128LessU64, referenceLessUnsigned, 64));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128LessI64, referenceLessSigned, 64));

	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128LessEqualU8, referenceLessEqualUnsigned, 8));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128LessEqualI8, referenceLessEqualSigned, 8));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128LessEqualU16, referenceLessEqualUnsigned, 16));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128LessEqualI16, referenceLessEqualSigned, 16));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128LessEqualU32, referenceLessEqualUnsigned, 32));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128LessEqualI32, referenceLessEqualSigned, 32));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128LessEqualU64, referenceLessEqualUnsigned, 64));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128LessEqualI64, referenceLessEqualSigned, 64));

	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128GreaterU8, referenceGreaterUnsigned, 8));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128GreaterI8, referenceGreaterSigned, 8));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128GreaterU16, referenceGreaterUnsigned, 16));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128GreaterI16, referenceGreaterSigned, 16));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128GreaterU32, referenceGreaterUnsigned, 32));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128GreaterI32, referenceGreaterSigned, 32));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128GreaterU64, referenceGreaterUnsigned, 64));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128GreaterI64, referenceGreaterSigned, 64));

	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128GreaterEqualU8, referenceGreaterEqualUnsigned, 8));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128GreaterEqualI8, referenceGreaterEqualSigned, 8));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128GreaterEqualU16, referenceGreaterEqualUnsigned, 16));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128GreaterEqualI16, referenceGreaterEqualSigned, 16));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128GreaterEqualU32, referenceGreaterEqualUnsigned, 32));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128GreaterEqualI32, referenceGreaterEqualSigned, 32));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128GreaterEqualU64, referenceGreaterEqualUnsigned, 64));
	CRUST_TEST_CHECK(checkBinaryInteger(crustSimd128GreaterEqualI64, referenceGreaterEqualSigned, 64));

	return 0;
}

static int testConvertInteger(void)
{
	CRUST_TEST_CHECK(checkWidenInteger(crustSimd128ConvertU16FromU8Low, 8, 0, 0));
	CRUST_TEST_CHECK(checkWidenInteger(crustSimd128ConvertU16FromU8High, 8, 1, 0));
	CRUST_TEST_CHECK(checkWidenInteger(crustSimd128ConvertI16FromI8Low, 8, 0, 1));
	CRUST_TEST_CHECK(checkWidenInteger(crustSimd128ConvertI16FromI8High, 8, 1, 1));
	CRUST_TEST_CHECK(checkWidenInteger(crustSimd128ConvertU32FromU16Low, 16, 0, 0));
	CRUST_TEST_CHECK(checkWidenInteger(crustSimd128ConvertU32FromU16High, 16, 1, 0));
	CRUST_TEST_CHECK(checkWidenInteger(crustSimd128ConvertI32FromI16Low, 16, 0, 1));
	CRUST_TEST_CHECK(checkWidenInteger(crustSimd128ConvertI32FromI16High, 16, 1, 1));
	CRUST_TEST_CHECK(checkWidenInteger(crustSimd128ConvertU64FromU32Low, 32, 0, 0));
	CRUST_TEST_CHECK(checkWidenInteger(crustSimd128ConvertU64FromU32High, 32, 1, 0));
	CRUST_TEST_CHECK(checkWidenInteger(crustSimd128ConvertI64FromI32Low, 32, 0, 1));
	CRUST_TEST_CHECK(checkWidenInteger(crustSimd128ConvertI64FromI32High, 32, 1, 1));

	CRUST_TEST_CHECK(checkNarrowInteger(crustSimd128ConvertI8FromI16Saturate, 16, 1));
	CRUST_TEST_CHECK(checkNarrowInteger(crustSimd128ConvertU8FromI16Saturate, 16, 0));
	CRUST_TEST_CHECK(checkNarrowInteger(crustSimd128ConvertI16FromI32Saturate, 32, 1));
	CRUST_TEST_CHECK(checkNarrowInteger(crustSimd128ConvertU16FromI32Saturate, 32, 0));

	return 0;
}

static int testArithmeticF32(void)
{
	CRUST_TEST_CHECK(checkBinaryF32(crustSimd128AddF32, addF32, 0));
	CRUST_TEST_CHECK(checkBinaryF32(crustSimd128SubF32, subF32, 0));
	CRUST_TEST_CHECK(checkBinaryF32(crustSimd128MulF32, mulF32, 0));
	CRUST_TEST_CHECK(checkBinaryF32(crustSimd128DivF32, divF32, 0));
	CRUST_TEST_CHECK(checkBinaryF32(crustSimd128MinF32, minF32, 1));
	CRUST_TEST_CHECK(checkBinaryF32(crustSimd128MaxF32, maxF32, 1));

	CRUST_TEST_CHECK(checkUnaryF32(crustSimd128SqrtF32, sqrtF32, 0));
	CRUST_TEST_CHECK(checkUnaryF32(crustSimd128AbsF32, absF32, 1));
	CRUST_TEST_CHECK(checkUnaryF32(crustSimd128NegateF32, negateF32, 1));
	CRUST_TEST_CHECK(checkUnaryF32(crustSimd128FloorF32, floorF32, 0));
	CRUST_TEST_CHECK(checkUnaryF32(crustSimd128CeilF32, ceilF32, 0));
	CRUST_TEST_CHECK(checkUnaryF32(crustSimd128RoundEvenF32, roundEvenF32, 0));
	CRUST_TEST_CHECK(checkUnaryF32(crustSimd128TruncateF32, truncateF32, 0));

	return 0;
}

static int testCompareF32(void)
{
	CRUST_TEST_CHECK(checkCompareF32(crustSimd128EqualF32, equalF32));
	CRUST_TEST_CHECK(checkCompareF32(crustSimd128NotEqualF32, notEqualF32));
	CRUST_TEST_CHECK(checkCompareF32(crustSimd128LessF32, lessF32));
	CRUST_TEST_CHECK(checkCompareF32(crustSimd128LessEqualF32, lessEqualF32));
	CRUST_TEST_CHECK(checkCompareF32(crustSimd128GreaterF32, greaterF32));
	CRUST_TEST_CHECK(checkCompareF32(crustSimd128GreaterEqualF32, greaterEqualF32));

	return 0;
}

static int testArithmeticF64(void)
{
	CRUST_TEST_CHECK(checkBinaryF64(crustSimd128AddF64, addF64, 0));
	CRUST_TEST_CHECK(checkBinaryF64(crustSimd128SubF64, subF64, 0));
	CRUST_TEST_CHECK(checkBinaryF64(crustSimd128MulF64, mulF64, 0));
	CRUST_TEST_CHECK(checkBinaryF64(crustSimd128DivF64, divF64, 0));
	CRUST_TEST_CHECK(checkBinaryF64(crustSimd128MinF64, minF64, 1));
	CRUST_TEST_CHECK(checkBinaryF64(crustSimd128MaxF64, maxF64, 1));

	CRUST_TEST_CHECK(checkUnaryF64(crustSimd128SqrtF64, sqrtF64, 0));
	CRUST_TEST_CHECK(checkUnaryF64(crustSimd128AbsF64, absF64, 1));
	CRUST_TEST_CHECK(checkUnaryF64(crustSimd128NegateF64, negateF64, 1));
	CRUST_TEST_CHECK(checkUnaryF64(crustSimd128FloorF64, floorF64, 0));
	CRUST_TEST_CHECK(checkUnaryF64(crustSimd128CeilF64, ceilF64, 0));
	CRUST_TEST_CHECK(checkUnaryF64(crustSimd128RoundEvenF64, roundEvenF64, 0));
	CRUST_TEST_CHECK(checkUnaryF64(crustSimd128TruncateF64, truncateF64, 0));

	return 0;
}

static int testCompareF64(void)
{
	CRUST_TEST_CHECK(checkCompareF64(crustSimd128EqualF64, equalF64));
	CRUST_TEST_CHECK(checkCompareF64(crustSimd128NotEqualF64, notEqualF64));
	CRUST_TEST_CHECK(checkCompareF64(crustSimd128LessF64, lessF64));
	CRUST_TEST_CHECK(checkCompareF64(crustSimd128LessEqualF64, lessEqualF64));
	CRUST_TEST_CHECK(checkCompareF64(crustSimd128GreaterF64, greaterF64));
	CRUST_TEST_CHECK(checkCompareF64(crustSimd128GreaterEqualF64, greaterEqualF64));

	return 0;
}

static int testConvert(void)
{
	CRUST_TEST_CHECK(checkConvertF32FromI32());
	CRUST_TEST_CHECK(checkConvertI32FromF32());
	CRUST_TEST_CHECK(checkConvertI32FromF32Saturate());
	CRUST_TEST_CHECK(checkConvertF64FromF32Low());
	CRUST_TEST_CHECK(checkConvertF64FromF32High());
	CRUST_TEST_CHECK(checkConvertF32FromF64());

	return 0;
}

int main(void)
{
	CRUST_TEST_RUN(testLayout);
	CRUST_TEST_RUN(testMemory);
	CRUST_TEST_RUN(testConstructors);
	CRUST_TEST_RUN(testLanes);
	CRUST_TEST_RUN(testZip);
	CRUST_TEST_RUN(testSwizzle);
	CRUST_TEST_RUN(testBitwise);
	CRUST_TEST_RUN(testMasks);
	CRUST_TEST_RUN(testArithmeticInteger);
	CRUST_TEST_RUN(testShiftInteger);
	CRUST_TEST_RUN(testCompareInteger);
	CRUST_TEST_RUN(testConvertInteger);
	CRUST_TEST_RUN(testArithmeticF32);
	CRUST_TEST_RUN(testCompareF32);
	CRUST_TEST_RUN(testArithmeticF64);
	CRUST_TEST_RUN(testCompareF64);
	CRUST_TEST_RUN(testConvert);

	return crustTestResult();
}
