#include "crust_test.h"

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

	return 0;
}

static int testVecProperties(void)
{
	for (u32 i = 0; i < ITERATIONS; ++i)
	{
		Crust_Vec2 a2 = randomVec2();
		Crust_Vec3 a3 = randomVec3();
		Crust_Vec3 b3 = randomVec3();
		Crust_Vec4 a4 = randomVec4();
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
	CRUST_TEST_RUN(testVecKnownValues);
	CRUST_TEST_RUN(testVecProperties);
	CRUST_TEST_RUN(testQuatKnownValues);
	CRUST_TEST_RUN(testQuatProperties);
	CRUST_TEST_RUN(testQuatInterpolation);
	CRUST_TEST_RUN(testTransformKnownValues);
	CRUST_TEST_RUN(testTransformProperties);

	return crustTestResult();
}
