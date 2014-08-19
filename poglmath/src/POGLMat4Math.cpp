#include "config.h"

void POGLMat4Ortho(POGL_FLOAT left, POGL_FLOAT right, POGL_FLOAT bottom, POGL_FLOAT top, POGL_FLOAT zNear, POGL_FLOAT zFar, POGL_MAT4* _out_Mat4)
{
	POGL_FLOAT* m = _out_Mat4->vec;
#define M(row,col)  m[col*4+row]
	M(0, 0) = 2.0f / (right - left);
	M(0, 1) = 0.0f;
	M(0, 2) = 0.0f;
	M(0, 3) = -(right + left) / (right - left);

	M(1, 0) = 0.0f;
	M(1, 1) = 2.0f / (top - bottom);
	M(1, 2) = 0.0f;
	M(1, 3) = -(top + bottom) / (top - bottom);

	M(2, 0) = 0.0f;
	M(2, 1) = 0.0f;
	M(2, 2) = -2.0f / (zFar - zNear);
	M(2, 3) = -(zFar + zNear) / (zFar - zNear);

	M(3, 0) = 0.0f;
	M(3, 1) = 0.0f;
	M(3, 2) = 0.0f;
	M(3, 3) = 1.0f;
#undef M
}

void POGLMat4Ortho2D(POGL_FLOAT left, POGL_FLOAT right, POGL_FLOAT bottom, POGL_FLOAT top, POGL_MAT4* _out_Mat4)
{
	POGLMat4Ortho(left, right, bottom, top, -1.0f, 1.0f, _out_Mat4);
}

void POGLMat4LookAt(const POGL_VECTOR3& eye, const POGL_VECTOR3& center, const POGL_VECTOR3& up, POGL_MAT4* _out_Mat4)
{
	POGL_FLOAT* m = _out_Mat4->vec;

	POGL_VECTOR3 forward(center.x - eye.x, center.y - eye.y, center.z - eye.z);
	POGLVec3Normalize(&forward);

	POGL_VECTOR3 side;
	POGLVec3Cross(forward, up, &side);
	POGLVec3Normalize(&side);

	POGL_VECTOR3 newUp;
	POGLVec3Cross(side, forward, &newUp);

#define M(row,col)  m[col*4+row]
	M(0, 0) = side.x;
	M(0, 1) = side.y;
	M(0, 2) = side.z;
	M(0, 3) = 0.0f;

	M(1, 0) = newUp.x;
	M(1, 1) = newUp.y;
	M(1, 2) = newUp.z;
	M(1, 3) = 0.0f;

	M(2, 0) = -forward.x;
	M(2, 1) = -forward.y;
	M(2, 2) = -forward.z;
	M(2, 3) = 0.0f;

	M(3, 0) = 0.0f;
	M(3, 1) = 0.0f;
	M(3, 2) = 0.0f;
	M(3, 3) = 1.0f;
#undef M

	POGL_VECTOR3 negativeEye(-eye.x, -eye.y, -eye.z);
	POGLMat4Translate(negativeEye, _out_Mat4);
}

void POGLMat4Perspective(POGL_FLOAT fovy, POGL_FLOAT aspect, POGL_FLOAT zNear, POGL_FLOAT zFar, POGL_MAT4* _out_Mat4)
{
	POGL_FLOAT* m = _out_Mat4->vec;
#define M(row,col)  m[col*4+row]

	const POGL_FLOAT top = zNear * tanf(fovy * (POGL_FLOAT)(POGL_M_PI / 360.0));
	const POGL_FLOAT bottom = -top;
	const POGL_FLOAT right = top * aspect;
	const POGL_FLOAT left = -right;

	M(0, 0) = (2.0f * zNear) / (right - left);
	M(0, 1) = 0.0f;
	M(0, 2) = (right + left) / (right - left);
	M(0, 3) = 0.0f;

	M(1, 0) = 0.0f;
	M(1, 1) = (2.0f * zNear) / (top - bottom);
	M(1, 2) = (top + bottom) / (top - bottom);
	M(1, 3) = 0.0f;

	M(2, 0) = 0.0f;
	M(2, 1) = 0.0f;
	M(2, 2) = -((zFar + zNear) / (zFar - zNear));
	M(2, 3) = -((2.0f * zFar * zNear) / (zFar - zNear));

	M(3, 0) = 0.0f;
	M(3, 1) = 0.0f;
	M(3, 2) = -1.0f;
	M(3, 3) = 1.0f;
#undef M
}

void POGLMat4Translate(const POGL_VECTOR3& v, POGL_MAT4* _out_Mat4)
{
	POGL_FLOAT* m = _out_Mat4->vec;
	m[12] = m[0] * v.x + m[4] * v.y + m[8] * v.z + m[12];
	m[13] = m[1] * v.x + m[5] * v.y + m[9] * v.z + m[13];
	m[14] = m[2] * v.x + m[6] * v.y + m[10] * v.z + m[14];
	m[15] = m[3] * v.x + m[7] * v.y + m[11] * v.z + m[15];
}
