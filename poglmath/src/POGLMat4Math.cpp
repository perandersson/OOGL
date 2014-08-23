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

#ifndef MREF
#define MREF(m, row, col) m[col * 4 + row]
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

	POGL_VECTOR3 negativeEye;
	POGLVec3Invert(eye, &negativeEye);
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
	POGLMat4Rotate(angle, POGL_MAT4_IDENTITY, v, _out_Mat4);
}

void POGLMat4Rotate(POGL_FLOAT angle, const POGL_MAT4& mat4, const POGL_VECTOR3& v, POGL_MAT4* _out_Mat4)
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

	POGLMat4Multiply(mat4, tmp, _out_Mat4);
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

POGLAPI void POGLMat4Transpose(const POGL_MAT4& from, POGL_MAT4* _out_Mat4)
{
	const POGL_FLOAT* m = from.vec;
	POGL_FLOAT* to = _out_Mat4->vec;

	to[0] = m[0];
	to[1] = m[4];
	to[2] = m[8];
	to[3] = m[12];
	to[4] = m[1];
	to[5] = m[5];
	to[6] = m[9];
	to[7] = m[13];
	to[8] = m[2];
	to[9] = m[6];
	to[10] = m[10];
	to[11] = m[14];
	to[12] = m[3];
	to[13] = m[7];
	to[14] = m[11];
	to[15] = m[15];
}

bool POGLMat4Inverse(const POGL_MAT4& from, POGL_MAT4* _out_Mat4)
{
#define SWAP_ROWS(a, b) { POGL_FLOAT *_tmp = a; (a)=(b); (b)=_tmp; }
	const POGL_FLOAT* m = from.vec;
	POGL_FLOAT* out = _out_Mat4->vec;
	POGL_FLOAT wtmp[4][8];
	POGL_FLOAT m0, m1, m2, m3, s;
	POGL_FLOAT *r0, *r1, *r2, *r3;

	r0 = wtmp[0], r1 = wtmp[1], r2 = wtmp[2], r3 = wtmp[3];

	r0[0] = MREF(m, 0, 0), r0[1] = MREF(m, 0, 1),
	r0[2] = MREF(m, 0, 2), r0[3] = MREF(m, 0, 3),
	r0[4] = 1.0, r0[5] = r0[6] = r0[7] = 0.0,

	r1[0] = MREF(m, 1, 0), r1[1] = MREF(m, 1, 1),
	r1[2] = MREF(m, 1, 2), r1[3] = MREF(m, 1, 3),
	r1[5] = 1.0, r1[4] = r1[6] = r1[7] = 0.0,

	r2[0] = MREF(m, 2, 0), r2[1] = MREF(m, 2, 1),
	r2[2] = MREF(m, 2, 2), r2[3] = MREF(m, 2, 3),
	r2[6] = 1.0, r2[4] = r2[5] = r2[7] = 0.0,

	r3[0] = MREF(m, 3, 0), r3[1] = MREF(m, 3, 1),
	r3[2] = MREF(m, 3, 2), r3[3] = MREF(m, 3, 3),
	r3[7] = 1.0, r3[4] = r3[5] = r3[6] = 0.0;

	/* choose pivot - or die */
	if (fabsf(r3[0])>fabsf(r2[0])) SWAP_ROWS(r3, r2);
	if (fabsf(r2[0])>fabsf(r1[0])) SWAP_ROWS(r2, r1);
	if (fabsf(r1[0])>fabsf(r0[0])) SWAP_ROWS(r1, r0);
	if (0.0 == r0[0]) return false;

	/* eliminate first variable     */
	m1 = r1[0]/r0[0]; m2 = r2[0]/r0[0]; m3 = r3[0]/r0[0];
	s = r0[1]; r1[1] -= m1 * s; r2[1] -= m2 * s; r3[1] -= m3 * s;
	s = r0[2]; r1[2] -= m1 * s; r2[2] -= m2 * s; r3[2] -= m3 * s;
	s = r0[3]; r1[3] -= m1 * s; r2[3] -= m2 * s; r3[3] -= m3 * s;
	s = r0[4];
	if (s != 0.0) { r1[4] -= m1 * s; r2[4] -= m2 * s; r3[4] -= m3 * s; }
	s = r0[5];
	if (s != 0.0) { r1[5] -= m1 * s; r2[5] -= m2 * s; r3[5] -= m3 * s; }
	s = r0[6];
	if (s != 0.0) { r1[6] -= m1 * s; r2[6] -= m2 * s; r3[6] -= m3 * s; }
	s = r0[7];
	if (s != 0.0) { r1[7] -= m1 * s; r2[7] -= m2 * s; r3[7] -= m3 * s; }

	/* choose pivot - or die */
	if (fabsf(r3[1])>fabsf(r2[1])) SWAP_ROWS(r3, r2);
	if (fabsf(r2[1])>fabsf(r1[1])) SWAP_ROWS(r2, r1);
	if (0.0 == r1[1]) return false;

	/* eliminate second variable */
	m2 = r2[1]/r1[1]; m3 = r3[1]/r1[1];
	r2[2] -= m2 * r1[2]; r3[2] -= m3 * r1[2];
	r2[3] -= m2 * r1[3]; r3[3] -= m3 * r1[3];
	s = r1[4]; if (0.0 != s) { r2[4] -= m2 * s; r3[4] -= m3 * s; }
	s = r1[5]; if (0.0 != s) { r2[5] -= m2 * s; r3[5] -= m3 * s; }
	s = r1[6]; if (0.0 != s) { r2[6] -= m2 * s; r3[6] -= m3 * s; }
	s = r1[7]; if (0.0 != s) { r2[7] -= m2 * s; r3[7] -= m3 * s; }

	/* choose pivot - or die */
	if (fabsf(r3[2])>fabsf(r2[2])) SWAP_ROWS(r3, r2);
	if (0.0 == r2[2])  return false;

	/* eliminate third variable */
	m3 = r3[2]/r2[2];
	r3[3] -= m3 * r2[3], r3[4] -= m3 * r2[4],
	r3[5] -= m3 * r2[5], r3[6] -= m3 * r2[6],
	r3[7] -= m3 * r2[7];

	/* last check */
	if (0.0 == r3[3]) return false;

	s = 1.0F/r3[3];             /* now back substitute row 3 */
	r3[4] *= s; r3[5] *= s; r3[6] *= s; r3[7] *= s;

	m2 = r2[3];                 /* now back substitute row 2 */
	s  = 1.0F/r2[2];
	r2[4] = s * (r2[4] - r3[4] * m2), r2[5] = s * (r2[5] - r3[5] * m2),
	r2[6] = s * (r2[6] - r3[6] * m2), r2[7] = s * (r2[7] - r3[7] * m2);
	m1 = r1[3];
	r1[4] -= r3[4] * m1, r1[5] -= r3[5] * m1,
	r1[6] -= r3[6] * m1, r1[7] -= r3[7] * m1;
	m0 = r0[3];
	r0[4] -= r3[4] * m0, r0[5] -= r3[5] * m0,
	r0[6] -= r3[6] * m0, r0[7] -= r3[7] * m0;

	m1 = r1[2];                 /* now back substitute row 1 */
	s  = 1.0F/r1[1];
	r1[4] = s * (r1[4] - r2[4] * m1), r1[5] = s * (r1[5] - r2[5] * m1),
	r1[6] = s * (r1[6] - r2[6] * m1), r1[7] = s * (r1[7] - r2[7] * m1);
	m0 = r0[2];
	r0[4] -= r2[4] * m0, r0[5] -= r2[5] * m0,
	r0[6] -= r2[6] * m0, r0[7] -= r2[7] * m0;

	m0 = r0[1];                 /* now back substitute row 0 */
	s  = 1.0F/r0[0];
	r0[4] = s * (r0[4] - r1[4] * m0), r0[5] = s * (r0[5] - r1[5] * m0),
	r0[6] = s * (r0[6] - r1[6] * m0), r0[7] = s * (r0[7] - r1[7] * m0);

	MREF(out, 0, 0) = r0[4]; MREF(out, 0, 1) = r0[5],
	MREF(out, 0, 2) = r0[6]; MREF(out, 0, 3) = r0[7],
	MREF(out, 1, 0) = r1[4]; MREF(out, 1, 1) = r1[5],
	MREF(out, 1, 2) = r1[6]; MREF(out, 1, 3) = r1[7],
	MREF(out, 2, 0) = r2[4]; MREF(out, 2, 1) = r2[5],
	MREF(out, 2, 2) = r2[6]; MREF(out, 2, 3) = r2[7],
	MREF(out, 3, 0) = r3[4]; MREF(out, 3, 1) = r3[5],
	MREF(out, 3, 2) = r3[6]; MREF(out, 3, 3) = r3[7];

#undef SWAP_ROWS
	return true;
}
