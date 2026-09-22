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
	#include <malloc.h>
	#include <string.h>

	#define CRUST_ASSERT(x)		do { (!!(x)) || (__debugbreak(), 0); } while(0)
	#define CRUST_UNUSED(x)		do { (void)(x); } while(0)
	#define CRUST_ALIGNOF(x)	__alignof(x)

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

	CRUST_INLINE void crustMemset(void *dst, u8 value, usize size)
	{
		memset(dst, value, size);
	}

	CRUST_INLINE void *crustAlignedMalloc(usize size, usize alignment)
	{
		return _aligned_malloc(size, alignment);
	}

	CRUST_INLINE void *crustAlignedRealloc(void *ptr, usize size, usize alignment)
	{
		return _aligned_realloc(ptr, size, alignment);
	}

	CRUST_INLINE void crustAlignedFree(void *ptr)
	{
		_aligned_free(ptr);
	}

	CRUST_INLINE f32 crustRsqrtF32(f32 v)
	{
		return _mm_cvtss_f32(_mm_rsqrt_ss(_mm_set_ss(v)));
	}

	CRUST_INLINE f32 crustSqrtF32(f32 v)
	{
		return _mm_cvtss_f32(_mm_sqrt_ss(_mm_set_ss(v)));
	}

	CRUST_INLINE f32 crustCosF32(f32 v)
	{
		return _mm_cvtss_f32(_mm_cos_ps(_mm_set_ss(v)));
	}

	CRUST_INLINE f32 crustAcosF32(f32 v)
	{
		return _mm_cvtss_f32(_mm_acos_ps(_mm_set_ss(v)));
	}

	CRUST_INLINE f32 crustSinF32(f32 v)
	{
		return _mm_cvtss_f32(_mm_sin_ps(_mm_set_ss(v)));
	}

	CRUST_INLINE f32 crustAsinF32(f32 v)
	{
		return _mm_cvtss_f32(_mm_asin_ps(_mm_set_ss(v)));
	}

	CRUST_INLINE f32 crustTanF32(f32 v)
	{
		return _mm_cvtss_f32(_mm_tan_ps(_mm_set_ss(v)));
	}

	CRUST_INLINE f32 crustAtan2F32(f32 y, f32 x)
	{
		return _mm_cvtss_f32(_mm_atan2_ps(_mm_set_ss(y), _mm_set_ss(x)));
	}

	CRUST_INLINE u32 crustLzcntU32(u32 value)
	{
		CRUST_ASSERT(value != 0);

		unsigned long result = 0;
		_BitScanReverse(&result, value);
		return 31 - result;
	}

	CRUST_INLINE u32 crustTzcntU32(u32 value)
	{
		CRUST_ASSERT(value != 0);

		unsigned long result = 0;
		_BitScanForward(&result, value);
		return result;
	}

	CRUST_INLINE u32 crustPopcntU32(u32 value)
	{
		return __popcnt(value);
	}
#else
	#error "Unsupported platform"
#endif

//
CRUST_INLINE u8 crustIsPow2U8(u8 v)
{
	CRUST_ASSERT(v != 0);
	return (v & (v - 1)) == 0;
}

CRUST_INLINE u16 crustIsPow2U16(u16 v)
{
	CRUST_ASSERT(v != 0);
	return (v & (v - 1)) == 0;
}

CRUST_INLINE u32 crustIsPow2U32(u32 v)
{
	CRUST_ASSERT(v != 0);
	return (v & (v - 1)) == 0;
}

CRUST_INLINE u64 crustIsPow2U64(u64 v)
{
	CRUST_ASSERT(v != 0);
	return (v & (v - 1)) == 0;
}

CRUST_INLINE usize crustIsPow2USize(usize v)
{
	CRUST_ASSERT(v != 0);
	return (v & (v - 1)) == 0;
}

CRUST_INLINE u8 crustIsAlignedU8(u8 v, u8 alignment)
{
	CRUST_ASSERT(alignment != 0);
	CRUST_ASSERT(crustIsPow2U8(alignment) != 0);

	return (v & (alignment - 1)) == 0;
}

CRUST_INLINE u16 crustIsAlignedU16(u16 v, u16 alignment)
{
	CRUST_ASSERT(alignment != 0);
	CRUST_ASSERT(crustIsPow2U16(alignment) != 0);

	return (v & (alignment - 1)) == 0;
}

CRUST_INLINE u32 crustIsAlignedU32(u32 v, u32 alignment)
{
	CRUST_ASSERT(alignment != 0);
	CRUST_ASSERT(crustIsPow2U32(alignment) != 0);

	return (v & (alignment - 1)) == 0;
}

CRUST_INLINE u64 crustIsAlignedU64(u64 v, u64 alignment)
{
	CRUST_ASSERT(alignment != 0);
	CRUST_ASSERT(crustIsPow2U64(alignment) != 0);

	return (v & (alignment - 1)) == 0;
}

CRUST_INLINE usize crustIsAlignedUSize(usize v, usize alignment)
{
	CRUST_ASSERT(alignment != 0);
	CRUST_ASSERT(crustIsPow2USize(alignment) != 0);

	return (v & (alignment - 1)) == 0;
}

CRUST_INLINE u8 crustAlignDownU8(u8 value, u8 alignment)
{
	CRUST_ASSERT(alignment != 0);
	CRUST_ASSERT(crustIsPow2U8(alignment) != 0);

	return value & ~(alignment - 1);
}

CRUST_INLINE u16 crustAlignDownU16(u16 value, u16 alignment)
{
	CRUST_ASSERT(alignment != 0);
	CRUST_ASSERT(crustIsPow2U16(alignment) != 0);

	return value & ~(alignment - 1);
}

CRUST_INLINE u32 crustAlignDownU32(u32 value, u32 alignment)
{
	CRUST_ASSERT(alignment != 0);
	CRUST_ASSERT(crustIsPow2U32(alignment) != 0);

	return value & ~(alignment - 1);
}

CRUST_INLINE u64 crustAlignDownU64(u64 value, u64 alignment)
{
	CRUST_ASSERT(alignment != 0);
	CRUST_ASSERT(crustIsPow2U64(alignment) != 0);

	return value & ~(alignment - 1);
}

CRUST_INLINE usize crustAlignDownUSize(usize value, usize alignment)
{
	CRUST_ASSERT(alignment != 0);
	CRUST_ASSERT(crustIsPow2USize(alignment) != 0);

	return value & ~(alignment - 1);
}

CRUST_INLINE u8 crustAlignUpU8(u8 value, u8 alignment)
{
	CRUST_ASSERT(alignment != 0);
	CRUST_ASSERT(crustIsPow2U8(alignment) != 0);

	u8 mask = alignment - 1;
	return (value + mask) & ~mask;
}

CRUST_INLINE u16 crustAlignUpU16(u16 value, u16 alignment)
{
	CRUST_ASSERT(alignment != 0);
	CRUST_ASSERT(crustIsPow2U16(alignment) != 0);

	u16 mask = alignment - 1;
	return (value + mask) & ~mask;
}

CRUST_INLINE u32 crustAlignUpU32(u32 value, u32 alignment)
{
	CRUST_ASSERT(alignment != 0);
	CRUST_ASSERT(crustIsPow2U32(alignment) != 0);

	u32 mask = alignment - 1;
	return (value + mask) & ~mask;
}

CRUST_INLINE u64 crustAlignUpU64(u64 value, u64 alignment)
{
	CRUST_ASSERT(alignment != 0);
	CRUST_ASSERT(crustIsPow2U64(alignment) != 0);

	u64 mask = alignment - 1;
	return (value + mask) & ~mask;
}

CRUST_INLINE usize crustAlignUpUSize(usize value, usize alignment)
{
	CRUST_ASSERT(alignment != 0);
	CRUST_ASSERT(crustIsPow2USize(alignment) != 0);

	usize mask = alignment - 1;
	return (value + mask) & ~mask;
}

CRUST_INLINE u8 crustMinU8(u8 a, u8 b)
{
	return (a < b) ? a : b;
}

CRUST_INLINE u16 crustMinU16(u16 a, u16 b)
{
	return (a < b) ? a : b;
}

CRUST_INLINE u32 crustMinU32(u32 a, u32 b)
{
	return (a < b) ? a : b;
}

CRUST_INLINE u64 crustMinU64(u64 a, u64 b)
{
	return (a < b) ? a : b;
}

CRUST_INLINE usize crustMinUSize(usize a, usize b)
{
	return (a < b) ? a : b;
}

CRUST_INLINE i8 crustMinI8(i8 a, i8 b)
{
	return (a < b) ? a : b;
}

CRUST_INLINE i16 crustMinI16(i16 a, i16 b)
{
	return (a < b) ? a : b;
}

CRUST_INLINE i32 crustMinI32(i32 a, i32 b)
{
	return (a < b) ? a : b;
}

CRUST_INLINE i64 crustMinI64(i64 a, i64 b)
{
	return (a < b) ? a : b;
}

CRUST_INLINE isize crustMinISize(isize a, isize b)
{
	return (a < b) ? a : b;
}

CRUST_INLINE f32 crustMinF32(f32 a, f32 b)
{
	return (a < b) ? a : b;
}

CRUST_INLINE f64 crustMinF64(f64 a, f64 b)
{
	return (a < b) ? a : b;
}

CRUST_INLINE u8 crustMaxU8(u8 a, u8 b)
{
	return (a < b) ? b : a;
}

CRUST_INLINE u16 crustMaxU16(u16 a, u16 b)
{
	return (a < b) ? b : a;
}

CRUST_INLINE u32 crustMaxU32(u32 a, u32 b)
{
	return (a < b) ? b : a;
}

CRUST_INLINE u64 crustMaxU64(u64 a, u64 b)
{
	return (a < b) ? b : a;
}

CRUST_INLINE usize crustMaxUSize(usize a, usize b)
{
	return (a < b) ? b : a;
}

CRUST_INLINE i8 crustMaxI8(i8 a, i8 b)
{
	return (a < b) ? b : a;
}

CRUST_INLINE i16 crustMaxI16(i16 a, i16 b)
{
	return (a < b) ? b : a;
}

CRUST_INLINE i32 crustMaxI32(i32 a, i32 b)
{
	return (a < b) ? b : a;
}

CRUST_INLINE i64 crustMaxI64(i64 a, i64 b)
{
	return (a < b) ? b : a;
}

CRUST_INLINE isize crustMaxISize(isize a, isize b)
{
	return (a < b) ? b : a;
}

CRUST_INLINE f32 crustMaxF32(f32 a, f32 b)
{
	return (a < b) ? b : a;
}

CRUST_INLINE f64 crustMaxF64(f64 a, f64 b)
{
	return (a < b) ? b : a;
}

CRUST_INLINE u8 crustClampU8(u8 v, u8 v_min, u8 v_max)
{
	return crustMaxU8(v_min, crustMinU8(v, v_max));
}

CRUST_INLINE u16 crustClampU16(u16 v, u16 v_min, u16 v_max)
{
	return crustMaxU16(v_min, crustMinU16(v, v_max));
}

CRUST_INLINE u32 crustClampU32(u32 v, u32 v_min, u32 v_max)
{
	return crustMaxU32(v_min, crustMinU32(v, v_max));
}

CRUST_INLINE u64 crustClampU64(u64 v, u64 v_min, u64 v_max)
{
	return crustMaxU64(v_min, crustMinU64(v, v_max));
}

CRUST_INLINE usize crustClampUSize(usize v, usize v_min, usize v_max)
{
	return crustMaxUSize(v_min, crustMinUSize(v, v_max));
}

CRUST_INLINE i8 crustClampI8(i8 v, i8 v_min, i8 v_max)
{
	return crustMaxI8(v_min, crustMinI8(v, v_max));
}

CRUST_INLINE i16 crustClampI16(i16 v, i16 v_min, i16 v_max)
{
	return crustMaxI16(v_min, crustMinI16(v, v_max));
}

CRUST_INLINE i32 crustClampI32(i32 v, i32 v_min, i32 v_max)
{
	return crustMaxI32(v_min, crustMinI32(v, v_max));
}

CRUST_INLINE i64 crustClampI64(i64 v, i64 v_min, i64 v_max)
{
	return crustMaxI64(v_min, crustMinI64(v, v_max));
}

CRUST_INLINE isize crustClampISize(isize v, isize v_min, isize v_max)
{
	return crustMaxISize(v_min, crustMinISize(v, v_max));
}

CRUST_INLINE f32 crustClampF32(f32 v, f32 v_min, f32 v_max)
{
	return crustMaxF32(v_min, crustMinF32(v, v_max));
}

CRUST_INLINE f64 crustClampF64(f64 v, f64 v_min, f64 v_max)
{
	return crustMaxF64(v_min, crustMinF64(v, v_max));
}

CRUST_INLINE i8 crustAbsI8(i8 v)
{
	return (v < 0) ? -v : v;
}

CRUST_INLINE i16 crustAbsI16(i16 v)
{
	return (v < 0) ? -v : v;
}

CRUST_INLINE i32 crustAbsI32(i32 v)
{
	return (v < 0) ? -v : v;
}

CRUST_INLINE i64 crustAbsI64(i64 v)
{
	return (v < 0) ? -v : v;
}

CRUST_INLINE isize crustAbsISize(isize v)
{
	return (v < 0) ? -v : v;
}

CRUST_INLINE f32 crustAbsF32(f32 v)
{
	u32 bits;
	crustMemcpy(&bits, &v, sizeof(u32));
	bits &= 0x7FFFFFFF;

	f32 result;
	crustMemcpy(&result, &bits, sizeof(f32));

	return result;
}

CRUST_INLINE f64 crustAbsF64(f64 v)
{
	u64 bits;
	crustMemcpy(&bits, &v, sizeof(u64));
	bits &= 0x7FFFFFFFFFFFFFFF;

	f64 result;
	crustMemcpy(&result, &bits, sizeof(f64));

	return result;
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

CRUST_INLINE Crust_Vec2 crustVec2AddS(Crust_Vec2 a, f32 s)
{
	Crust_Vec2 result;
	result.x = a.x + s;
	result.y = a.y + s;

	return result;
}

CRUST_INLINE Crust_Vec2 crustVec2AddV(Crust_Vec2 a, Crust_Vec2 b)
{
	Crust_Vec2 result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;

	return result;
}

CRUST_INLINE Crust_Vec2 crustVec2SubS(Crust_Vec2 a, f32 s)
{
	Crust_Vec2 result;
	result.x = a.x - s;
	result.y = a.y - s;

	return result;
}

CRUST_INLINE Crust_Vec2 crustVec2SubV(Crust_Vec2 a, Crust_Vec2 b)
{
	Crust_Vec2 result;
	result.x = a.x - b.x;
	result.y = a.y - b.y;

	return result;
}

CRUST_INLINE Crust_Vec2 crustVec2MulS(Crust_Vec2 a, f32 s)
{
	Crust_Vec2 result;
	result.x = a.x * s;
	result.y = a.y * s;

	return result;
}

CRUST_INLINE Crust_Vec2 crustVec2MulV(Crust_Vec2 a, Crust_Vec2 b)
{
	Crust_Vec2 result;
	result.x = a.x * b.x;
	result.y = a.y * b.y;

	return result;
}

CRUST_INLINE Crust_Vec2 crustVec2DivS(Crust_Vec2 a, f32 s)
{
	float s_inv = 1.0f / s;

	Crust_Vec2 result;
	result.x = a.x * s_inv;
	result.y = a.y * s_inv;

	return result;
}

CRUST_INLINE Crust_Vec2 crustVec2DivV(Crust_Vec2 a, Crust_Vec2 b)
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
	f32 len_inv = crustRsqrtF32(v.x * v.x + v.y * v.y);

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
	return crustSqrtF32(v.x * v.x + v.y * v.y);
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

CRUST_INLINE Crust_Vec3 crustVec3AddS(Crust_Vec3 a, f32 s)
{
	Crust_Vec3 result;
	result.x = a.x + s;
	result.y = a.y + s;
	result.z = a.z + s;

	return result;
}

CRUST_INLINE Crust_Vec3 crustVec3AddV(Crust_Vec3 a, Crust_Vec3 b)
{
	Crust_Vec3 result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;

	return result;
}

CRUST_INLINE Crust_Vec3 crustVec3SubS(Crust_Vec3 a, f32 s)
{
	Crust_Vec3 result;
	result.x = a.x - s;
	result.y = a.y - s;
	result.z = a.z - s;

	return result;
}

CRUST_INLINE Crust_Vec3 crustVec3SubV(Crust_Vec3 a, Crust_Vec3 b)
{
	Crust_Vec3 result;
	result.x = a.x - b.x;
	result.y = a.y - b.y;
	result.z = a.z - b.z;

	return result;
}

CRUST_INLINE Crust_Vec3 crustVec3MulS(Crust_Vec3 a, f32 s)
{
	Crust_Vec3 result;
	result.x = a.x * s;
	result.y = a.y * s;
	result.z = a.z * s;

	return result;
}

CRUST_INLINE Crust_Vec3 crustVec3MulV(Crust_Vec3 a, Crust_Vec3 b)
{
	Crust_Vec3 result;
	result.x = a.x * b.x;
	result.y = a.y * b.y;
	result.z = a.z * b.z;

	return result;
}

CRUST_INLINE Crust_Vec3 crustVec3DivS(Crust_Vec3 a, f32 s)
{
	float s_inv = 1.0f / s;

	Crust_Vec3 result;
	result.x = a.x * s_inv;
	result.y = a.y * s_inv;
	result.z = a.z * s_inv;

	return result;
}

CRUST_INLINE Crust_Vec3 crustVec3DivV(Crust_Vec3 a, Crust_Vec3 b)
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
	f32 len_inv = crustRsqrtF32(v.x * v.x + v.y * v.y + v.z * v.z);

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
	return crustSqrtF32(v.x * v.x + v.y * v.y + v.z * v.z);
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

CRUST_INLINE Crust_Vec4 crustVec4AddS(Crust_Vec4 a, f32 s)
{
	Crust_Vec4 result;
	result.x = a.x + s;
	result.y = a.y + s;
	result.z = a.z + s;
	result.w = a.w + s;

	return result;
}

CRUST_INLINE Crust_Vec4 crustVec4AddV(Crust_Vec4 a, Crust_Vec4 b)
{
	Crust_Vec4 result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;
	result.w = a.w + b.w;

	return result;
}

CRUST_INLINE Crust_Vec4 crustVec4SubS(Crust_Vec4 a, f32 s)
{
	Crust_Vec4 result;
	result.x = a.x - s;
	result.y = a.y - s;
	result.z = a.z - s;
	result.w = a.w - s;

	return result;
}

CRUST_INLINE Crust_Vec4 crustVec4SubV(Crust_Vec4 a, Crust_Vec4 b)
{
	Crust_Vec4 result;
	result.x = a.x - b.x;
	result.y = a.y - b.y;
	result.z = a.z - b.z;
	result.w = a.w - b.w;

	return result;
}

CRUST_INLINE Crust_Vec4 crustVec4MulS(Crust_Vec4 a, f32 s)
{
	Crust_Vec4 result;
	result.x = a.x * s;
	result.y = a.y * s;
	result.z = a.z * s;
	result.w = a.w * s;

	return result;
}

CRUST_INLINE Crust_Vec4 crustVec4MulV(Crust_Vec4 a, Crust_Vec4 b)
{
	Crust_Vec4 result;
	result.x = a.x * b.x;
	result.y = a.y * b.y;
	result.z = a.z * b.z;
	result.w = a.w * b.w;

	return result;
}

CRUST_INLINE Crust_Vec4 crustVec4DivS(Crust_Vec4 a, f32 s)
{
	float s_inv = 1.0f / s;

	Crust_Vec4 result;
	result.x = a.x * s_inv;
	result.y = a.y * s_inv;
	result.z = a.z * s_inv;
	result.w = a.w * s_inv;

	return result;
}

CRUST_INLINE Crust_Vec4 crustVec4DivV(Crust_Vec4 a, Crust_Vec4 b)
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
	f32 len_inv = crustRsqrtF32(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);

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
	return crustSqrtF32(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
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
	f32 len_inv = crustRsqrtF32(crustQuatDot(q, q));

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

	f32 half_theta = crustAcosF32(cos_half_theta);
	f32 sin_half_theta = crustSqrtF32(1.0f - cos_half_theta * cos_half_theta);

	f32 ra = crustSinF32((1.0f - t) * half_theta) / sin_half_theta;
	f32 rb = crustSinF32(t * half_theta) / sin_half_theta;

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
CRUST_INLINE Crust_Transform crustTransformInvert(Crust_Transform t)
{
	Crust_Transform result;
	result.scale.x = 1.0f / t.scale.x;
	result.scale.y = 1.0f / t.scale.y;
	result.scale.z = 1.0f / t.scale.z;
	result.rotation = crustQuatConjugate(t.rotation);
	result.position = crustQuatRotate(result.rotation, crustVec3MulV(result.scale, crustVec3MulS(t.position, -1.0f)));

	return result;
}

CRUST_INLINE Crust_Transform crustTransformMul(Crust_Transform a, Crust_Transform b)
{
	Crust_Transform result;
	result.scale = crustVec3MulV(a.scale, b.scale);
	result.rotation = crustQuatMul(a.rotation, b.rotation);
	result.position = crustVec3AddV(a.position, crustQuatRotate(a.rotation, crustVec3MulV(a.scale, b.position)));

	return result;
}

//
typedef struct Crust_RingBuffer_t
{
	void *memory;
	usize capacity;
	usize size;
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
	result.size = 0;
	result.read = 0;
	result.write = 0;

	return result;
}

CRUST_INLINE void crustRingBufferRead(Crust_RingBuffer *ring, void *data, usize size)
{
	CRUST_ASSERT(ring != CRUST_NULL);
	CRUST_ASSERT(ring->memory != CRUST_NULL);
	CRUST_ASSERT(ring->size >= size);
	CRUST_ASSERT(data != CRUST_NULL);

	if (size == 0)
		return;

	usize begin = ring->read;
	usize end = (begin + size) % ring->capacity;

	ring->read = end;
	ring->size -= size;

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
	CRUST_ASSERT(ring->size + size <= ring->capacity);
	CRUST_ASSERT(data != CRUST_NULL);

	if (size == 0)
		return;

	usize begin = ring->write;
	usize end = (begin + size) % ring->capacity;

	ring->write = end;
	ring->size += size;

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
	crustMemcpy(ptr, data, size);

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
	crustMemcpy(data, ptr, size);
}

CRUST_INLINE void crustStackPop(Crust_Stack *stack, void *data, usize size)
{
	CRUST_ASSERT(stack != CRUST_NULL);
	CRUST_ASSERT(stack->memory != CRUST_NULL);
	CRUST_ASSERT(stack->head >= size);
	CRUST_ASSERT(data != CRUST_NULL);

	stack->head -= size;

	u8 *ptr = (u8 *)stack->memory + stack->head;
	crustMemcpy(data, ptr, size);
}

//
typedef struct Crust_Arena_t
{
	void *memory;
	usize capacity;
	usize size;
} Crust_Arena;

CRUST_INLINE Crust_Arena crustArenaInit(void *memory, usize capacity)
{
	CRUST_ASSERT(memory != CRUST_NULL);
	CRUST_ASSERT(capacity > 0);

	Crust_Arena result;
	result.memory = memory;
	result.capacity = capacity;
	result.size = 0;

	return result;
}

CRUST_INLINE void *crustArenaAlloc(Crust_Arena *arena, usize size, usize alignment)
{
	CRUST_ASSERT(arena != CRUST_NULL);
	CRUST_ASSERT(arena->memory != CRUST_NULL);
	CRUST_ASSERT(alignment > 0);
	CRUST_ASSERT(crustIsPow2USize(alignment));

	u8 *base = (u8 *)arena->memory;
	u8 *current = base + arena->size;
	u8 *aligned = (u8 *)crustAlignUpUSize((usize)current, alignment);

	usize offset = (usize)(aligned - base);

	if (offset > arena->capacity)
		return CRUST_NULL;

	if (size > arena->capacity - offset)
		return CRUST_NULL;

	arena->size = offset + size;
	return (void *)aligned;
}

CRUST_INLINE void crustArenaReset(Crust_Arena *arena)
{
	CRUST_ASSERT(arena != CRUST_NULL);
	CRUST_ASSERT(arena->memory != CRUST_NULL);

	arena->size = 0;
}

//
typedef void *(*PFN_crustAllocatorAlloc)(void *context, usize size, usize alignment);
typedef void *(*PFN_crustAllocatorRealloc)(void *context, void *ptr, usize old_size, usize new_size, usize alignment);
typedef void (*PFN_crustAllocatorFree)(void *allocator, void *ptr, usize size, usize alignment);

typedef struct Crust_AllocatorVtbl_t
{
	PFN_crustAllocatorAlloc alloc;
	PFN_crustAllocatorRealloc realloc;
	PFN_crustAllocatorFree free;
} Crust_AllocatorVtbl;

typedef struct Crust_Allocator_t
{
	void *context;
	const Crust_AllocatorVtbl *vtbl;
} Crust_Allocator;

CRUST_INLINE void *crustAllocatorAlloc(Crust_Allocator allocator, usize size, usize alignment)
{
	CRUST_ASSERT(allocator.vtbl != CRUST_NULL);
	CRUST_ASSERT(allocator.vtbl->alloc != CRUST_NULL);

	CRUST_ASSERT(size > 0);
	CRUST_ASSERT(alignment > 0);
	CRUST_ASSERT(crustIsPow2USize(alignment));

	return allocator.vtbl->alloc(allocator.context, size, alignment);
}

CRUST_INLINE void *crustAllocatorRealloc(Crust_Allocator allocator, void *ptr, usize old_size, usize new_size, usize alignment)
{
	CRUST_ASSERT(allocator.vtbl != CRUST_NULL);
	CRUST_ASSERT(allocator.vtbl->alloc != CRUST_NULL);
	CRUST_ASSERT(allocator.vtbl->realloc != CRUST_NULL);
	CRUST_ASSERT(allocator.vtbl->free != CRUST_NULL);

	CRUST_ASSERT(alignment > 0);
	CRUST_ASSERT(crustIsPow2USize(alignment));
	
	if (ptr == CRUST_NULL)
	{
		CRUST_ASSERT(old_size == 0);

		if (new_size == 0)
			return CRUST_NULL;

		return allocator.vtbl->alloc(allocator.context, new_size, alignment);
	}

	CRUST_ASSERT(old_size > 0);

	if (new_size == 0)
	{
		allocator.vtbl->free(allocator.context, ptr, old_size, alignment);
		return CRUST_NULL;
	}

	return allocator.vtbl->realloc(allocator.context, ptr, old_size, new_size, alignment);
}

CRUST_INLINE void crustAllocatorFree(Crust_Allocator allocator, void *ptr, usize size, usize alignment)
{
	CRUST_ASSERT(allocator.vtbl != CRUST_NULL);
	CRUST_ASSERT(allocator.vtbl->free != CRUST_NULL);

	if (ptr == CRUST_NULL)
		return;

	CRUST_ASSERT(size > 0);
	CRUST_ASSERT(alignment > 0);
	CRUST_ASSERT(crustIsPow2USize(alignment));

	allocator.vtbl->free(allocator.context, ptr, size, alignment);
}

CRUST_INLINE usize crustAllocatorSizeMul(usize a, usize b)
{
	CRUST_ASSERT(a == 0 || b <= USIZE_MAX / a);
	return a * b;
}

#define crustAlloc(allocator, T)                                     \
	((T *)crustAllocatorAlloc(                                       \
		(allocator),                                                 \
		sizeof(T),                                                   \
		CRUST_ALIGNOF(T)))

#define crustAllocArray(allocator, T, count)                         \
	((T *)crustAllocatorAlloc(                                       \
		(allocator),                                                 \
		crustAllocatorSizeMul(sizeof(T), (count)),                   \
		CRUST_ALIGNOF(T)))

#define crustReallocArray(allocator, ptr, T, old_count, new_count)   \
	((T *)crustAllocatorRealloc(                                     \
		(allocator),                                                 \
		(ptr),                                                       \
		crustAllocatorSizeMul(sizeof(T), (old_count)),               \
		crustAllocatorSizeMul(sizeof(T), (new_count)),               \
		CRUST_ALIGNOF(T)))

#define crustFree(allocator, ptr, T)                                 \
	crustAllocatorFree(                                              \
		(allocator),                                                 \
		(ptr),                                                       \
		sizeof(T),                                                   \
		CRUST_ALIGNOF(T))

#define crustFreeArray(allocator, ptr, T, count)                     \
	crustAllocatorFree(                                              \
		(allocator),                                                 \
		(ptr),                                                       \
		crustAllocatorSizeMul(sizeof(T), (count)),                   \
		CRUST_ALIGNOF(T))

//
CRUST_INLINE void *crustSystemAllocatorAlloc(void *context, usize size, usize alignment)
{
	CRUST_UNUSED(context);

	return crustAlignedMalloc(size, alignment);
}

CRUST_INLINE void *crustSystemAllocatorRealloc(void *context, void *ptr, usize old_size, usize new_size, usize alignment)
{
	CRUST_UNUSED(context);
	CRUST_UNUSED(old_size);

	return crustAlignedRealloc(ptr, new_size, alignment);
}

CRUST_INLINE void crustSystemAllocatorFree(void *context, void *ptr, usize size, usize alignment)
{
	CRUST_UNUSED(context);
	CRUST_UNUSED(size);
	CRUST_UNUSED(alignment);

	crustAlignedFree(ptr);
}

static const Crust_AllocatorVtbl crustSystemAllocatorVtbl =
{
	crustSystemAllocatorAlloc,
	crustSystemAllocatorRealloc,
	crustSystemAllocatorFree,
};

CRUST_INLINE Crust_Allocator crustSystemAllocatorInit()
{
	Crust_Allocator result;
	result.context = CRUST_NULL;
	result.vtbl = &crustSystemAllocatorVtbl;

	return result;
}

//
CRUST_INLINE void *crustArenaAllocatorAlloc(void *context, usize size, usize alignment)
{
	return crustArenaAlloc((Crust_Arena *)context, size, alignment);
}

CRUST_INLINE void *crustArenaAllocatorRealloc(void *context, void *ptr, usize old_size, usize new_size, usize alignment)
{
	Crust_Arena *arena = (Crust_Arena *)context;
	CRUST_ASSERT(arena != CRUST_NULL);
	CRUST_ASSERT(arena->memory != CRUST_NULL);

	u8 *base = (u8 *)arena->memory;

	u8 *end = base + arena->size;
	u8 *allocation_end = (u8 *)ptr + old_size;

	if (allocation_end == end)
	{
		usize offset = (usize)((u8 *)ptr - base);

		if (new_size <= arena->capacity - offset)
		{
			arena->size = offset + new_size;
			return ptr;
		}

		return CRUST_NULL;
	}

	void *new_ptr = crustArenaAlloc(arena, new_size, alignment);
	if (new_ptr == CRUST_NULL)
		return CRUST_NULL;

	crustMemcpy(new_ptr, ptr, crustMinUSize(old_size, new_size));
	return new_ptr;
}

CRUST_INLINE void crustArenaAllocatorFree(void *context, void *ptr, usize size, usize alignment)
{
	CRUST_UNUSED(context);
	CRUST_UNUSED(ptr);
	CRUST_UNUSED(size);
	CRUST_UNUSED(alignment);
}

static const Crust_AllocatorVtbl crustArenaAllocatorVtbl =
{
	crustArenaAllocatorAlloc,
	crustArenaAllocatorRealloc,
	crustArenaAllocatorFree,
};

CRUST_INLINE Crust_Allocator crustArenaAllocatorInit(Crust_Arena *arena)
{
	CRUST_ASSERT(arena != CRUST_NULL);
	CRUST_ASSERT(arena->memory != CRUST_NULL);

	Crust_Allocator result;
	result.context = arena;
	result.vtbl = &crustArenaAllocatorVtbl;

	return result;
}

//
typedef usize (*PFN_crustHashIndexMapHash)(void *context, const void *key, usize key_size);
typedef u8 (*PFN_crustHashIndexMapEqual)(void *context, const void *key1, const void *key2, usize key_size);
typedef void (*PFN_crustHashIndexMapCopy)(void *context, void *key_dst, const void *key_src, usize key_size);
typedef void (*PFN_crustHashIndexMapMove)(void *context, void *key_dst, void *key_src, usize key_size);
typedef void (*PFN_crustHashIndexMapDestroy)(void *context, void *key, usize key_size);

typedef struct Crust_HashIndexMapVtbl_t
{
	PFN_crustHashIndexMapHash hash;
	PFN_crustHashIndexMapEqual equal;
	PFN_crustHashIndexMapCopy copy;
	PFN_crustHashIndexMapMove move;
	PFN_crustHashIndexMapDestroy destroy;
} Crust_HashIndexMapVtbl;

typedef struct Crust_HashIndexMapProbe_t
{
	usize bucket;
	u8 found;
} Crust_HashIndexMapProbe;

typedef struct Crust_HashIndexMap_t
{
	void *context;
	const Crust_HashIndexMapVtbl *vtbl;

	u8 *keys;
	usize *indices;
	usize *hashes;
	u32 *masks;

	usize count;
	usize capacity;
	usize key_size;
	usize key_alignment;
	usize key_stride;
} Crust_HashIndexMap;

CRUST_INLINE Crust_HashIndexMap crustHashIndexMapInit(void *context, const Crust_HashIndexMapVtbl *vtbl, usize key_size, usize key_alignment)
{
	CRUST_ASSERT(vtbl != CRUST_NULL);
	CRUST_ASSERT(vtbl->hash != CRUST_NULL);
	CRUST_ASSERT(vtbl->equal != CRUST_NULL);
	CRUST_ASSERT(vtbl->copy != CRUST_NULL);
	CRUST_ASSERT(vtbl->move != CRUST_NULL);
	CRUST_ASSERT(vtbl->destroy != CRUST_NULL);

	CRUST_ASSERT(key_size > 0);
	CRUST_ASSERT(key_alignment > 0);
	CRUST_ASSERT(crustIsPow2USize(key_alignment) != 0);

	Crust_HashIndexMap result;
	result.context = context;
	result.vtbl = vtbl;

	result.keys = CRUST_NULL;
	result.indices = CRUST_NULL;
	result.hashes = CRUST_NULL;
	result.masks = CRUST_NULL;

	result.count = 0;
	result.capacity = 0;
	result.key_size = key_size;
	result.key_alignment = key_alignment;
	result.key_stride = crustAlignUpUSize(key_size, key_alignment);

	return result;
}

CRUST_INLINE u8 crustHashIndexMapCheckMask(const Crust_HashIndexMap *hash_map, usize bucket)
{
	CRUST_ASSERT(hash_map != CRUST_NULL);
	CRUST_ASSERT(bucket < hash_map->capacity);

	usize mask_index = bucket / 32;
	u32 bit_mask = (1u << (bucket % 32));

	return (hash_map->masks[mask_index] & bit_mask) != 0;
}

CRUST_INLINE void crustHashIndexMapAddMask(Crust_HashIndexMap *hash_map, usize bucket)
{
	CRUST_ASSERT(hash_map != CRUST_NULL);
	CRUST_ASSERT(bucket < hash_map->capacity);

	usize mask_index = bucket / 32;
	u32 bit_mask = (1u << (bucket % 32));

	hash_map->masks[mask_index] |= bit_mask;
}

CRUST_INLINE void crustHashIndexMapRemoveMask(Crust_HashIndexMap *hash_map, usize bucket)
{
	CRUST_ASSERT(hash_map != CRUST_NULL);
	CRUST_ASSERT(bucket < hash_map->capacity);

	usize mask_index = bucket / 32;
	u32 bit_mask = (1u << (bucket % 32));

	hash_map->masks[mask_index] &= ~bit_mask;
}

CRUST_INLINE u8 *crustHashIndexMapKey(Crust_HashIndexMap *hash_map, usize bucket)
{
	CRUST_ASSERT(hash_map != CRUST_NULL);
	CRUST_ASSERT(bucket < hash_map->capacity);

	return hash_map->keys + hash_map->key_stride * bucket;
}

CRUST_INLINE const u8 *crustHashIndexMapConstKey(const Crust_HashIndexMap *hash_map, usize bucket)
{
	CRUST_ASSERT(hash_map != CRUST_NULL);
	CRUST_ASSERT(bucket < hash_map->capacity);

	return hash_map->keys + hash_map->key_stride * bucket;
}

CRUST_INLINE void crustHashIndexMapFreeStorage(Crust_Allocator allocator, Crust_HashIndexMap *hash_map)
{
	CRUST_ASSERT(hash_map != CRUST_NULL);

	usize masks_capacity = crustAlignUpUSize(hash_map->capacity, 32) / 32;

	crustAllocatorFree(allocator, hash_map->keys, hash_map->key_stride * hash_map->capacity, hash_map->key_alignment);
	crustFreeArray(allocator, hash_map->hashes, usize, hash_map->capacity);
	crustFreeArray(allocator, hash_map->indices, usize, hash_map->capacity);
	crustFreeArray(allocator, hash_map->masks, u32, masks_capacity);

	hash_map->keys = CRUST_NULL;
	hash_map->hashes = CRUST_NULL;
	hash_map->indices = CRUST_NULL;
	hash_map->masks = CRUST_NULL;

	hash_map->capacity = 0;
	hash_map->count = 0;
}

CRUST_INLINE Crust_HashIndexMapProbe crustHashIndexMapProbeKey(const Crust_HashIndexMap *hash_map, const void *key, usize key_size, usize hash)
{
	CRUST_ASSERT(hash_map != CRUST_NULL);
	CRUST_ASSERT(hash_map->capacity > 0);

	CRUST_ASSERT(key != CRUST_NULL);
	CRUST_ASSERT(key_size == hash_map->key_size);

	usize bucket = hash % hash_map->capacity;

	for (usize probe = 0; probe < hash_map->capacity; ++probe)
	{
		if (crustHashIndexMapCheckMask(hash_map, bucket) == 0)
		{
			Crust_HashIndexMapProbe result = { bucket, 0 };
			return result;
		}

		if (hash_map->hashes[bucket] == hash)
		{
			const u8 *stored_key = crustHashIndexMapConstKey(hash_map, bucket);

			if (hash_map->vtbl->equal(hash_map->context, stored_key, key, key_size))
			{
				Crust_HashIndexMapProbe result = { bucket, 1 };
				return result;
			}
		}

		bucket = (bucket + 1) % hash_map->capacity;
	}

	Crust_HashIndexMapProbe result = { USIZE_MAX, 0 };
	return result;
}

CRUST_INLINE usize crustHashIndexMapFindFreeBucket(const Crust_HashIndexMap *hash_map, usize hash)
{
	CRUST_ASSERT(hash_map != CRUST_NULL);
	CRUST_ASSERT(hash_map->capacity > 0);
	CRUST_ASSERT(hash_map->count < hash_map->capacity);

	usize bucket = hash % hash_map->capacity;

	for (usize probe = 0; probe < hash_map->capacity; ++probe)
	{
		if (crustHashIndexMapCheckMask(hash_map, bucket) == 0)
			return bucket;

		bucket = (bucket + 1) % hash_map->capacity;
	}

	CRUST_ASSERT(0);
	return USIZE_MAX;
}

CRUST_INLINE void crustHashIndexMapAlloc(Crust_Allocator allocator, Crust_HashIndexMap *hash_map, usize capacity)
{
	CRUST_ASSERT(hash_map != CRUST_NULL);
	CRUST_ASSERT(hash_map->capacity == 0);

	CRUST_ASSERT(capacity > 0);

	usize masks_capacity = crustAlignUpUSize(capacity, 32) / 32;

	hash_map->keys = (u8 *)crustAllocatorAlloc(allocator, hash_map->key_stride * capacity, hash_map->key_alignment);
	hash_map->hashes = crustAllocArray(allocator, usize, capacity);
	hash_map->indices = crustAllocArray(allocator, usize, capacity);
	hash_map->masks = crustAllocArray(allocator, u32, masks_capacity);
	hash_map->capacity = capacity;
	hash_map->count = 0;

	for (usize i = 0; i < masks_capacity; ++i)
		hash_map->masks[i] = 0;
}

CRUST_INLINE void crustHashIndexMapFree(Crust_Allocator allocator, Crust_HashIndexMap *hash_map)
{
	CRUST_ASSERT(hash_map != CRUST_NULL);

	for (usize i = 0; i < hash_map->capacity; ++i)
	{
		if (crustHashIndexMapCheckMask(hash_map, i) == 0)
			continue;

		u8 *key = crustHashIndexMapKey(hash_map, i);
		hash_map->vtbl->destroy(hash_map->context, key, hash_map->key_size);
	}

	crustHashIndexMapFreeStorage(allocator, hash_map);
}

CRUST_INLINE void crustHashIndexMapGrow(Crust_Allocator allocator, Crust_HashIndexMap *hash_map, usize new_capacity)
{
	CRUST_ASSERT(hash_map != CRUST_NULL);

	CRUST_ASSERT(new_capacity > hash_map->capacity);

	Crust_HashIndexMap new_hash_map = crustHashIndexMapInit(hash_map->context, hash_map->vtbl, hash_map->key_size, hash_map->key_alignment);
	crustHashIndexMapAlloc(allocator, &new_hash_map, new_capacity);
	
	for (usize i = 0; i < hash_map->capacity; ++i)
	{
		if (crustHashIndexMapCheckMask(hash_map, i) == 0)
			continue;

		usize hash = hash_map->hashes[i];
		usize index = hash_map->indices[i];

		usize new_bucket = crustHashIndexMapFindFreeBucket(&new_hash_map, hash);

		u8 *dst_key = crustHashIndexMapKey(&new_hash_map, new_bucket);
		u8 *src_key = crustHashIndexMapKey(hash_map, i);

		hash_map->vtbl->move(hash_map->context, dst_key, src_key, hash_map->key_size);

		new_hash_map.hashes[new_bucket] = hash;
		new_hash_map.indices[new_bucket] = index;
		
		crustHashIndexMapAddMask(&new_hash_map, new_bucket);
		new_hash_map.count++;
	}

	crustHashIndexMapFreeStorage(allocator, hash_map);
	*hash_map = new_hash_map;
}

CRUST_INLINE usize crustHashIndexMapFind(const Crust_HashIndexMap *hash_map, const void *key, usize key_size)
{
	CRUST_ASSERT(hash_map != CRUST_NULL);

	CRUST_ASSERT(key != CRUST_NULL);
	CRUST_ASSERT(key_size == hash_map->key_size);

	if (hash_map->count == 0)
		return USIZE_MAX;

	usize hash = hash_map->vtbl->hash(hash_map->context, key, key_size);
	Crust_HashIndexMapProbe probe = crustHashIndexMapProbeKey(hash_map, key, key_size, hash);

	if (probe.found == 0)
		return USIZE_MAX;

	return hash_map->indices[probe.bucket];
}

CRUST_INLINE usize crustHashIndexMapAdd(Crust_HashIndexMap *hash_map, const void *key, usize key_size, usize index)
{
	CRUST_ASSERT(hash_map != CRUST_NULL);
	CRUST_ASSERT(hash_map->capacity > 0);

	CRUST_ASSERT(key != CRUST_NULL);
	CRUST_ASSERT(key_size == hash_map->key_size);

	CRUST_ASSERT(index != USIZE_MAX);

	usize hash = hash_map->vtbl->hash(hash_map->context, key, key_size);

	Crust_HashIndexMapProbe probe = crustHashIndexMapProbeKey(hash_map, key, key_size, hash);

	if (probe.found)
	{
		usize stored_index = hash_map->indices[probe.bucket];
		CRUST_ASSERT(stored_index == index);

		return stored_index;
	}

	if (probe.bucket == USIZE_MAX)
		return USIZE_MAX;

	u8 *stored_key = crustHashIndexMapKey(hash_map, probe.bucket);

	hash_map->vtbl->copy(hash_map->context, stored_key, key, key_size);

	hash_map->hashes[probe.bucket] = hash;
	hash_map->indices[probe.bucket] = index;

	crustHashIndexMapAddMask(hash_map, probe.bucket);
	hash_map->count++;

	return index;
}

CRUST_INLINE usize crustHashIndexMapRemove(Crust_HashIndexMap *hash_map, const void *key, usize key_size)
{
	CRUST_ASSERT(hash_map != CRUST_NULL);

	CRUST_ASSERT(key != CRUST_NULL);
	CRUST_ASSERT(key_size == hash_map->key_size);

	if (hash_map->count == 0)
		return USIZE_MAX;

	usize hash = hash_map->vtbl->hash(hash_map->context, key, key_size);
	Crust_HashIndexMapProbe probe = crustHashIndexMapProbeKey(hash_map, key, key_size, hash);

	if (probe.found == 0)
		return USIZE_MAX;

	usize removed_index = hash_map->indices[probe.bucket];
	u8 *stored_key = crustHashIndexMapKey(hash_map, probe.bucket);

	hash_map->vtbl->destroy(hash_map->context, stored_key, hash_map->key_size);

	usize hole = probe.bucket;
	usize scan = (hole + 1) % hash_map->capacity;

	for (usize shift_probe = 0; shift_probe < hash_map->capacity - 1; ++shift_probe)
	{
		if (crustHashIndexMapCheckMask(hash_map, scan) == 0)
			break;

		usize scan_hash = hash_map->hashes[scan];
		usize home = scan_hash % hash_map->capacity;

		usize scan_distance = (scan + hash_map->capacity - home) % hash_map->capacity;
		usize hole_distance = (hole + hash_map->capacity - home) % hash_map->capacity;

		if (hole_distance < scan_distance)
		{
			u8 *dst_key = crustHashIndexMapKey(hash_map, hole);
			u8 *src_key = crustHashIndexMapKey(hash_map, scan);

			hash_map->vtbl->move(hash_map->context, dst_key, src_key, hash_map->key_size);

			hash_map->hashes[hole] = hash_map->hashes[scan];
			hash_map->indices[hole] = hash_map->indices[scan];

			hole = scan;
		}

		scan = (scan + 1) % hash_map->capacity;
	}

	crustHashIndexMapRemoveMask(hash_map, hole);
	hash_map->count--;

	return removed_index;
}

//
typedef struct Crust_GenerationalHandle_t
{
	usize generation;
	usize index;
} Crust_GenerationalHandle;

typedef struct Crust_GenerationalHandlePool_t
{
	usize *generations;
	usize *prevs;
	usize *nexts;
	u32 *masks;
	usize capacity;
	usize free_head;
	usize free_tail;
	usize used_head;
	usize used_tail;
} Crust_GenerationalHandlePool;

CRUST_INLINE void crustGenerationalHandlePoolAlloc(Crust_Allocator allocator, Crust_GenerationalHandlePool *pool, usize capacity)
{
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(capacity > 0);

	usize masks_capacity = crustAlignUpUSize(capacity, 32) / 32;

	pool->generations = crustAllocArray(allocator, usize, capacity);
	pool->prevs = crustAllocArray(allocator, usize, capacity);
	pool->nexts = crustAllocArray(allocator, usize, capacity);
	pool->masks = crustAllocArray(allocator, u32, masks_capacity);
	pool->capacity = capacity;

	for (usize i = 0; i < capacity; ++i)
	{
		usize next = (i + 1 < capacity) ? i + 1 : USIZE_MAX;
		usize prev = (i > 0) ? i - 1 : USIZE_MAX;

		pool->nexts[i] = next;
		pool->prevs[i] = prev;
		pool->generations[i] = 0;
	}

	for (usize i = 0; i < masks_capacity; ++i)
		pool->masks[i] = 0;

	pool->free_head = 0;
	pool->free_tail = capacity - 1;

	pool->used_head = USIZE_MAX;
	pool->used_tail = USIZE_MAX;
}

CRUST_INLINE void crustGenerationalHandlePoolFree(Crust_Allocator allocator, Crust_GenerationalHandlePool *pool)
{
	CRUST_ASSERT(pool != CRUST_NULL);

	usize masks_capacity = crustAlignUpUSize(pool->capacity, 32) / 32;

	crustFreeArray(allocator, pool->generations, usize, pool->capacity);
	crustFreeArray(allocator, pool->prevs, usize, pool->capacity);
	crustFreeArray(allocator, pool->nexts, usize, pool->capacity);
	crustFreeArray(allocator, pool->masks, u32, masks_capacity);

	pool->generations = CRUST_NULL;
	pool->prevs = CRUST_NULL;
	pool->nexts = CRUST_NULL;
	pool->masks = CRUST_NULL;

	pool->capacity = 0;

	pool->free_head = USIZE_MAX;
	pool->free_tail = USIZE_MAX;

	pool->used_head = USIZE_MAX;
	pool->used_tail = USIZE_MAX;
}

CRUST_INLINE void crustGenerationalHandlePoolGrow(Crust_Allocator allocator, Crust_GenerationalHandlePool *pool, usize new_capacity)
{
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(pool->capacity < new_capacity);

	usize masks_capacity = crustAlignUpUSize(pool->capacity, 32) / 32;
	usize new_masks_capacity = crustAlignUpUSize(new_capacity, 32) / 32;

	pool->generations = crustReallocArray(allocator, pool->generations, usize, pool->capacity, new_capacity);
	pool->prevs = crustReallocArray(allocator, pool->prevs, usize, pool->capacity, new_capacity);
	pool->nexts = crustReallocArray(allocator, pool->nexts, usize, pool->capacity, new_capacity);

	if (masks_capacity != new_masks_capacity)
		pool->masks = crustReallocArray(allocator, pool->masks, u32, masks_capacity, new_masks_capacity);

	for (usize i = masks_capacity; i < new_masks_capacity; ++i)
		pool->masks[i] = 0;

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
		pool->free_head = pool->capacity;

	pool->free_tail = new_capacity - 1;
	pool->capacity = new_capacity;
}

CRUST_INLINE void crustGenerationalHandlePoolAddMask(Crust_GenerationalHandlePool *pool, usize index)
{
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(pool->masks != CRUST_NULL);
	CRUST_ASSERT(pool->capacity > index);

	usize mask_index = index / 32;
	usize bit_index = index % 32;

	pool->masks[mask_index] |= (1u << bit_index);
}

CRUST_INLINE void crustGenerationalHandlePoolRemoveMask(Crust_GenerationalHandlePool *pool, usize index)
{
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(pool->masks != CRUST_NULL);
	CRUST_ASSERT(pool->capacity > index);

	usize mask_index = index / 32;
	usize bit_index = index % 32;

	pool->masks[mask_index] &= ~(1u << bit_index);
}

CRUST_INLINE u8 crustGenerationalHandlePoolCheckMask(const Crust_GenerationalHandlePool *pool, usize index)
{
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(pool->masks != CRUST_NULL);
	CRUST_ASSERT(index < pool->capacity);

	usize mask_index = index / 32;
	usize bit_index = index % 32;

	return (pool->masks[mask_index] & (1u << bit_index)) != 0;
}

CRUST_INLINE u32 crustGenerationalHandlePoolCheck(const Crust_GenerationalHandlePool *pool, Crust_GenerationalHandle handle)
{
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(handle.index < pool->capacity);

	usize generation = pool->generations[handle.index];

	if (generation == 0 || generation != handle.generation)
		return 0;

	if (crustGenerationalHandlePoolCheckMask(pool, handle.index) == 0)
		return 0;

	return 1;
}

CRUST_INLINE Crust_GenerationalHandle crustGenerationalHandlePoolAdd(Crust_GenerationalHandlePool *pool, usize max_generation)
{
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(pool->prevs != CRUST_NULL);
	CRUST_ASSERT(pool->nexts != CRUST_NULL);
	CRUST_ASSERT(pool->generations != CRUST_NULL);
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

	crustGenerationalHandlePoolAddMask(pool, index);

	usize generation = pool->generations[index];
	generation = crustMaxUSize(1, (generation + 1) % max_generation);

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

	Crust_GenerationalHandle result;
	result.index = index;
	result.generation = generation;

	return result;
}

CRUST_INLINE void crustGenerationalHandlePoolRemove(Crust_GenerationalHandlePool *pool, Crust_GenerationalHandle handle)
{
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(pool->prevs != CRUST_NULL);
	CRUST_ASSERT(pool->nexts != CRUST_NULL);
	CRUST_ASSERT(pool->generations != CRUST_NULL);
	CRUST_ASSERT(pool->used_tail != USIZE_MAX);
	CRUST_ASSERT(crustGenerationalHandlePoolCheck(pool, handle) != 0);

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

	crustGenerationalHandlePoolRemoveMask(pool, index);
}

CRUST_INLINE Crust_GenerationalHandle crustGenerationalHandlePoolHead(const Crust_GenerationalHandlePool *pool)
{
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(pool->generations != CRUST_NULL);

	Crust_GenerationalHandle result;
	result.index = pool->used_head;
	result.generation = 0;

	if (result.index != USIZE_MAX)
		result.generation = pool->generations[result.index];

	return result;
}

CRUST_INLINE Crust_GenerationalHandle crustGenerationalHandlePoolTail(const Crust_GenerationalHandlePool *pool)
{
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(pool->generations != CRUST_NULL);

	Crust_GenerationalHandle result;
	result.index = pool->used_tail;
	result.generation = 0;

	if (result.index != USIZE_MAX)
		result.generation = pool->generations[result.index];

	return result;
}

CRUST_INLINE Crust_GenerationalHandle crustGenerationalHandlePoolNext(const Crust_GenerationalHandlePool *pool, Crust_GenerationalHandle handle)
{
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(pool->nexts != CRUST_NULL);
	CRUST_ASSERT(pool->generations != CRUST_NULL);
	CRUST_ASSERT(crustGenerationalHandlePoolCheck(pool, handle) != 0);

	Crust_GenerationalHandle result;
	result.index = pool->nexts[handle.index];
	result.generation = 0;

	if (result.index != USIZE_MAX)
		result.generation = pool->generations[result.index];

	return result;
}

CRUST_INLINE Crust_GenerationalHandle crustGenerationalHandlePoolPrev(const Crust_GenerationalHandlePool *pool, Crust_GenerationalHandle handle)
{
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(pool->prevs != CRUST_NULL);
	CRUST_ASSERT(pool->generations != CRUST_NULL);
	CRUST_ASSERT(crustGenerationalHandlePoolCheck(pool, handle) != 0);

	Crust_GenerationalHandle result;
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

CRUST_INLINE void crustHeapNodePoolAlloc(Crust_Allocator allocator, Crust_HeapNodePool *pool, u32 capacity)
{
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(capacity > 0);

	usize masks_capacity = crustAlignUpUSize(capacity, 32) / 32;

	pool->nodes = crustAllocArray(allocator, Crust_HeapNode, capacity);
	pool->node_masks = crustAllocArray(allocator, u32, masks_capacity);
	pool->free_indices = crustAllocArray(allocator, u32, capacity);
	pool->num_free_indices = capacity;
	pool->capacity = capacity;

	for (u32 i = 0; i < capacity; ++i)
		pool->free_indices[i] = capacity - i - 1;

	for (u32 i = 0; i < masks_capacity; ++i)
		pool->node_masks[i] = 0;
}

CRUST_INLINE void crustHeapNodePoolFree(Crust_Allocator allocator, Crust_HeapNodePool *pool)
{
	CRUST_ASSERT(pool != CRUST_NULL);

	usize masks_capacity = crustAlignUpUSize(pool->capacity, 32) / 32;

	crustFreeArray(allocator, pool->nodes, Crust_HeapNode, pool->capacity);
	crustFreeArray(allocator, pool->node_masks, u32, masks_capacity);
	crustFreeArray(allocator, pool->free_indices, u32, pool->capacity);

	pool->nodes = CRUST_NULL;
	pool->node_masks = CRUST_NULL;
	pool->free_indices = CRUST_NULL;

	pool->num_free_indices = 0;
	pool->capacity = 0;
}

CRUST_INLINE u32 crustHeapNodePoolAcquire(Crust_HeapNodePool *pool)
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

CRUST_INLINE void crustHeapNodePoolAddMask(Crust_HeapNodePool *pool, u32 index)
{
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(pool->node_masks != CRUST_NULL);
	CRUST_ASSERT(pool->capacity > index);

	u32 mask_index = index / 32;
	u32 bit_index = index % 32;

	pool->node_masks[mask_index] |= (1u << bit_index);
}

CRUST_INLINE void crustHeapNodePoolRemoveMask(Crust_HeapNodePool *pool, u32 index)
{
	CRUST_ASSERT(pool != CRUST_NULL);
	CRUST_ASSERT(pool->node_masks != CRUST_NULL);
	CRUST_ASSERT(pool->capacity > index);

	u32 mask_index = index / 32;
	u32 bit_index = index % 32;

	pool->node_masks[mask_index] &= ~(1u << bit_index);
}

CRUST_INLINE u8 crustHeapNodePoolCheckMask(const Crust_HeapNodePool *pool, u32 index)
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

	u32 leading_zeroes = crustLzcntU32(size);
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

	u32 leading_zeroes = crustLzcntU32(size);
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

	return (u8)crustTzcntU32(masked_bins);
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

		linear_bin_index = (u8)crustTzcntU32(used_linear_bins);
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
	crustHeapNodePoolRemoveMask(pool, index);

	u32 bin_head_index = heap->heads[bin_index];

	if (bin_head_index != U32_MAX)
	{
		Crust_HeapNode *bin_head_node = &pool->nodes[bin_head_index];
		CRUST_ASSERT(crustHeapNodePoolCheckMask(pool, bin_head_index) == 0);

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
	CRUST_ASSERT(crustHeapNodePoolCheckMask(pool, index) == 0);

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

	u32 index = crustHeapNodePoolAcquire(pool);
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
	CRUST_ASSERT(crustIsPow2U32(alignment) != 0);

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

	result.offset = crustAlignUpU32(node->offset, alignment);

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

		result.offset = crustAlignUpU32(node->offset, alignment);
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
	CRUST_ASSERT(crustHeapNodePoolCheckMask(pool, allocation.index) == 0);
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

	crustHeapNodePoolAddMask(pool, allocation.index);

	if (remainder_begin_size > 0)
	{
		Crust_HeapNode *prev_node = (prev_index != U32_MAX) ? &pool->nodes[prev_index] : CRUST_NULL;

		// try merge with previous free node
		if (prev_node != CRUST_NULL && crustHeapNodePoolCheckMask(pool, prev_index) == 0)
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

		u32 new_index = crustHeapNodePoolAcquire(pool);
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
		if (next_node != CRUST_NULL && crustHeapNodePoolCheckMask(pool, next_index) == 0)
		{
			CRUST_ASSERT(next_node->prev_neighbour == allocation.index);

			remainder_end_size += next_node->size;

			u32 next_next_index = next_node->next_neighbour;

			crustHeapRemoveNodeFromBin(heap, next_index);
			crustHeapNodePoolRelease(pool, next_index);

			next_index = next_next_index;
			next_node = (next_index != U32_MAX) ? &pool->nodes[next_index] : CRUST_NULL;
		}

		u32 new_index = crustHeapNodePoolAcquire(pool);
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
	CRUST_ASSERT(crustIsPow2U32(alignment) != 0);

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
	CRUST_ASSERT(crustHeapNodePoolCheckMask(pool, allocation.index) != 0);
	CRUST_ASSERT(allocation.offset >= node->offset);

	u32 prev_index = node->prev_neighbour;
	u32 next_index = node->next_neighbour;

	u32 size = node->size;
	u32 offset = node->offset;

	Crust_HeapNode *prev_node = (prev_index != U32_MAX) ? &pool->nodes[prev_index] : CRUST_NULL;

	// try merge with previous free node
	if (prev_node != CRUST_NULL && crustHeapNodePoolCheckMask(pool, prev_index) == 0)
	{
		offset = prev_node->offset;
		size += prev_node->size;

		crustHeapRemoveNodeFromBin(heap, prev_index);
		crustHeapNodePoolRelease(pool, prev_index);

		prev_index = prev_node->prev_neighbour;
	}

	Crust_HeapNode *next_node = (next_index != U32_MAX) ? &pool->nodes[next_index] : CRUST_NULL;

	// try merge with next free node
	if (next_node != CRUST_NULL && crustHeapNodePoolCheckMask(pool, next_index) == 0)
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
