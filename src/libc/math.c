#include <crust.h>
#include <math.h>

//
CRUST_APIENTRY f32 crustAbsF32(f32 v) { return fabsf(v); }
CRUST_APIENTRY f32 crustSqrtF32(f32 v) { return sqrtf(v); }
CRUST_APIENTRY f32 crustRsqrtF32(f32 v) { return 1.0f / sqrtf(v); }

//
CRUST_APIENTRY f64 crustAbsF64(f64 v) { return fabs(v); }
CRUST_APIENTRY f64 crustSqrtF64(f64 v) { return sqrt(v); }
CRUST_APIENTRY f64 crustRsqrtF64(f64 v) { return 1.0 / sqrt(v); }
