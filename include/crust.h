#pragma once

// Version
#define CRUST_VERSION_MAJOR 1
#define CRUST_VERSION_MINOR 0
#define CRUST_VERSION_PATCH 0
#define CRUST_VERSION "1.0.0-dev"

// Compiler
#define CRUST_COMPILER_MSVC  0
#define CRUST_COMPILER_GCC   0
#define CRUST_COMPILER_CLANG 0

#if defined(__clang__)
	#undef  CRUST_COMPILER_CLANG
	#define CRUST_COMPILER_CLANG 1
#elif defined(_MSC_VER)
	#undef  CRUST_COMPILER_MSVC
	#define CRUST_COMPILER_MSVC 1
#elif defined(__GNUC__)
	#undef  CRUST_COMPILER_GCC
	#define CRUST_COMPILER_GCC 1
#else
	#error Unsupported compiler
#endif

// Toolchain
#define CRUST_TOOLCHAIN_MSVC       0
#define CRUST_TOOLCHAIN_MINGW      0
#define CRUST_TOOLCHAIN_ANDROID    0
#define CRUST_TOOLCHAIN_EMSCRIPTEN 0
#define CRUST_TOOLCHAIN_WASI       0
#define CRUST_TOOLCHAIN_APPLE      0

#if defined(__EMSCRIPTEN__)
	#undef  CRUST_TOOLCHAIN_EMSCRIPTEN
	#define CRUST_TOOLCHAIN_EMSCRIPTEN 1
#elif defined(__wasip1__) || defined(__wasip2__) || defined(__wasip3__) || defined(__wasi__)
	#undef  CRUST_TOOLCHAIN_WASI
	#define CRUST_TOOLCHAIN_WASI 1
#elif defined(__ANDROID__)
	#undef  CRUST_TOOLCHAIN_ANDROID
	#define CRUST_TOOLCHAIN_ANDROID 1
#elif defined(__MINGW32__) || defined(__MINGW64__)
	#undef  CRUST_TOOLCHAIN_MINGW
	#define CRUST_TOOLCHAIN_MINGW 1
#elif defined(_MSC_VER)
	#undef  CRUST_TOOLCHAIN_MSVC
	#define CRUST_TOOLCHAIN_MSVC 1
#elif defined(__apple_build_version__)
	#undef  CRUST_TOOLCHAIN_APPLE
	#define CRUST_TOOLCHAIN_APPLE 1
#endif

// Architecture
#define CRUST_ARCH_X86      0
#define CRUST_ARCH_X64      0
#define CRUST_ARCH_ARM      0
#define CRUST_ARCH_ARM64    0
#define CRUST_ARCH_WASM32   0
#define CRUST_ARCH_WASM64   0

#define CRUST_ARCH_32BIT 0
#define CRUST_ARCH_64BIT 0

#if defined(__wasm64__)
	#undef  CRUST_ARCH_WASM64
	#define CRUST_ARCH_WASM64 1
	#undef  CRUST_ARCH_64BIT
	#define CRUST_ARCH_64BIT 1
#elif defined(__wasm32__) || (defined(__wasm__) && !defined(__wasm64__))
	#undef  CRUST_ARCH_WASM32
	#define CRUST_ARCH_WASM32 1
	#undef  CRUST_ARCH_32BIT
	#define CRUST_ARCH_32BIT 1
#elif defined(_M_ARM64EC) || defined(_M_ARM64) || defined(__aarch64__)
	#undef  CRUST_ARCH_ARM64
	#define CRUST_ARCH_ARM64 1
	#undef  CRUST_ARCH_64BIT
	#define CRUST_ARCH_64BIT 1
#elif defined(_M_ARM) || defined(__arm__)
	#undef  CRUST_ARCH_ARM
	#define CRUST_ARCH_ARM 1
	#undef  CRUST_ARCH_32BIT
	#define CRUST_ARCH_32BIT 1
#elif defined(_M_X64) || defined(_M_AMD64) || defined(__x86_64__)
	#undef  CRUST_ARCH_X64
	#define CRUST_ARCH_X64 1
	#undef  CRUST_ARCH_64BIT
	#define CRUST_ARCH_64BIT 1
#elif defined(_M_IX86) || defined(__i386__)
	#undef  CRUST_ARCH_X86
	#define CRUST_ARCH_X86 1
	#undef  CRUST_ARCH_32BIT
	#define CRUST_ARCH_32BIT 1
#else
	#error "Unsupported architecture"
#endif

// Defines
#if CRUST_COMPILER_MSVC
    #define CRUST_ALIGNOF(x) __alignof(x)
    #define CRUST_ALIGNAS(n) __declspec(align(n))
    #define CRUST_DEBUG_BREAK() __debugbreak()
    #define CRUST_INLINE __forceinline
    #define CRUST_RESTRICT __restrict
#else
    #define CRUST_ALIGNOF(x) __alignof__(x)
    #define CRUST_ALIGNAS(n) __attribute__((aligned(n)))
    #define CRUST_DEBUG_BREAK() __builtin_trap()
    #define CRUST_INLINE inline __attribute__((always_inline))
    #define CRUST_RESTRICT __restrict__
#endif

#if defined(NDEBUG)
    #define CRUST_ASSERT(x) ((void)0)
#else
    #define CRUST_ASSERT(x) do { (void)(!!(x) || (crustAssertFailure(#x, __FILE__, (u32)__LINE__), 0)); } while (0)
#endif

#define CRUST_UNUSED(x) do { (void)(x); } while (0)

#ifdef __cplusplus
	#define CRUST_NULL 0
#else
	#define CRUST_NULL ((void *)0)
#endif

#if defined(CRUST_SHARED_LIBRARY)
    #if CRUST_TOOLCHAIN_MSVC || CRUST_TOOLCHAIN_MINGW
        #if defined(CRUST_IMPLEMENTATION)
            #define CRUST_APIENTRY __declspec(dllexport)
        #else
            #define CRUST_APIENTRY __declspec(dllimport)
        #endif
    #else
        #define CRUST_APIENTRY __attribute__((visibility("default")))
    #endif
#else
    #define CRUST_APIENTRY
#endif

// Types
typedef signed char					i8;
typedef signed short				i16;
typedef signed int					i32;
typedef signed long long			i64;

typedef unsigned char				u8;
typedef unsigned short				u16;
typedef unsigned int				u32;
typedef unsigned long long			u64;

#if CRUST_ARCH_64BIT
	typedef i64						isize;
	typedef u64						usize;
#else
	typedef i32						isize;
	typedef u32						usize;
#endif

typedef float						f32;
typedef double						f64;

#define CRUST_I8_MIN				(-128)
#define CRUST_I16_MIN				(-32768)
#define CRUST_I32_MIN				(-2147483647 - 1)
#define CRUST_I64_MIN				(-9223372036854775807LL - 1)
#define CRUST_I8_MAX				127
#define CRUST_I16_MAX				32767
#define CRUST_I32_MAX				2147483647
#define CRUST_I64_MAX				9223372036854775807LL
#define CRUST_U8_MAX				255
#define CRUST_U16_MAX				65535
#define CRUST_U32_MAX				4294967295U
#define CRUST_U64_MAX				18446744073709551615ULL

#if CRUST_ARCH_64BIT
	#define CRUST_USIZE_MAX			CRUST_U64_MAX
	#define CRUST_ISIZE_MAX			CRUST_I64_MAX
	#define CRUST_ISIZE_MIN			CRUST_I64_MIN
#else
	#define CRUST_USIZE_MAX			CRUST_U32_MAX
	#define CRUST_ISIZE_MAX			CRUST_I32_MAX
	#define CRUST_ISIZE_MIN			CRUST_I32_MIN
#endif

#define CRUST_F64_EPSILON			2.2204460492503131e-016
#define CRUST_F64_MAX				1.7976931348623158e+308
#define CRUST_F64_MIN				(-CRUST_F64_MAX)
#define CRUST_F64_POSITIVE_MIN		2.2250738585072014e-308

#define CRUST_F32_EPSILON			1.192092896e-07F
#define CRUST_F32_MAX				3.402823466e+38F
#define CRUST_F32_MIN				(-CRUST_F32_MAX)
#define CRUST_F32_POSITIVE_MIN		1.175494351e-38F

#ifdef __cplusplus
extern "C" {
#endif

CRUST_APIENTRY void crustAssertFailure(const char *expression, const char *file, u32 line);

CRUST_APIENTRY f32 crustAbsF32(f32 v);
CRUST_APIENTRY f64 crustAbsF64(f64 v);
CRUST_APIENTRY f32 crustSqrtF32(f32 v);
CRUST_APIENTRY f32 crustRsqrtF32(f32 v);
CRUST_APIENTRY f32 crustCosF32(f32 v);
CRUST_APIENTRY f32 crustAcosF32(f32 v);
CRUST_APIENTRY f32 crustSinF32(f32 v);
CRUST_APIENTRY f32 crustAsinF32(f32 v);
CRUST_APIENTRY f32 crustTanF32(f32 v);
CRUST_APIENTRY f32 crustAtan2F32(f32 y, f32 x);

//
CRUST_APIENTRY void crustMemcpy(void *dst, const void *src, usize size);
CRUST_APIENTRY void crustMemset(void *dst, u8 value, usize size);

//
static CRUST_INLINE u8 crustIsPow2U8(u8 v)
{
	CRUST_ASSERT(v != 0);
	return (v & (v - 1)) == 0;
}

static CRUST_INLINE u16 crustIsPow2U16(u16 v)
{
	CRUST_ASSERT(v != 0);
	return (v & (v - 1)) == 0;
}

static CRUST_INLINE u32 crustIsPow2U32(u32 v)
{
	CRUST_ASSERT(v != 0);
	return (v & (v - 1)) == 0;
}

static CRUST_INLINE u64 crustIsPow2U64(u64 v)
{
	CRUST_ASSERT(v != 0);
	return (v & (v - 1)) == 0;
}

static CRUST_INLINE usize crustIsPow2USize(usize v)
{
	CRUST_ASSERT(v != 0);
	return (v & (v - 1)) == 0;
}

static CRUST_INLINE u8 crustIsAlignedU8(u8 v, u8 alignment)
{
	CRUST_ASSERT(alignment != 0);
	CRUST_ASSERT(crustIsPow2U8(alignment) != 0);

	return (v & (alignment - 1)) == 0;
}

static CRUST_INLINE u16 crustIsAlignedU16(u16 v, u16 alignment)
{
	CRUST_ASSERT(alignment != 0);
	CRUST_ASSERT(crustIsPow2U16(alignment) != 0);

	return (v & (alignment - 1)) == 0;
}

static CRUST_INLINE u32 crustIsAlignedU32(u32 v, u32 alignment)
{
	CRUST_ASSERT(alignment != 0);
	CRUST_ASSERT(crustIsPow2U32(alignment) != 0);

	return (v & (alignment - 1)) == 0;
}

static CRUST_INLINE u64 crustIsAlignedU64(u64 v, u64 alignment)
{
	CRUST_ASSERT(alignment != 0);
	CRUST_ASSERT(crustIsPow2U64(alignment) != 0);

	return (v & (alignment - 1)) == 0;
}

static CRUST_INLINE usize crustIsAlignedUSize(usize v, usize alignment)
{
	CRUST_ASSERT(alignment != 0);
	CRUST_ASSERT(crustIsPow2USize(alignment) != 0);

	return (v & (alignment - 1)) == 0;
}

static CRUST_INLINE u8 crustAlignDownU8(u8 value, u8 alignment)
{
	CRUST_ASSERT(alignment != 0);
	CRUST_ASSERT(crustIsPow2U8(alignment) != 0);

	return value & ~(alignment - 1);
}

static CRUST_INLINE u16 crustAlignDownU16(u16 value, u16 alignment)
{
	CRUST_ASSERT(alignment != 0);
	CRUST_ASSERT(crustIsPow2U16(alignment) != 0);

	return value & ~(alignment - 1);
}

static CRUST_INLINE u32 crustAlignDownU32(u32 value, u32 alignment)
{
	CRUST_ASSERT(alignment != 0);
	CRUST_ASSERT(crustIsPow2U32(alignment) != 0);

	return value & ~(alignment - 1);
}

static CRUST_INLINE u64 crustAlignDownU64(u64 value, u64 alignment)
{
	CRUST_ASSERT(alignment != 0);
	CRUST_ASSERT(crustIsPow2U64(alignment) != 0);

	return value & ~(alignment - 1);
}

static CRUST_INLINE usize crustAlignDownUSize(usize value, usize alignment)
{
	CRUST_ASSERT(alignment != 0);
	CRUST_ASSERT(crustIsPow2USize(alignment) != 0);

	return value & ~(alignment - 1);
}

static CRUST_INLINE u8 crustAlignUpU8(u8 value, u8 alignment)
{
	CRUST_ASSERT(alignment != 0);
	CRUST_ASSERT(crustIsPow2U8(alignment) != 0);

	u8 mask = alignment - 1;
	return (value + mask) & ~mask;
}

static CRUST_INLINE u16 crustAlignUpU16(u16 value, u16 alignment)
{
	CRUST_ASSERT(alignment != 0);
	CRUST_ASSERT(crustIsPow2U16(alignment) != 0);

	u16 mask = alignment - 1;
	return (value + mask) & ~mask;
}

static CRUST_INLINE u32 crustAlignUpU32(u32 value, u32 alignment)
{
	CRUST_ASSERT(alignment != 0);
	CRUST_ASSERT(crustIsPow2U32(alignment) != 0);

	u32 mask = alignment - 1;
	return (value + mask) & ~mask;
}

static CRUST_INLINE u64 crustAlignUpU64(u64 value, u64 alignment)
{
	CRUST_ASSERT(alignment != 0);
	CRUST_ASSERT(crustIsPow2U64(alignment) != 0);

	u64 mask = alignment - 1;
	return (value + mask) & ~mask;
}

static CRUST_INLINE usize crustAlignUpUSize(usize value, usize alignment)
{
	CRUST_ASSERT(alignment != 0);
	CRUST_ASSERT(crustIsPow2USize(alignment) != 0);

	usize mask = alignment - 1;
	return (value + mask) & ~mask;
}

static CRUST_INLINE u8 crustMinU8(u8 a, u8 b)
{
	return (a < b) ? a : b;
}

static CRUST_INLINE u16 crustMinU16(u16 a, u16 b)
{
	return (a < b) ? a : b;
}

static CRUST_INLINE u32 crustMinU32(u32 a, u32 b)
{
	return (a < b) ? a : b;
}

static CRUST_INLINE u64 crustMinU64(u64 a, u64 b)
{
	return (a < b) ? a : b;
}

static CRUST_INLINE usize crustMinUSize(usize a, usize b)
{
	return (a < b) ? a : b;
}

static CRUST_INLINE i8 crustMinI8(i8 a, i8 b)
{
	return (a < b) ? a : b;
}

static CRUST_INLINE i16 crustMinI16(i16 a, i16 b)
{
	return (a < b) ? a : b;
}

static CRUST_INLINE i32 crustMinI32(i32 a, i32 b)
{
	return (a < b) ? a : b;
}

static CRUST_INLINE i64 crustMinI64(i64 a, i64 b)
{
	return (a < b) ? a : b;
}

static CRUST_INLINE isize crustMinISize(isize a, isize b)
{
	return (a < b) ? a : b;
}

static CRUST_INLINE f32 crustMinF32(f32 a, f32 b)
{
	return (a < b) ? a : b;
}

static CRUST_INLINE f64 crustMinF64(f64 a, f64 b)
{
	return (a < b) ? a : b;
}

static CRUST_INLINE u8 crustMaxU8(u8 a, u8 b)
{
	return (a < b) ? b : a;
}

static CRUST_INLINE u16 crustMaxU16(u16 a, u16 b)
{
	return (a < b) ? b : a;
}

static CRUST_INLINE u32 crustMaxU32(u32 a, u32 b)
{
	return (a < b) ? b : a;
}

static CRUST_INLINE u64 crustMaxU64(u64 a, u64 b)
{
	return (a < b) ? b : a;
}

static CRUST_INLINE usize crustMaxUSize(usize a, usize b)
{
	return (a < b) ? b : a;
}

static CRUST_INLINE i8 crustMaxI8(i8 a, i8 b)
{
	return (a < b) ? b : a;
}

static CRUST_INLINE i16 crustMaxI16(i16 a, i16 b)
{
	return (a < b) ? b : a;
}

static CRUST_INLINE i32 crustMaxI32(i32 a, i32 b)
{
	return (a < b) ? b : a;
}

static CRUST_INLINE i64 crustMaxI64(i64 a, i64 b)
{
	return (a < b) ? b : a;
}

static CRUST_INLINE isize crustMaxISize(isize a, isize b)
{
	return (a < b) ? b : a;
}

static CRUST_INLINE f32 crustMaxF32(f32 a, f32 b)
{
	return (a < b) ? b : a;
}

static CRUST_INLINE f64 crustMaxF64(f64 a, f64 b)
{
	return (a < b) ? b : a;
}

static CRUST_INLINE u8 crustClampU8(u8 v, u8 v_min, u8 v_max)
{
	return crustMaxU8(v_min, crustMinU8(v, v_max));
}

static CRUST_INLINE u16 crustClampU16(u16 v, u16 v_min, u16 v_max)
{
	return crustMaxU16(v_min, crustMinU16(v, v_max));
}

static CRUST_INLINE u32 crustClampU32(u32 v, u32 v_min, u32 v_max)
{
	return crustMaxU32(v_min, crustMinU32(v, v_max));
}

static CRUST_INLINE u64 crustClampU64(u64 v, u64 v_min, u64 v_max)
{
	return crustMaxU64(v_min, crustMinU64(v, v_max));
}

static CRUST_INLINE usize crustClampUSize(usize v, usize v_min, usize v_max)
{
	return crustMaxUSize(v_min, crustMinUSize(v, v_max));
}

static CRUST_INLINE i8 crustClampI8(i8 v, i8 v_min, i8 v_max)
{
	return crustMaxI8(v_min, crustMinI8(v, v_max));
}

static CRUST_INLINE i16 crustClampI16(i16 v, i16 v_min, i16 v_max)
{
	return crustMaxI16(v_min, crustMinI16(v, v_max));
}

static CRUST_INLINE i32 crustClampI32(i32 v, i32 v_min, i32 v_max)
{
	return crustMaxI32(v_min, crustMinI32(v, v_max));
}

static CRUST_INLINE i64 crustClampI64(i64 v, i64 v_min, i64 v_max)
{
	return crustMaxI64(v_min, crustMinI64(v, v_max));
}

static CRUST_INLINE isize crustClampISize(isize v, isize v_min, isize v_max)
{
	return crustMaxISize(v_min, crustMinISize(v, v_max));
}

static CRUST_INLINE f32 crustClampF32(f32 v, f32 v_min, f32 v_max)
{
	return crustMaxF32(v_min, crustMinF32(v, v_max));
}

static CRUST_INLINE f64 crustClampF64(f64 v, f64 v_min, f64 v_max)
{
	return crustMaxF64(v_min, crustMinF64(v, v_max));
}

static CRUST_INLINE u8 crustAbsI8(i8 v)
{
	return (v < 0) ? (u8)(-(i32)v) : (u8)v;
}

static CRUST_INLINE u16 crustAbsI16(i16 v)
{
	return (v < 0) ? (u16)(-(i32)v) : (u16)v;
}

static CRUST_INLINE u32 crustAbsI32(i32 v)
{
	return (v < 0) ? (u32)((u32)0 - (u32)v) : (u32)v;
}

static CRUST_INLINE u64 crustAbsI64(i64 v)
{
	return (v < 0) ? (u64)((u64)0 - (u64)v) : (u64)v;
}

static CRUST_INLINE usize crustAbsISize(isize v)
{
	return (v < 0) ? (usize)((usize)0 - (usize)v) : (usize)v;
}

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
	Crust_Quat result;
	result.x = a.x + (b.x - a.x) * t;
	result.y = a.y + (b.y - a.y) * t;
	result.z = a.z + (b.z - a.z) * t;
	result.w = a.w + (b.w - a.w) * t;

	return crustQuatNormalize(result);
}

static CRUST_INLINE Crust_Quat crustQuatSlerp(Crust_Quat a, Crust_Quat b, f32 t)
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

//
typedef struct Crust_Transform_t
{
	Crust_Vec3 position;
	Crust_Quat rotation;
	Crust_Vec3 scale;
} Crust_Transform;

static CRUST_INLINE Crust_Transform crustTransformInvert(Crust_Transform t)
{
	Crust_Transform result;
	result.scale.x = 1.0f / t.scale.x;
	result.scale.y = 1.0f / t.scale.y;
	result.scale.z = 1.0f / t.scale.z;
	result.rotation = crustQuatConjugate(t.rotation);
	result.position = crustQuatRotate(result.rotation, crustVec3MulV(result.scale, crustVec3MulS(t.position, -1.0f)));

	return result;
}

static CRUST_INLINE Crust_Transform crustTransformMul(Crust_Transform a, Crust_Transform b)
{
	Crust_Transform result;
	result.scale = crustVec3MulV(a.scale, b.scale);
	result.rotation = crustQuatMul(a.rotation, b.rotation);
	result.position = crustVec3AddV(a.position, crustQuatRotate(a.rotation, crustVec3MulV(a.scale, b.position)));

	return result;
}

//
typedef struct Crust_SpinLock_t
{
	volatile u32 counter;
} Crust_SpinLock;

CRUST_APIENTRY Crust_SpinLock crustSpinLockInit(void);
CRUST_APIENTRY void crustSpinLockAcquire(Crust_SpinLock *lock);
CRUST_APIENTRY u8 crustSpinLockTryAcquire(Crust_SpinLock *lock);
CRUST_APIENTRY void crustSpinLockRelease(Crust_SpinLock *lock);

//
typedef struct Crust_RwLock_t
{
	volatile u32 counter;
} Crust_RwLock;

CRUST_APIENTRY Crust_RwLock crustRwLockInit(void);
CRUST_APIENTRY void crustRwLockAcquireRead(Crust_RwLock *lock);
CRUST_APIENTRY void crustRwLockAcquireWrite(Crust_RwLock *lock);
CRUST_APIENTRY u8 crustRwLockTryAcquireRead(Crust_RwLock *lock);
CRUST_APIENTRY u8 crustRwLockTryAcquireWrite(Crust_RwLock *lock);
CRUST_APIENTRY void crustRwLockReleaseRead(Crust_RwLock *lock);
CRUST_APIENTRY void crustRwLockReleaseWrite(Crust_RwLock *lock);

//
typedef struct Crust_RingBuffer_t
{
	void *memory;
	usize capacity;
	usize size;
	usize read;
	usize write;
} Crust_RingBuffer;

CRUST_APIENTRY Crust_RingBuffer crustRingBufferAttach(void *memory, usize capacity);
CRUST_APIENTRY void crustRingBufferReset(Crust_RingBuffer *ring);
CRUST_APIENTRY void crustRingBufferRead(Crust_RingBuffer *ring, void *data, usize size);
CRUST_APIENTRY void crustRingBufferWrite(Crust_RingBuffer *ring, const void *data, usize size);

//
typedef struct Crust_BipBuffer_t
{
	void *memory;
	usize capacity;
	usize begin[2];
	usize end[2];
	usize read_staged;
	usize write_staged;
	usize reader;
	usize writer;
} Crust_BipBuffer;

CRUST_APIENTRY Crust_BipBuffer crustBipBufferAttach(void *memory, usize capacity);
CRUST_APIENTRY void crustBipBufferReset(Crust_BipBuffer *bip);
CRUST_APIENTRY const void *crustBipBufferStageRead(Crust_BipBuffer *bip, usize size);
CRUST_APIENTRY void crustBipBufferCommitRead(Crust_BipBuffer *bip, usize size);
CRUST_APIENTRY void *crustBipBufferStageWrite(Crust_BipBuffer *bip, usize size);
CRUST_APIENTRY void crustBipBufferCommitWrite(Crust_BipBuffer *bip, usize size);

//
typedef struct Crust_Stack_t
{
	void *memory;
	usize capacity;
	usize head;
} Crust_Stack;

CRUST_APIENTRY Crust_Stack crustStackAttach(void *memory, usize capacity);
CRUST_APIENTRY void crustStackReset(Crust_Stack *stack);
CRUST_APIENTRY void crustStackPush(Crust_Stack *stack, const void *data, usize size);
CRUST_APIENTRY void crustStackPeek(const Crust_Stack *stack, void *data, usize size);
CRUST_APIENTRY void crustStackPop(Crust_Stack *stack, void *data, usize size);

//
typedef struct Crust_Arena_t
{
	void *memory;
	usize capacity;
	usize size;
} Crust_Arena;

CRUST_APIENTRY Crust_Arena crustArenaAttach(void *memory, usize capacity);
CRUST_APIENTRY void crustArenaReset(Crust_Arena *arena);
CRUST_APIENTRY void *crustArenaAlloc(Crust_Arena *arena, usize size, usize alignment);

#if CRUST_COMPILER_MSVC
	#pragma warning(push)
	#pragma warning(disable: 4324)
#endif

//
typedef struct Crust_SpscRingBuffer_t
{
	void *memory;
	usize capacity;
	usize mask;

	CRUST_ALIGNAS(128) volatile usize read;
	CRUST_ALIGNAS(128) volatile usize write;
} Crust_SpscRingBuffer;

CRUST_APIENTRY Crust_SpscRingBuffer crustSpscRingBufferAttach(void *memory, usize capacity);
CRUST_APIENTRY void crustSpscRingBufferReset(Crust_SpscRingBuffer *ring);
CRUST_APIENTRY usize crustSpscRingBufferRead(Crust_SpscRingBuffer *ring, void *data, usize size);
CRUST_APIENTRY usize crustSpscRingBufferWrite(Crust_SpscRingBuffer *ring, const void *data, usize size);

//
typedef struct Crust_SpscBipBuffer_t
{
	void *memory;
	usize capacity;

	CRUST_ALIGNAS(128) volatile usize begin[2];
	volatile usize reader;
	usize read_staged;

	CRUST_ALIGNAS(128) volatile usize end[2];
	volatile usize writer;
	usize write_staged;
} Crust_SpscBipBuffer;

CRUST_APIENTRY Crust_SpscBipBuffer crustSpscBipBufferAttach(void *memory, usize capacity);
CRUST_APIENTRY void crustSpscBipBufferReset(Crust_SpscBipBuffer *bip);
CRUST_APIENTRY const void *crustSpscBipBufferStageRead(Crust_SpscBipBuffer *bip, usize size);
CRUST_APIENTRY void crustSpscBipBufferCommitRead(Crust_SpscBipBuffer *bip, usize size);
CRUST_APIENTRY void *crustSpscBipBufferStageWrite(Crust_SpscBipBuffer *bip, usize size);
CRUST_APIENTRY void crustSpscBipBufferCommitWrite(Crust_SpscBipBuffer *bip, usize size);

#if CRUST_COMPILER_MSVC
	#pragma warning(pop)
#endif

//
typedef void *(*PFN_crustAllocatorAlloc)(void *context, usize size, usize alignment);
typedef void *(*PFN_crustAllocatorRealloc)(void *context, void *ptr, usize old_size, usize new_size, usize alignment);
typedef void (*PFN_crustAllocatorFree)(void *context, void *ptr, usize size, usize alignment);

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

CRUST_APIENTRY void *crustAllocatorAlloc(Crust_Allocator allocator, usize size, usize alignment);
CRUST_APIENTRY void *crustAllocatorRealloc(Crust_Allocator allocator, void *ptr, usize old_size, usize new_size, usize alignment);
CRUST_APIENTRY void crustAllocatorFree(Crust_Allocator allocator, void *ptr, usize size, usize alignment);

static CRUST_INLINE usize crustAllocatorSizeMul(usize a, usize b)
{
	CRUST_ASSERT(a == 0 || b <= CRUST_USIZE_MAX / a);
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

CRUST_APIENTRY Crust_Allocator crustSystemAllocatorInit(void);
CRUST_APIENTRY Crust_Allocator crustArenaAllocatorInit(Crust_Arena *arena);

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

CRUST_APIENTRY Crust_HashIndexMap crustHashIndexMapInit(void *context, const Crust_HashIndexMapVtbl *vtbl, usize key_size, usize key_alignment);
CRUST_APIENTRY void crustHashIndexMapAlloc(Crust_Allocator allocator, Crust_HashIndexMap *hash_map, usize capacity);
CRUST_APIENTRY void crustHashIndexMapFree(Crust_Allocator allocator, Crust_HashIndexMap *hash_map);
CRUST_APIENTRY void crustHashIndexMapGrow(Crust_Allocator allocator, Crust_HashIndexMap *hash_map, usize new_capacity);
CRUST_APIENTRY usize crustHashIndexMapFind(const Crust_HashIndexMap *hash_map, const void *key, usize key_size);
CRUST_APIENTRY usize crustHashIndexMapAdd(Crust_HashIndexMap *hash_map, const void *key, usize key_size, usize index);
CRUST_APIENTRY usize crustHashIndexMapRemove(Crust_HashIndexMap *hash_map, const void *key, usize key_size);

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

CRUST_APIENTRY void crustGenerationalHandlePoolAlloc(Crust_Allocator allocator, Crust_GenerationalHandlePool *pool, usize capacity);
CRUST_APIENTRY void crustGenerationalHandlePoolFree(Crust_Allocator allocator, Crust_GenerationalHandlePool *pool);
CRUST_APIENTRY void crustGenerationalHandlePoolGrow(Crust_Allocator allocator, Crust_GenerationalHandlePool *pool, usize new_capacity);
CRUST_APIENTRY u32 crustGenerationalHandlePoolCheck(const Crust_GenerationalHandlePool *pool, Crust_GenerationalHandle handle);
CRUST_APIENTRY Crust_GenerationalHandle crustGenerationalHandlePoolAdd(Crust_GenerationalHandlePool *pool, usize max_generation);
CRUST_APIENTRY void crustGenerationalHandlePoolRemove(Crust_GenerationalHandlePool *pool, Crust_GenerationalHandle handle);
CRUST_APIENTRY Crust_GenerationalHandle crustGenerationalHandlePoolHead(const Crust_GenerationalHandlePool *pool);
CRUST_APIENTRY Crust_GenerationalHandle crustGenerationalHandlePoolTail(const Crust_GenerationalHandlePool *pool);
CRUST_APIENTRY Crust_GenerationalHandle crustGenerationalHandlePoolNext(const Crust_GenerationalHandlePool *pool, Crust_GenerationalHandle handle);
CRUST_APIENTRY Crust_GenerationalHandle crustGenerationalHandlePoolPrev(const Crust_GenerationalHandlePool *pool, Crust_GenerationalHandle handle);

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

CRUST_APIENTRY void crustHeapNodePoolAlloc(Crust_Allocator allocator, Crust_HeapNodePool *pool, u32 capacity);
CRUST_APIENTRY void crustHeapNodePoolFree(Crust_Allocator allocator, Crust_HeapNodePool *pool);

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

CRUST_APIENTRY Crust_Heap crustHeapInit(Crust_HeapNodePool *pool, u32 size);
CRUST_APIENTRY Crust_HeapAllocation crustHeapStageAlloc(const Crust_Heap *heap, u32 size);
CRUST_APIENTRY Crust_HeapAllocation crustHeapStageAllocAligned(const Crust_Heap *heap, u32 size, u32 alignment);
CRUST_APIENTRY void crustHeapCommitAlloc(Crust_Heap *heap, Crust_HeapAllocation allocation, u32 size);
CRUST_APIENTRY Crust_HeapAllocation crustHeapAlloc(Crust_Heap *heap, u32 size);
CRUST_APIENTRY Crust_HeapAllocation crustHeapAllocAligned(Crust_Heap *heap, u32 size, u32 alignment);
CRUST_APIENTRY void crustHeapFree(Crust_Heap *heap, Crust_HeapAllocation allocation);

#ifdef __cplusplus
}
#endif
