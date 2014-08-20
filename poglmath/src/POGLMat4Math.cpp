#include "config.h"

/*
	Parts of the mathematical algorithms used below has been influenced by Mesa3D (http://www.mesa3d.org)

	MESA3D LICENSE:

	Copyright (C) 1999-2007  Brian Paul   All Rights Reserved.

	Permission is hereby granted, free of charge, to any person obtaining a
	copy of this software and associated documentation files (the "Software"),
	to deal in the Software without restriction, including without limitation
	the rights to use, copy, modify, merge, publish, distribute, sublicense,
	and/or sell copies of the Software, and to permit persons to whom the
	Software is furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included
	in all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
	OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
	BRIAN PAUL BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
	AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
	CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef M
#define M(row, col) m[col * 4 + row]
#endif

static const POGL_MAT4 POGL_MAT4_IDENTITY;

void POGLMat4Ortho(POGL_FLOAT left, POGL_FLOAT right, POGL_FLOAT bottom, POGL_FLOAT top, POGL_FLOAT zNear, POGL_FLOAT zFar, POGL_MAT4* _out_Mat4)
{
	POGL_FLOAT* m = _out_Mat4->vec;

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

	POGL_VECTOR3 negativeEye(-eye.x, -eye.y, -eye.z);
	POGLMat4Translate(negativeEye, _out_Mat4);
}

void POGLMat4Perspective(POGL_FLOAT fovy, POGL_FLOAT aspect, POGL_FLOAT zNear, POGL_FLOAT zFar, POGL_MAT4* _out_Mat4)
{
	POGL_FLOAT* m = _out_Mat4->vec;

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
}

void POGLMat4Translate(const POGL_VECTOR3& v, POGL_MAT4* _out_Mat4)
{
	POGL_FLOAT* m = _out_Mat4->vec;
	const POGL_FLOAT x = v.x;
	const POGL_FLOAT y = v.y;
	const POGL_FLOAT z = v.z;

	m[12] = m[0] * x + m[4] * y + m[8] * z + m[12];
	m[13] = m[1] * x + m[5] * y + m[9] * z + m[13];
	m[14] = m[2] * x + m[6] * y + m[10] * z + m[14];
	m[15] = m[3] * x + m[7] * y + m[11] * z + m[15];
}

void POGLMat4Rotate(POGL_FLOAT angle, const POGL_VECTOR3& v, POGL_MAT4* _out_Mat4)
{
	const POGL_FLOAT s = (POGL_FLOAT)sin(angle * POGL_ANG2RAD_F);
	const POGL_FLOAT c = (POGL_FLOAT)cos(angle * POGL_ANG2RAD_F);

	POGL_FLOAT x = v.x;
	POGL_FLOAT y = v.y;
	POGL_FLOAT z = v.z;

	POGL_MAT4 tmp; 
	POGL_FLOAT* m = tmp.vec;

	bool optimized = false;

	if (x == 0.0f) {
		if (y == 0.0f) {
			if (z != 0.0f) {
				optimized = true;
				/* rotate only around z-axis */
				M(0, 0) = c;
				M(1, 1) = c;
				if (z < 0.0F) {
					M(0, 1) = s;
					M(1, 0) = -s;
				}
				else {
					M(0, 1) = -s;
					M(1, 0) = s;
				}
			}
		}
		else if (z == 0.0f) {
			optimized = true;
			/* rotate only around y-axis */
			M(0, 0) = c;
			M(2, 2) = c;
			if (y < 0.0f) {
				M(0, 2) = -s;
				M(2, 0) = s;
			}
			else {
				M(0, 2) = s;
				M(2, 0) = -s;
			}
		}
	}
	else if (y == 0.0f) {
		if (z == 0.0f) {
			optimized = true;
			/* rotate only around x-axis */
			M(1, 1) = c;
			M(2, 2) = c;
			if (x < 0.0f) {
				M(1, 2) = s;
				M(2, 1) = -s;
			}
			else {
				M(1, 2) = -s;
				M(2, 1) = s;
			}
		}
	}

	if (!optimized) {
		const POGL_FLOAT mag = sqrtf(x * x + y * y + z * z);

		if (mag <= 1.0e-4) {
			/* no rotation, leave mat as-is */
			return;
		}

		x /= mag;
		y /= mag;
		z /= mag;

		POGL_FLOAT xx = x * x;
		POGL_FLOAT yy = y * y;
		POGL_FLOAT zz = z * z;
		POGL_FLOAT xy = x * y;
		POGL_FLOAT yz = y * z;
		POGL_FLOAT zx = z * x;
		POGL_FLOAT xs = x * s;
		POGL_FLOAT ys = y * s;
		POGL_FLOAT zs = z * s;
		POGL_FLOAT one_c = 1.0F - c;

		/* We already hold the identity-matrix so we can skip some statements */
		M(0, 0) = (one_c * xx) + c;
		M(0, 1) = (one_c * xy) - zs;
		M(0, 2) = (one_c * zx) + ys;

		M(1, 0) = (one_c * xy) + zs;
		M(1, 1) = (one_c * yy) + c;
		M(1, 2) = (one_c * yz) - xs;

		M(2, 0) = (one_c * zx) - ys;
		M(2, 1) = (one_c * yz) + xs;
		M(2, 2) = (one_c * zz) + c;
	}

	POGLMat4Multiply(*_out_Mat4, tmp, _out_Mat4);
}

void POGLMat4Scale(const POGL_VECTOR3& v, POGL_MAT4* _out_Mat4)
{
	POGL_FLOAT* m = _out_Mat4->vec;
	const POGL_FLOAT x = v.x;
	const POGL_FLOAT y = v.x;
	const POGL_FLOAT z = v.x;

	m[0] *= x;   m[4] *= y;   m[8] *= z;
	m[1] *= x;   m[5] *= y;   m[9] *= z;
	m[2] *= x;   m[6] *= y;   m[10] *= z;
	m[3] *= x;   m[7] *= y;   m[11] *= z;
}

void POGLMat4Multiply(const POGL_MAT4& lhs, const POGL_MAT4& rhs, POGL_MAT4* _out_Mat4)
{
	const POGL_FLOAT* a = lhs.vec;
	const POGL_FLOAT* b = rhs.vec;
	POGL_MAT4 tmp;
	POGL_FLOAT* tmp_M = tmp.vec;

	tmp_M[0] = (a[0] * b[0] + a[4] * b[1] + a[8] * b[2] + a[12] * b[3]);
	tmp_M[1] = (a[1] * b[0] + a[5] * b[1] + a[9] * b[2] + a[13] * b[3]);
	tmp_M[2] = (a[2] * b[0] + a[6] * b[1] + a[10] * b[2] + a[14] * b[3]);
	tmp_M[3] = (a[3] * b[0] + a[7] * b[1] + a[11] * b[2] + a[15] * b[3]);

	tmp_M[4] = (a[0] * b[4] + a[4] * b[5] + a[8] * b[6] + a[12] * b[7]);
	tmp_M[5] = (a[1] * b[4] + a[5] * b[5] + a[9] * b[6] + a[13] * b[7]);
	tmp_M[6] = (a[2] * b[4] + a[6] * b[5] + a[10] * b[6] + a[14] * b[7]);
	tmp_M[7] = (a[3] * b[4] + a[7] * b[5] + a[11] * b[6] + a[15] * b[7]);

	tmp_M[8] = (a[0] * b[8] + a[4] * b[9] + a[8] * b[10] + a[12] * b[11]);
	tmp_M[9] = (a[1] * b[8] + a[5] * b[9] + a[9] * b[10] + a[13] * b[11]);
	tmp_M[10] = (a[2] * b[8] + a[6] * b[9] + a[10] * b[10] + a[14] * b[11]);
	tmp_M[11] = (a[3] * b[8] + a[7] * b[9] + a[11] * b[10] + a[15] * b[11]);

	tmp_M[12] = (a[0] * b[12] + a[4] * b[13] + a[8] * b[14] + a[12] * b[15]);
	tmp_M[13] = (a[1] * b[12] + a[5] * b[13] + a[9] * b[14] + a[13] * b[15]);
	tmp_M[14] = (a[2] * b[12] + a[6] * b[13] + a[10] * b[14] + a[14] * b[15]);
	tmp_M[15] = (a[3] * b[12] + a[7] * b[13] + a[11] * b[14] + a[15] * b[15]);

	*_out_Mat4 = tmp;
}
