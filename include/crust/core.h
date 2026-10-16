#pragma once

// Version
#define CRUST_VERSION_MAJOR 1
#define CRUST_VERSION_MINOR 6
#define CRUST_VERSION_PATCH 0
#define CRUST_VERSION "1.6.0"

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
#elif defined(__APPLE__)
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

// Defines
#if CRUST_COMPILER_MSVC
	#define CRUST_ALIGNOF(x) __alignof(x)
	#define CRUST_ALIGNAS(n) __declspec(align(n))
	#define CRUST_INLINE __forceinline
	#define CRUST_RESTRICT __restrict
#else
	#define CRUST_ALIGNOF(x) __alignof__(x)
	#define CRUST_ALIGNAS(n) __attribute__((aligned(n)))
	#define CRUST_INLINE inline __attribute__((always_inline))
	#define CRUST_RESTRICT __restrict__
#endif

#if CRUST_COMPILER_MSVC
	#define CRUST_DEBUG_BREAK() __debugbreak()
#elif CRUST_COMPILER_CLANG
	#define CRUST_DEBUG_BREAK() __builtin_debugtrap()
#else
	#define CRUST_DEBUG_BREAK() __builtin_trap()
#endif

#define CRUST_CACHE_LINE_SIZE 128

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

#if (CRUST_COMPILER_GCC || CRUST_COMPILER_CLANG) && !CRUST_TOOLCHAIN_MSVC && !CRUST_TOOLCHAIN_MINGW
	#define CRUST_APIENTRY __attribute__((visibility("hidden")))
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

#define CRUST_F32_PI				3.14159265358979323846F
#define CRUST_F32_TAU				6.28318530717958647693F
#define CRUST_F64_PI				3.14159265358979323846
#define CRUST_F64_TAU				6.28318530717958647693

#define CRUST_PI					CRUST_F64_PI
#define CRUST_TAU					CRUST_F64_TAU

#ifdef __cplusplus
extern "C" {
#endif

//
static CRUST_INLINE void crustAssertFailure(const char *expression, const char *file, u32 line)
{
	CRUST_UNUSED(expression);
	CRUST_UNUSED(file);
	CRUST_UNUSED(line);

	CRUST_DEBUG_BREAK();
}

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

#ifdef __cplusplus
}
#endif
