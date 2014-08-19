#include "config.h"

POGL_FLOAT POGLVec3Length(const POGL_VECTOR3& v)
{
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

void POGLVec3Normalize(POGL_VECTOR3* v)
{
	POGL_FLOAT len = POGLVec3Length(*v);
	len = (len != 0.0f ? len : 1.0f);

	POGL_FLOAT lengthMul = 1.0f / len;
	v->x *= lengthMul;
	v->y *= lengthMul;
	v->z *= lengthMul;
}

void POGLVec3Cross(const POGL_VECTOR3& v1, const POGL_VECTOR3& v2, POGL_VECTOR3* result)
{
	result->x = (v1.y * v2.z - v1.z * v2.y);
	result->y = (v1.z * v2.x - v1.x * v2.z);
	result->z = (v1.x * v2.y - v1.y * v2.x);
}
