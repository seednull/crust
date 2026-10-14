#pragma once

#include "scalar.h"
#include "platform/intrinsics_simd.inl"

#ifdef __cplusplus
extern "C" {
#endif

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
#include "platform/simd_common.inl"

#ifdef __cplusplus
}
#endif
