#pragma once

// Version
#define CRUST_VERSION_MAJOR 1
#define CRUST_VERSION_MINOR 0
#define CRUST_VERSION_PATCH 0
#define CRUST_VERSION "1.0.0-dev"

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_MSC_VER)
	#include <intrin.h>
	#include <string.h>

	#define CRUST_ASSERT(x)		do { (!!(x)) || (__debugbreak(), 0); } while(0);
	#define CRUST_UNUSED(x)		do { (void)(x); } while(0)

	#define CRUST_NULL			NULL
	#define CRUST_INLINE		__forceinline
	#define CRUST_RESTRICT		__restrict

	typedef signed char			i8;
	typedef signed short		i16;
	typedef signed int			i32;
	typedef signed long long	i64;

	typedef unsigned char		u8;
	typedef unsigned short		u16;
	typedef unsigned int		u32;
	typedef unsigned long long	u64;

	#if defined(_WIN64)
		typedef i64				isize;
		typedef u64				usize;
	#else
		typedef i32				isize;
		typedef u32				usize;
	#endif

	typedef float				f32;
	typedef double				f64;

	#define I8_MIN				-128i8
	#define I16_MIN				-32768i16
	#define I32_MIN				-2147483648i32
	#define I64_MIN				-9223372036854775808i64
	#define I8_MAX				127i8
	#define I16_MAX				32767i16
	#define I32_MAX				2147483647i32
	#define I64_MAX				9223372036854775807i64
	#define U8_MAX				255ui8
	#define U16_MAX				65535ui16
	#define U32_MAX				4294967295ui32
	#define U64_MAX				18446744073709551615ui64

	#if defined(_WIN64)
		#define USIZE_MAX		U64_MAX
		#define ISIZE_MAX		I64_MAX
		#define ISIZE_MIN		I64_MIN
	#else
		#define USIZE_MAX		U32_MAX
		#define ISIZE_MAX		I32_MAX
		#define ISIZE_MIN		I32_MIN
	#endif

	#define F64_EPSILON			2.2204460492503131e-016
	#define F64_MAX				1.7976931348623158e+308
	#define F64_MIN				-1.7976931348623158e+308
	#define F64_POSITIVE_MIN	2.2250738585072014e-308

	#define F32_EPSILON			1.192092896e-07F
	#define F32_MAX				3.402823466e+38F
	#define F32_MIN				-3.402823466e+38F
	#define F32_POSITIVE_MIN	1.175494351e-38F

	CRUST_INLINE void crustMemcpy(void *dst, const void *src, usize size)
	{
		memcpy(dst, src, size);
	}

	CRUST_INLINE void *crustMalloc(usize size)
	{
		return malloc(size);
	}

	CRUST_INLINE void *crustRealloc(void *ptr, usize size)
	{
		return realloc(ptr, size);
	}

	CRUST_INLINE void crustFree(void *ptr)
	{
		free(ptr);
	}

	CRUST_INLINE f32 crustRsqrtf32(f32 v)
	{
		return _mm_cvtss_f32(_mm_rsqrt_ss(_mm_set_ss(v)));
	}

	CRUST_INLINE f32 crustSqrtf32(f32 v)
	{
		return _mm_cvtss_f32(_mm_sqrt_ss(_mm_set_ss(v)));
	}

	CRUST_INLINE f32 crustCosf32(f32 v)
	{
		return _mm_cvtss_f32(_mm_cos_ps(_mm_set_ss(v)));
	}

	CRUST_INLINE f32 crustAcosf32(f32 v)
	{
		return _mm_cvtss_f32(_mm_acos_ps(_mm_set_ss(v)));
	}

	CRUST_INLINE f32 crustSinf32(f32 v)
	{
		return _mm_cvtss_f32(_mm_sin_ps(_mm_set_ss(v)));
	}

	CRUST_INLINE f32 crustAsinf32(f32 v)
	{
		return _mm_cvtss_f32(_mm_asin_ps(_mm_set_ss(v)));
	}

	CRUST_INLINE f32 crustTanf32(f32 v)
	{
		return _mm_cvtss_f32(_mm_tan_ps(_mm_set_ss(v)));
	}

	CRUST_INLINE f32 crustAtan2f32(f32 y, f32 x)
	{
		return _mm_cvtss_f32(_mm_atan2_ps(_mm_set_ss(y), _mm_set_ss(x)));
	}

	CRUST_INLINE u32 crustLzcntu32(u32 value)
	{
		CRUST_ASSERT(value != 0);

		unsigned long result = 0;
		_BitScanReverse(&result, value);
		return 31 - result;
	}

	CRUST_INLINE u32 crustTzcntu32(u32 value)
	{
		CRUST_ASSERT(value != 0);

		unsigned long result = 0;
		_BitScanForward(&result, value);
		return result;
	}

	CRUST_INLINE u32 crustPopcntu32(u32 value)
	{
		return __popcnt(value);
	}
#else
	#error "Unsupported platform"
#endif

//
CRUST_INLINE u8 crustIsPow2u8(u8 v)
{
	CRUST_ASSERT(v != 0);
	return (v & (v - 1)) == 0;
}

CRUST_INLINE u16 crustIsPow2u16(u16 v)
{
	CRUST_ASSERT(v != 0);
	return (v & (v - 1)) == 0;
}

CRUST_INLINE u32 crustIsPow2u32(u32 v)
{
	CRUST_ASSERT(v != 0);
	return (v & (v - 1)) == 0;
}

CRUST_INLINE u64 crustIsPow2u64(u64 v)
{
	CRUST_ASSERT(v != 0);
	return (v & (v - 1)) == 0;
}

CRUST_INLINE usize crustIsPow2us(usize v)
{
	CRUST_ASSERT(v != 0);
	return (v & (v - 1)) == 0;
}

CRUST_INLINE u8 crustIsAlignedu8(u8 v, u8 alignment)
{
	CRUST_ASSERT(alignment != 0);
	CRUST_ASSERT(crustIsPow2u8(alignment) != 0);

	return (v & (alignment - 1)) == 0;
}

CRUST_INLINE u16 crustIsAlignedu16(u16 v, u16 alignment)
{
	CRUST_ASSERT(alignment != 0);
	CRUST_ASSERT(crustIsPow2u16(alignment) != 0);

	return (v & (alignment - 1)) == 0;
}

CRUST_INLINE u32 crustIsAlignedu32(u32 v, u32 alignment)
{
	CRUST_ASSERT(alignment != 0);
	CRUST_ASSERT(crustIsPow2u32(alignment) != 0);

	return (v & (alignment - 1)) == 0;
}

CRUST_INLINE u64 crustIsAlignedu64(u64 v, u64 alignment)
{
	CRUST_ASSERT(alignment != 0);
	CRUST_ASSERT(crustIsPow2u64(alignment) != 0);

	return (v & (alignment - 1)) == 0;
}

CRUST_INLINE u8 crustAlignDownu8(u8 value, u8 alignment)
{
	CRUST_ASSERT(alignment != 0);
	CRUST_ASSERT(crustIsPow2u8(alignment) != 0);

	return value & ~(alignment - 1);
}

CRUST_INLINE u16 crustAlignDownu16(u16 value, u16 alignment)
{
	CRUST_ASSERT(alignment != 0);
	CRUST_ASSERT(crustIsPow2u16(alignment) != 0);

	return value & ~(alignment - 1);
}

CRUST_INLINE u32 crustAlignDownu32(u32 value, u32 alignment)
{
	CRUST_ASSERT(alignment != 0);
	CRUST_ASSERT(crustIsPow2u32(alignment) != 0);

	return value & ~(alignment - 1);
}

CRUST_INLINE u64 crustAlignDownu64(u64 value, u64 alignment)
{
	CRUST_ASSERT(alignment != 0);
	CRUST_ASSERT(crustIsPow2u64(alignment) != 0);

	return value & ~(alignment - 1);
}

CRUST_INLINE usize crustAlignDownus(usize value, usize alignment)
{
	CRUST_ASSERT(alignment != 0);
	CRUST_ASSERT(crustIsPow2us(alignment) != 0);

	return value & ~(alignment - 1);
}

CRUST_INLINE u8 crustAlignUpu8(u8 value, u8 alignment)
{
	CRUST_ASSERT(alignment != 0);
	CRUST_ASSERT(crustIsPow2u8(alignment) != 0);

	u8 mask = alignment - 1;
	return (value + mask) & ~mask;
}

CRUST_INLINE u16 crustAlignUpu16(u16 value, u16 alignment)
{
	CRUST_ASSERT(alignment != 0);
	CRUST_ASSERT(crustIsPow2u16(alignment) != 0);

	u16 mask = alignment - 1;
	return (value + mask) & ~mask;
}

CRUST_INLINE u32 crustAlignUpu32(u32 value, u32 alignment)
{
	CRUST_ASSERT(alignment != 0);
	CRUST_ASSERT(crustIsPow2u32(alignment) != 0);

	u32 mask = alignment - 1;
	return (value + mask) & ~mask;
}

CRUST_INLINE u64 crustAlignUpu64(u64 value, u64 alignment)
{
	CRUST_ASSERT(alignment != 0);
	CRUST_ASSERT(crustIsPow2u64(alignment) != 0);

	u64 mask = alignment - 1;
	return (value + mask) & ~mask;
}

CRUST_INLINE usize crustAlignUpus(usize value, usize alignment)
{
	CRUST_ASSERT(alignment != 0);
	CRUST_ASSERT(crustIsPow2us(alignment) != 0);

	usize mask = alignment - 1;
	return (value + mask) & ~mask;
}

CRUST_INLINE u8 crustMinu8(u8 a, u8 b)
{
	return (a < b) ? a : b;
}

CRUST_INLINE u16 crustMinu16(u16 a, u16 b)
{
	return (a < b) ? a : b;
}

CRUST_INLINE u32 crustMinu32(u32 a, u32 b)
{
	return (a < b) ? a : b;
}

CRUST_INLINE u64 crustMinu64(u64 a, u64 b)
{
	return (a < b) ? a : b;
}

CRUST_INLINE i8 crustMini8(i8 a, i8 b)
{
	return (a < b) ? a : b;
}

CRUST_INLINE i16 crustMini16(i16 a, i16 b)
{
	return (a < b) ? a : b;
}

CRUST_INLINE i32 crustMini32(i32 a, i32 b)
{
	return (a < b) ? a : b;
}

CRUST_INLINE i64 crustMini64(i64 a, i64 b)
{
	return (a < b) ? a : b;
}

CRUST_INLINE f32 crustMinf32(f32 a, f32 b)
{
	return (a < b) ? a : b;
}

CRUST_INLINE f64 crustMinf64(f64 a, f64 b)
{
	return (a < b) ? a : b;
}

CRUST_INLINE u8 crustMaxu8(u8 a, u8 b)
{
	return (a < b) ? b : a;
}

CRUST_INLINE u16 crustMaxu16(u16 a, u16 b)
{
	return (a < b) ? b : a;
}

CRUST_INLINE u32 crustMaxu32(u32 a, u32 b)
{
	return (a < b) ? b : a;
}

CRUST_INLINE u64 crustMaxu64(u64 a, u64 b)
{
	return (a < b) ? b : a;
}

CRUST_INLINE usize crustMaxus(usize a, usize b)
{
	return (a < b) ? b : a;
}

CRUST_INLINE i8 crustMaxi8(i8 a, i8 b)
{
	return (a < b) ? b : a;
}

CRUST_INLINE i16 crustMaxi16(i16 a, i16 b)
{
	return (a < b) ? b : a;
}

CRUST_INLINE i32 crustMaxi32(i32 a, i32 b)
{
	return (a < b) ? b : a;
}

CRUST_INLINE i64 crustMaxi64(i64 a, i64 b)
{
	return (a < b) ? b : a;
}

CRUST_INLINE isize crustMaxis(isize a, isize b)
{
	return (a < b) ? b : a;
}

CRUST_INLINE f32 crustMaxf32(f32 a, f32 b)
{
	return (a < b) ? b : a;
}

CRUST_INLINE f64 crustMaxf64(f64 a, f64 b)
{
	return (a < b) ? b : a;
}

//
typedef struct Crust_Vec2_t
{
	f32 x, y;
} Crust_Vec2;

typedef struct Crust_Vec3_t
{
	f32 x, y, z;
} Crust_Vec3;

typedef struct Crust_Vec4_t
{
	f32 x, y, z, w;
} Crust_Vec4;

typedef struct Crust_Quat_t
{
	f32 x, y, z, w;
} Crust_Quat;

typedef struct Crust_Transform_t
{
	Crust_Vec3 position;
	Crust_Quat rotation;
	Crust_Vec3 scale;
} Crust_Transform;

//
CRUST_INLINE Crust_Vec2 crustVec2Mad(Crust_Vec2 a, f32 s, Crust_Vec2 b)
{
	Crust_Vec2 result;
	result.x = a.x * s + b.x;
	result.y = a.y * s + b.y;

	return result;
}

CRUST_INLINE Crust_Vec2 crustVec2Adds(Crust_Vec2 a, f32 s)
{
	Crust_Vec2 result;
	result.x = a.x + s;
	result.y = a.y + s;

	return result;
}

CRUST_INLINE Crust_Vec2 crustVec2Addv(Crust_Vec2 a, Crust_Vec2 b)
{
	Crust_Vec2 result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;

	return result;
}

CRUST_INLINE Crust_Vec2 crustVec2Subs(Crust_Vec2 a, f32 s)
{
	Crust_Vec2 result;
	result.x = a.x - s;
	result.y = a.y - s;

	return result;
}

CRUST_INLINE Crust_Vec2 crustVec2Subv(Crust_Vec2 a, Crust_Vec2 b)
{
	Crust_Vec2 result;
	result.x = a.x - b.x;
	result.y = a.y - b.y;

	return result;
}

CRUST_INLINE Crust_Vec2 crustVec2Muls(Crust_Vec2 a, f32 s)
{
	Crust_Vec2 result;
	result.x = a.x * s;
	result.y = a.y * s;

	return result;
}

CRUST_INLINE Crust_Vec2 crustVec2Mulv(Crust_Vec2 a, Crust_Vec2 b)
{
	Crust_Vec2 result;
	result.x = a.x * b.x;
	result.y = a.y * b.y;

	return result;
}

CRUST_INLINE Crust_Vec2 crustVec2Divs(Crust_Vec2 a, f32 s)
{
	float s_inv = 1.0f / s;

	Crust_Vec2 result;
	result.x = a.x * s_inv;
	result.y = a.y * s_inv;

	return result;
}

CRUST_INLINE Crust_Vec2 crustVec2Divv(Crust_Vec2 a, Crust_Vec2 b)
{
	Crust_Vec2 result;
	result.x = a.x / b.x;
	result.y = a.y / b.y;

	return result;
}

CRUST_INLINE Crust_Vec2 crustVec2Lerp(Crust_Vec2 a, Crust_Vec2 b, f32 t)
{
	Crust_Vec2 result;
	result.x = a.x + (b.x - a.x) * t;
	result.y = a.y + (b.y - a.y) * t;

	return result;
}

CRUST_INLINE Crust_Vec2 crustVec2Normalize(Crust_Vec2 v)
{
	f32 len_inv = crustRsqrtf32(v.x * v.x + v.y * v.y);

	Crust_Vec2 result;
	result.x = v.x * len_inv;
	result.y = v.y * len_inv;

	return result;
}

CRUST_INLINE f32 crustVec2Dot(Crust_Vec2 a, Crust_Vec2 b)
{
	return a.x * b.x + a.y * b.y;
}

CRUST_INLINE f32 crustVec2Length(Crust_Vec2 v)
{
	return crustSqrtf32(v.x * v.x + v.y * v.y);
}

CRUST_INLINE f32 crustVec2Length2(Crust_Vec2 v)
{
	return v.x * v.x + v.y * v.y;
}

//
CRUST_INLINE Crust_Vec3 crustVec3Mad(Crust_Vec3 a, f32 s, Crust_Vec3 b)
{
	Crust_Vec3 result;
	result.x = a.x * s + b.x;
	result.y = a.y * s + b.y;
	result.z = a.z * s + b.z;

	return result;
}

CRUST_INLINE Crust_Vec3 crustVec3Adds(Crust_Vec3 a, f32 s)
{
	Crust_Vec3 result;
	result.x = a.x + s;
	result.y = a.y + s;
	result.z = a.z + s;

	return result;
}

CRUST_INLINE Crust_Vec3 crustVec3Addv(Crust_Vec3 a, Crust_Vec3 b)
{
	Crust_Vec3 result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;

	return result;
}

CRUST_INLINE Crust_Vec3 crustVec3Subs(Crust_Vec3 a, f32 s)
{
	Crust_Vec3 result;
	result.x = a.x - s;
	result.y = a.y - s;
	result.z = a.z - s;

	return result;
}

CRUST_INLINE Crust_Vec3 crustVec3Subv(Crust_Vec3 a, Crust_Vec3 b)
{
	Crust_Vec3 result;
	result.x = a.x - b.x;
	result.y = a.y - b.y;
	result.z = a.z - b.z;

	return result;
}

CRUST_INLINE Crust_Vec3 crustVec3Muls(Crust_Vec3 a, f32 s)
{
	Crust_Vec3 result;
	result.x = a.x * s;
	result.y = a.y * s;
	result.z = a.z * s;

	return result;
}

CRUST_INLINE Crust_Vec3 crustVec3Mulv(Crust_Vec3 a, Crust_Vec3 b)
{
	Crust_Vec3 result;
	result.x = a.x * b.x;
	result.y = a.y * b.y;
	result.z = a.z * b.z;

	return result;
}

CRUST_INLINE Crust_Vec3 crustVec3Divs(Crust_Vec3 a, f32 s)
{
	float s_inv = 1.0f / s;

	Crust_Vec3 result;
	result.x = a.x * s_inv;
	result.y = a.y * s_inv;
	result.z = a.z * s_inv;

	return result;
}

CRUST_INLINE Crust_Vec3 crustVec3Divv(Crust_Vec3 a, Crust_Vec3 b)
{
	Crust_Vec3 result;
	result.x = a.x / b.x;
	result.y = a.y / b.y;
	result.z = a.z / b.z;

	return result;
}

CRUST_INLINE Crust_Vec3 crustVec3Lerp(Crust_Vec3 a, Crust_Vec3 b, f32 t)
{
	Crust_Vec3 result;
	result.x = a.x + (b.x - a.x) * t;
	result.y = a.y + (b.y - a.y) * t;
	result.z = a.z + (b.z - a.z) * t;

	return result;
}

CRUST_INLINE Crust_Vec3 crustVec3Normalize(Crust_Vec3 v)
{
	f32 len_inv = crustRsqrtf32(v.x * v.x + v.y * v.y + v.z * v.z);

	Crust_Vec3 result;
	result.x = v.x * len_inv;
	result.y = v.y * len_inv;
	result.z = v.z * len_inv;

	return result;
}

CRUST_INLINE Crust_Vec3 crustVec3Cross(Crust_Vec3 a, Crust_Vec3 b)
{
	Crust_Vec3 result;
	result.x = a.y * b.z - a.z * b.y;
	result.y = a.z * b.x - a.x * b.z;
	result.z = a.x * b.y - a.y * b.x;

	return result;
}

CRUST_INLINE f32 crustVec3Dot(Crust_Vec3 a, Crust_Vec3 b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

CRUST_INLINE f32 crustVec3Length(Crust_Vec3 v)
{
	return crustSqrtf32(v.x * v.x + v.y * v.y + v.z * v.z);
}

CRUST_INLINE f32 crustVec3Length2(Crust_Vec3 v)
{
	return v.x * v.x + v.y * v.y + v.z * v.z;
}

//
CRUST_INLINE Crust_Vec4 crustVec4Mad(Crust_Vec4 a, f32 s, Crust_Vec4 b)
{
	Crust_Vec4 result;
	result.x = a.x * s + b.x;
	result.y = a.y * s + b.y;
	result.z = a.z * s + b.z;
	result.w = a.w * s + b.w;

	return result;
}

CRUST_INLINE Crust_Vec4 crustVec4Adds(Crust_Vec4 a, f32 s)
{
	Crust_Vec4 result;
	result.x = a.x + s;
	result.y = a.y + s;
	result.z = a.z + s;
	result.w = a.w + s;

	return result;
}

CRUST_INLINE Crust_Vec4 crustVec4Addv(Crust_Vec4 a, Crust_Vec4 b)
{
	Crust_Vec4 result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;
	result.w = a.w + b.w;

	return result;
}

CRUST_INLINE Crust_Vec4 crustVec4Subs(Crust_Vec4 a, f32 s)
{
	Crust_Vec4 result;
	result.x = a.x - s;
	result.y = a.y - s;
	result.z = a.z - s;
	result.w = a.w - s;

	return result;
}

CRUST_INLINE Crust_Vec4 crustVec4Subv(Crust_Vec4 a, Crust_Vec4 b)
{
	Crust_Vec4 result;
	result.x = a.x - b.x;
	result.y = a.y - b.y;
	result.z = a.z - b.z;
	result.w = a.w - b.w;

	return result;
}

CRUST_INLINE Crust_Vec4 crustVec4Muls(Crust_Vec4 a, f32 s)
{
	Crust_Vec4 result;
	result.x = a.x * s;
	result.y = a.y * s;
	result.z = a.z * s;
	result.w = a.w * s;

	return result;
}

CRUST_INLINE Crust_Vec4 crustVec4Mulv(Crust_Vec4 a, Crust_Vec4 b)
{
	Crust_Vec4 result;
	result.x = a.x * b.x;
	result.y = a.y * b.y;
	result.z = a.z * b.z;
	result.w = a.w * b.w;

	return result;
}

CRUST_INLINE Crust_Vec4 crustVec4Divs(Crust_Vec4 a, f32 s)
{
	float s_inv = 1.0f / s;

	Crust_Vec4 result;
	result.x = a.x * s_inv;
	result.y = a.y * s_inv;
	result.z = a.z * s_inv;
	result.w = a.w * s_inv;

	return result;
}

CRUST_INLINE Crust_Vec4 crustVec4Divv(Crust_Vec4 a, Crust_Vec4 b)
{
	Crust_Vec4 result;
	result.x = a.x / b.x;
	result.y = a.y / b.y;
	result.z = a.z / b.z;
	result.w = a.w / b.w;

	return result;
}

CRUST_INLINE Crust_Vec4 crustVec4Lerp(Crust_Vec4 a, Crust_Vec4 b, f32 t)
{
	Crust_Vec4 result;
	result.x = a.x + (b.x - a.x) * t;
	result.y = a.y + (b.y - a.y) * t;
	result.z = a.z + (b.z - a.z) * t;
	result.w = a.w + (b.w - a.w) * t;

	return result;
}

CRUST_INLINE Crust_Vec4 crustVec4Normalize(Crust_Vec4 v)
{
	f32 len_inv = crustRsqrtf32(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);

	Crust_Vec4 result;
	result.x = v.x * len_inv;
	result.y = v.y * len_inv;
	result.z = v.z * len_inv;
	result.w = v.w * len_inv;

	return result;
}

CRUST_INLINE f32 crustVec4Dot(Crust_Vec4 a, Crust_Vec4 b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

CRUST_INLINE f32 crustVec4Length(Crust_Vec4 v)
{
	return crustSqrtf32(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
}

CRUST_INLINE f32 crustVec4Length2(Crust_Vec4 v)
{
	return v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w;
}

//
CRUST_INLINE Crust_Quat crustQuatMad(Crust_Quat a, f32 s, Crust_Quat b)
{
	Crust_Quat result;
	result.x = a.x * s + b.x;
	result.y = a.y * s + b.y;
	result.z = a.z * s + b.z;
	result.w = a.w * s + b.w;

	return result;
}

CRUST_INLINE Crust_Quat crustQuatMul(Crust_Quat a, Crust_Quat b)
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

CRUST_INLINE Crust_Quat crustQuatConjugate(Crust_Quat q)
{
	Crust_Quat result;
	result.x = -q.x;
	result.y = -q.y;
	result.z = -q.z;
	result.w =  q.w;

	return result;
}

CRUST_INLINE f32 crustQuatDot(Crust_Quat a, Crust_Quat b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

CRUST_INLINE Crust_Quat crustQuatNormalize(Crust_Quat q)
{
	f32 len_inv = crustRsqrtf32(crustQuatDot(q, q));

	Crust_Quat result;
	result.x = q.x * len_inv;
	result.y = q.y * len_inv;
	result.z = q.z * len_inv;
	result.w = q.w * len_inv;

	return result;
}

CRUST_INLINE Crust_Quat crustQuatNlerp(Crust_Quat a, Crust_Quat b, f32 t)
{
	Crust_Quat result;
	result.x = a.x + (b.x - a.x) * t;
	result.y = a.y + (b.y - a.y) * t;
	result.z = a.z + (b.z - a.z) * t;
	result.w = a.w + (b.w - a.w) * t;

	return crustQuatNormalize(result);
}

CRUST_INLINE Crust_Quat crustQuatSlerp(Crust_Quat a, Crust_Quat b, f32 t)
{
	f32 cos_half_theta = crustQuatDot(a, b);

	f32 half_theta = crustAcosf32(cos_half_theta);
	f32 sin_half_theta = crustSqrtf32(1.0f - cos_half_theta * cos_half_theta);

	f32 ra = crustSinf32((1.0f - t) * half_theta) / sin_half_theta;
	f32 rb = crustSinf32(t * half_theta) / sin_half_theta;

	Crust_Quat result;
	result.x = a.x * ra + b.x * rb;
	result.y = a.y * ra + b.y * rb;
	result.z = a.z * ra + b.z * rb;
	result.w = a.w * ra + b.w * rb;

	return result;
}

CRUST_INLINE Crust_Vec3 crustQuatRotate(Crust_Quat a, Crust_Vec3 v)
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

//
CRUST_INLINE Crust_Transform crustInvertTransform(Crust_Transform t)
{
	Crust_Transform result;
	result.scale = {1.0f / t.scale.x, 1.0f / t.scale.y, 1.0f / t.scale.z};
	result.rotation = crustQuatConjugate(t.rotation);
	result.position = crustQuatRotate(result.rotation, crustVec3Mulv(result.scale, crustVec3Muls(t.position, -1.0f)));

	return result;
}

CRUST_INLINE Crust_Transform crustMulTransform(Crust_Transform a, Crust_Transform b)
{
	Crust_Transform result;
	result.scale = crustVec3Mulv(a.scale, b.scale);
	result.rotation = crustQuatMul(a.rotation, b.rotation);
	result.position = crustVec3Addv(a.position, crustQuatRotate(a.rotation, crustVec3Mulv(a.scale, b.position)));

	return result;
}

//
typedef enum Crust_AllocatorType_t
{
	CRUST_ALLOCATOR_TYPE_SYSTEM = 0,
	CRUST_ALLOCATOR_TYPE_ARENA,

	CRUST_ALLOCATOR_TYPE_ENUM_MAX,
	CRUST_ALLOCATOR_TYPE_ENUM_FORCE32 = 0x7FFFFFFF,
} Crust_AllocatorType;

typedef void *(*PFN_crustAlloc)(void *allocator, usize size);
typedef void *(*PFN_crustRealloc)(void *allocator, void *ptr, usize size);
typedef void (*PFN_crustFree)(void *allocator, void *ptr);

typedef struct Crust_AllocatorVtbl_t
{
	PFN_crustAlloc alloc;
	PFN_crustRealloc realloc;
	PFN_crustFree free;
} Crust_AllocatorVtbl;

typedef struct Crust_Allocator_t
{
	Crust_AllocatorVtbl vtbl;
	Crust_AllocatorType type;
} Crust_Allocator;

CRUST_INLINE void *crustAllocatorAlloc(Crust_Allocator *allocator, usize size)
{
	CRUST_ASSERT(allocator != CRUST_NULL);
	return allocator->vtbl.alloc(allocator, size);
}

CRUST_INLINE void *crustAllocatorRealloc(Crust_Allocator *allocator, void *ptr, usize size)
{
	CRUST_ASSERT(allocator != CRUST_NULL);
	return allocator->vtbl.realloc(allocator, ptr, size);
}

CRUST_INLINE void crustAllocatorFree(Crust_Allocator *allocator, void *ptr)
{
	CRUST_ASSERT(allocator != CRUST_NULL);
	return allocator->vtbl.free(allocator, ptr);
}

//
typedef struct Crust_SystemAllocator_t
{
	Crust_AllocatorVtbl vtbl;
	Crust_AllocatorType type;
} Crust_SystemAllocator;

CRUST_INLINE void *crustSystemAllocatorAlloc(void *allocator, usize size)
{
	Crust_SystemAllocator *system = (Crust_SystemAllocator *)allocator;
	
	CRUST_ASSERT(system != CRUST_NULL);
	CRUST_ASSERT(system->type == CRUST_ALLOCATOR_TYPE_SYSTEM);
	CRUST_UNUSED(system);

	return crustMalloc(size);
}

CRUST_INLINE void *crustSystemAllocatorRealloc(void *allocator, void *ptr, usize size)
{
	Crust_SystemAllocator *system = (Crust_SystemAllocator *)allocator;
	
	CRUST_ASSERT(system != CRUST_NULL);
	CRUST_ASSERT(system->type == CRUST_ALLOCATOR_TYPE_SYSTEM);
	CRUST_UNUSED(system);

	return crustRealloc(ptr, size);
}

CRUST_INLINE void crustSystemAllocatorFree(void *allocator, void *ptr)
{
	Crust_SystemAllocator *system = (Crust_SystemAllocator *)allocator;
	
	CRUST_ASSERT(system != CRUST_NULL);
	CRUST_ASSERT(system->type == CRUST_ALLOCATOR_TYPE_SYSTEM);
	CRUST_UNUSED(system);

	crustFree(ptr);
}

CRUST_INLINE Crust_SystemAllocator crustSystemAllocatorInit()
{
	Crust_SystemAllocator result;
	result.vtbl.alloc = crustSystemAllocatorAlloc;
	result.vtbl.realloc = crustSystemAllocatorRealloc;
	result.vtbl.free = crustSystemAllocatorFree;
	result.type = CRUST_ALLOCATOR_TYPE_SYSTEM;

	return result;
}

//
typedef struct Crust_ArenaAllocator_t
{
	Crust_AllocatorVtbl vtbl;
	Crust_AllocatorType type;

	void *memory;
	usize size;
	usize capacity;
} Crust_ArenaAllocator;

CRUST_INLINE void *crustArenaAllocatorAlloc(void *allocator, usize size)
{
	Crust_ArenaAllocator *arena = (Crust_ArenaAllocator *)allocator;

	CRUST_ASSERT(arena != CRUST_NULL);
	CRUST_ASSERT(arena->type == CRUST_ALLOCATOR_TYPE_ARENA);
	CRUST_ASSERT(arena->memory != CRUST_NULL);
	CRUST_ASSERT(arena->capacity > 0);
	CRUST_ASSERT(arena->size + size <= arena->capacity);

	u8 *ptr = (u8 *)arena->memory + arena->size;
	arena->size += size;

	return ptr;
}

CRUST_INLINE void *crustArenaAllocatorRealloc(void *allocator, void *ptr, usize size)
{
	CRUST_UNUSED(ptr);
	return crustArenaAllocatorAlloc(allocator, size);
}

CRUST_INLINE void crustArenaAllocatorFree(void *allocator, void *ptr)
{
	Crust_ArenaAllocator *arena = (Crust_ArenaAllocator *)allocator;

	CRUST_ASSERT(arena != CRUST_NULL);
	CRUST_ASSERT(arena->type == CRUST_ALLOCATOR_TYPE_ARENA);
	CRUST_ASSERT(arena->memory != CRUST_NULL);
	CRUST_ASSERT(arena->capacity > 0);

	CRUST_UNUSED(arena);
	CRUST_UNUSED(ptr);
}

CRUST_INLINE Crust_ArenaAllocator crustArenaAllocatorInit(void *memory, usize capacity)
{
	CRUST_ASSERT(memory != CRUST_NULL);
	CRUST_ASSERT(capacity > 0);

	Crust_ArenaAllocator result;
	result.vtbl.alloc = crustArenaAllocatorAlloc;
	result.vtbl.realloc = crustArenaAllocatorRealloc;
	result.vtbl.free = crustArenaAllocatorFree;

	result.type = CRUST_ALLOCATOR_TYPE_ARENA;
	result.memory = memory;
	result.capacity = capacity;
	result.size = 0;

	return result;
}

CRUST_INLINE void crustArenaAllocatorReset(Crust_ArenaAllocator *arena)
{
	CRUST_ASSERT(arena != CRUST_NULL);
	CRUST_ASSERT(arena->memory != CRUST_NULL);
	CRUST_ASSERT(arena->capacity > 0);

	arena->size = 0;
}

//
typedef struct Crust_RingBuffer_t
{
	void *memory;
	usize capacity;
	usize read;
	usize write;
} Crust_RingBuffer;

CRUST_INLINE Crust_RingBuffer crustRingBufferInit(void *memory, usize capacity)
{
	CRUST_ASSERT(memory != CRUST_NULL);
	CRUST_ASSERT(capacity > 0);

	Crust_RingBuffer result;
	result.memory = memory;
	result.capacity = capacity;
	result.read = 0;
	result.write = 0;

	return result;
}

CRUST_INLINE usize crustRingBufferSize(const Crust_RingBuffer *ring)
{
	CRUST_ASSERT(ring != CRUST_NULL);
	CRUST_ASSERT(ring->memory != CRUST_NULL);

	return (ring->write - ring->read + ring->capacity) % ring->capacity;
}

CRUST_INLINE void crustRingBufferRead(Crust_RingBuffer *ring, void *data, usize size)
{
	CRUST_ASSERT(ring != CRUST_NULL);
	CRUST_ASSERT(ring->memory != CRUST_NULL);
	CRUST_ASSERT(crustRingBufferSize(ring) >= size);
	CRUST_ASSERT(data != CRUST_NULL);

	if (size == 0)
		return;

	usize begin = ring->read;
	usize end = (begin + size) % ring->capacity;

	ring->read = end;

	u8 *ptr = (u8 *)ring->memory + begin;
	if (begin < end)
	{
		crustMemcpy(data, ptr, size);
	}
	else
	{
		usize remainder = end;
		usize base = size - remainder;
		crustMemcpy(data, ptr, base);
		crustMemcpy((u8 *)data + base, ring->memory, remainder);
	}

}

CRUST_INLINE void crustRingBufferWrite(Crust_RingBuffer *ring, const void *data, usize size)
{
	CRUST_ASSERT(ring != CRUST_NULL);
	CRUST_ASSERT(ring->memory != CRUST_NULL);
	CRUST_ASSERT(crustRingBufferSize(ring) + size <= ring->capacity);
	CRUST_ASSERT(data != CRUST_NULL);

	if (size == 0)
		return;

	usize begin = ring->write;
	usize end = (begin + size) % ring->capacity;

	ring->write = end;

	u8 *ptr = (u8 *)ring->memory + begin;
	if (begin < end)
	{
		crustMemcpy(ptr, data, size);
	}
	else
	{
		usize remainder = end;
		usize base = size - remainder;
		crustMemcpy(ptr, data, base);
		crustMemcpy(ring->memory, (const u8 *)data + base, remainder);
	}
}

//
typedef struct Crust_BipBuffer_t
{
	void *memory;
	usize capacity;
	usize begin[2];
	usize end[2];
	usize staged;
	u16 reader;
	u16 writer;
} Crust_BipBuffer;

CRUST_INLINE Crust_BipBuffer crustBipBufferInit(void *memory, usize capacity)
{
	CRUST_ASSERT(memory != CRUST_NULL);
	CRUST_ASSERT(capacity > 0);

	Crust_BipBuffer result;
	result.memory = memory;
	result.capacity = capacity;

	for (u32 i = 0; i < 2; ++i)
	{
		result.begin[i] = 0;
		result.end[i] = 0;
	}
	result.staged = 0;
	result.reader = 0;
	result.writer = 0;

	return result;
}

CRUST_INLINE void crustBipBufferConsume(Crust_BipBuffer *bip, void *data, usize size)
{
	CRUST_ASSERT(bip != CRUST_NULL);
	CRUST_ASSERT(bip->memory != CRUST_NULL);
	CRUST_ASSERT(bip->staged == 0);
	CRUST_ASSERT(data != CRUST_NULL);
	CRUST_ASSERT(size > 0);

	u16 r = bip->reader;
	u16 w = bip->writer;

	usize sizes[2];
	for (u32 i = 0; i < 2; ++i)
		sizes[i] = bip->end[i] - bip->begin[i];

	if (sizes[r] >= size)
	{
		crustMemcpy(data, (const u8 *)bip->memory + bip->begin[r], size);
		bip->begin[r] += size;
	}
	else
	{
		CRUST_ASSERT(r != w);
		CRUST_ASSERT(sizes[r] + sizes[w] >= size);

		u8 *dst = (u8 *)data;
		usize amount = sizes[r];

		crustMemcpy(dst, (const u8 *)bip->memory + bip->begin[r], amount);

		dst += amount;
		amount = size - amount;
		bip->begin[r] = 0;
		bip->end[r] = 0;

		r = (r + 1) % 2;
		bip->reader = r;

		crustMemcpy(dst, (const u8 *)bip->memory + bip->begin[r], amount);
		bip->begin[r] += amount;
	}

	if (bip->begin[r] == bip->end[r])
	{
		bip->begin[r] = 0;
		bip->end[r] = 0;

		if (r != w)
		{
			r = (r + 1) % 2;
			bip->reader = r;
		}
	}
}

CRUST_INLINE void *crustBipBufferStage(Crust_BipBuffer *bip, usize size)
{
	CRUST_ASSERT(bip != CRUST_NULL);
	CRUST_ASSERT(bip->memory != CRUST_NULL);
	CRUST_ASSERT(bip->staged == 0);
	CRUST_ASSERT(size > 0);

	u16 r = bip->reader;
	u16 w = bip->writer;

	if (r == w)
	{
		usize leftover = bip->capacity - bip->end[w];
		if (leftover >= size)
		{
			bip->staged = size;
			return (u8 *)bip->memory + bip->end[w];
		}

		leftover = bip->begin[w];
		if (leftover < size)
			return CRUST_NULL;

		w = (w + 1) % 2;

		bip->begin[w] = 0;
		bip->end[w] = 0;
		bip->writer = w;

		bip->staged = size;
		return bip->memory;
	}

	if (bip->begin[r] - bip->end[w] < size)
		return CRUST_NULL;

	bip->staged = size;
	return (u8 *)bip->memory + bip->end[w];
}

CRUST_INLINE void crustBipBufferCommit(Crust_BipBuffer *bip, usize size)
{
	CRUST_ASSERT(bip != CRUST_NULL);
	CRUST_ASSERT(bip->memory != CRUST_NULL);
	CRUST_ASSERT(bip->staged != 0);
	CRUST_ASSERT(bip->staged >= size);
	CRUST_ASSERT(size > 0);

	u16 w = bip->writer;

	bip->end[w] += size;
	bip->staged = 0;
}

//
typedef struct Crust_Stack_t
{
	void *memory;
	usize capacity;
	usize head;
} Crust_Stack;

CRUST_INLINE Crust_Stack crustStackInit(void *memory, usize capacity)
{
	CRUST_ASSERT(memory != CRUST_NULL);
	CRUST_ASSERT(capacity > 0);

	Crust_Stack result;
	result.memory = memory;
	result.capacity = capacity;
	result.head = 0;

	return result;
}

CRUST_INLINE void crustStackPush(Crust_Stack *stack, const void *data, usize size)
{
	CRUST_ASSERT(stack != CRUST_NULL);
	CRUST_ASSERT(stack->memory != CRUST_NULL);
	CRUST_ASSERT(stack->head + size <= stack->capacity);
	CRUST_ASSERT(data != CRUST_NULL);

	if (size == 0)
		return;

	u8 *ptr = (u8 *)stack->memory + stack->head;
	memcpy(ptr, data, size);

	stack->head += size;
}

CRUST_INLINE void crustStackPeek(const Crust_Stack *stack, void *data, usize size)
{
	CRUST_ASSERT(stack != CRUST_NULL);
	CRUST_ASSERT(stack->memory != CRUST_NULL);
	CRUST_ASSERT(stack->head >= size);
	CRUST_ASSERT(data != CRUST_NULL);

	usize offset = stack->head - size;

	const u8 *ptr = (const u8 *)stack->memory + offset;
	memcpy(data, ptr, size);
}

CRUST_INLINE void crustStackPop(Crust_Stack *stack, void *data, usize size)
{
	CRUST_ASSERT(stack != CRUST_NULL);
	CRUST_ASSERT(stack->memory != CRUST_NULL);
	CRUST_ASSERT(stack->head >= size);
	CRUST_ASSERT(data != CRUST_NULL);

	stack->head -= size;

	u8 *ptr = (u8 *)stack->memory + stack->head;
	memcpy(data, ptr, size);
}

//
typedef struct Crust_PoolHandle_t
{
	usize generation;
	usize index;
} Crust_PoolHandle;

typedef struct Crust_Pool_t
{
	usize *generations;
	usize *prevs;
	usize *nexts;
	usize capacity;
	usize free_head;
	usize free_tail;
	usize used_head;
	usize used_tail;
} Crust_Pool;

CRUST_INLINE void crustPoolAlloc(Crust_Allocator *allocator, Crust_Pool *pool, usize capacity)
{
	CRUST_ASSERT(allocator != CRUST_NULL);
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(capacity > 0);

	pool->generations = (usize *)crustAllocatorAlloc(allocator, sizeof(usize) * capacity);
	pool->prevs = (usize *)crustAllocatorAlloc(allocator, sizeof(usize) * capacity);
	pool->nexts = (usize *)crustAllocatorAlloc(allocator, sizeof(usize) * capacity);
	pool->capacity = capacity;

	for (usize i = 0; i < capacity; ++i)
	{
		usize next = (i + 1 < capacity) ? i + 1 : USIZE_MAX;
		usize prev = (i > 0) ? i - 1 : USIZE_MAX;

		pool->nexts[i] = next;
		pool->prevs[i] = prev;
		pool->generations[i] = 0;
	}

	pool->free_head = 0;
	pool->free_tail = capacity - 1;

	pool->used_head = USIZE_MAX;
	pool->used_tail = USIZE_MAX;
}

CRUST_INLINE void crustPoolFree(Crust_Allocator *allocator, Crust_Pool *pool)
{
	CRUST_ASSERT(allocator != CRUST_NULL);
	CRUST_ASSERT(pool != CRUST_NULL);

	crustAllocatorFree(allocator, pool->generations);
	crustAllocatorFree(allocator, pool->prevs);
	crustAllocatorFree(allocator, pool->nexts);

	pool->generations = CRUST_NULL;
	pool->prevs = CRUST_NULL;
	pool->nexts = CRUST_NULL;

	pool->capacity = 0;

	pool->free_head = USIZE_MAX;
	pool->free_tail = USIZE_MAX;

	pool->used_head = USIZE_MAX;
	pool->used_tail = USIZE_MAX;
}

CRUST_INLINE void crustPoolGrow(Crust_Allocator *allocator, Crust_Pool *pool, usize new_capacity)
{
	CRUST_ASSERT(allocator != CRUST_NULL);
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(pool->capacity < new_capacity);

	pool->generations = (usize *)crustAllocatorRealloc(allocator, pool->generations, sizeof(usize) * new_capacity);
	pool->prevs = (usize *)crustAllocatorRealloc(allocator, pool->prevs, sizeof(usize) * new_capacity);
	pool->nexts = (usize *)crustAllocatorRealloc(allocator, pool->nexts, sizeof(usize) * new_capacity);

	for (usize i = pool->capacity; i < new_capacity; ++i)
	{
		usize next = (i + 1 < new_capacity) ? i + 1 : USIZE_MAX;
		usize prev = (i > pool->capacity) ? i - 1 : pool->free_tail;

		pool->nexts[i] = next;
		pool->prevs[i] = prev;
		pool->generations[i] = 0;
	}

	if (pool->free_tail != USIZE_MAX)
		pool->nexts[pool->free_tail] = pool->capacity;

	if (pool->free_head == USIZE_MAX)
		pool->free_head = new_capacity - 1;

	pool->free_tail = new_capacity - 1;
	pool->capacity = new_capacity;
}

CRUST_INLINE u32 crustPoolCheck(const Crust_Pool *pool, Crust_PoolHandle handle)
{
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(handle.index < pool->capacity);

	usize generation = pool->generations[handle.index];

	if (generation == 0 || generation != handle.generation)
		return 0;

	return 1;
}

CRUST_INLINE Crust_PoolHandle crustPoolAdd(Crust_Pool *pool, usize max_generation)
{
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(pool->free_head != USIZE_MAX);

	usize index = pool->free_head;
	usize next = pool->nexts[index];
	usize prev = pool->prevs[index];

	pool->prevs[index] = USIZE_MAX;
	pool->nexts[index] = USIZE_MAX;

	if (prev != USIZE_MAX)
		pool->nexts[prev] = next;

	if (next != USIZE_MAX)
		pool->prevs[next] = prev;

	if (pool->free_head == index)
		pool->free_head = next;

	if (pool->free_tail == index)
		pool->free_tail = prev;

	usize generation = pool->generations[index];
	generation = crustMaxus(1, (generation + 1) % max_generation);

	pool->generations[index] = generation;

	if (pool->used_tail == USIZE_MAX)
	{
		CRUST_ASSERT(pool->used_head == USIZE_MAX);

		pool->used_head = index;
		pool->used_tail = index;
	}
	else
	{
		usize used_tail = pool->used_tail;

		pool->nexts[used_tail] = index;
		pool->prevs[index] = used_tail;

		pool->used_tail = index;
	}

	Crust_PoolHandle result;
	result.index = index;
	result.generation = generation;

	return result;
}

CRUST_INLINE void crustPoolRemove(Crust_Pool *pool, Crust_PoolHandle handle)
{
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(pool->used_tail != USIZE_MAX);
	CRUST_ASSERT(crustPoolCheck(pool, handle) != 0);

	usize index = handle.index;
	usize next = pool->nexts[index];
	usize prev = pool->prevs[index];

	pool->nexts[index] = USIZE_MAX;
	pool->prevs[index] = USIZE_MAX;

	if (prev != USIZE_MAX)
		pool->nexts[prev] = next;

	if (next != USIZE_MAX)
		pool->prevs[next] = prev;

	if (pool->used_head == index)
		pool->used_head = next;

	if (pool->used_tail == index)
		pool->used_tail = prev;

	if (pool->free_head == USIZE_MAX)
	{
		CRUST_ASSERT(pool->free_tail == USIZE_MAX);

		pool->free_head = index;
		pool->free_tail = index;
	}
	else
	{
		usize free_head = pool->free_head;

		pool->prevs[free_head] = index;
		pool->nexts[index] = free_head;

		pool->free_head = index;
	}
}

CRUST_INLINE Crust_PoolHandle crustPoolHead(const Crust_Pool *pool)
{
	CRUST_ASSERT(pool != CRUST_NULL);

	Crust_PoolHandle result;
	result.index = pool->used_head;
	result.generation = 0;

	if (result.index != USIZE_MAX)
		result.generation = pool->generations[result.index];

	return result;
}

CRUST_INLINE Crust_PoolHandle crustPoolTail(const Crust_Pool *pool)
{
	CRUST_ASSERT(pool != CRUST_NULL);

	Crust_PoolHandle result;
	result.index = pool->used_tail;
	result.generation = 0;

	if (result.index != USIZE_MAX)
		result.generation = pool->generations[result.index];

	return result;
}

CRUST_INLINE Crust_PoolHandle crustPoolNext(const Crust_Pool *pool, Crust_PoolHandle handle)
{
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(crustPoolCheck(pool, handle) != 0);

	Crust_PoolHandle result;
	result.index = pool->nexts[handle.index];
	result.generation = 0;

	if (result.index != USIZE_MAX)
		result.generation = pool->generations[result.index];

	return result;
}

CRUST_INLINE Crust_PoolHandle crustPoolPrev(const Crust_Pool *pool, Crust_PoolHandle handle)
{
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(crustPoolCheck(pool, handle) != 0);

	Crust_PoolHandle result;
	result.index = pool->prevs[handle.index];
	result.generation = 0;

	if (result.index != USIZE_MAX)
		result.generation = pool->generations[result.index];

	return result;
}

//
typedef struct Crust_HeapNode_t
{
	u32 offset;
	u32 size;
	u32 prev_bin;
	u32 next_bin;
	u32 prev_neighbour;
	u32 next_neighbour;
} Crust_HeapNode;

typedef struct Crust_HeapNodePool_t
{
	Crust_HeapNode *nodes;
	u32 *node_masks;
	u32 *free_indices;
	u32 num_free_indices;
	u32 capacity;
} Crust_HeapNodePool;

CRUST_INLINE void crustHeapNodePoolAlloc(Crust_Allocator *allocator, Crust_HeapNodePool *pool, u32 capacity)
{
	CRUST_ASSERT(allocator != CRUST_NULL);
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(capacity > 0);

	usize masks_capacity = crustAlignUpus(capacity, 32) / 32;

	pool->nodes = (Crust_HeapNode *)crustAllocatorAlloc(allocator, sizeof(Crust_HeapNode) * capacity);
	pool->node_masks = (u32 *)crustAllocatorAlloc(allocator, sizeof(u32) * masks_capacity);
	pool->free_indices = (u32 *)crustAllocatorAlloc(allocator, sizeof(u32) * capacity);
	pool->num_free_indices = capacity;
	pool->capacity = capacity;

	for (u32 i = 0; i < capacity; ++i)
		pool->free_indices[i] = capacity - i - 1;

	for (u32 i = 0; i < masks_capacity; ++i)
		pool->node_masks[i] = 0;
}

CRUST_INLINE void crustHeapNodePoolFree(Crust_Allocator *allocator, Crust_HeapNodePool *pool)
{
	CRUST_ASSERT(allocator != CRUST_NULL);
	CRUST_ASSERT(pool != CRUST_NULL);

	crustAllocatorFree(allocator, pool->nodes);
	crustAllocatorFree(allocator, pool->node_masks);
	crustAllocatorFree(allocator, pool->free_indices);

	pool->nodes = CRUST_NULL;
	pool->node_masks = CRUST_NULL;
	pool->free_indices = CRUST_NULL;

	pool->num_free_indices = 0;
	pool->capacity = 0;
}

CRUST_INLINE u32 crustHeapNodePoolGrab(Crust_HeapNodePool *pool)
{
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(pool->free_indices != CRUST_NULL);
	CRUST_ASSERT(pool->num_free_indices > 0);

	return pool->free_indices[--pool->num_free_indices];
}

CRUST_INLINE void crustHeapNodePoolRelease(Crust_HeapNodePool *pool, u32 index)
{
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(pool->free_indices != CRUST_NULL);
	CRUST_ASSERT(pool->num_free_indices < pool->capacity);
	CRUST_ASSERT(pool->capacity > index);

	pool->free_indices[pool->num_free_indices++] = index;
}

CRUST_INLINE void crustHeapNodePoolSetMask(Crust_HeapNodePool *pool, u32 index, u8 value)
{
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(pool->node_masks != CRUST_NULL);
	CRUST_ASSERT(pool->capacity > index);

	u32 mask_index = index / 32;
	u32 bit_index = index % 32;

	if (value > 0)
		pool->node_masks[mask_index] |= (1u << bit_index);
	else
		pool->node_masks[mask_index] &= ~(1u << bit_index);
}

CRUST_INLINE u8 crustHeapNodePoolGetMask(const Crust_HeapNodePool *pool, u32 index)
{
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(pool->node_masks != CRUST_NULL);
	CRUST_ASSERT(pool->capacity > index);

	u32 mask_index = index / 32;
	u32 bit_index = index % 32;

	return (pool->node_masks[mask_index] & (1u << bit_index)) != 0;
}

//
typedef struct Crust_HeapAllocation_t
{
	u32 offset;
	u32 index;
} Crust_HeapAllocation;

typedef enum Crust_HeapParameters_t
{
	CRUST_HEAP_PARAMETERS_MANTISSA_BITS = 3,
	CRUST_HEAP_PARAMETERS_MANTISSA_MAX = 0x00000008,
	CRUST_HEAP_PARAMETERS_MANTISSA_MASK = 0x00000007,

	CRUST_HEAP_PARAMETERS_EXPONENT_BITS = 5,
	CRUST_HEAP_PARAMETERS_EXPONENT_MAX = 0x00000020,

	CRUST_HEAP_PARAMETERS_NUM_SPARSE_BINS = CRUST_HEAP_PARAMETERS_EXPONENT_MAX,
	CRUST_HEAP_PARAMETERS_NUM_LINEAR_BINS = CRUST_HEAP_PARAMETERS_MANTISSA_MAX,
	CRUST_HEAP_PARAMETERS_NUM_BINS = CRUST_HEAP_PARAMETERS_NUM_SPARSE_BINS * CRUST_HEAP_PARAMETERS_NUM_LINEAR_BINS,
} Crust_HeapParameters;

typedef struct Crust_Heap_t
{
	Crust_HeapNodePool *pool;

	u32 size;
	u32 heads[CRUST_HEAP_PARAMETERS_NUM_BINS];
	u32 used_sparse_bins;
	u8 used_linear_bins[CRUST_HEAP_PARAMETERS_NUM_SPARSE_BINS];
} Crust_Heap;

CRUST_INLINE u32 crustHeapToBinSize(u8 bin)
{
	u32 mantissa = bin & CRUST_HEAP_PARAMETERS_MANTISSA_MASK;
	u32 exponent = bin >> CRUST_HEAP_PARAMETERS_MANTISSA_BITS;

	if (exponent == 0)
		return mantissa;

	return (mantissa | CRUST_HEAP_PARAMETERS_MANTISSA_MAX) << (exponent - 1);
}

CRUST_INLINE u8 crustHeapToBinIndex(u32 size)
{
	if (size < CRUST_HEAP_PARAMETERS_MANTISSA_MAX)
		return (u8)size;

	u32 leading_zeroes = crustLzcntu32(size);
	u32 highest_bit = 31 - leading_zeroes;

	u32 mantissa_bit = highest_bit - CRUST_HEAP_PARAMETERS_MANTISSA_BITS;

	u32 exponent = mantissa_bit + 1;
	u32 mantissa = (size >> mantissa_bit) & CRUST_HEAP_PARAMETERS_MANTISSA_MASK;

	return (u8)((exponent << CRUST_HEAP_PARAMETERS_MANTISSA_BITS) + mantissa);
}

CRUST_INLINE u8 crustHeapToBinIndexRoundUp(u32 size)
{
	if (size < CRUST_HEAP_PARAMETERS_MANTISSA_MAX)
		return (u8)size;

	u32 leading_zeroes = crustLzcntu32(size);
	u32 highest_bit = 31 - leading_zeroes;

	u32 mantissa_bit = highest_bit - CRUST_HEAP_PARAMETERS_MANTISSA_BITS;

	u32 exponent = mantissa_bit + 1;
	u32 mantissa = (size >> mantissa_bit) & CRUST_HEAP_PARAMETERS_MANTISSA_MASK;

	u32 lower_bits_mask = (1u << mantissa_bit) - 1;

	if ((size & lower_bits_mask) != 0)
		mantissa++;

	return (u8)((exponent << CRUST_HEAP_PARAMETERS_MANTISSA_BITS) + mantissa);
}

CRUST_INLINE u8 crustHeapFindBin(u32 bin_mask, u8 index)
{
	u32 mask = (1u << index) - 1;

	u32 masked_bins = bin_mask & ~mask;
	if (masked_bins == 0)
		return U8_MAX;

	return (u8)crustTzcntu32(masked_bins);
}

CRUST_INLINE u8 crustHeapFindBinForSize(const Crust_Heap *heap, u32 size)
{
	CRUST_ASSERT(heap != CRUST_NULL);
	CRUST_ASSERT(heap->size >= size);

	Crust_HeapNodePool *pool = heap->pool;
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(pool->nodes != CRUST_NULL);

	u8 bin_index = crustHeapToBinIndexRoundUp(size);
	CRUST_ASSERT(crustHeapToBinSize(bin_index) >= size);

	u8 sparse_bin_index = bin_index >> CRUST_HEAP_PARAMETERS_MANTISSA_BITS;
	u8 linear_bin_index = U8_MAX;

	u8 used_linear_bins = heap->used_linear_bins[sparse_bin_index];

	if (used_linear_bins != 0)
	{
		u8 min_linear_bin = bin_index & CRUST_HEAP_PARAMETERS_MANTISSA_MASK;
		linear_bin_index = crustHeapFindBin(used_linear_bins, min_linear_bin);
	}

	if (linear_bin_index == U8_MAX)
	{
		sparse_bin_index = crustHeapFindBin(heap->used_sparse_bins, sparse_bin_index + 1);
		if (sparse_bin_index == U8_MAX)
			return U8_MAX;

		CRUST_ASSERT(sparse_bin_index != 0);

		used_linear_bins = heap->used_linear_bins[sparse_bin_index];
		CRUST_ASSERT(used_linear_bins != 0);

		linear_bin_index = (u8)crustTzcntu32(used_linear_bins);
	}

	CRUST_ASSERT(linear_bin_index != U8_MAX);
	return (sparse_bin_index << CRUST_HEAP_PARAMETERS_MANTISSA_BITS) | (linear_bin_index & CRUST_HEAP_PARAMETERS_MANTISSA_MASK);
}

CRUST_INLINE void crustHeapAddNodeToBin(Crust_Heap *heap, u32 index, u32 size, u32 offset)
{
	CRUST_ASSERT(heap != CRUST_NULL);
	CRUST_ASSERT(heap->size >= size + offset);
	CRUST_ASSERT(index != U32_MAX);

	Crust_HeapNodePool *pool = heap->pool;
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(pool->nodes != CRUST_NULL);
	CRUST_ASSERT(index < pool->capacity);

	u8 bin_index = crustHeapToBinIndex(size);
	CRUST_ASSERT(crustHeapToBinSize(bin_index) <= size);

	u8 sparse_bin_index = bin_index >> CRUST_HEAP_PARAMETERS_MANTISSA_BITS;
	u8 linear_bin_index = bin_index & CRUST_HEAP_PARAMETERS_MANTISSA_MASK;

	heap->used_sparse_bins |= 1u << sparse_bin_index;
	heap->used_linear_bins[sparse_bin_index] |= 1u << linear_bin_index;

	Crust_HeapNode *node = &pool->nodes[index];

	node->offset = offset;
	node->size = size;
	node->prev_bin = U32_MAX;
	node->next_bin = U32_MAX;
	node->prev_neighbour = U32_MAX;
	node->next_neighbour = U32_MAX;
	crustHeapNodePoolSetMask(pool, index, 0);

	u32 bin_head_index = heap->heads[bin_index];

	if (bin_head_index != U32_MAX)
	{
		Crust_HeapNode *bin_head_node = &pool->nodes[bin_head_index];
		CRUST_ASSERT(crustHeapNodePoolGetMask(pool, bin_head_index) == 0);

		bin_head_node->prev_bin = index;
		node->next_bin = bin_head_index;
	}

	heap->heads[bin_index] = index;
}

CRUST_INLINE void crustHeapRemoveNodeFromBin(Crust_Heap *heap, u32 index)
{
	CRUST_ASSERT(heap != CRUST_NULL);
	CRUST_ASSERT(index != U32_MAX);

	Crust_HeapNodePool *pool = heap->pool;
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(pool->nodes != CRUST_NULL);
	CRUST_ASSERT(index < pool->capacity);

	Crust_HeapNode *node = &pool->nodes[index];
	CRUST_ASSERT(node != CRUST_NULL);
	CRUST_ASSERT(crustHeapNodePoolGetMask(pool, index) == 0);

	u8 bin_index = crustHeapToBinIndex(node->size);
	CRUST_ASSERT(crustHeapToBinSize(bin_index) <= node->size);

	if (node->prev_bin != U32_MAX)
	{
		Crust_HeapNode *prev_node = &pool->nodes[node->prev_bin];
		CRUST_ASSERT(prev_node != CRUST_NULL);

		prev_node->next_bin = node->next_bin;
	}

	if (node->next_bin != U32_MAX)
	{
		Crust_HeapNode *next_node = &pool->nodes[node->next_bin];
		CRUST_ASSERT(next_node != CRUST_NULL);

		next_node->prev_bin = node->prev_bin;
	}

	u8 sparse_bin_index = bin_index >> CRUST_HEAP_PARAMETERS_MANTISSA_BITS;
	u8 linear_bin_index = bin_index & CRUST_HEAP_PARAMETERS_MANTISSA_MASK;

	u32 bin_head_index = heap->heads[bin_index];
	CRUST_ASSERT(bin_head_index != U32_MAX);

	if (bin_head_index == index)
		bin_head_index = node->next_bin;

	if (bin_head_index == U32_MAX)
	{
		u8 linear_bin_mask = heap->used_linear_bins[sparse_bin_index];
		u32 sparse_bin_mask = heap->used_sparse_bins;

		linear_bin_mask &= ~(1u << linear_bin_index);
		if (linear_bin_mask == 0)
			sparse_bin_mask &= ~(1u << sparse_bin_index);

		heap->used_linear_bins[sparse_bin_index] = linear_bin_mask;
		heap->used_sparse_bins = sparse_bin_mask;
	}

	heap->heads[bin_index] = bin_head_index;
}

CRUST_INLINE Crust_Heap crustHeapInit(Crust_HeapNodePool *pool, u32 size)
{
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(size > 0);

	Crust_Heap result;
	result.pool = pool;
	result.size = size;

	for (u32 i = 0; i < CRUST_HEAP_PARAMETERS_NUM_BINS; ++i)
		result.heads[i] = U32_MAX;

	result.used_sparse_bins = 0;
	for (u32 i = 0; i < CRUST_HEAP_PARAMETERS_NUM_SPARSE_BINS; ++i)
		result.used_linear_bins[i] = 0;

	u32 index = crustHeapNodePoolGrab(pool);
	crustHeapAddNodeToBin(&result, index, size, 0);

	return result;
}

CRUST_INLINE Crust_HeapAllocation crustHeapStageAlloc(const Crust_Heap *heap, u32 size)
{
	CRUST_ASSERT(heap != CRUST_NULL);
	CRUST_ASSERT(heap->size >= size);
	CRUST_ASSERT(size > 0);
	
	const Crust_HeapNodePool *pool = heap->pool;
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(pool->nodes != CRUST_NULL);

	Crust_HeapAllocation result;
	result.offset = U32_MAX;
	result.index = U32_MAX;

	u8 bin_index = crustHeapFindBinForSize(heap, size);
	if (bin_index == U8_MAX)
		return result;

	CRUST_ASSERT(bin_index != 0);
	CRUST_ASSERT(crustHeapToBinSize(bin_index) >= size);

	result.index = heap->heads[bin_index];
	CRUST_ASSERT(result.index != U32_MAX);

	const Crust_HeapNode *node = &pool->nodes[result.index];
	CRUST_ASSERT(node != CRUST_NULL);
	CRUST_ASSERT(node->size >= size);

	result.offset = node->offset;

	return result;
}

CRUST_INLINE Crust_HeapAllocation crustHeapStageAllocAligned(const Crust_Heap *heap, u32 size, u32 alignment)
{
	CRUST_ASSERT(heap != CRUST_NULL);
	CRUST_ASSERT(heap->size >= size);
	CRUST_ASSERT(size > 0);
	CRUST_ASSERT(crustIsPow2u32(alignment) != 0);

	const Crust_HeapNodePool *pool = heap->pool;
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(pool->nodes != CRUST_NULL);

	Crust_HeapAllocation result;
	result.offset = U32_MAX;
	result.index = U32_MAX;

	u8 bin_index = crustHeapFindBinForSize(heap, size);
	if (bin_index == U8_MAX)
		return result;

	CRUST_ASSERT(bin_index != 0);
	CRUST_ASSERT(crustHeapToBinSize(bin_index) >= size);

	result.index = heap->heads[bin_index];
	CRUST_ASSERT(result.index != U32_MAX);

	const Crust_HeapNode *node = &pool->nodes[result.index];
	CRUST_ASSERT(node != CRUST_NULL);
	CRUST_ASSERT(node->size >= size);

	result.offset = crustAlignUpu32(node->offset, alignment);

	u32 remainder = result.offset - node->offset;
	if (remainder + size > node->size)
	{
		u32 max_size = size + alignment - 1;
		bin_index = crustHeapFindBinForSize(heap, max_size);
		if (bin_index == U8_MAX)
		{
			result.offset = U32_MAX;
			result.index = U32_MAX;

			return result;
		}

		CRUST_ASSERT(bin_index != 0);
		result.index = heap->heads[bin_index];

		CRUST_ASSERT(result.index != U32_MAX);

		node = &pool->nodes[result.index];
		CRUST_ASSERT(node != CRUST_NULL);

		result.offset = crustAlignUpu32(node->offset, alignment);
	}

	return result;
}

CRUST_INLINE void crustHeapCommitAlloc(Crust_Heap *heap, Crust_HeapAllocation allocation, u32 size)
{
	CRUST_ASSERT(heap != CRUST_NULL);
	CRUST_ASSERT(heap->size >= size);
	CRUST_ASSERT(size > 0);

	CRUST_ASSERT(allocation.index != U32_MAX);
	CRUST_ASSERT(allocation.offset != U32_MAX);

	Crust_HeapNodePool *pool = heap->pool;
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(pool->nodes != CRUST_NULL);
	CRUST_ASSERT(pool->capacity > allocation.index);

	Crust_HeapNode *node = &pool->nodes[allocation.index];
	CRUST_ASSERT(node != CRUST_NULL);
	CRUST_ASSERT(crustHeapNodePoolGetMask(pool, allocation.index) == 0);
	CRUST_ASSERT(allocation.offset >= node->offset);

	u32 remainder_begin_size = allocation.offset - node->offset;
	u32 remainder_begin_offset = node->offset;
	u32 remainder_end_size = node->size - remainder_begin_size - size;
	u32 remainder_end_offset = allocation.offset + size;

	CRUST_ASSERT(remainder_begin_size + size <= node->size);

	u32 prev_index = node->prev_neighbour;
	u32 next_index = node->next_neighbour;

	crustHeapRemoveNodeFromBin(heap, allocation.index);

	node->offset = allocation.offset;
	node->size = size;

	crustHeapNodePoolSetMask(pool, allocation.index, 1);

	if (remainder_begin_size > 0)
	{
		Crust_HeapNode *prev_node = (prev_index != U32_MAX) ? &pool->nodes[prev_index] : CRUST_NULL;

		// try merge with previous free node
		if (prev_node != CRUST_NULL && crustHeapNodePoolGetMask(pool, prev_index) == 0)
		{
			CRUST_ASSERT(prev_node->next_neighbour == allocation.index);

			remainder_begin_offset = prev_node->offset;
			remainder_begin_size += prev_node->size;

			u32 prev_prev_index = prev_node->prev_neighbour;

			crustHeapRemoveNodeFromBin(heap, prev_index);
			crustHeapNodePoolRelease(pool, prev_index);

			prev_index = prev_prev_index;
			prev_node = (prev_index != U32_MAX) ? &pool->nodes[prev_index] : CRUST_NULL;
		}

		u32 new_index = crustHeapNodePoolGrab(pool);
		crustHeapAddNodeToBin(heap, new_index, remainder_begin_size, remainder_begin_offset);

		Crust_HeapNode *new_node = &pool->nodes[new_index];
		CRUST_ASSERT(new_node != CRUST_NULL);

		node->prev_neighbour = new_index;
		new_node->next_neighbour = allocation.index;

		new_node->prev_neighbour = prev_index;
		if (prev_node)
			prev_node->next_neighbour = new_index;
	}

	if (remainder_end_size > 0)
	{
		Crust_HeapNode *next_node = (next_index != U32_MAX) ? &pool->nodes[next_index] : CRUST_NULL;

		// try merge with next free node
		if (next_node != CRUST_NULL && crustHeapNodePoolGetMask(pool, next_index) == 0)
		{
			CRUST_ASSERT(next_node->prev_neighbour == allocation.index);

			remainder_end_size += next_node->size;

			u32 next_next_index = next_node->next_neighbour;

			crustHeapRemoveNodeFromBin(heap, next_index);
			crustHeapNodePoolRelease(pool, next_index);

			next_index = next_next_index;
			next_node = (next_index != U32_MAX) ? &pool->nodes[next_index] : CRUST_NULL;
		}

		u32 new_index = crustHeapNodePoolGrab(pool);
		crustHeapAddNodeToBin(heap, new_index, remainder_end_size, remainder_end_offset);

		Crust_HeapNode *new_node = &pool->nodes[new_index];
		CRUST_ASSERT(new_node != CRUST_NULL);

		node->next_neighbour = new_index;
		new_node->prev_neighbour = allocation.index;

		new_node->next_neighbour = next_index;
		if (next_node)
			next_node->prev_neighbour = new_index;
	}
}

CRUST_INLINE Crust_HeapAllocation crustHeapAlloc(Crust_Heap *heap, u32 size)
{
	CRUST_ASSERT(heap != CRUST_NULL);
	CRUST_ASSERT(heap->size >= size);
	CRUST_ASSERT(size > 0);

	Crust_HeapAllocation result = crustHeapStageAlloc(heap, size);
	if (result.index == U32_MAX && result.offset == U32_MAX)
		return result;

	crustHeapCommitAlloc(heap, result, size);
	return result;
}

CRUST_INLINE Crust_HeapAllocation crustHeapAllocAligned(Crust_Heap *heap, u32 size, u32 alignment)
{
	CRUST_ASSERT(heap != CRUST_NULL);
	CRUST_ASSERT(heap->size >= size);
	CRUST_ASSERT(size > 0);
	CRUST_ASSERT(crustIsPow2u32(alignment) != 0);

	Crust_HeapAllocation result = crustHeapStageAllocAligned(heap, size, alignment);
	if (result.index == U32_MAX && result.offset == U32_MAX)
		return result;

	crustHeapCommitAlloc(heap, result, size);
	return result;
}

CRUST_INLINE void crustHeapFree(Crust_Heap *heap, Crust_HeapAllocation allocation)
{
	CRUST_ASSERT(heap != CRUST_NULL);

	CRUST_ASSERT(allocation.index != U32_MAX);
	CRUST_ASSERT(allocation.offset != U32_MAX);

	Crust_HeapNodePool *pool = heap->pool;
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(pool->nodes != CRUST_NULL);
	CRUST_ASSERT(pool->capacity > allocation.index);

	Crust_HeapNode *node = &pool->nodes[allocation.index];
	CRUST_ASSERT(node != CRUST_NULL);
	CRUST_ASSERT(crustHeapNodePoolGetMask(pool, allocation.index) != 0);
	CRUST_ASSERT(allocation.offset >= node->offset);

	u32 prev_index = node->prev_neighbour;
	u32 next_index = node->next_neighbour;

	u32 size = node->size;
	u32 offset = node->offset;

	Crust_HeapNode *prev_node = (prev_index != U32_MAX) ? &pool->nodes[prev_index] : CRUST_NULL;

	// try merge with previous free node
	if (prev_node != CRUST_NULL && crustHeapNodePoolGetMask(pool, prev_index) == 0)
	{
		offset = prev_node->offset;
		size += prev_node->size;

		crustHeapRemoveNodeFromBin(heap, prev_index);
		crustHeapNodePoolRelease(pool, prev_index);

		prev_index = prev_node->prev_neighbour;
	}

	Crust_HeapNode *next_node = (next_index != U32_MAX) ? &pool->nodes[next_index] : CRUST_NULL;

	// try merge with next free node
	if (next_node != CRUST_NULL && crustHeapNodePoolGetMask(pool, next_index) == 0)
	{
		size += next_node->size;

		crustHeapRemoveNodeFromBin(heap, next_index);
		crustHeapNodePoolRelease(pool, next_index);

		next_index = next_node->next_neighbour;
	}

	crustHeapAddNodeToBin(heap, allocation.index, size, offset);

	node->prev_neighbour = prev_index;
	node->next_neighbour = next_index;

	if (prev_index != U32_MAX)
	{
		Crust_HeapNode *prev_prev_node = &pool->nodes[prev_index];
		prev_prev_node->next_neighbour = allocation.index;
	}

	if (next_index != U32_MAX)
	{
		Crust_HeapNode *next_next_node = &pool->nodes[next_index];
		next_next_node->prev_neighbour = allocation.index;
	}
}

#ifdef __cplusplus
}
#endif
