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
static CRUST_INLINE Crust_Simd128 crustSimd128AddI32(Crust_Simd128 a, Crust_Simd128 b)
{
	return crustSimd128AddU32(a, b);
}

//
static CRUST_INLINE Crust_Simd128 crustSimd128GtF32(Crust_Simd128 a, Crust_Simd128 b)
{
	return crustSimd128LtF32(b, a);
}
