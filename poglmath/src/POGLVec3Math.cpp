#include "config.h"

POGL_FLOAT POGLVec3Length(const POGL_VECTOR3& v)
{
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

POGL_FLOAT POGLVec3SqrdLength(const POGL_VECTOR3& v)
{
	return v.x * v.x + v.y * v.y + v.z * v.z;
}

void POGLVec3Normalize(POGL_VECTOR3* _in_out_Vec3)
{
	POGL_FLOAT len = POGLVec3Length(*_in_out_Vec3);
	len = (len != 0.0f ? len : 1.0f);

	POGL_FLOAT lengthMul = 1.0f / len;
	_in_out_Vec3->x *= lengthMul;
	_in_out_Vec3->y *= lengthMul;
	_in_out_Vec3->z *= lengthMul;
}

void POGLVec3Cross(const POGL_VECTOR3& v1, const POGL_VECTOR3& v2, POGL_VECTOR3* _out_Vec3)
{
	_out_Vec3->x = (v1.y * v2.z - v1.z * v2.y);
	_out_Vec3->y = (v1.z * v2.x - v1.x * v2.z);
	_out_Vec3->z = (v1.x * v2.y - v1.y * v2.x);
}

void POGLVec3Invert(const POGL_VECTOR3& v, POGL_VECTOR3* _out_Vec3)
{
	_out_Vec3->x = v.x * -1.f;
	_out_Vec3->y = v.y * -1.f;
	_out_Vec3->z = v.z * -1.f;
}

void POGLVec3Invert(POGL_VECTOR3* _in_out_Vec3)
{
	_in_out_Vec3->x *= -1.f;
	_in_out_Vec3->y *= -1.f;
	_in_out_Vec3->z *= -1.f;
}

void POGLVec3Perp(const POGL_VECTOR3& v, POGL_VECTOR3* _out_Vec3)
{
	POGL_FLOAT min = fabs(v.x);
	POGL_VECTOR3 axis(1, 0, 0);

	const POGL_FLOAT ymin = fabs(v.y);
	if (ymin < min) {
		min = ymin;
		axis = POGL_VECTOR3(0, 1, 0);
	}

	const POGL_FLOAT zmin = fabs(v.z);
	if (zmin < min) {
		min = zmin;
		axis = POGL_VECTOR3(0, 0, 1);
	}

	POGLVec3Cross(v, axis, _out_Vec3);
}

POGL_FLOAT POGLVec3AngleInDegree(const POGL_VECTOR3& v1, const POGL_VECTOR3& v2)
{
	return POGLVec3AngleInRadians(v1, v2) * 57.29577951f;
}

POGL_FLOAT POGLVec3AngleInRadians(const POGL_VECTOR3& v1, const POGL_VECTOR3& v2)
{
	const POGL_FLOAT x = v1.x;
	const POGL_FLOAT y = v1.y;
	const POGL_FLOAT z = v1.z;
	return acosf((v2.x * x) + (v2.y * y) + (v2.z * z));
}

POGL_VECTOR3 POGL_VECTOR3X::GetNormalized() const
{
	POGL_VECTOR3 out;
	POGLVec3Normalize(&out);
	return out;
}

void POGL_VECTOR3X::Normalize()
{
	POGLVec3Normalize(this);
}

POGL_VECTOR3 POGL_VECTOR3X::CrossProduct(const POGL_VECTOR3& rhs) const
{
	POGL_VECTOR3 out;
	POGLVec3Cross(*this, rhs, &out);
	return out;
}

bool POGL_VECTOR3X::IsZero() const
{
	return FLT_EQ(x, 0) && FLT_EQ(y, 0) && FLT_EQ(z, 0);
}

bool POGL_VECTOR3X::IsNotZero() const
{
	return FLT_NEQ(x, 0) || FLT_NEQ(y, 0) || FLT_NEQ(z, 0);
}

void POGL_VECTOR3X::Invert()
{
	POGLVec3Invert(this);
}

POGL_VECTOR3 POGL_VECTOR3X::GetInverted() const
{
	POGL_VECTOR3 out;
	POGLVec3Invert(*this, &out);
	return out;
}

POGL_VECTOR3 POGL_VECTOR3X::Perp() const
{
	POGL_VECTOR3 out;
	POGLVec3Perp(*this, &out);
	return out;
}

POGL_FLOAT POGL_VECTOR3X::GetAngleDegree(const POGL_VECTOR3& v) const
{
	return POGLVec3AngleInDegree(*this, v);
}

POGL_FLOAT POGL_VECTOR3X::GetAngleRadians(const POGL_VECTOR3& v) const
{
	return POGLVec3AngleInRadians(*this, v);
}
