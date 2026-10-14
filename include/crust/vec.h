#pragma once

#include "scalar.h"

#ifdef __cplusplus
extern "C" {
#endif

//
typedef struct Crust_Vec2_t
{
	f32 x, y;
} Crust_Vec2;

static CRUST_INLINE Crust_Vec2 crustVec2Mad(Crust_Vec2 a, f32 s, Crust_Vec2 b)
{
	Crust_Vec2 result;
	result.x = a.x * s + b.x;
	result.y = a.y * s + b.y;

	return result;
}

static CRUST_INLINE Crust_Vec2 crustVec2AddS(Crust_Vec2 a, f32 s)
{
	Crust_Vec2 result;
	result.x = a.x + s;
	result.y = a.y + s;

	return result;
}

static CRUST_INLINE Crust_Vec2 crustVec2AddV(Crust_Vec2 a, Crust_Vec2 b)
{
	Crust_Vec2 result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;

	return result;
}

static CRUST_INLINE Crust_Vec2 crustVec2SubS(Crust_Vec2 a, f32 s)
{
	Crust_Vec2 result;
	result.x = a.x - s;
	result.y = a.y - s;

	return result;
}

static CRUST_INLINE Crust_Vec2 crustVec2SubV(Crust_Vec2 a, Crust_Vec2 b)
{
	Crust_Vec2 result;
	result.x = a.x - b.x;
	result.y = a.y - b.y;

	return result;
}

static CRUST_INLINE Crust_Vec2 crustVec2MulS(Crust_Vec2 a, f32 s)
{
	Crust_Vec2 result;
	result.x = a.x * s;
	result.y = a.y * s;

	return result;
}

static CRUST_INLINE Crust_Vec2 crustVec2MulV(Crust_Vec2 a, Crust_Vec2 b)
{
	Crust_Vec2 result;
	result.x = a.x * b.x;
	result.y = a.y * b.y;

	return result;
}

static CRUST_INLINE Crust_Vec2 crustVec2DivS(Crust_Vec2 a, f32 s)
{
	float s_inv = 1.0f / s;

	Crust_Vec2 result;
	result.x = a.x * s_inv;
	result.y = a.y * s_inv;

	return result;
}

static CRUST_INLINE Crust_Vec2 crustVec2DivV(Crust_Vec2 a, Crust_Vec2 b)
{
	Crust_Vec2 result;
	result.x = a.x / b.x;
	result.y = a.y / b.y;

	return result;
}

static CRUST_INLINE Crust_Vec2 crustVec2Lerp(Crust_Vec2 a, Crust_Vec2 b, f32 t)
{
	Crust_Vec2 result;
	result.x = a.x + (b.x - a.x) * t;
	result.y = a.y + (b.y - a.y) * t;

	return result;
}

static CRUST_INLINE Crust_Vec2 crustVec2Normalize(Crust_Vec2 v)
{
	f32 len_inv = crustRsqrtF32(v.x * v.x + v.y * v.y);

	Crust_Vec2 result;
	result.x = v.x * len_inv;
	result.y = v.y * len_inv;

	return result;
}

static CRUST_INLINE f32 crustVec2Dot(Crust_Vec2 a, Crust_Vec2 b)
{
	return a.x * b.x + a.y * b.y;
}

static CRUST_INLINE f32 crustVec2Length(Crust_Vec2 v)
{
	return crustSqrtF32(v.x * v.x + v.y * v.y);
}

static CRUST_INLINE f32 crustVec2LengthSquared(Crust_Vec2 v)
{
	return v.x * v.x + v.y * v.y;
}

//
typedef struct Crust_Vec3_t
{
	f32 x, y, z;
} Crust_Vec3;

static CRUST_INLINE Crust_Vec3 crustVec3Mad(Crust_Vec3 a, f32 s, Crust_Vec3 b)
{
	Crust_Vec3 result;
	result.x = a.x * s + b.x;
	result.y = a.y * s + b.y;
	result.z = a.z * s + b.z;

	return result;
}

static CRUST_INLINE Crust_Vec3 crustVec3AddS(Crust_Vec3 a, f32 s)
{
	Crust_Vec3 result;
	result.x = a.x + s;
	result.y = a.y + s;
	result.z = a.z + s;

	return result;
}

static CRUST_INLINE Crust_Vec3 crustVec3AddV(Crust_Vec3 a, Crust_Vec3 b)
{
	Crust_Vec3 result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;

	return result;
}

static CRUST_INLINE Crust_Vec3 crustVec3SubS(Crust_Vec3 a, f32 s)
{
	Crust_Vec3 result;
	result.x = a.x - s;
	result.y = a.y - s;
	result.z = a.z - s;

	return result;
}

static CRUST_INLINE Crust_Vec3 crustVec3SubV(Crust_Vec3 a, Crust_Vec3 b)
{
	Crust_Vec3 result;
	result.x = a.x - b.x;
	result.y = a.y - b.y;
	result.z = a.z - b.z;

	return result;
}

static CRUST_INLINE Crust_Vec3 crustVec3MulS(Crust_Vec3 a, f32 s)
{
	Crust_Vec3 result;
	result.x = a.x * s;
	result.y = a.y * s;
	result.z = a.z * s;

	return result;
}

static CRUST_INLINE Crust_Vec3 crustVec3MulV(Crust_Vec3 a, Crust_Vec3 b)
{
	Crust_Vec3 result;
	result.x = a.x * b.x;
	result.y = a.y * b.y;
	result.z = a.z * b.z;

	return result;
}

static CRUST_INLINE Crust_Vec3 crustVec3DivS(Crust_Vec3 a, f32 s)
{
	float s_inv = 1.0f / s;

	Crust_Vec3 result;
	result.x = a.x * s_inv;
	result.y = a.y * s_inv;
	result.z = a.z * s_inv;

	return result;
}

static CRUST_INLINE Crust_Vec3 crustVec3DivV(Crust_Vec3 a, Crust_Vec3 b)
{
	Crust_Vec3 result;
	result.x = a.x / b.x;
	result.y = a.y / b.y;
	result.z = a.z / b.z;

	return result;
}

static CRUST_INLINE Crust_Vec3 crustVec3Lerp(Crust_Vec3 a, Crust_Vec3 b, f32 t)
{
	Crust_Vec3 result;
	result.x = a.x + (b.x - a.x) * t;
	result.y = a.y + (b.y - a.y) * t;
	result.z = a.z + (b.z - a.z) * t;

	return result;
}

static CRUST_INLINE Crust_Vec3 crustVec3Normalize(Crust_Vec3 v)
{
	f32 len_inv = crustRsqrtF32(v.x * v.x + v.y * v.y + v.z * v.z);

	Crust_Vec3 result;
	result.x = v.x * len_inv;
	result.y = v.y * len_inv;
	result.z = v.z * len_inv;

	return result;
}

static CRUST_INLINE Crust_Vec3 crustVec3Cross(Crust_Vec3 a, Crust_Vec3 b)
{
	Crust_Vec3 result;
	result.x = a.y * b.z - a.z * b.y;
	result.y = a.z * b.x - a.x * b.z;
	result.z = a.x * b.y - a.y * b.x;

	return result;
}

static CRUST_INLINE f32 crustVec3Dot(Crust_Vec3 a, Crust_Vec3 b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

static CRUST_INLINE f32 crustVec3Length(Crust_Vec3 v)
{
	return crustSqrtF32(v.x * v.x + v.y * v.y + v.z * v.z);
}

static CRUST_INLINE f32 crustVec3LengthSquared(Crust_Vec3 v)
{
	return v.x * v.x + v.y * v.y + v.z * v.z;
}

//
typedef struct Crust_Vec4_t
{
	f32 x, y, z, w;
} Crust_Vec4;

static CRUST_INLINE Crust_Vec4 crustVec4Mad(Crust_Vec4 a, f32 s, Crust_Vec4 b)
{
	Crust_Vec4 result;
	result.x = a.x * s + b.x;
	result.y = a.y * s + b.y;
	result.z = a.z * s + b.z;
	result.w = a.w * s + b.w;

	return result;
}

static CRUST_INLINE Crust_Vec4 crustVec4AddS(Crust_Vec4 a, f32 s)
{
	Crust_Vec4 result;
	result.x = a.x + s;
	result.y = a.y + s;
	result.z = a.z + s;
	result.w = a.w + s;

	return result;
}

static CRUST_INLINE Crust_Vec4 crustVec4AddV(Crust_Vec4 a, Crust_Vec4 b)
{
	Crust_Vec4 result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;
	result.w = a.w + b.w;

	return result;
}

static CRUST_INLINE Crust_Vec4 crustVec4SubS(Crust_Vec4 a, f32 s)
{
	Crust_Vec4 result;
	result.x = a.x - s;
	result.y = a.y - s;
	result.z = a.z - s;
	result.w = a.w - s;

	return result;
}

static CRUST_INLINE Crust_Vec4 crustVec4SubV(Crust_Vec4 a, Crust_Vec4 b)
{
	Crust_Vec4 result;
	result.x = a.x - b.x;
	result.y = a.y - b.y;
	result.z = a.z - b.z;
	result.w = a.w - b.w;

	return result;
}

static CRUST_INLINE Crust_Vec4 crustVec4MulS(Crust_Vec4 a, f32 s)
{
	Crust_Vec4 result;
	result.x = a.x * s;
	result.y = a.y * s;
	result.z = a.z * s;
	result.w = a.w * s;

	return result;
}

static CRUST_INLINE Crust_Vec4 crustVec4MulV(Crust_Vec4 a, Crust_Vec4 b)
{
	Crust_Vec4 result;
	result.x = a.x * b.x;
	result.y = a.y * b.y;
	result.z = a.z * b.z;
	result.w = a.w * b.w;

	return result;
}

static CRUST_INLINE Crust_Vec4 crustVec4DivS(Crust_Vec4 a, f32 s)
{
	float s_inv = 1.0f / s;

	Crust_Vec4 result;
	result.x = a.x * s_inv;
	result.y = a.y * s_inv;
	result.z = a.z * s_inv;
	result.w = a.w * s_inv;

	return result;
}

static CRUST_INLINE Crust_Vec4 crustVec4DivV(Crust_Vec4 a, Crust_Vec4 b)
{
	Crust_Vec4 result;
	result.x = a.x / b.x;
	result.y = a.y / b.y;
	result.z = a.z / b.z;
	result.w = a.w / b.w;

	return result;
}

static CRUST_INLINE Crust_Vec4 crustVec4Lerp(Crust_Vec4 a, Crust_Vec4 b, f32 t)
{
	Crust_Vec4 result;
	result.x = a.x + (b.x - a.x) * t;
	result.y = a.y + (b.y - a.y) * t;
	result.z = a.z + (b.z - a.z) * t;
	result.w = a.w + (b.w - a.w) * t;

	return result;
}

static CRUST_INLINE Crust_Vec4 crustVec4Normalize(Crust_Vec4 v)
{
	f32 len_inv = crustRsqrtF32(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);

	Crust_Vec4 result;
	result.x = v.x * len_inv;
	result.y = v.y * len_inv;
	result.z = v.z * len_inv;
	result.w = v.w * len_inv;

	return result;
}

static CRUST_INLINE f32 crustVec4Dot(Crust_Vec4 a, Crust_Vec4 b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

static CRUST_INLINE f32 crustVec4Length(Crust_Vec4 v)
{
	return crustSqrtF32(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
}

static CRUST_INLINE f32 crustVec4LengthSquared(Crust_Vec4 v)
{
	return v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w;
}

#ifdef __cplusplus
}
#endif
