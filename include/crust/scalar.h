#pragma once

#include "core.h"
#include "platform/intrinsics_cpu.inl"

#ifdef __cplusplus
extern "C" {
#endif

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
static CRUST_INLINE u32 crustLzcntU32(u32 value);
static CRUST_INLINE u32 crustTzcntU32(u32 value);
static CRUST_INLINE u32 crustPopcntU32(u32 value);

//
#include "platform/scalar_common.inl"

#ifdef __cplusplus
}
#endif
