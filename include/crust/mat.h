#pragma once

#include "scalar.h"
#include "vec.h"

#ifdef __cplusplus
extern "C" {
#endif

//
typedef struct Crust_Mat2_t
{
	f32 m[4];
} Crust_Mat2;

static CRUST_INLINE Crust_Mat2 crustMat2Identity(void)
{
	Crust_Mat2 result;

	result.m[0] = 1.0f; result.m[2] = 0.0f;
	result.m[1] = 0.0f; result.m[3] = 1.0f;

	return result;
}

static CRUST_INLINE Crust_Mat2 crustMat2Mad(Crust_Mat2 a, f32 s, Crust_Mat2 b)
{
	Crust_Mat2 result;

	for (u32 i = 0; i < 4; ++i)
		result.m[i] = a.m[i] * s + b.m[i];

	return result;
}

static CRUST_INLINE Crust_Mat2 crustMat2AddS(Crust_Mat2 a, f32 s)
{
	Crust_Mat2 result;

	for (u32 i = 0; i < 4; ++i)
		result.m[i] = a.m[i] + s;

	return result;
}

static CRUST_INLINE Crust_Mat2 crustMat2AddM(Crust_Mat2 a, Crust_Mat2 b)
{
	Crust_Mat2 result;

	for (u32 i = 0; i < 4; ++i)
		result.m[i] = a.m[i] + b.m[i];

	return result;
}

static CRUST_INLINE Crust_Mat2 crustMat2SubS(Crust_Mat2 a, f32 s)
{
	Crust_Mat2 result;

	for (u32 i = 0; i < 4; ++i)
		result.m[i] = a.m[i] - s;

	return result;
}

static CRUST_INLINE Crust_Mat2 crustMat2SubM(Crust_Mat2 a, Crust_Mat2 b)
{
	Crust_Mat2 result;

	for (u32 i = 0; i < 4; ++i)
		result.m[i] = a.m[i] - b.m[i];

	return result;
}

static CRUST_INLINE Crust_Mat2 crustMat2MulS(Crust_Mat2 a, f32 s)
{
	Crust_Mat2 result;

	for (u32 i = 0; i < 4; ++i)
		result.m[i] = a.m[i] * s;

	return result;
}

static CRUST_INLINE Crust_Mat2 crustMat2MulM(Crust_Mat2 a, Crust_Mat2 b)
{
	Crust_Mat2 result;

	result.m[0] = a.m[0] * b.m[0] + a.m[2] * b.m[1];
	result.m[1] = a.m[1] * b.m[0] + a.m[3] * b.m[1];

	result.m[2] = a.m[0] * b.m[2] + a.m[2] * b.m[3];
	result.m[3] = a.m[1] * b.m[2] + a.m[3] * b.m[3];

	return result;
}

static CRUST_INLINE Crust_Vec2 crustMat2MulV(Crust_Mat2 a, Crust_Vec2 b)
{
	Crust_Vec2 result;

	result.x = a.m[0] * b.x + a.m[2] * b.y;
	result.y = a.m[1] * b.x + a.m[3] * b.y;

	return result;
}

static CRUST_INLINE Crust_Mat2 crustMat2Transpose(Crust_Mat2 m)
{
	Crust_Mat2 result;

	result.m[0] = m.m[0];
	result.m[1] = m.m[2];

	result.m[2] = m.m[1];
	result.m[3] = m.m[3];

	return result;
}

static CRUST_INLINE Crust_Mat2 crustMat2Inverse(Crust_Mat2 m)
{
	const f32 *p = m.m;
	Crust_Mat2 result;

	result.m[0] =  p[3];
	result.m[1] = -p[1];

	result.m[2] = -p[2];
	result.m[3] =  p[0];

	f32 det = p[0] * result.m[0] + p[2] * result.m[1];
	CRUST_ASSERT(det != 0.0f);

	f32 inv_det = 1.0f / det;

	for (u32 i = 0; i < 4; ++i)
		result.m[i] *= inv_det;

	return result;
}

//
typedef struct Crust_Mat3_t
{
	f32 m[9];
} Crust_Mat3;

static CRUST_INLINE Crust_Mat3 crustMat3Identity(void)
{
	Crust_Mat3 result;

	result.m[0] = 1.0f; result.m[3] = 0.0f; result.m[6] = 0.0f;
	result.m[1] = 0.0f; result.m[4] = 1.0f; result.m[7] = 0.0f;
	result.m[2] = 0.0f; result.m[5] = 0.0f; result.m[8] = 1.0f;

	return result;
}

static CRUST_INLINE Crust_Mat3 crustMat3Mad(Crust_Mat3 a, f32 s, Crust_Mat3 b)
{
	Crust_Mat3 result;

	for (u32 i = 0; i < 9; ++i)
		result.m[i] = a.m[i] * s + b.m[i];

	return result;
}

static CRUST_INLINE Crust_Mat3 crustMat3AddS(Crust_Mat3 a, f32 s)
{
	Crust_Mat3 result;

	for (u32 i = 0; i < 9; ++i)
		result.m[i] = a.m[i] + s;

	return result;
}

static CRUST_INLINE Crust_Mat3 crustMat3AddM(Crust_Mat3 a, Crust_Mat3 b)
{
	Crust_Mat3 result;

	for (u32 i = 0; i < 9; ++i)
		result.m[i] = a.m[i] + b.m[i];

	return result;
}

static CRUST_INLINE Crust_Mat3 crustMat3SubS(Crust_Mat3 a, f32 s)
{
	Crust_Mat3 result;

	for (u32 i = 0; i < 9; ++i)
		result.m[i] = a.m[i] - s;

	return result;
}

static CRUST_INLINE Crust_Mat3 crustMat3SubM(Crust_Mat3 a, Crust_Mat3 b)
{
	Crust_Mat3 result;

	for (u32 i = 0; i < 9; ++i)
		result.m[i] = a.m[i] - b.m[i];

	return result;
}

static CRUST_INLINE Crust_Mat3 crustMat3MulS(Crust_Mat3 a, f32 s)
{
	Crust_Mat3 result;

	for (u32 i = 0; i < 9; ++i)
		result.m[i] = a.m[i] * s;

	return result;
}

static CRUST_INLINE Crust_Mat3 crustMat3MulM(Crust_Mat3 a, Crust_Mat3 b)
{
	Crust_Mat3 result;

	result.m[0] = a.m[0] * b.m[0] + a.m[3] * b.m[1] + a.m[6] * b.m[2];
	result.m[1] = a.m[1] * b.m[0] + a.m[4] * b.m[1] + a.m[7] * b.m[2];
	result.m[2] = a.m[2] * b.m[0] + a.m[5] * b.m[1] + a.m[8] * b.m[2];

	result.m[3] = a.m[0] * b.m[3] + a.m[3] * b.m[4] + a.m[6] * b.m[5];
	result.m[4] = a.m[1] * b.m[3] + a.m[4] * b.m[4] + a.m[7] * b.m[5];
	result.m[5] = a.m[2] * b.m[3] + a.m[5] * b.m[4] + a.m[8] * b.m[5];

	result.m[6] = a.m[0] * b.m[6] + a.m[3] * b.m[7] + a.m[6] * b.m[8];
	result.m[7] = a.m[1] * b.m[6] + a.m[4] * b.m[7] + a.m[7] * b.m[8];
	result.m[8] = a.m[2] * b.m[6] + a.m[5] * b.m[7] + a.m[8] * b.m[8];

	return result;
}

static CRUST_INLINE Crust_Vec3 crustMat3MulV(Crust_Mat3 a, Crust_Vec3 b)
{
	Crust_Vec3 result;

	result.x = a.m[0] * b.x + a.m[3] * b.y + a.m[6] * b.z;
	result.y = a.m[1] * b.x + a.m[4] * b.y + a.m[7] * b.z;
	result.z = a.m[2] * b.x + a.m[5] * b.y + a.m[8] * b.z;

	return result;
}

static CRUST_INLINE Crust_Mat3 crustMat3Transpose(Crust_Mat3 m)
{
	Crust_Mat3 result;

	result.m[0] = m.m[0];
	result.m[1] = m.m[3];
	result.m[2] = m.m[6];

	result.m[3] = m.m[1];
	result.m[4] = m.m[4];
	result.m[5] = m.m[7];

	result.m[6] = m.m[2];
	result.m[7] = m.m[5];
	result.m[8] = m.m[8];

	return result;
}

static CRUST_INLINE Crust_Mat3 crustMat3Inverse(Crust_Mat3 m)
{
	const f32 *p = m.m;
	Crust_Mat3 result;

	result.m[0] =  p[4] * p[8] - p[7] * p[5];
	result.m[1] = -p[1] * p[8] + p[7] * p[2];
	result.m[2] =  p[1] * p[5] - p[4] * p[2];

	result.m[3] = -p[3] * p[8] + p[6] * p[5];
	result.m[4] =  p[0] * p[8] - p[6] * p[2];
	result.m[5] = -p[0] * p[5] + p[3] * p[2];

	result.m[6] =  p[3] * p[7] - p[6] * p[4];
	result.m[7] = -p[0] * p[7] + p[6] * p[1];
	result.m[8] =  p[0] * p[4] - p[3] * p[1];

	f32 det = p[0] * result.m[0] + p[3] * result.m[1] + p[6] * result.m[2];
	CRUST_ASSERT(det != 0.0f);

	f32 inv_det = 1.0f / det;

	for (u32 i = 0; i < 9; ++i)
		result.m[i] *= inv_det;

	return result;
}

//
typedef struct Crust_Mat4_t
{
	f32 m[16];
} Crust_Mat4;

static CRUST_INLINE Crust_Mat4 crustMat4Identity(void)
{
	Crust_Mat4 result;

	result.m[0] = 1.0f; result.m[4] = 0.0f; result.m[8 ] = 0.0f; result.m[12] = 0.0f;
	result.m[1] = 0.0f; result.m[5] = 1.0f; result.m[9 ] = 0.0f; result.m[13] = 0.0f;
	result.m[2] = 0.0f; result.m[6] = 0.0f; result.m[10] = 1.0f; result.m[14] = 0.0f;
	result.m[3] = 0.0f; result.m[7] = 0.0f; result.m[11] = 0.0f; result.m[15] = 1.0f;

	return result;
}

static CRUST_INLINE Crust_Mat4 crustMat4Mad(Crust_Mat4 a, f32 s, Crust_Mat4 b)
{
	Crust_Mat4 result;

	for (u32 i = 0; i < 16; ++i)
		result.m[i] = a.m[i] * s + b.m[i];

	return result;
}

static CRUST_INLINE Crust_Mat4 crustMat4AddS(Crust_Mat4 a, f32 s)
{
	Crust_Mat4 result;

	for (u32 i = 0; i < 16; ++i)
		result.m[i] = a.m[i] + s;

	return result;
}

static CRUST_INLINE Crust_Mat4 crustMat4AddM(Crust_Mat4 a, Crust_Mat4 b)
{
	Crust_Mat4 result;

	for (u32 i = 0; i < 16; ++i)
		result.m[i] = a.m[i] + b.m[i];

	return result;
}

static CRUST_INLINE Crust_Mat4 crustMat4SubS(Crust_Mat4 a, f32 s)
{
	Crust_Mat4 result;

	for (u32 i = 0; i < 16; ++i)
		result.m[i] = a.m[i] - s;

	return result;
}

static CRUST_INLINE Crust_Mat4 crustMat4SubM(Crust_Mat4 a, Crust_Mat4 b)
{
	Crust_Mat4 result;

	for (u32 i = 0; i < 16; ++i)
		result.m[i] = a.m[i] - b.m[i];

	return result;
}

static CRUST_INLINE Crust_Mat4 crustMat4MulS(Crust_Mat4 a, f32 s)
{
	Crust_Mat4 result;

	for (u32 i = 0; i < 16; ++i)
		result.m[i] = a.m[i] * s;

	return result;
}

static CRUST_INLINE Crust_Mat4 crustMat4MulM(Crust_Mat4 a, Crust_Mat4 b)
{
	Crust_Mat4 result;

	result.m[0 ] = a.m[0] * b.m[0 ] + a.m[4] * b.m[1]  + a.m[8 ] * b.m[2]  + a.m[12] * b.m[3];
	result.m[1 ] = a.m[1] * b.m[0 ] + a.m[5] * b.m[1]  + a.m[9 ] * b.m[2]  + a.m[13] * b.m[3];
	result.m[2 ] = a.m[2] * b.m[0 ] + a.m[6] * b.m[1]  + a.m[10] * b.m[2]  + a.m[14] * b.m[3];
	result.m[3 ] = a.m[3] * b.m[0 ] + a.m[7] * b.m[1]  + a.m[11] * b.m[2]  + a.m[15] * b.m[3];

	result.m[4 ] = a.m[0] * b.m[4 ] + a.m[4] * b.m[5]  + a.m[8 ] * b.m[6]  + a.m[12] * b.m[7];
	result.m[5 ] = a.m[1] * b.m[4 ] + a.m[5] * b.m[5]  + a.m[9 ] * b.m[6]  + a.m[13] * b.m[7];
	result.m[6 ] = a.m[2] * b.m[4 ] + a.m[6] * b.m[5]  + a.m[10] * b.m[6]  + a.m[14] * b.m[7];
	result.m[7 ] = a.m[3] * b.m[4 ] + a.m[7] * b.m[5]  + a.m[11] * b.m[6]  + a.m[15] * b.m[7];

	result.m[8 ] = a.m[0] * b.m[8 ] + a.m[4] * b.m[9]  + a.m[8 ] * b.m[10] + a.m[12] * b.m[11];
	result.m[9 ] = a.m[1] * b.m[8 ] + a.m[5] * b.m[9]  + a.m[9 ] * b.m[10] + a.m[13] * b.m[11];
	result.m[10] = a.m[2] * b.m[8 ] + a.m[6] * b.m[9]  + a.m[10] * b.m[10] + a.m[14] * b.m[11];
	result.m[11] = a.m[3] * b.m[8 ] + a.m[7] * b.m[9]  + a.m[11] * b.m[10] + a.m[15] * b.m[11];

	result.m[12] = a.m[0] * b.m[12] + a.m[4] * b.m[13] + a.m[8 ] * b.m[14] + a.m[12] * b.m[15];
	result.m[13] = a.m[1] * b.m[12] + a.m[5] * b.m[13] + a.m[9 ] * b.m[14] + a.m[13] * b.m[15];
	result.m[14] = a.m[2] * b.m[12] + a.m[6] * b.m[13] + a.m[10] * b.m[14] + a.m[14] * b.m[15];
	result.m[15] = a.m[3] * b.m[12] + a.m[7] * b.m[13] + a.m[11] * b.m[14] + a.m[15] * b.m[15];

	return result;
}

static CRUST_INLINE Crust_Vec4 crustMat4MulV(Crust_Mat4 a, Crust_Vec4 b)
{
	Crust_Vec4 result;

	result.x = a.m[0] * b.x + a.m[4] * b.y + a.m[8 ] * b.z + a.m[12] * b.w;
	result.y = a.m[1] * b.x + a.m[5] * b.y + a.m[9 ] * b.z + a.m[13] * b.w;
	result.z = a.m[2] * b.x + a.m[6] * b.y + a.m[10] * b.z + a.m[14] * b.w;
	result.w = a.m[3] * b.x + a.m[7] * b.y + a.m[11] * b.z + a.m[15] * b.w;

	return result;
}

static CRUST_INLINE Crust_Mat4 crustMat4Transpose(Crust_Mat4 m)
{
	Crust_Mat4 result;

	result.m[0 ] = m.m[0];
	result.m[1 ] = m.m[4];
	result.m[2 ] = m.m[8];
	result.m[3 ] = m.m[12];

	result.m[4 ] = m.m[1];
	result.m[5 ] = m.m[5];
	result.m[6 ] = m.m[9];
	result.m[7 ] = m.m[13];

	result.m[8 ] = m.m[2];
	result.m[9 ] = m.m[6];
	result.m[10] = m.m[10];
	result.m[11] = m.m[14];

	result.m[12] = m.m[3];
	result.m[13] = m.m[7];
	result.m[14] = m.m[11];
	result.m[15] = m.m[15];

	return result;
}

static CRUST_INLINE Crust_Mat4 crustMat4Inverse(Crust_Mat4 m)
{
	const f32 *p = m.m;
	Crust_Mat4 result;

	f32 c1 = p[10] * p[15] - p[11] * p[14];
	f32 c2 = p[7 ] * p[14] - p[6 ] * p[15];
	f32 c3 = p[6 ] * p[11] - p[7 ] * p[10];
	f32 c4 = p[3 ] * p[14] - p[2 ] * p[15];
	f32 c5 = p[2 ] * p[11] - p[3 ] * p[10];
	f32 c6 = p[2 ] * p[7 ] - p[3 ] * p[6 ];

	f32 s1 = p[8 ] * p[13] - p[9 ] * p[12];
	f32 s2 = p[4 ] * p[13] - p[5 ] * p[12];
	f32 s3 = p[4 ] * p[9 ] - p[5 ] * p[8 ];
	f32 s4 = p[0 ] * p[13] - p[1 ] * p[12];
	f32 s5 = p[0 ] * p[9 ] - p[1 ] * p[8 ];
	f32 s6 = p[0 ] * p[5 ] - p[1 ] * p[4 ];

	result.m[0 ] =  p[5] * c1 + p[9] * c2 + p[13] * c3;
	result.m[1 ] = -p[1] * c1 - p[9] * c4 - p[13] * c5;
	result.m[2 ] = -p[1] * c2 + p[5] * c4 + p[13] * c6;
	result.m[3 ] = -p[1] * c3 + p[5] * c5 - p[9 ] * c6;

	result.m[4 ] = -p[4] * c1 - p[8] * c2 - p[12] * c3;
	result.m[5 ] =  p[0] * c1 + p[8] * c4 + p[12] * c5;
	result.m[6 ] =  p[0] * c2 - p[4] * c4 - p[12] * c6;
	result.m[7 ] =  p[0] * c3 - p[4] * c5 + p[8 ] * c6;

	result.m[8 ] =  p[7] * s1 - p[11] * s2 + p[15] * s3;
	result.m[9 ] = -p[3] * s1 + p[11] * s4 - p[15] * s5;
	result.m[10] =  p[3] * s2 - p[7 ] * s4 + p[15] * s6;
	result.m[11] = -p[3] * s3 + p[7 ] * s5 - p[11] * s6;

	result.m[12] = -p[6] * s1 + p[10] * s2 - p[14] * s3;
	result.m[13] =  p[2] * s1 - p[10] * s4 + p[14] * s5;
	result.m[14] = -p[2] * s2 + p[6 ] * s4 - p[14] * s6;
	result.m[15] =  p[2] * s3 - p[6 ] * s5 + p[10] * s6;

	f32 det = p[0] * result.m[0] + p[4] * result.m[1] + p[8] * result.m[2] + p[12] * result.m[3];
	CRUST_ASSERT(det != 0.0f);

	f32 inv_det = 1.0f / det;

	for (u32 i = 0; i < 16; ++i)
		result.m[i] *= inv_det;

	return result;
}

#ifdef __cplusplus
}
#endif
