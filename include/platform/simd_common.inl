//
#if CRUST_SIMD_SSE
	#include "simd_sse.inl"
#elif CRUST_SIMD_NEON
	#include "simd_neon.inl"
#elif CRUST_SIMD_WASM
	#include "simd_wasm.inl"
#else
	#include "simd_scalar.inl"
#endif

//
static CRUST_INLINE Crust_Simd128 crustSimd128LoadI8(const i8 *ptr)
{
	return crustSimd128LoadU8((const u8 *)ptr);
}

static CRUST_INLINE Crust_Simd128 crustSimd128LoadI16(const i16 *ptr)
{
	return crustSimd128LoadU16((const u16 *)ptr);
}

static CRUST_INLINE Crust_Simd128 crustSimd128LoadI32(const i32 *ptr)
{
	return crustSimd128LoadU32((const u32 *)ptr);
}

static CRUST_INLINE Crust_Simd128 crustSimd128LoadI64(const i64 *ptr)
{
	return crustSimd128LoadU64((const u64 *)ptr);
}

static CRUST_INLINE Crust_Simd128 crustSimd128LoadAlignedI8(const i8 *ptr)
{
	return crustSimd128LoadAlignedU8((const u8 *)ptr);
}

static CRUST_INLINE Crust_Simd128 crustSimd128LoadAlignedI16(const i16 *ptr)
{
	return crustSimd128LoadAlignedU16((const u16 *)ptr);
}

static CRUST_INLINE Crust_Simd128 crustSimd128LoadAlignedI32(const i32 *ptr)
{
	return crustSimd128LoadAlignedU32((const u32 *)ptr);
}

static CRUST_INLINE Crust_Simd128 crustSimd128LoadAlignedI64(const i64 *ptr)
{
	return crustSimd128LoadAlignedU64((const u64 *)ptr);
}

static CRUST_INLINE void crustSimd128StoreI8(i8 *ptr, Crust_Simd128 v)
{
	crustSimd128StoreU8((u8 *)ptr, v);
}

static CRUST_INLINE void crustSimd128StoreI16(i16 *ptr, Crust_Simd128 v)
{
	crustSimd128StoreU16((u16 *)ptr, v);
}

static CRUST_INLINE void crustSimd128StoreI32(i32 *ptr, Crust_Simd128 v)
{
	crustSimd128StoreU32((u32 *)ptr, v);
}

static CRUST_INLINE void crustSimd128StoreI64(i64 *ptr, Crust_Simd128 v)
{
	crustSimd128StoreU64((u64 *)ptr, v);
}

static CRUST_INLINE void crustSimd128StoreAlignedI8(i8 *ptr, Crust_Simd128 v)
{
	crustSimd128StoreAlignedU8((u8 *)ptr, v);
}

static CRUST_INLINE void crustSimd128StoreAlignedI16(i16 *ptr, Crust_Simd128 v)
{
	crustSimd128StoreAlignedU16((u16 *)ptr, v);
}

static CRUST_INLINE void crustSimd128StoreAlignedI32(i32 *ptr, Crust_Simd128 v)
{
	crustSimd128StoreAlignedU32((u32 *)ptr, v);
}

static CRUST_INLINE void crustSimd128StoreAlignedI64(i64 *ptr, Crust_Simd128 v)
{
	crustSimd128StoreAlignedU64((u64 *)ptr, v);
}

//
static CRUST_INLINE Crust_Simd128 crustSimd128SplatI8(i8 v)
{
	return crustSimd128SplatU8((u8)v);
}

static CRUST_INLINE Crust_Simd128 crustSimd128SplatI16(i16 v)
{
	return crustSimd128SplatU16((u16)v);
}

static CRUST_INLINE Crust_Simd128 crustSimd128SplatI32(i32 v)
{
	return crustSimd128SplatU32((u32)v);
}

static CRUST_INLINE Crust_Simd128 crustSimd128SplatI64(i64 v)
{
	return crustSimd128SplatU64((u64)v);
}

static CRUST_INLINE Crust_Simd128 crustSimd128MakeI32(i32 v0, i32 v1, i32 v2, i32 v3)
{
	return crustSimd128MakeU32((u32)v0, (u32)v1, (u32)v2, (u32)v3);
}

static CRUST_INLINE Crust_Simd128 crustSimd128MakeI64(i64 v0, i64 v1)
{
	return crustSimd128MakeU64((u64)v0, (u64)v1);
}

//
static CRUST_INLINE u8 crustSimd128GetLaneU8(Crust_Simd128 v, u32 lane)
{
	CRUST_ASSERT(lane < 16);

	u8 lanes[16];
	crustSimd128StoreU8(lanes, v);

	return lanes[lane];
}

static CRUST_INLINE i8 crustSimd128GetLaneI8(Crust_Simd128 v, u32 lane)
{
	CRUST_ASSERT(lane < 16);

	i8 lanes[16];
	crustSimd128StoreI8(lanes, v);

	return lanes[lane];
}

static CRUST_INLINE u16 crustSimd128GetLaneU16(Crust_Simd128 v, u32 lane)
{
	CRUST_ASSERT(lane < 8);

	u16 lanes[8];
	crustSimd128StoreU16(lanes, v);

	return lanes[lane];
}

static CRUST_INLINE i16 crustSimd128GetLaneI16(Crust_Simd128 v, u32 lane)
{
	CRUST_ASSERT(lane < 8);

	i16 lanes[8];
	crustSimd128StoreI16(lanes, v);

	return lanes[lane];
}

static CRUST_INLINE u32 crustSimd128GetLaneU32(Crust_Simd128 v, u32 lane)
{
	CRUST_ASSERT(lane < 4);

	u32 lanes[4];
	crustSimd128StoreU32(lanes, v);

	return lanes[lane];
}

static CRUST_INLINE i32 crustSimd128GetLaneI32(Crust_Simd128 v, u32 lane)
{
	CRUST_ASSERT(lane < 4);

	i32 lanes[4];
	crustSimd128StoreI32(lanes, v);

	return lanes[lane];
}

static CRUST_INLINE u64 crustSimd128GetLaneU64(Crust_Simd128 v, u32 lane)
{
	CRUST_ASSERT(lane < 2);

	u64 lanes[2];
	crustSimd128StoreU64(lanes, v);

	return lanes[lane];
}

static CRUST_INLINE i64 crustSimd128GetLaneI64(Crust_Simd128 v, u32 lane)
{
	CRUST_ASSERT(lane < 2);

	i64 lanes[2];
	crustSimd128StoreI64(lanes, v);

	return lanes[lane];
}

static CRUST_INLINE f32 crustSimd128GetLaneF32(Crust_Simd128 v, u32 lane)
{
	CRUST_ASSERT(lane < 4);

	f32 lanes[4];
	crustSimd128StoreF32(lanes, v);

	return lanes[lane];
}

static CRUST_INLINE f64 crustSimd128GetLaneF64(Crust_Simd128 v, u32 lane)
{
	CRUST_ASSERT(lane < 2);

	f64 lanes[2];
	crustSimd128StoreF64(lanes, v);

	return lanes[lane];
}

static CRUST_INLINE Crust_Simd128 crustSimd128SetLaneU8(Crust_Simd128 v, u32 lane, u8 value)
{
	CRUST_ASSERT(lane < 16);

	u8 lanes[16];
	crustSimd128StoreU8(lanes, v);
	lanes[lane] = value;

	return crustSimd128LoadU8(lanes);
}

static CRUST_INLINE Crust_Simd128 crustSimd128SetLaneI8(Crust_Simd128 v, u32 lane, i8 value)
{
	CRUST_ASSERT(lane < 16);

	i8 lanes[16];
	crustSimd128StoreI8(lanes, v);
	lanes[lane] = value;

	return crustSimd128LoadI8(lanes);
}

static CRUST_INLINE Crust_Simd128 crustSimd128SetLaneU16(Crust_Simd128 v, u32 lane, u16 value)
{
	CRUST_ASSERT(lane < 8);

	u16 lanes[8];
	crustSimd128StoreU16(lanes, v);
	lanes[lane] = value;

	return crustSimd128LoadU16(lanes);
}

static CRUST_INLINE Crust_Simd128 crustSimd128SetLaneI16(Crust_Simd128 v, u32 lane, i16 value)
{
	CRUST_ASSERT(lane < 8);

	i16 lanes[8];
	crustSimd128StoreI16(lanes, v);
	lanes[lane] = value;

	return crustSimd128LoadI16(lanes);
}

static CRUST_INLINE Crust_Simd128 crustSimd128SetLaneU32(Crust_Simd128 v, u32 lane, u32 value)
{
	CRUST_ASSERT(lane < 4);

	u32 lanes[4];
	crustSimd128StoreU32(lanes, v);
	lanes[lane] = value;

	return crustSimd128LoadU32(lanes);
}

static CRUST_INLINE Crust_Simd128 crustSimd128SetLaneI32(Crust_Simd128 v, u32 lane, i32 value)
{
	CRUST_ASSERT(lane < 4);

	i32 lanes[4];
	crustSimd128StoreI32(lanes, v);
	lanes[lane] = value;

	return crustSimd128LoadI32(lanes);
}

static CRUST_INLINE Crust_Simd128 crustSimd128SetLaneU64(Crust_Simd128 v, u32 lane, u64 value)
{
	CRUST_ASSERT(lane < 2);

	u64 lanes[2];
	crustSimd128StoreU64(lanes, v);
	lanes[lane] = value;

	return crustSimd128LoadU64(lanes);
}

static CRUST_INLINE Crust_Simd128 crustSimd128SetLaneI64(Crust_Simd128 v, u32 lane, i64 value)
{
	CRUST_ASSERT(lane < 2);

	i64 lanes[2];
	crustSimd128StoreI64(lanes, v);
	lanes[lane] = value;

	return crustSimd128LoadI64(lanes);
}

static CRUST_INLINE Crust_Simd128 crustSimd128SetLaneF32(Crust_Simd128 v, u32 lane, f32 value)
{
	CRUST_ASSERT(lane < 4);

	f32 lanes[4];
	crustSimd128StoreF32(lanes, v);
	lanes[lane] = value;

	return crustSimd128LoadF32(lanes);
}

static CRUST_INLINE Crust_Simd128 crustSimd128SetLaneF64(Crust_Simd128 v, u32 lane, f64 value)
{
	CRUST_ASSERT(lane < 2);

	f64 lanes[2];
	crustSimd128StoreF64(lanes, v);
	lanes[lane] = value;

	return crustSimd128LoadF64(lanes);
}

//
static CRUST_INLINE Crust_Simd128 crustSimd128AddI32(Crust_Simd128 a, Crust_Simd128 b)
{
	return crustSimd128AddU32(a, b);
}

//
static CRUST_INLINE Crust_Simd128 crustSimd128GreaterF32(Crust_Simd128 a, Crust_Simd128 b)
{
	return crustSimd128LessF32(b, a);
}

static CRUST_INLINE Crust_Simd128 crustSimd128GreaterEqualF32(Crust_Simd128 a, Crust_Simd128 b)
{
	return crustSimd128LessEqualF32(b, a);
}

//
static CRUST_INLINE Crust_Simd128 crustSimd128GreaterF64(Crust_Simd128 a, Crust_Simd128 b)
{
	return crustSimd128LessF64(b, a);
}

static CRUST_INLINE Crust_Simd128 crustSimd128GreaterEqualF64(Crust_Simd128 a, Crust_Simd128 b)
{
	return crustSimd128LessEqualF64(b, a);
}
