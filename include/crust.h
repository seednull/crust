#pragma once

// Version
#define CRUST_VERSION_MAJOR 1
#define CRUST_VERSION_MINOR 0
#define CRUST_VERSION_PATCH 0
#define CRUST_VERSION "1.0.0"

// Compiler
#define CRUST_COMPILER_MSVC 0
#define CRUST_COMPILER_GCC 0
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
#define CRUST_TOOLCHAIN_MSVC 0
#define CRUST_TOOLCHAIN_MINGW 0
#define CRUST_TOOLCHAIN_ANDROID 0
#define CRUST_TOOLCHAIN_EMSCRIPTEN 0
#define CRUST_TOOLCHAIN_WASI 0
#define CRUST_TOOLCHAIN_APPLE 0

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
#define CRUST_ARCH_X86 0
#define CRUST_ARCH_X64 0
#define CRUST_ARCH_ARM 0
#define CRUST_ARCH_ARM64 0
#define CRUST_ARCH_WASM32 0
#define CRUST_ARCH_WASM64 0

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
#elif defined(__arm__)
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

// SIMD
#define CRUST_SIMD_SSE 0
#define CRUST_SIMD_NEON 0
#define CRUST_SIMD_WASM 0
#define CRUST_SIMD_SCALAR 0

#if !defined(CRUST_USE_SIMD)
	#define CRUST_USE_SIMD 0
#endif

#if !CRUST_USE_SIMD
	#undef  CRUST_SIMD_SCALAR
	#define CRUST_SIMD_SCALAR 1
#elif CRUST_ARCH_X86 || CRUST_ARCH_X64
	#if !CRUST_COMPILER_MSVC && (!defined(__SSE4_2__) || !defined(__POPCNT__))
		#error "CRUST_USE_SIMD requires SSE4.2 and POPCNT on x86: compile with -msse4.2 -mpopcnt"
	#endif
	#undef  CRUST_SIMD_SSE
	#define CRUST_SIMD_SSE 1
#elif CRUST_ARCH_ARM64
	#undef  CRUST_SIMD_NEON
	#define CRUST_SIMD_NEON 1
#elif CRUST_ARCH_WASM32 || CRUST_ARCH_WASM64
	#if !defined(__wasm_simd128__)
		#error "CRUST_USE_SIMD requires WebAssembly SIMD: compile with -msimd128"
	#endif
	#undef  CRUST_SIMD_WASM
	#define CRUST_SIMD_WASM 1
#else
	#undef  CRUST_SIMD_SCALAR
	#define CRUST_SIMD_SCALAR 1
#endif

// Includes
#if CRUST_COMPILER_MSVC
	#include <intrin0.h>
#endif

#if CRUST_SIMD_SSE || (CRUST_COMPILER_MSVC && (CRUST_ARCH_X86 || CRUST_ARCH_X64))
	#if !defined(_INC_MALLOC) && !defined(_MM_MALLOC_H_INCLUDED) && !defined(__MM_MALLOC_H)
		#define _INC_MALLOC
		#define _MM_MALLOC_H_INCLUDED
		#define __MM_MALLOC_H
		#define CRUST_SKIPPED_MM_MALLOC
	#endif

	#include <nmmintrin.h>

	#if defined(CRUST_SKIPPED_MM_MALLOC)
		#undef _INC_MALLOC
		#undef _MM_MALLOC_H_INCLUDED
		#undef __MM_MALLOC_H
		#undef CRUST_SKIPPED_MM_MALLOC
	#endif
#elif CRUST_SIMD_NEON || (CRUST_COMPILER_MSVC && CRUST_ARCH_ARM64)
	#include <arm_neon.h>
#elif CRUST_SIMD_WASM
	#include <wasm_simd128.h>
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
#if CRUST_COMPILER_MSVC
	typedef signed char				i8;
	typedef signed short			i16;
	typedef signed int				i32;
	typedef signed long long		i64;

	typedef unsigned char			u8;
	typedef unsigned short			u16;
	typedef unsigned int			u32;
	typedef unsigned long long		u64;

	#if CRUST_ARCH_64BIT
		typedef i64					isize;
		typedef u64					usize;
	#else
		typedef i32					isize;
		typedef u32					usize;
	#endif
#else
	typedef __INT8_TYPE__			i8;
	typedef __INT16_TYPE__			i16;
	typedef __INT32_TYPE__			i32;
	typedef __INT64_TYPE__			i64;

	typedef __UINT8_TYPE__			u8;
	typedef __UINT16_TYPE__			u16;
	typedef __UINT32_TYPE__			u32;
	typedef __UINT64_TYPE__			u64;

	typedef __PTRDIFF_TYPE__		isize;
	typedef __SIZE_TYPE__			usize;
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

#define CRUST_PI					3.14159265358979323846
#define CRUST_TAU					6.28318530717958647693

#ifdef __cplusplus
extern "C" {
#endif

//
static CRUST_INLINE void crustAssertFailure(const char *expression, const char *file, u32 line)
{
	CRUST_UNUSED(expression);
	CRUST_UNUSED(file);
	CRUST_UNUSED(line);

#if CRUST_COMPILER_MSVC
	__debugbreak();
#elif CRUST_COMPILER_CLANG
	__builtin_debugtrap();
#else
	__builtin_trap();
#endif
}

//
CRUST_APIENTRY void crustMemcpy(void *dst, const void *src, usize size);
CRUST_APIENTRY void crustMemset(void *dst, u8 value, usize size);

//
CRUST_APIENTRY f32 crustCosF32(f32 v);
CRUST_APIENTRY f32 crustAcosF32(f32 v);
CRUST_APIENTRY f32 crustSinF32(f32 v);
CRUST_APIENTRY f32 crustAsinF32(f32 v);
CRUST_APIENTRY f32 crustTanF32(f32 v);
CRUST_APIENTRY f32 crustAtan2F32(f32 y, f32 x);

CRUST_APIENTRY f64 crustCosF64(f64 v);
CRUST_APIENTRY f64 crustAcosF64(f64 v);
CRUST_APIENTRY f64 crustSinF64(f64 v);
CRUST_APIENTRY f64 crustAsinF64(f64 v);
CRUST_APIENTRY f64 crustTanF64(f64 v);
CRUST_APIENTRY f64 crustAtan2F64(f64 y, f64 x);

//
static CRUST_INLINE u8 crustMinU8(u8 a, u8 b) { return (a < b) ? a : b; }
static CRUST_INLINE u16 crustMinU16(u16 a, u16 b) { return (a < b) ? a : b; }
static CRUST_INLINE u32 crustMinU32(u32 a, u32 b) { return (a < b) ? a : b; }
static CRUST_INLINE u64 crustMinU64(u64 a, u64 b) { return (a < b) ? a : b; }
static CRUST_INLINE usize crustMinUSize(usize a, usize b) { return (a < b) ? a : b; }
static CRUST_INLINE i8 crustMinI8(i8 a, i8 b) { return (a < b) ? a : b; }
static CRUST_INLINE i16 crustMinI16(i16 a, i16 b) { return (a < b) ? a : b; }
static CRUST_INLINE i32 crustMinI32(i32 a, i32 b) { return (a < b) ? a : b; }
static CRUST_INLINE i64 crustMinI64(i64 a, i64 b) { return (a < b) ? a : b; }
static CRUST_INLINE isize crustMinISize(isize a, isize b) { return (a < b) ? a : b; }
static CRUST_INLINE f32 crustMinF32(f32 a, f32 b) { return (a < b) ? a : b; }
static CRUST_INLINE f64 crustMinF64(f64 a, f64 b) { return (a < b) ? a : b; }

static CRUST_INLINE u8 crustMaxU8(u8 a, u8 b) { return (a < b) ? b : a; }
static CRUST_INLINE u16 crustMaxU16(u16 a, u16 b) { return (a < b) ? b : a; }
static CRUST_INLINE u32 crustMaxU32(u32 a, u32 b) { return (a < b) ? b : a; }
static CRUST_INLINE u64 crustMaxU64(u64 a, u64 b) { return (a < b) ? b : a; }
static CRUST_INLINE usize crustMaxUSize(usize a, usize b) { return (a < b) ? b : a; }
static CRUST_INLINE i8 crustMaxI8(i8 a, i8 b) { return (a < b) ? b : a; }
static CRUST_INLINE i16 crustMaxI16(i16 a, i16 b) { return (a < b) ? b : a; }
static CRUST_INLINE i32 crustMaxI32(i32 a, i32 b) { return (a < b) ? b : a; }
static CRUST_INLINE i64 crustMaxI64(i64 a, i64 b) { return (a < b) ? b : a; }
static CRUST_INLINE isize crustMaxISize(isize a, isize b) { return (a < b) ? b : a; }
static CRUST_INLINE f32 crustMaxF32(f32 a, f32 b) { return (a < b) ? b : a; }
static CRUST_INLINE f64 crustMaxF64(f64 a, f64 b) { return (a < b) ? b : a; }

static CRUST_INLINE u8 crustClampU8(u8 v, u8 v_min, u8 v_max) { return crustMaxU8(v_min, crustMinU8(v, v_max)); }
static CRUST_INLINE u16 crustClampU16(u16 v, u16 v_min, u16 v_max) { return crustMaxU16(v_min, crustMinU16(v, v_max)); }
static CRUST_INLINE u32 crustClampU32(u32 v, u32 v_min, u32 v_max) { return crustMaxU32(v_min, crustMinU32(v, v_max)); }
static CRUST_INLINE u64 crustClampU64(u64 v, u64 v_min, u64 v_max) { return crustMaxU64(v_min, crustMinU64(v, v_max)); }
static CRUST_INLINE usize crustClampUSize(usize v, usize v_min, usize v_max) { return crustMaxUSize(v_min, crustMinUSize(v, v_max)); }
static CRUST_INLINE i8 crustClampI8(i8 v, i8 v_min, i8 v_max) { return crustMaxI8(v_min, crustMinI8(v, v_max)); }
static CRUST_INLINE i16 crustClampI16(i16 v, i16 v_min, i16 v_max) { return crustMaxI16(v_min, crustMinI16(v, v_max)); }
static CRUST_INLINE i32 crustClampI32(i32 v, i32 v_min, i32 v_max) { return crustMaxI32(v_min, crustMinI32(v, v_max)); }
static CRUST_INLINE i64 crustClampI64(i64 v, i64 v_min, i64 v_max) { return crustMaxI64(v_min, crustMinI64(v, v_max)); }
static CRUST_INLINE isize crustClampISize(isize v, isize v_min, isize v_max) { return crustMaxISize(v_min, crustMinISize(v, v_max)); }
static CRUST_INLINE f32 crustClampF32(f32 v, f32 v_min, f32 v_max) { return crustMaxF32(v_min, crustMinF32(v, v_max)); }
static CRUST_INLINE f64 crustClampF64(f64 v, f64 v_min, f64 v_max) { return crustMaxF64(v_min, crustMinF64(v, v_max)); }

static CRUST_INLINE u8 crustAbsI8(i8 v) { return (v < 0) ? (u8)(-(i32)v) : (u8)v; }
static CRUST_INLINE u16 crustAbsI16(i16 v) { return (v < 0) ? (u16)(-(i32)v) : (u16)v; }
static CRUST_INLINE u32 crustAbsI32(i32 v) { return (v < 0) ? (u32)((u32)0 - (u32)v) : (u32)v; }
static CRUST_INLINE u64 crustAbsI64(i64 v) { return (v < 0) ? (u64)((u64)0 - (u64)v) : (u64)v; }
static CRUST_INLINE usize crustAbsISize(isize v) { return (v < 0) ? (usize)((usize)0 - (usize)v) : (usize)v; }

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

//
static CRUST_INLINE f32 crustAbsF32(f32 v);
static CRUST_INLINE f64 crustAbsF64(f64 v);
static CRUST_INLINE f32 crustSqrtF32(f32 v);
static CRUST_INLINE f64 crustSqrtF64(f64 v);
static CRUST_INLINE f32 crustRsqrtF32(f32 v);
static CRUST_INLINE f64 crustRsqrtF64(f64 v);
static CRUST_INLINE f32 crustFloorF32(f32 v);
static CRUST_INLINE f64 crustFloorF64(f64 v);
static CRUST_INLINE f32 crustCeilF32(f32 v);
static CRUST_INLINE f64 crustCeilF64(f64 v);
static CRUST_INLINE f32 crustRoundEvenF32(f32 v);
static CRUST_INLINE f64 crustRoundEvenF64(f64 v);
static CRUST_INLINE f32 crustTruncateF32(f32 v);
static CRUST_INLINE f64 crustTruncateF64(f64 v);

//
typedef enum Crust_MemoryOrder_t
{
	CRUST_MEMORY_ORDER_RELAXED = 0,
	CRUST_MEMORY_ORDER_CONSUME = 1,
	CRUST_MEMORY_ORDER_ACQUIRE = 2,
	CRUST_MEMORY_ORDER_RELEASE = 3,
	CRUST_MEMORY_ORDER_ACQ_REL = 4,
	CRUST_MEMORY_ORDER_SEQ_CST = 5,
} Crust_MemoryOrder;

static CRUST_INLINE u32 crustAtomicSwapU32(volatile u32 *p, u32 v, Crust_MemoryOrder order);
static CRUST_INLINE u32 crustAtomicCompareAndSwapU32(volatile u32 *p, u32 desired, u32 expected, Crust_MemoryOrder success_order, Crust_MemoryOrder failure_order);
static CRUST_INLINE u32 crustAtomicLoadU32(volatile u32 *p, Crust_MemoryOrder order);
static CRUST_INLINE void crustAtomicStoreU32(volatile u32 *p, u32 v, Crust_MemoryOrder order);
static CRUST_INLINE u32 crustAtomicIncrementU32(volatile u32 *p, Crust_MemoryOrder order);
static CRUST_INLINE u32 crustAtomicDecrementU32(volatile u32 *p, Crust_MemoryOrder order);

static CRUST_INLINE u64 crustAtomicSwapU64(volatile u64 *p, u64 v, Crust_MemoryOrder order);
static CRUST_INLINE u64 crustAtomicCompareAndSwapU64(volatile u64 *p, u64 desired, u64 expected, Crust_MemoryOrder success_order, Crust_MemoryOrder failure_order);
static CRUST_INLINE u64 crustAtomicLoadU64(volatile u64 *p, Crust_MemoryOrder order);
static CRUST_INLINE void crustAtomicStoreU64(volatile u64 *p, u64 v, Crust_MemoryOrder order);
static CRUST_INLINE u64 crustAtomicIncrementU64(volatile u64 *p, Crust_MemoryOrder order);
static CRUST_INLINE u64 crustAtomicDecrementU64(volatile u64 *p, Crust_MemoryOrder order);

static CRUST_INLINE usize crustAtomicSwapUSize(volatile usize *p, usize v, Crust_MemoryOrder order);
static CRUST_INLINE usize crustAtomicCompareAndSwapUSize(volatile usize *p, usize desired, usize expected, Crust_MemoryOrder success_order, Crust_MemoryOrder failure_order);
static CRUST_INLINE usize crustAtomicLoadUSize(volatile usize *p, Crust_MemoryOrder order);
static CRUST_INLINE void crustAtomicStoreUSize(volatile usize *p, usize v, Crust_MemoryOrder order);
static CRUST_INLINE usize crustAtomicIncrementUSize(volatile usize *p, Crust_MemoryOrder order);
static CRUST_INLINE usize crustAtomicDecrementUSize(volatile usize *p, Crust_MemoryOrder order);

//
static CRUST_INLINE void crustCpuRelax(void);

//
static CRUST_INLINE u32 crustLzcntU32(u32 value);
static CRUST_INLINE u32 crustTzcntU32(u32 value);
static CRUST_INLINE u32 crustPopcntU32(u32 value);

//
#if CRUST_SIMD_SSE
	typedef __m128i Crust_Simd128;
#elif CRUST_SIMD_NEON
	typedef uint8x16_t Crust_Simd128;
#elif CRUST_SIMD_WASM
	typedef v128_t Crust_Simd128;
#else
	typedef struct Crust_Simd128_t
	{
		CRUST_ALIGNAS(16) u8 bytes[16];
	} Crust_Simd128;
#endif

static CRUST_INLINE Crust_Simd128 crustSimd128AllZeros(void);
static CRUST_INLINE Crust_Simd128 crustSimd128AllOnes(void);

static CRUST_INLINE Crust_Simd128 crustSimd128LoadU8(const u8 *ptr);
static CRUST_INLINE Crust_Simd128 crustSimd128LoadI8(const i8 *ptr);
static CRUST_INLINE Crust_Simd128 crustSimd128LoadU16(const u16 *ptr);
static CRUST_INLINE Crust_Simd128 crustSimd128LoadI16(const i16 *ptr);
static CRUST_INLINE Crust_Simd128 crustSimd128LoadU32(const u32 *ptr);
static CRUST_INLINE Crust_Simd128 crustSimd128LoadI32(const i32 *ptr);
static CRUST_INLINE Crust_Simd128 crustSimd128LoadU64(const u64 *ptr);
static CRUST_INLINE Crust_Simd128 crustSimd128LoadI64(const i64 *ptr);
static CRUST_INLINE Crust_Simd128 crustSimd128LoadF32(const f32 *ptr);
static CRUST_INLINE Crust_Simd128 crustSimd128LoadF64(const f64 *ptr);

static CRUST_INLINE Crust_Simd128 crustSimd128LoadAlignedU8(const u8 *ptr);
static CRUST_INLINE Crust_Simd128 crustSimd128LoadAlignedI8(const i8 *ptr);
static CRUST_INLINE Crust_Simd128 crustSimd128LoadAlignedU16(const u16 *ptr);
static CRUST_INLINE Crust_Simd128 crustSimd128LoadAlignedI16(const i16 *ptr);
static CRUST_INLINE Crust_Simd128 crustSimd128LoadAlignedU32(const u32 *ptr);
static CRUST_INLINE Crust_Simd128 crustSimd128LoadAlignedI32(const i32 *ptr);
static CRUST_INLINE Crust_Simd128 crustSimd128LoadAlignedU64(const u64 *ptr);
static CRUST_INLINE Crust_Simd128 crustSimd128LoadAlignedI64(const i64 *ptr);
static CRUST_INLINE Crust_Simd128 crustSimd128LoadAlignedF32(const f32 *ptr);
static CRUST_INLINE Crust_Simd128 crustSimd128LoadAlignedF64(const f64 *ptr);

static CRUST_INLINE void crustSimd128StoreU8(u8 *ptr, Crust_Simd128 v);
static CRUST_INLINE void crustSimd128StoreI8(i8 *ptr, Crust_Simd128 v);
static CRUST_INLINE void crustSimd128StoreU16(u16 *ptr, Crust_Simd128 v);
static CRUST_INLINE void crustSimd128StoreI16(i16 *ptr, Crust_Simd128 v);
static CRUST_INLINE void crustSimd128StoreU32(u32 *ptr, Crust_Simd128 v);
static CRUST_INLINE void crustSimd128StoreI32(i32 *ptr, Crust_Simd128 v);
static CRUST_INLINE void crustSimd128StoreU64(u64 *ptr, Crust_Simd128 v);
static CRUST_INLINE void crustSimd128StoreI64(i64 *ptr, Crust_Simd128 v);
static CRUST_INLINE void crustSimd128StoreF32(f32 *ptr, Crust_Simd128 v);
static CRUST_INLINE void crustSimd128StoreF64(f64 *ptr, Crust_Simd128 v);

static CRUST_INLINE void crustSimd128StoreAlignedU8(u8 *ptr, Crust_Simd128 v);
static CRUST_INLINE void crustSimd128StoreAlignedI8(i8 *ptr, Crust_Simd128 v);
static CRUST_INLINE void crustSimd128StoreAlignedU16(u16 *ptr, Crust_Simd128 v);
static CRUST_INLINE void crustSimd128StoreAlignedI16(i16 *ptr, Crust_Simd128 v);
static CRUST_INLINE void crustSimd128StoreAlignedU32(u32 *ptr, Crust_Simd128 v);
static CRUST_INLINE void crustSimd128StoreAlignedI32(i32 *ptr, Crust_Simd128 v);
static CRUST_INLINE void crustSimd128StoreAlignedU64(u64 *ptr, Crust_Simd128 v);
static CRUST_INLINE void crustSimd128StoreAlignedI64(i64 *ptr, Crust_Simd128 v);
static CRUST_INLINE void crustSimd128StoreAlignedF32(f32 *ptr, Crust_Simd128 v);
static CRUST_INLINE void crustSimd128StoreAlignedF64(f64 *ptr, Crust_Simd128 v);

static CRUST_INLINE Crust_Simd128 crustSimd128SplatU8(u8 v);
static CRUST_INLINE Crust_Simd128 crustSimd128SplatI8(i8 v);
static CRUST_INLINE Crust_Simd128 crustSimd128SplatU16(u16 v);
static CRUST_INLINE Crust_Simd128 crustSimd128SplatI16(i16 v);
static CRUST_INLINE Crust_Simd128 crustSimd128SplatU32(u32 v);
static CRUST_INLINE Crust_Simd128 crustSimd128SplatI32(i32 v);
static CRUST_INLINE Crust_Simd128 crustSimd128SplatU64(u64 v);
static CRUST_INLINE Crust_Simd128 crustSimd128SplatI64(i64 v);
static CRUST_INLINE Crust_Simd128 crustSimd128SplatF32(f32 v);
static CRUST_INLINE Crust_Simd128 crustSimd128SplatF64(f64 v);

static CRUST_INLINE Crust_Simd128 crustSimd128MakeU32(u32 v0, u32 v1, u32 v2, u32 v3);
static CRUST_INLINE Crust_Simd128 crustSimd128MakeI32(i32 v0, i32 v1, i32 v2, i32 v3);
static CRUST_INLINE Crust_Simd128 crustSimd128MakeF32(f32 v0, f32 v1, f32 v2, f32 v3);
static CRUST_INLINE Crust_Simd128 crustSimd128MakeU64(u64 v0, u64 v1);
static CRUST_INLINE Crust_Simd128 crustSimd128MakeI64(i64 v0, i64 v1);
static CRUST_INLINE Crust_Simd128 crustSimd128MakeF64(f64 v0, f64 v1);

static CRUST_INLINE u8 crustSimd128GetLaneU8(Crust_Simd128 v, u32 lane);
static CRUST_INLINE i8 crustSimd128GetLaneI8(Crust_Simd128 v, u32 lane);
static CRUST_INLINE u16 crustSimd128GetLaneU16(Crust_Simd128 v, u32 lane);
static CRUST_INLINE i16 crustSimd128GetLaneI16(Crust_Simd128 v, u32 lane);
static CRUST_INLINE u32 crustSimd128GetLaneU32(Crust_Simd128 v, u32 lane);
static CRUST_INLINE i32 crustSimd128GetLaneI32(Crust_Simd128 v, u32 lane);
static CRUST_INLINE u64 crustSimd128GetLaneU64(Crust_Simd128 v, u32 lane);
static CRUST_INLINE i64 crustSimd128GetLaneI64(Crust_Simd128 v, u32 lane);
static CRUST_INLINE f32 crustSimd128GetLaneF32(Crust_Simd128 v, u32 lane);
static CRUST_INLINE f64 crustSimd128GetLaneF64(Crust_Simd128 v, u32 lane);

static CRUST_INLINE Crust_Simd128 crustSimd128SetLaneU8(Crust_Simd128 v, u32 lane, u8 value);
static CRUST_INLINE Crust_Simd128 crustSimd128SetLaneI8(Crust_Simd128 v, u32 lane, i8 value);
static CRUST_INLINE Crust_Simd128 crustSimd128SetLaneU16(Crust_Simd128 v, u32 lane, u16 value);
static CRUST_INLINE Crust_Simd128 crustSimd128SetLaneI16(Crust_Simd128 v, u32 lane, i16 value);
static CRUST_INLINE Crust_Simd128 crustSimd128SetLaneU32(Crust_Simd128 v, u32 lane, u32 value);
static CRUST_INLINE Crust_Simd128 crustSimd128SetLaneI32(Crust_Simd128 v, u32 lane, i32 value);
static CRUST_INLINE Crust_Simd128 crustSimd128SetLaneU64(Crust_Simd128 v, u32 lane, u64 value);
static CRUST_INLINE Crust_Simd128 crustSimd128SetLaneI64(Crust_Simd128 v, u32 lane, i64 value);
static CRUST_INLINE Crust_Simd128 crustSimd128SetLaneF32(Crust_Simd128 v, u32 lane, f32 value);
static CRUST_INLINE Crust_Simd128 crustSimd128SetLaneF64(Crust_Simd128 v, u32 lane, f64 value);

static CRUST_INLINE Crust_Simd128 crustSimd128ZipLowU8(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128ZipLowI8(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128ZipLowU16(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128ZipLowI16(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128ZipLowU32(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128ZipLowI32(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128ZipLowU64(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128ZipLowI64(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128ZipLowF32(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128ZipLowF64(Crust_Simd128 a, Crust_Simd128 b);

static CRUST_INLINE Crust_Simd128 crustSimd128ZipHighU8(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128ZipHighI8(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128ZipHighU16(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128ZipHighI16(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128ZipHighU32(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128ZipHighI32(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128ZipHighU64(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128ZipHighI64(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128ZipHighF32(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128ZipHighF64(Crust_Simd128 a, Crust_Simd128 b);

static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32XYWZ(Crust_Simd128 v);
static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32XZYW(Crust_Simd128 v);
static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32XZWY(Crust_Simd128 v);
static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32XWYZ(Crust_Simd128 v);
static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32XWZY(Crust_Simd128 v);
static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32YXZW(Crust_Simd128 v);
static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32YXWZ(Crust_Simd128 v);
static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32YZXW(Crust_Simd128 v);
static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32YZWX(Crust_Simd128 v);
static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32YWXZ(Crust_Simd128 v);
static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32YWZX(Crust_Simd128 v);
static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32ZXYW(Crust_Simd128 v);
static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32ZXWY(Crust_Simd128 v);
static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32ZYXW(Crust_Simd128 v);
static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32ZYWX(Crust_Simd128 v);
static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32ZWXY(Crust_Simd128 v);
static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32ZWYX(Crust_Simd128 v);
static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32WXYZ(Crust_Simd128 v);
static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32WXZY(Crust_Simd128 v);
static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32WYXZ(Crust_Simd128 v);
static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32WYZX(Crust_Simd128 v);
static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32WZXY(Crust_Simd128 v);
static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32WZYX(Crust_Simd128 v);
static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32XXXX(Crust_Simd128 v);
static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32YYYY(Crust_Simd128 v);
static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32ZZZZ(Crust_Simd128 v);
static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle32WWWW(Crust_Simd128 v);

static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle64YX(Crust_Simd128 v);
static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle64XX(Crust_Simd128 v);
static CRUST_INLINE Crust_Simd128 crustSimd128Swizzle64YY(Crust_Simd128 v);

static CRUST_INLINE Crust_Simd128 crustSimd128And(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128Or(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128Xor(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128AndNot(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128Not(Crust_Simd128 v);
static CRUST_INLINE Crust_Simd128 crustSimd128Select(Crust_Simd128 mask, Crust_Simd128 a, Crust_Simd128 b);

static CRUST_INLINE u32 crustSimd128Any(Crust_Simd128 v);
static CRUST_INLINE u32 crustSimd128All(Crust_Simd128 v);
static CRUST_INLINE u32 crustSimd128BitmaskU8(Crust_Simd128 v);
static CRUST_INLINE u32 crustSimd128BitmaskU16(Crust_Simd128 v);
static CRUST_INLINE u32 crustSimd128BitmaskU32(Crust_Simd128 v);
static CRUST_INLINE u32 crustSimd128BitmaskU64(Crust_Simd128 v);

static CRUST_INLINE Crust_Simd128 crustSimd128AddU8(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128AddI8(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128AddU16(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128AddI16(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128AddU32(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128AddI32(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128AddU64(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128AddI64(Crust_Simd128 a, Crust_Simd128 b);

static CRUST_INLINE Crust_Simd128 crustSimd128SubU8(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128SubI8(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128SubU16(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128SubI16(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128SubU32(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128SubI32(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128SubU64(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128SubI64(Crust_Simd128 a, Crust_Simd128 b);

static CRUST_INLINE Crust_Simd128 crustSimd128AddSaturateU8(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128AddSaturateI8(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128AddSaturateU16(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128AddSaturateI16(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128SubSaturateU8(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128SubSaturateI8(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128SubSaturateU16(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128SubSaturateI16(Crust_Simd128 a, Crust_Simd128 b);

static CRUST_INLINE Crust_Simd128 crustSimd128MulU16(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128MulI16(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128MulU32(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128MulI32(Crust_Simd128 a, Crust_Simd128 b);

static CRUST_INLINE Crust_Simd128 crustSimd128MinU8(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128MinI8(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128MinU16(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128MinI16(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128MinU32(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128MinI32(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128MinU64(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128MinI64(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128MaxU8(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128MaxI8(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128MaxU16(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128MaxI16(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128MaxU32(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128MaxI32(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128MaxU64(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128MaxI64(Crust_Simd128 a, Crust_Simd128 b);

static CRUST_INLINE Crust_Simd128 crustSimd128ShiftLeftU8(Crust_Simd128 v, u32 count);
static CRUST_INLINE Crust_Simd128 crustSimd128ShiftLeftI8(Crust_Simd128 v, u32 count);
static CRUST_INLINE Crust_Simd128 crustSimd128ShiftLeftU16(Crust_Simd128 v, u32 count);
static CRUST_INLINE Crust_Simd128 crustSimd128ShiftLeftI16(Crust_Simd128 v, u32 count);
static CRUST_INLINE Crust_Simd128 crustSimd128ShiftLeftU32(Crust_Simd128 v, u32 count);
static CRUST_INLINE Crust_Simd128 crustSimd128ShiftLeftI32(Crust_Simd128 v, u32 count);
static CRUST_INLINE Crust_Simd128 crustSimd128ShiftLeftU64(Crust_Simd128 v, u32 count);
static CRUST_INLINE Crust_Simd128 crustSimd128ShiftLeftI64(Crust_Simd128 v, u32 count);
static CRUST_INLINE Crust_Simd128 crustSimd128ShiftRightU8(Crust_Simd128 v, u32 count);
static CRUST_INLINE Crust_Simd128 crustSimd128ShiftRightI8(Crust_Simd128 v, u32 count);
static CRUST_INLINE Crust_Simd128 crustSimd128ShiftRightU16(Crust_Simd128 v, u32 count);
static CRUST_INLINE Crust_Simd128 crustSimd128ShiftRightI16(Crust_Simd128 v, u32 count);
static CRUST_INLINE Crust_Simd128 crustSimd128ShiftRightU32(Crust_Simd128 v, u32 count);
static CRUST_INLINE Crust_Simd128 crustSimd128ShiftRightI32(Crust_Simd128 v, u32 count);
static CRUST_INLINE Crust_Simd128 crustSimd128ShiftRightU64(Crust_Simd128 v, u32 count);
static CRUST_INLINE Crust_Simd128 crustSimd128ShiftRightI64(Crust_Simd128 v, u32 count);

static CRUST_INLINE Crust_Simd128 crustSimd128EqualU8(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128EqualI8(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128EqualU16(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128EqualI16(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128EqualU32(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128EqualI32(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128EqualU64(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128EqualI64(Crust_Simd128 a, Crust_Simd128 b);

static CRUST_INLINE Crust_Simd128 crustSimd128NotEqualU8(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128NotEqualI8(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128NotEqualU16(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128NotEqualI16(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128NotEqualU32(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128NotEqualI32(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128NotEqualU64(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128NotEqualI64(Crust_Simd128 a, Crust_Simd128 b);

static CRUST_INLINE Crust_Simd128 crustSimd128LessU8(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128LessI8(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128LessU16(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128LessI16(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128LessU32(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128LessI32(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128LessU64(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128LessI64(Crust_Simd128 a, Crust_Simd128 b);

static CRUST_INLINE Crust_Simd128 crustSimd128LessEqualU8(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128LessEqualI8(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128LessEqualU16(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128LessEqualI16(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128LessEqualU32(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128LessEqualI32(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128LessEqualU64(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128LessEqualI64(Crust_Simd128 a, Crust_Simd128 b);

static CRUST_INLINE Crust_Simd128 crustSimd128GreaterU8(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128GreaterI8(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128GreaterU16(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128GreaterI16(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128GreaterU32(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128GreaterI32(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128GreaterU64(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128GreaterI64(Crust_Simd128 a, Crust_Simd128 b);

static CRUST_INLINE Crust_Simd128 crustSimd128GreaterEqualU8(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128GreaterEqualI8(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128GreaterEqualU16(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128GreaterEqualI16(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128GreaterEqualU32(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128GreaterEqualI32(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128GreaterEqualU64(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128GreaterEqualI64(Crust_Simd128 a, Crust_Simd128 b);

static CRUST_INLINE Crust_Simd128 crustSimd128AddF32(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128SubF32(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128MulF32(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128DivF32(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128SqrtF32(Crust_Simd128 v);
static CRUST_INLINE Crust_Simd128 crustSimd128AbsF32(Crust_Simd128 v);
static CRUST_INLINE Crust_Simd128 crustSimd128NegateF32(Crust_Simd128 v);
static CRUST_INLINE Crust_Simd128 crustSimd128MinF32(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128MaxF32(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128EqualF32(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128NotEqualF32(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128LessF32(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128LessEqualF32(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128GreaterF32(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128GreaterEqualF32(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128FloorF32(Crust_Simd128 v);
static CRUST_INLINE Crust_Simd128 crustSimd128CeilF32(Crust_Simd128 v);
static CRUST_INLINE Crust_Simd128 crustSimd128RoundEvenF32(Crust_Simd128 v);
static CRUST_INLINE Crust_Simd128 crustSimd128TruncateF32(Crust_Simd128 v);

static CRUST_INLINE Crust_Simd128 crustSimd128AddF64(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128SubF64(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128MulF64(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128DivF64(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128SqrtF64(Crust_Simd128 v);
static CRUST_INLINE Crust_Simd128 crustSimd128AbsF64(Crust_Simd128 v);
static CRUST_INLINE Crust_Simd128 crustSimd128NegateF64(Crust_Simd128 v);
static CRUST_INLINE Crust_Simd128 crustSimd128MinF64(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128MaxF64(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128EqualF64(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128NotEqualF64(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128LessF64(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128LessEqualF64(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128GreaterF64(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128GreaterEqualF64(Crust_Simd128 a, Crust_Simd128 b);
static CRUST_INLINE Crust_Simd128 crustSimd128FloorF64(Crust_Simd128 v);
static CRUST_INLINE Crust_Simd128 crustSimd128CeilF64(Crust_Simd128 v);
static CRUST_INLINE Crust_Simd128 crustSimd128RoundEvenF64(Crust_Simd128 v);
static CRUST_INLINE Crust_Simd128 crustSimd128TruncateF64(Crust_Simd128 v);

static CRUST_INLINE Crust_Simd128 crustSimd128ConvertF32FromI32(Crust_Simd128 v);
static CRUST_INLINE Crust_Simd128 crustSimd128ConvertI32FromF32(Crust_Simd128 v);
static CRUST_INLINE Crust_Simd128 crustSimd128ConvertI32FromF32Saturate(Crust_Simd128 v);
static CRUST_INLINE Crust_Simd128 crustSimd128ConvertF64FromF32Low(Crust_Simd128 v);
static CRUST_INLINE Crust_Simd128 crustSimd128ConvertF64FromF32High(Crust_Simd128 v);
static CRUST_INLINE Crust_Simd128 crustSimd128ConvertF32FromF64(Crust_Simd128 low, Crust_Simd128 high);

static CRUST_INLINE Crust_Simd128 crustSimd128ConvertU16FromU8Low(Crust_Simd128 v);
static CRUST_INLINE Crust_Simd128 crustSimd128ConvertU16FromU8High(Crust_Simd128 v);
static CRUST_INLINE Crust_Simd128 crustSimd128ConvertI16FromI8Low(Crust_Simd128 v);
static CRUST_INLINE Crust_Simd128 crustSimd128ConvertI16FromI8High(Crust_Simd128 v);
static CRUST_INLINE Crust_Simd128 crustSimd128ConvertU32FromU16Low(Crust_Simd128 v);
static CRUST_INLINE Crust_Simd128 crustSimd128ConvertU32FromU16High(Crust_Simd128 v);
static CRUST_INLINE Crust_Simd128 crustSimd128ConvertI32FromI16Low(Crust_Simd128 v);
static CRUST_INLINE Crust_Simd128 crustSimd128ConvertI32FromI16High(Crust_Simd128 v);
static CRUST_INLINE Crust_Simd128 crustSimd128ConvertU64FromU32Low(Crust_Simd128 v);
static CRUST_INLINE Crust_Simd128 crustSimd128ConvertU64FromU32High(Crust_Simd128 v);
static CRUST_INLINE Crust_Simd128 crustSimd128ConvertI64FromI32Low(Crust_Simd128 v);
static CRUST_INLINE Crust_Simd128 crustSimd128ConvertI64FromI32High(Crust_Simd128 v);

static CRUST_INLINE Crust_Simd128 crustSimd128ConvertI8FromI16Saturate(Crust_Simd128 low, Crust_Simd128 high);
static CRUST_INLINE Crust_Simd128 crustSimd128ConvertU8FromI16Saturate(Crust_Simd128 low, Crust_Simd128 high);
static CRUST_INLINE Crust_Simd128 crustSimd128ConvertI16FromI32Saturate(Crust_Simd128 low, Crust_Simd128 high);
static CRUST_INLINE Crust_Simd128 crustSimd128ConvertU16FromI32Saturate(Crust_Simd128 low, Crust_Simd128 high);

//
#include "platform/cpu_common.inl"
#include "platform/simd_common.inl"

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

static CRUST_INLINE void *crustAllocatorAlloc(Crust_Allocator allocator, usize size, usize alignment)
{
	CRUST_ASSERT(allocator.vtbl != CRUST_NULL);
	CRUST_ASSERT(allocator.vtbl->alloc != CRUST_NULL);

	CRUST_ASSERT(size > 0);
	CRUST_ASSERT(alignment > 0);
	CRUST_ASSERT(crustIsPow2USize(alignment));

	return allocator.vtbl->alloc(allocator.context, size, alignment);
}

static CRUST_INLINE void *crustAllocatorRealloc(Crust_Allocator allocator, void *ptr, usize old_size, usize new_size, usize alignment)
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

static CRUST_INLINE void crustAllocatorFree(Crust_Allocator allocator, void *ptr, usize size, usize alignment)
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
	u32 free_head;
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
