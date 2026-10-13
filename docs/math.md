# Math

Crust's math types are plain structs of `f32`, passed and returned by value:

| Type | Fields | Size |
|---|---|---|
| `Crust_Vec2` | `x, y` | 8 bytes |
| `Crust_Vec3` | `x, y, z` | 12 bytes, no padding |
| `Crust_Vec4` | `x, y, z, w` | 16 bytes, 4-byte aligned |
| `Crust_Quat` | `x, y, z, w` | 16 bytes; `w` is the scalar part |
| `Crust_Transform` | `position` (`Crust_Vec3`), `rotation` (`Crust_Quat`), `scale` (`Crust_Vec3`) | 40 bytes |

They have no hidden alignment or padding, so they can be laid out in GPU buffers or files as is, and initialized with C99 aggregate syntax:

```c
Crust_Vec3 up = { 0.0f, 1.0f, 0.0f };
Crust_Quat identity = { 0.0f, 0.0f, 0.0f, 1.0f };
```

All functions are `static CRUST_INLINE`. They are scalar code: for batch processing of many vectors, use the [SIMD API](simd.md#examples).

## Vectors

The same functions exist for `Vec2`, `Vec3` and `Vec4`. The suffix of the binary operations says what the second operand is: `S` for a scalar, `V` for a vector.

| Function | Result |
|---|---|
| `crustVec3AddS(a, s)`, `crustVec3AddV(a, b)` | `a + s`, `a + b` |
| `crustVec3SubS(a, s)`, `crustVec3SubV(a, b)` | `a − s`, `a − b` |
| `crustVec3MulS(a, s)`, `crustVec3MulV(a, b)` | `a · s`, component-wise `a · b` |
| `crustVec3DivS(a, s)` | `a · (1 / s)`: one division and three multiplies |
| `crustVec3DivV(a, b)` | Component-wise `a / b` |
| `crustVec3Mad(a, s, b)` | `a · s + b` |
| `crustVec3Lerp(a, b, t)` | `a + (b − a) · t` |
| `crustVec3Dot(a, b)` | Dot product |
| `crustVec3Length(v)`, `crustVec3LengthSquared(v)` | `‖v‖`, `‖v‖²` |
| `crustVec3Normalize(v)` | `v / ‖v‖` |
| `crustVec3Cross(a, b)` | Cross product, `Vec3` only |

```c
Crust_Vec3 velocity = crustVec3MulS(direction, speed);
position = crustVec3Mad(velocity, dt, position);

f32 facing = crustVec3Dot(crustVec3Normalize(forward), crustVec3Normalize(to_target));
```

Things to keep in mind:

- **`DivS` multiplies by the reciprocal.** The result can differ from dividing each component by `s` in the last bit.
- **`Normalize` does not check for zero.** A zero vector gives NaNs. Check `LengthSquared` first when the input can be degenerate.
- **`Lerp` is not exact at `t = 1`.** `a + (b − a)` can differ from `b` by rounding.
- **`Mad` may be fused.** Depending on the compiler and its `-ffp-contract` setting, `a · s + b` can become a fused multiply-add. Results can then differ between compilers and platforms in the last bit. The same applies to `Dot`, `Lerp` and anything else written as products plus sums.

## Quaternions

`Crust_Quat` stores `x, y, z` as the vector part and `w` as the scalar part. The identity rotation is `{ 0, 0, 0, 1 }`.

| Function | Result |
|---|---|
| `crustQuatMul(a, b)` | Hamilton product `a · b`: rotating by it applies `b` first, then `a` |
| `crustQuatConjugate(q)` | `(−x, −y, −z, w)`. For a unit quaternion this is the inverse rotation |
| `crustQuatDot(a, b)` | 4D dot product |
| `crustQuatNormalize(q)` | `q / ‖q‖`, no zero check |
| `crustQuatMad(a, s, b)` | `a · s + b` component-wise |
| `crustQuatNlerp(a, b, t)` | Normalized linear interpolation along the shortest path |
| `crustQuatSlerp(a, b, t)` | Spherical linear interpolation along the shortest path |
| `crustQuatRotate(q, v)` | Rotates the vector `v` by `q`: `q · v · q*` |

```c
Crust_Quat turn = crustQuatMul(yaw, pitch);
Crust_Vec3 forward = crustQuatRotate(turn, base_forward);

Crust_Quat blended = crustQuatSlerp(from, to, 0.25f);
```

Details:

- **Unit quaternions.** `Rotate`, `Conjugate` as inverse and the interpolation results assume unit length. Renormalize from time to time after many `Mul`s, because rounding errors accumulate.
- **Shortest path.** `q` and `−q` are the same rotation. `Nlerp` and `Slerp` flip `b` when `Dot(a, b) < 0`, so they always take the short way around.
- **Slerp near zero.** When `|Dot(a, b)| > 0.9995`, `Slerp` uses linear weights, because the sine of the angle is too small to divide by. Both functions normalize their result.
- **Cost.** `Slerp` calls `crustAcosF32` and `crustSinF32`, which are libm functions in `src/libc`. `Nlerp` is much cheaper and is usually good enough for small angles and for animation blending.
- **`Rotate`** is implemented as two quaternion products. It is exact enough, but not the cheapest form.

## Transforms

`Crust_Transform` is a position, a rotation and a scale. A point `p` in the transform's local space maps to

```
position + rotate(rotation, scale · p)
```

There is no function for that, write it with the functions above:

```c
Crust_Vec3 transformPoint(Crust_Transform t, Crust_Vec3 p)
{
	return crustVec3AddV(t.position, crustQuatRotate(t.rotation, crustVec3MulV(t.scale, p)));
}
```

| Function | Result |
|---|---|
| `crustTransformMul(a, b)` | The transform that applies `b`, then `a`: `Mul(parent, local)` gives the world transform of a child |
| `crustTransformInvert(t)` | The inverse transform: `Mul(t, Invert(t))` is the identity |

```c
Crust_Transform world = crustTransformMul(parent_world, local);
Crust_Transform to_local = crustTransformInvert(world);
```

**Uniform scale only.** The scale is stored per axis, but a rotation combined with non-uniform scale cannot be represented exactly as position, rotation and scale after composition. `Mul` and `Invert` are exact only when all three scale components are equal. With non-uniform scale, use matrices instead.
