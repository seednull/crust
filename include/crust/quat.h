#pragma once

#include "scalar.h"
#include "vec.h"

#ifdef __cplusplus
extern "C" {
#endif

//
typedef struct Crust_Quat_t
{
	f32 x, y, z, w;
} Crust_Quat;

static CRUST_INLINE Crust_Quat crustQuatMad(Crust_Quat a, f32 s, Crust_Quat b)
{
	Crust_Quat result;
	result.x = a.x * s + b.x;
	result.y = a.y * s + b.y;
	result.z = a.z * s + b.z;
	result.w = a.w * s + b.w;

	return result;
}

static CRUST_INLINE Crust_Quat crustQuatMul(Crust_Quat a, Crust_Quat b)
{
	Crust_Quat result;

	//         linear combination    + cross product
	result.x = a.w * b.x + b.w * a.x + a.y * b.z - a.z * b.y;
	result.y = a.w * b.y + b.w * a.y + a.z * b.x - a.x * b.z;
	result.z = a.w * b.z + b.w * a.z + a.x * b.y - a.y * b.x;

	//         mul                   - dot product
	result.w = a.w * b.w             - a.x * b.x - a.y * b.y - a.z * b.z;

	return result;
}

static CRUST_INLINE Crust_Quat crustQuatConjugate(Crust_Quat q)
{
	Crust_Quat result;
	result.x = -q.x;
	result.y = -q.y;
	result.z = -q.z;
	result.w =  q.w;

	return result;
}

static CRUST_INLINE f32 crustQuatDot(Crust_Quat a, Crust_Quat b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

static CRUST_INLINE Crust_Quat crustQuatNormalize(Crust_Quat q)
{
	f32 len_inv = crustRsqrtF32(crustQuatDot(q, q));

	Crust_Quat result;
	result.x = q.x * len_inv;
	result.y = q.y * len_inv;
	result.z = q.z * len_inv;
	result.w = q.w * len_inv;

	return result;
}

static CRUST_INLINE Crust_Quat crustQuatNlerp(Crust_Quat a, Crust_Quat b, f32 t)
{
	f32 ta = 1.0f - t;
	f32 tb = t;

	if (crustQuatDot(a, b) < 0.0f)
		tb = -tb;

	Crust_Quat result;
	result.x = a.x * ta + b.x * tb;
	result.y = a.y * ta + b.y * tb;
	result.z = a.z * ta + b.z * tb;
	result.w = a.w * ta + b.w * tb;

	return crustQuatNormalize(result);
}

static CRUST_INLINE Crust_Quat crustQuatSlerp(Crust_Quat a, Crust_Quat b, f32 t)
{
	f32 dot = crustQuatDot(a, b);
	f32 cos_half_theta = crustAbsF32(dot);

	f32 ta = 1.0f - t;
	f32 tb = t;

	if (cos_half_theta <= 0.9995f)
	{
		f32 half_theta = crustAcosF32(cos_half_theta);
		f32 sin_half_theta = crustSqrtF32(1.0f - cos_half_theta * cos_half_theta);

		ta = crustSinF32((1.0f - t) * half_theta) / sin_half_theta;
		tb = crustSinF32(t * half_theta) / sin_half_theta;
	}

	if (dot < 0.0f)
		tb = -tb;

	Crust_Quat result;
	result.x = a.x * ta + b.x * tb;
	result.y = a.y * ta + b.y * tb;
	result.z = a.z * ta + b.z * tb;
	result.w = a.w * ta + b.w * tb;

	return crustQuatNormalize(result);
}

static CRUST_INLINE Crust_Vec3 crustQuatRotate(Crust_Quat a, Crust_Vec3 v)
{
	Crust_Quat qv;
	qv.x = v.x;
	qv.y = v.y;
	qv.z = v.z;
	qv.w = 0.0f;

	Crust_Quat q = crustQuatMul(a, qv);
	q = crustQuatMul(q, crustQuatConjugate(a));

	Crust_Vec3 result;
	result.x = q.x;
	result.y = q.y;
	result.z = q.z;
	
	return result;
}

#ifdef __cplusplus
}
#endif
