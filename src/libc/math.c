#include <crust.h>
#include <math.h>

//
CRUST_APIENTRY f32 crustCosF32(f32 v) { return cosf(v); }
CRUST_APIENTRY f32 crustAcosF32(f32 v) { return acosf(v); }
CRUST_APIENTRY f32 crustSinF32(f32 v) { return sinf(v); }
CRUST_APIENTRY f32 crustAsinF32(f32 v) { return asinf(v); }
CRUST_APIENTRY f32 crustTanF32(f32 v) { return tanf(v); }
CRUST_APIENTRY f32 crustAtan2F32(f32 y, f32 x) { return atan2f(y, x); }

//
CRUST_APIENTRY f64 crustCosF64(f64 v) { return cos(v); }
CRUST_APIENTRY f64 crustAcosF64(f64 v) { return acos(v); }
CRUST_APIENTRY f64 crustSinF64(f64 v) { return sin(v); }
CRUST_APIENTRY f64 crustAsinF64(f64 v) { return asin(v); }
CRUST_APIENTRY f64 crustTanF64(f64 v) { return tan(v); }
CRUST_APIENTRY f64 crustAtan2F64(f64 y, f64 x) { return atan2(y, x); }
