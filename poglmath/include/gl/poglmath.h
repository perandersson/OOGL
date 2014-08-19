#pragma once
#ifndef _POGLEXT_H_
#define _POGLEXT_H_

#ifndef _POGL_H_
#error "You must include pogl.h before poglmath.h"
#endif

/*
	Parts of the mathematical algorithms used have been influenced by Mesa3D (http://www.mesa3d.org)
	License: LICENSE_mesa3d.txt
*/

/*!
	\brief
*/
extern POGLAPI POGL_FLOAT POGLVec3Length(const POGL_VECTOR3& v);

/*!
	\brief Normalize the supplied 3-d vector

	\param v
*/
extern POGLAPI void POGLVec3Normalize(POGL_VECTOR3* v);

/*!

*/
extern POGLAPI void POGLVec3Cross(const POGL_VECTOR3& v1, const POGL_VECTOR3& v2, POGL_VECTOR3* result);

/*!
	\brief Calculate the orthographic matrix based on the supplied values

	\param left
	\param right
	\param bottom
	\param top
	\param zNear
	\param zFar
	\param _out_Mat4
*/
extern POGLAPI void POGLMat4Ortho(POGL_FLOAT left, POGL_FLOAT right, POGL_FLOAT bottom, POGL_FLOAT top, POGL_FLOAT zNear, POGL_FLOAT zFar, POGL_MAT4* _out_Mat4);

/*!
	\brief Calculate the orthographic matrix based on the supplied values

	\param left
	\param right
	\param bottom
	\param top
	\param _out_Mat4
*/
extern POGLAPI void POGLMat4Ortho2D(POGL_FLOAT left, POGL_FLOAT right, POGL_FLOAT bottom, POGL_FLOAT top, POGL_MAT4* _out_Mat4);

/*!
	\brief Calculate the lookat matrix based on the supplied values

	\param eye
	\param center
	\param up
	\param _out_Mat4
*/
extern POGLAPI void POGLMat4LookAt(const POGL_VECTOR3& eye, const POGL_VECTOR3& center, const POGL_VECTOR3& up, POGL_MAT4* _out_Mat4);

/*!
	\brief Calculates the perspective matrix

	\param fovy
	\param aspect
	\param zNear
	\param zFar
	\param _out_Mat4
*/
extern POGLAPI void POGLMat4Perspective(POGL_FLOAT fovy, POGL_FLOAT aspect, POGL_FLOAT zNear, POGL_FLOAT zFar, POGL_MAT4* _out_Mat4);

/*!
	\brief Multiply

	\param lhs
	\param rhs
	\param _out_Mat4
*/
extern POGLAPI void POGLMat4Multiply(const POGL_MAT4& lhs, const POGL_MAT4& rhs, POGL_MAT4* _out_Mat4);

/*!
	\brief Translate the supplied output matrix with the supplied vector

	\param v
	\param _out_Mat4
*/
extern POGLAPI void POGLMat4Translate(const POGL_VECTOR3& v, POGL_MAT4* _out_Mat4);

/*!
	\brief Rotate the supplied output matrix with the supplied vector

	\param angle
			The angle in degrees
	\param v
	\param _out_Mat4
*/
extern POGLAPI void POGLMat4Rotate(POGL_FLOAT angle, const POGL_VECTOR3& v, POGL_MAT4* _out_Mat4);

/*!
	\brief Rotate the supplied output matrix with the supplied vector

	\param v
	\param _out_Mat4
*/
extern POGLAPI void POGLMat4Scale(const POGL_VECTOR3& v, POGL_MAT4* _out_Mat4);

#endif
