#include "crust_test.h"

#include <math.h>

#define SQRT_HALF 0.70710678f
#define SIN_22_5 0.38268343f
#define COS_22_5 0.92387953f
#define SIN_175 0.08715574f
#define COS_175 -0.99619470f
#define SIN_5 0.08715574f
#define COS_5 0.99619470f

enum
{
	ITERATIONS = 10000,
};

static const Crust_Quat identity = { 0.0f, 0.0f, 0.0f, 1.0f };
static const Crust_Quat z90 = { 0.0f, 0.0f, SQRT_HALF, SQRT_HALF };

//
static u8 approxF32(f32 a, f32 b, f32 epsilon)
{
	return crustAbsF32(a - b) <= epsilon;
}

static u8 approxVec2(Crust_Vec2 a, Crust_Vec2 b, f32 epsilon)
{
	return approxF32(a.x, b.x, epsilon) && approxF32(a.y, b.y, epsilon);
}

static u8 approxVec3(Crust_Vec3 a, Crust_Vec3 b, f32 epsilon)
{
	return approxF32(a.x, b.x, epsilon) && approxF32(a.y, b.y, epsilon) && approxF32(a.z, b.z, epsilon);
}

static u8 approxVec4(Crust_Vec4 a, Crust_Vec4 b, f32 epsilon)
{
	return approxF32(a.x, b.x, epsilon) && approxF32(a.y, b.y, epsilon) && approxF32(a.z, b.z, epsilon) && approxF32(a.w, b.w, epsilon);
}

static Crust_Quat quatNegate(Crust_Quat q)
{
	Crust_Quat result = { -q.x, -q.y, -q.z, -q.w };
	return result;
}

static u8 approxRotation(Crust_Quat a, Crust_Quat b, f32 epsilon)
{
	if (crustQuatDot(a, b) < 0.0f)
		b = quatNegate(b);

	return approxF32(a.x, b.x, epsilon) && approxF32(a.y, b.y, epsilon) && approxF32(a.z, b.z, epsilon) && approxF32(a.w, b.w, epsilon);
}

static u8 approxTransform(Crust_Transform a, Crust_Transform b, f32 epsilon)
{
	return approxVec3(a.position, b.position, epsilon) && approxRotation(a.rotation, b.rotation, epsilon) && approxVec3(a.scale, b.scale, epsilon);
}

static u8 isNan(Crust_Quat q)
{
	return q.x != q.x || q.y != q.y || q.z != q.z || q.w != q.w;
}

static f32 angleBetween(Crust_Quat a, Crust_Quat b)
{
	Crust_Quat delta = crustQuatMul(crustQuatConjugate(a), b);
	f32 sin_half = crustSqrtF32(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);

	return 2.0f * crustAtan2F32(sin_half, crustAbsF32(delta.w));
}

//
static f32 randomRange(f32 min, f32 max)
{
	return min + (max - min) * (f32)crustTestRandomF64();
}

static Crust_Vec2 randomVec2(void)
{
	Crust_Vec2 result = { randomRange(-10.0f, 10.0f), randomRange(-10.0f, 10.0f) };
	return result;
}

static Crust_Vec3 randomVec3(void)
{
	Crust_Vec3 result = { randomRange(-10.0f, 10.0f), randomRange(-10.0f, 10.0f), randomRange(-10.0f, 10.0f) };
	return result;
}

static Crust_Vec4 randomVec4(void)
{
	Crust_Vec4 result = { randomRange(-10.0f, 10.0f), randomRange(-10.0f, 10.0f), randomRange(-10.0f, 10.0f), randomRange(-10.0f, 10.0f) };
	return result;
}

static Crust_Quat randomRotation(void)
{
	Crust_Quat q;

	do
	{
		q.x = randomRange(-1.0f, 1.0f);
		q.y = randomRange(-1.0f, 1.0f);
		q.z = randomRange(-1.0f, 1.0f);
		q.w = randomRange(-1.0f, 1.0f);
	}
	while (crustQuatDot(q, q) < 0.01f);

	return crustQuatNormalize(q);
}

static Crust_Transform randomTransform(void)
{
	f32 scale = randomRange(0.5f, 2.0f);

	Crust_Transform result;
	result.position = randomVec3();
	result.rotation = randomRotation();
	result.scale = (Crust_Vec3){ scale, scale, scale };

	return result;
}

//
static const u32 rounding_f32_bits[] =
{
	0x00000000u, 0x80000000u, 0x3f000000u, 0xbf000000u, 0x3fc00000u, 0xbfc00000u, 0x40200000u, 0xc0200000u,
	0x3effffffu, 0xbeffffffu, 0x4affffffu, 0xcaffffffu, 0x4b000000u, 0x4b800000u, 0x7f7fffffu, 0xff7fffffu,
	0x00000001u, 0x80000001u, 0x00800000u, 0x7f800000u, 0xff800000u, 0x7fc00000u, 0xffc00001u,
};

static const u64 rounding_f64_bits[] =
{
	0x0000000000000000ULL, 0x8000000000000000ULL, 0x3fe0000000000000ULL, 0xbfe0000000000000ULL,
	0x3ff8000000000000ULL, 0xbff8000000000000ULL, 0x4004000000000000ULL, 0xc004000000000000ULL,
	0x3fdfffffffffffffULL, 0xbfdfffffffffffffULL, 0x432fffffffffffffULL, 0xc32fffffffffffffULL,
	0x4330000000000000ULL, 0x4340000000000000ULL, 0x7fefffffffffffffULL, 0xffefffffffffffffULL,
	0x0000000000000001ULL, 0x8000000000000001ULL, 0x0010000000000000ULL, 0x7ff0000000000000ULL,
	0xfff0000000000000ULL, 0x7ff8000000000000ULL, 0xfff8000000000001ULL,
};

static f32 fromBitsF32(u32 v)
{
	f32 result;
	crustMemcpy(&result, &v, sizeof(result));
	return result;
}

static f64 fromBitsF64(u64 v)
{
	f64 result;
	crustMemcpy(&result, &v, sizeof(result));
	return result;
}

static u8 sameF32(f32 expected, f32 actual)
{
	u32 expected_bits;
	u32 actual_bits;
	crustMemcpy(&expected_bits, &expected, sizeof(expected_bits));
	crustMemcpy(&actual_bits, &actual, sizeof(actual_bits));

	if (expected != expected)
		return actual != actual;

	return expected_bits == actual_bits;
}

static u8 sameF64(f64 expected, f64 actual)
{
	u64 expected_bits;
	u64 actual_bits;
	crustMemcpy(&expected_bits, &expected, sizeof(expected_bits));
	crustMemcpy(&actual_bits, &actual, sizeof(actual_bits));

	if (expected != expected)
		return actual != actual;

	return expected_bits == actual_bits;
}

static f32 roundingInputF32(u32 index)
{
	u32 count = (u32)(sizeof(rounding_f32_bits) / sizeof(rounding_f32_bits[0]));

	if (index < count)
		return fromBitsF32(rounding_f32_bits[index]);

	switch (crustTestRandom() % 3)
	{
		case 0:
			return (f32)((i32)(crustTestRandom() % 2001) - 1000) + 0.5f;

		case 1:
			return randomRange(-1000.0f, 1000.0f);

		default:
		{
			u32 bits = crustTestRandom();
			if ((bits & 0x7f800000u) == 0x7f800000u)
				bits |= 0x00400000u;

			return fromBitsF32(bits);
		}
	}
}

static f64 roundingInputF64(u32 index)
{
	u32 count = (u32)(sizeof(rounding_f64_bits) / sizeof(rounding_f64_bits[0]));

	if (index < count)
		return fromBitsF64(rounding_f64_bits[index]);

	switch (crustTestRandom() % 3)
	{
		case 0:
			return (f64)((i32)(crustTestRandom() % 2001) - 1000) + 0.5;

		case 1:
			return crustTestRandomF64() * 2000.0 - 1000.0;

		default:
		{
			u64 bits = ((u64)crustTestRandom() << 32) | crustTestRandom();
			if ((bits & 0x7ff0000000000000ULL) == 0x7ff0000000000000ULL)
				bits |= 0x0008000000000000ULL;

			return fromBitsF64(bits);
		}
	}
}

//
static int testRounding(void)
{
	for (u32 i = 0; i < ITERATIONS; ++i)
	{
		f32 v = roundingInputF32(i);

		CRUST_TEST_CHECK(sameF32(floorf(v), crustFloorF32(v)));
		CRUST_TEST_CHECK(sameF32(ceilf(v), crustCeilF32(v)));
		CRUST_TEST_CHECK(sameF32(nearbyintf(v), crustRoundEvenF32(v)));
		CRUST_TEST_CHECK(sameF32(truncf(v), crustTruncateF32(v)));
	}

	for (u32 i = 0; i < ITERATIONS; ++i)
	{
		f64 v = roundingInputF64(i);

		CRUST_TEST_CHECK(sameF64(floor(v), crustFloorF64(v)));
		CRUST_TEST_CHECK(sameF64(ceil(v), crustCeilF64(v)));
		CRUST_TEST_CHECK(sameF64(nearbyint(v), crustRoundEvenF64(v)));
		CRUST_TEST_CHECK(sameF64(trunc(v), crustTruncateF64(v)));
	}

	return 0;
}

static int testVecKnownValues(void)
{
	CRUST_TEST_CHECK(approxVec3(crustVec3Cross((Crust_Vec3){ 1, 0, 0 }, (Crust_Vec3){ 0, 1, 0 }), (Crust_Vec3){ 0, 0, 1 }, 0.0f));
	CRUST_TEST_CHECK(approxVec3(crustVec3Cross((Crust_Vec3){ 0, 1, 0 }, (Crust_Vec3){ 0, 0, 1 }), (Crust_Vec3){ 1, 0, 0 }, 0.0f));
	CRUST_TEST_CHECK(approxVec3(crustVec3Cross((Crust_Vec3){ 0, 0, 1 }, (Crust_Vec3){ 1, 0, 0 }), (Crust_Vec3){ 0, 1, 0 }, 0.0f));

	CRUST_TEST_CHECK(crustVec2Dot((Crust_Vec2){ 1, 2 }, (Crust_Vec2){ 3, 4 }) == 11.0f);
	CRUST_TEST_CHECK(crustVec3Dot((Crust_Vec3){ 1, 2, 3 }, (Crust_Vec3){ 4, 5, 6 }) == 32.0f);
	CRUST_TEST_CHECK(crustVec4Dot((Crust_Vec4){ 1, 2, 3, 4 }, (Crust_Vec4){ 5, 6, 7, 8 }) == 70.0f);

	CRUST_TEST_CHECK(approxF32(crustVec2Length((Crust_Vec2){ 3, 4 }), 5.0f, 1e-6f));
	CRUST_TEST_CHECK(approxF32(crustVec3Length((Crust_Vec3){ 2, 3, 6 }), 7.0f, 1e-6f));
	CRUST_TEST_CHECK(approxF32(crustVec4Length((Crust_Vec4){ 1, 1, 1, 1 }), 2.0f, 1e-6f));

	CRUST_TEST_CHECK(approxVec2(crustVec2Normalize((Crust_Vec2){ 3, 4 }), (Crust_Vec2){ 0.6f, 0.8f }, 1e-6f));
	CRUST_TEST_CHECK(approxVec3(crustVec3Normalize((Crust_Vec3){ 2, 3, 6 }), (Crust_Vec3){ 2.0f / 7.0f, 3.0f / 7.0f, 6.0f / 7.0f }, 1e-6f));
	CRUST_TEST_CHECK(approxVec4(crustVec4Normalize((Crust_Vec4){ 1, 1, 1, 1 }), (Crust_Vec4){ 0.5f, 0.5f, 0.5f, 0.5f }, 1e-6f));

	CRUST_TEST_CHECK(approxVec3(crustVec3Lerp((Crust_Vec3){ 0, 0, 0 }, (Crust_Vec3){ 10, 20, 30 }, 0.25f), (Crust_Vec3){ 2.5f, 5.0f, 7.5f }, 1e-6f));
	CRUST_TEST_CHECK(approxVec3(crustVec3Mad((Crust_Vec3){ 1, 2, 3 }, 2.0f, (Crust_Vec3){ 1, 1, 1 }), (Crust_Vec3){ 3, 5, 7 }, 0.0f));

	CRUST_TEST_CHECK(approxVec3(crustVec3AddS((Crust_Vec3){ 1, 2, 3 }, 1.0f), (Crust_Vec3){ 2, 3, 4 }, 0.0f));
	CRUST_TEST_CHECK(approxVec3(crustVec3SubS((Crust_Vec3){ 1, 2, 3 }, 1.0f), (Crust_Vec3){ 0, 1, 2 }, 0.0f));
	CRUST_TEST_CHECK(approxVec3(crustVec3MulS((Crust_Vec3){ 1, 2, 3 }, 2.0f), (Crust_Vec3){ 2, 4, 6 }, 0.0f));
	CRUST_TEST_CHECK(approxVec3(crustVec3DivS((Crust_Vec3){ 2, 4, 8 }, 2.0f), (Crust_Vec3){ 1, 2, 4 }, 0.0f));

	CRUST_TEST_CHECK(approxVec3(crustVec3AddV((Crust_Vec3){ 1, 2, 3 }, (Crust_Vec3){ 3, 2, 1 }), (Crust_Vec3){ 4, 4, 4 }, 0.0f));
	CRUST_TEST_CHECK(approxVec3(crustVec3SubV((Crust_Vec3){ 1, 2, 3 }, (Crust_Vec3){ 1, 1, 1 }), (Crust_Vec3){ 0, 1, 2 }, 0.0f));
	CRUST_TEST_CHECK(approxVec3(crustVec3MulV((Crust_Vec3){ 1, 2, 3 }, (Crust_Vec3){ 2, 3, 4 }), (Crust_Vec3){ 2, 6, 12 }, 0.0f));
	CRUST_TEST_CHECK(approxVec3(crustVec3DivV((Crust_Vec3){ 2, 6, 12 }, (Crust_Vec3){ 2, 3, 4 }), (Crust_Vec3){ 1, 2, 3 }, 0.0f));

	CRUST_TEST_CHECK(approxVec2(crustVec2Lerp((Crust_Vec2){ 0, 0 }, (Crust_Vec2){ 10, 20 }, 0.25f), (Crust_Vec2){ 2.5f, 5.0f }, 1e-6f));
	CRUST_TEST_CHECK(approxVec2(crustVec2Mad((Crust_Vec2){ 1, 2 }, 2.0f, (Crust_Vec2){ 1, 1 }), (Crust_Vec2){ 3, 5 }, 0.0f));

	CRUST_TEST_CHECK(approxVec2(crustVec2AddS((Crust_Vec2){ 1, 2 }, 1.0f), (Crust_Vec2){ 2, 3 }, 0.0f));
	CRUST_TEST_CHECK(approxVec2(crustVec2SubS((Crust_Vec2){ 1, 2 }, 1.0f), (Crust_Vec2){ 0, 1 }, 0.0f));
	CRUST_TEST_CHECK(approxVec2(crustVec2MulS((Crust_Vec2){ 1, 2 }, 2.0f), (Crust_Vec2){ 2, 4 }, 0.0f));
	CRUST_TEST_CHECK(approxVec2(crustVec2DivS((Crust_Vec2){ 2, 4 }, 2.0f), (Crust_Vec2){ 1, 2 }, 0.0f));

	CRUST_TEST_CHECK(approxVec2(crustVec2AddV((Crust_Vec2){ 1, 2 }, (Crust_Vec2){ 3, 2 }), (Crust_Vec2){ 4, 4 }, 0.0f));
	CRUST_TEST_CHECK(approxVec2(crustVec2SubV((Crust_Vec2){ 1, 2 }, (Crust_Vec2){ 1, 1 }), (Crust_Vec2){ 0, 1 }, 0.0f));
	CRUST_TEST_CHECK(approxVec2(crustVec2MulV((Crust_Vec2){ 1, 2 }, (Crust_Vec2){ 2, 3 }), (Crust_Vec2){ 2, 6 }, 0.0f));
	CRUST_TEST_CHECK(approxVec2(crustVec2DivV((Crust_Vec2){ 2, 6 }, (Crust_Vec2){ 2, 3 }), (Crust_Vec2){ 1, 2 }, 0.0f));

	CRUST_TEST_CHECK(approxVec4(crustVec4Lerp((Crust_Vec4){ 0, 0, 0, 0 }, (Crust_Vec4){ 10, 20, 30, 40 }, 0.25f), (Crust_Vec4){ 2.5f, 5.0f, 7.5f, 10.0f }, 1e-6f));
	CRUST_TEST_CHECK(approxVec4(crustVec4Mad((Crust_Vec4){ 1, 2, 3, 4 }, 2.0f, (Crust_Vec4){ 1, 1, 1, 1 }), (Crust_Vec4){ 3, 5, 7, 9 }, 0.0f));

	CRUST_TEST_CHECK(approxVec4(crustVec4AddS((Crust_Vec4){ 1, 2, 3, 4 }, 1.0f), (Crust_Vec4){ 2, 3, 4, 5 }, 0.0f));
	CRUST_TEST_CHECK(approxVec4(crustVec4SubS((Crust_Vec4){ 1, 2, 3, 4 }, 1.0f), (Crust_Vec4){ 0, 1, 2, 3 }, 0.0f));
	CRUST_TEST_CHECK(approxVec4(crustVec4MulS((Crust_Vec4){ 1, 2, 3, 4 }, 2.0f), (Crust_Vec4){ 2, 4, 6, 8 }, 0.0f));
	CRUST_TEST_CHECK(approxVec4(crustVec4DivS((Crust_Vec4){ 2, 4, 8, 16 }, 2.0f), (Crust_Vec4){ 1, 2, 4, 8 }, 0.0f));

	CRUST_TEST_CHECK(approxVec4(crustVec4AddV((Crust_Vec4){ 1, 2, 3, 4 }, (Crust_Vec4){ 4, 3, 2, 1 }), (Crust_Vec4){ 5, 5, 5, 5 }, 0.0f));
	CRUST_TEST_CHECK(approxVec4(crustVec4SubV((Crust_Vec4){ 1, 2, 3, 4 }, (Crust_Vec4){ 1, 1, 1, 1 }), (Crust_Vec4){ 0, 1, 2, 3 }, 0.0f));
	CRUST_TEST_CHECK(approxVec4(crustVec4MulV((Crust_Vec4){ 1, 2, 3, 4 }, (Crust_Vec4){ 2, 3, 4, 5 }), (Crust_Vec4){ 2, 6, 12, 20 }, 0.0f));
	CRUST_TEST_CHECK(approxVec4(crustVec4DivV((Crust_Vec4){ 2, 6, 12, 20 }, (Crust_Vec4){ 2, 3, 4, 5 }), (Crust_Vec4){ 1, 2, 3, 4 }, 0.0f));

	return 0;
}

static int testVecProperties(void)
{
	for (u32 i = 0; i < ITERATIONS; ++i)
	{
		Crust_Vec2 a2 = randomVec2();
		Crust_Vec2 b2 = randomVec2();
		Crust_Vec3 a3 = randomVec3();
		Crust_Vec3 b3 = randomVec3();
		Crust_Vec4 a4 = randomVec4();
		Crust_Vec4 b4 = randomVec4();
		f32 s = randomRange(0.5f, 2.0f);

		CRUST_TEST_REQUIRE(approxF32(crustVec2Dot(a2, a2), crustVec2LengthSquared(a2), 1e-6f * crustVec2LengthSquared(a2)));
		CRUST_TEST_REQUIRE(approxF32(crustVec3Dot(a3, a3), crustVec3LengthSquared(a3), 1e-6f * crustVec3LengthSquared(a3)));
		CRUST_TEST_REQUIRE(approxF32(crustVec4Dot(a4, a4), crustVec4LengthSquared(a4), 1e-6f * crustVec4LengthSquared(a4)));

		if (crustVec3Length(a3) > 0.1f && crustVec3Length(b3) > 0.1f)
		{
			Crust_Vec3 na = crustVec3Normalize(a3);
			Crust_Vec3 nb = crustVec3Normalize(b3);
			Crust_Vec3 cross = crustVec3Cross(na, nb);

			CRUST_TEST_REQUIRE(approxF32(crustVec3Length(na), 1.0f, 1e-5f));
			CRUST_TEST_REQUIRE(approxF32(crustVec3Dot(cross, na), 0.0f, 1e-5f));
			CRUST_TEST_REQUIRE(approxF32(crustVec3Dot(cross, nb), 0.0f, 1e-5f));
		}

		if (crustVec2Length(a2) > 0.1f)
			CRUST_TEST_REQUIRE(approxF32(crustVec2Length(crustVec2Normalize(a2)), 1.0f, 1e-5f));

		if (crustVec4Length(a4) > 0.1f)
			CRUST_TEST_REQUIRE(approxF32(crustVec4Length(crustVec4Normalize(a4)), 1.0f, 1e-5f));

		CRUST_TEST_REQUIRE(approxVec3(crustVec3Lerp(a3, b3, 0.0f), a3, 1e-5f));
		CRUST_TEST_REQUIRE(approxVec3(crustVec3Lerp(a3, b3, 1.0f), b3, 1e-5f));
		CRUST_TEST_REQUIRE(approxVec3(crustVec3Mad(a3, s, b3), crustVec3AddV(crustVec3MulS(a3, s), b3), 1e-5f));
		CRUST_TEST_REQUIRE(approxVec3(crustVec3DivS(a3, s), crustVec3MulS(a3, 1.0f / s), 1e-5f));
		CRUST_TEST_REQUIRE(approxVec3(crustVec3SubV(crustVec3AddV(a3, b3), b3), a3, 1e-5f));

		CRUST_TEST_REQUIRE(approxVec2(crustVec2Lerp(a2, b2, 0.0f), a2, 1e-5f));
		CRUST_TEST_REQUIRE(approxVec2(crustVec2Lerp(a2, b2, 1.0f), b2, 1e-5f));
		CRUST_TEST_REQUIRE(approxVec2(crustVec2Mad(a2, s, b2), crustVec2AddV(crustVec2MulS(a2, s), b2), 1e-5f));
		CRUST_TEST_REQUIRE(approxVec2(crustVec2DivS(a2, s), crustVec2MulS(a2, 1.0f / s), 1e-5f));
		CRUST_TEST_REQUIRE(approxVec2(crustVec2SubV(crustVec2AddV(a2, b2), b2), a2, 1e-5f));

		CRUST_TEST_REQUIRE(approxVec4(crustVec4Lerp(a4, b4, 0.0f), a4, 1e-5f));
		CRUST_TEST_REQUIRE(approxVec4(crustVec4Lerp(a4, b4, 1.0f), b4, 1e-5f));
		CRUST_TEST_REQUIRE(approxVec4(crustVec4Mad(a4, s, b4), crustVec4AddV(crustVec4MulS(a4, s), b4), 1e-5f));
		CRUST_TEST_REQUIRE(approxVec4(crustVec4DivS(a4, s), crustVec4MulS(a4, 1.0f / s), 1e-5f));
		CRUST_TEST_REQUIRE(approxVec4(crustVec4SubV(crustVec4AddV(a4, b4), b4), a4, 1e-5f));
	}

	return 0;
}

//
static int testQuatKnownValues(void)
{
	CRUST_TEST_CHECK(approxVec3(crustQuatRotate(z90, (Crust_Vec3){ 1, 0, 0 }), (Crust_Vec3){ 0, 1, 0 }, 1e-6f));
	CRUST_TEST_CHECK(approxVec3(crustQuatRotate(z90, (Crust_Vec3){ 0, 1, 0 }), (Crust_Vec3){ -1, 0, 0 }, 1e-6f));
	CRUST_TEST_CHECK(approxVec3(crustQuatRotate(z90, (Crust_Vec3){ 0, 0, 1 }), (Crust_Vec3){ 0, 0, 1 }, 1e-6f));
	CRUST_TEST_CHECK(approxVec3(crustQuatRotate(crustQuatConjugate(z90), (Crust_Vec3){ 0, 1, 0 }), (Crust_Vec3){ 1, 0, 0 }, 1e-6f));

	CRUST_TEST_CHECK(approxRotation(crustQuatMul(z90, z90), (Crust_Quat){ 0, 0, 1, 0 }, 1e-6f));

	Crust_Quat mad = crustQuatMad((Crust_Quat){ 1, 2, 3, 4 }, 2.0f, (Crust_Quat){ 1, 1, 1, -1 });
	CRUST_TEST_CHECK(mad.x == 3.0f && mad.y == 5.0f && mad.z == 7.0f && mad.w == 7.0f);

	Crust_Quat z45 = { 0.0f, 0.0f, SIN_22_5, COS_22_5 };
	CRUST_TEST_CHECK(approxRotation(crustQuatSlerp(identity, z90, 0.5f), z45, 1e-6f));
	CRUST_TEST_CHECK(approxRotation(crustQuatNlerp(identity, z90, 0.5f), z45, 1e-6f));

	Crust_Quat minus_ten_degrees = { 0.0f, 0.0f, SIN_175, COS_175 };
	Crust_Vec3 minus_five_degrees = { COS_5, -SIN_5, 0.0f };

	CRUST_TEST_CHECK(approxVec3(crustQuatRotate(crustQuatSlerp(identity, minus_ten_degrees, 0.5f), (Crust_Vec3){ 1, 0, 0 }), minus_five_degrees, 1e-6f));
	CRUST_TEST_CHECK(approxVec3(crustQuatRotate(crustQuatNlerp(identity, minus_ten_degrees, 0.5f), (Crust_Vec3){ 1, 0, 0 }), minus_five_degrees, 1e-6f));

	f32 ts[] = { 0.0f, 0.25f, 0.5f, 0.75f, 1.0f };

	for (u32 i = 0; i < sizeof(ts) / sizeof(ts[0]); ++i)
	{
		CRUST_TEST_CHECK(approxRotation(crustQuatSlerp(identity, identity, ts[i]), identity, 1e-6f));
		CRUST_TEST_CHECK(approxRotation(crustQuatSlerp(z90, z90, ts[i]), z90, 1e-6f));
		CRUST_TEST_CHECK(approxRotation(crustQuatSlerp(z90, quatNegate(z90), ts[i]), z90, 1e-6f));
		CRUST_TEST_CHECK(approxRotation(crustQuatNlerp(z90, quatNegate(z90), ts[i]), z90, 1e-6f));
	}

	return 0;
}

static int testQuatProperties(void)
{
	for (u32 i = 0; i < ITERATIONS; ++i)
	{
		Crust_Quat a = randomRotation();
		Crust_Quat b = randomRotation();
		Crust_Vec3 v = randomVec3();

		Crust_Vec3 rotated = crustQuatRotate(a, v);
		CRUST_TEST_REQUIRE(approxF32(crustVec3Length(rotated), crustVec3Length(v), 1e-4f));
		CRUST_TEST_REQUIRE(approxVec3(crustQuatRotate(crustQuatConjugate(a), rotated), v, 1e-4f));
		CRUST_TEST_REQUIRE(approxVec3(crustQuatRotate(crustQuatMul(a, b), v), crustQuatRotate(a, crustQuatRotate(b, v)), 1e-4f));
		CRUST_TEST_REQUIRE(approxRotation(crustQuatMul(a, crustQuatConjugate(a)), identity, 1e-6f));
	}

	return 0;
}

static int testQuatInterpolation(void)
{
	for (u32 i = 0; i < ITERATIONS; ++i)
	{
		Crust_Quat a = randomRotation();
		Crust_Quat b = randomRotation();
		f32 t = randomRange(0.0f, 1.0f);

		CRUST_TEST_REQUIRE(approxRotation(crustQuatSlerp(a, b, 0.0f), a, 1e-5f));
		CRUST_TEST_REQUIRE(approxRotation(crustQuatSlerp(a, b, 1.0f), b, 1e-5f));

		Crust_Quat slerp = crustQuatSlerp(a, b, t);
		CRUST_TEST_REQUIRE(isNan(slerp) == 0);
		CRUST_TEST_REQUIRE(approxF32(crustQuatDot(slerp, slerp), 1.0f, 1e-5f));
		CRUST_TEST_REQUIRE(approxF32(angleBetween(a, slerp), t * angleBetween(a, b), 1e-4f));

		Crust_Quat nlerp = crustQuatNlerp(a, b, t);
		CRUST_TEST_REQUIRE(isNan(nlerp) == 0);
		CRUST_TEST_REQUIRE(approxF32(crustQuatDot(nlerp, nlerp), 1.0f, 1e-5f));
		CRUST_TEST_REQUIRE(approxF32(angleBetween(a, nlerp) + angleBetween(nlerp, b), angleBetween(a, b), 1e-4f));

		CRUST_TEST_REQUIRE(approxRotation(crustQuatSlerp(a, a, t), a, 1e-5f));
		CRUST_TEST_REQUIRE(approxRotation(crustQuatSlerp(a, quatNegate(a), t), a, 1e-5f));
	}

	return 0;
}

//
static int testTransformKnownValues(void)
{
	Crust_Transform a = { { 1, 0, 0 }, z90, { 1, 1, 1 } };
	Crust_Transform b = { { 1, 0, 0 }, identity, { 1, 1, 1 } };

	Crust_Transform ab = crustTransformMul(a, b);
	CRUST_TEST_CHECK(approxVec3(ab.position, (Crust_Vec3){ 1, 1, 0 }, 1e-6f));
	CRUST_TEST_CHECK(approxRotation(ab.rotation, z90, 1e-6f));

	Crust_Transform scaled = { { 0, 0, 0 }, identity, { 2, 2, 2 } };
	Crust_Transform offset = { { 1, 2, 3 }, identity, { 3, 3, 3 } };

	Crust_Transform combined = crustTransformMul(scaled, offset);
	CRUST_TEST_CHECK(approxVec3(combined.position, (Crust_Vec3){ 2, 4, 6 }, 1e-6f));
	CRUST_TEST_CHECK(approxVec3(combined.scale, (Crust_Vec3){ 6, 6, 6 }, 1e-6f));

	Crust_Transform t = { { 1, 2, 3 }, identity, { 2, 2, 2 } };
	Crust_Transform inverse = crustTransformInvert(t);
	CRUST_TEST_CHECK(approxVec3(inverse.position, (Crust_Vec3){ -0.5f, -1.0f, -1.5f }, 1e-6f));
	CRUST_TEST_CHECK(approxVec3(inverse.scale, (Crust_Vec3){ 0.5f, 0.5f, 0.5f }, 1e-6f));

	return 0;
}

static int testTransformProperties(void)
{
	Crust_Transform identity_transform = { { 0, 0, 0 }, identity, { 1, 1, 1 } };

	for (u32 i = 0; i < ITERATIONS; ++i)
	{
		Crust_Transform a = randomTransform();
		Crust_Transform b = randomTransform();
		Crust_Transform c = randomTransform();

		CRUST_TEST_REQUIRE(approxTransform(crustTransformMul(identity_transform, a), a, 1e-5f));
		CRUST_TEST_REQUIRE(approxTransform(crustTransformMul(a, identity_transform), a, 1e-5f));

		CRUST_TEST_REQUIRE(approxTransform(crustTransformInvert(crustTransformInvert(a)), a, 1e-4f));
		CRUST_TEST_REQUIRE(approxTransform(crustTransformMul(a, crustTransformInvert(a)), identity_transform, 1e-4f));
		CRUST_TEST_REQUIRE(approxTransform(crustTransformMul(crustTransformInvert(a), a), identity_transform, 1e-4f));

		Crust_Transform left = crustTransformMul(crustTransformMul(a, b), c);
		Crust_Transform right = crustTransformMul(a, crustTransformMul(b, c));
		CRUST_TEST_REQUIRE(approxTransform(left, right, 1e-3f));
	}

	return 0;
}

int main(void)
{
	CRUST_TEST_RUN(testRounding);
	CRUST_TEST_RUN(testVecKnownValues);
	CRUST_TEST_RUN(testVecProperties);
	CRUST_TEST_RUN(testQuatKnownValues);
	CRUST_TEST_RUN(testQuatProperties);
	CRUST_TEST_RUN(testQuatInterpolation);
	CRUST_TEST_RUN(testTransformKnownValues);
	CRUST_TEST_RUN(testTransformProperties);

	return crustTestResult();
}
