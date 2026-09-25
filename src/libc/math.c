#include <crust.h>
#include <math.h>

//
CRUST_APIENTRY f32 crustAbsF32(f32 v)
{
	return fabsf(v);
}

CRUST_APIENTRY f64 crustAbsF64(f64 v)
{
	return fabs(v);
}

CRUST_APIENTRY f32 crustSqrtF32(f32 v)
{
	return sqrtf(v);
}

CRUST_APIENTRY f32 crustRsqrtF32(f32 v)
{
	return 1.0f / sqrtf(v);
}

CRUST_APIENTRY f32 crustCosF32(f32 v)
{
	return cosf(v);
}

CRUST_APIENTRY f32 crustAcosF32(f32 v)
{
	return acosf(v);
}

CRUST_APIENTRY f32 crustSinF32(f32 v)
{
	return sinf(v);
}

CRUST_APIENTRY f32 crustAsinF32(f32 v)
{
	return asinf(v);
}

CRUST_APIENTRY f32 crustTanF32(f32 v)
{
	return tanf(v);
}

CRUST_APIENTRY f32 crustAtan2F32(f32 y, f32 x)
{
	return atan2f(y, x);
}
