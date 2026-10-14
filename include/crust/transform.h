#pragma once

#include "vec.h"
#include "quat.h"

#ifdef __cplusplus
extern "C" {
#endif

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

#ifdef __cplusplus
}
#endif
