#pragma once
#ifndef _POGLEXT_H_
#define _POGLEXT_H_

#ifndef _POGL_H_
#error "You must include pogl.h before poglmath.h"
#endif

/*!
	\brief Calculate the length of the supplied vector

	\param v
	\return
*/
extern POGLAPI POGL_FLOAT POGLVec3Length(const POGL_VECTOR3& v);

/*!
	\brief Calculates the squarede length of the supplied vector

	\param v
	\return
*/
extern POGLAPI POGL_FLOAT POGLVec3SqrdLength(const POGL_VECTOR3& v);

/*!
	\brief Normalize the supplied 3-d vector

	\param _in_out_Vec3
*/
extern POGLAPI void POGLVec3Normalize(POGL_VECTOR3* _in_out_Vec3);

/*!
	\brief Invert the supplied vector
*/
extern POGLAPI void POGLVec3Invert(POGL_VECTOR3* _in_out_Vec3);

/*!
	\brief Invert the supplied vector and put the result into the out vector
*/
extern POGLAPI void POGLVec3Invert(const POGL_VECTOR3& v, POGL_VECTOR3* _out_Vec3);

/*!
	\brief Calculate the cross product between the first- and second vector.

	_out_Vec3 = v1 x v2
*/
extern POGLAPI void POGLVec3Cross(const POGL_VECTOR3& v1, const POGL_VECTOR3& v2, POGL_VECTOR3* _out_Vec3);

/*!
	\brief Invert the supplied vector and put the result into the out vector
*/
extern POGLAPI void POGLVec3Perp(const POGL_VECTOR3& v, POGL_VECTOR3* _out_Vec3);

/*
	\brief Retrieves the angle in degrees between v1 and v2
	
	\remark
			Both vectors must be normalized before using this function
*/
POGL_FLOAT POGLVec3AngleInDegree(const POGL_VECTOR3& v1, const POGL_VECTOR3& v2);

/*
	\brief Retrieves the angle in radians between v1 and v2

	\remark
			Both vectors must be normalized before using this function
*/
POGL_FLOAT POGLVec3AngleInRadians(const POGL_VECTOR3& v1, const POGL_VECTOR3& v2);

/*!
	\brief Extended 3D vector structure

	You can use this structure as if it's a POGL_VECTOR3 structure inside POGL
*/
struct POGL_VECTOR3X : POGL_VECTOR3
{
	POGL_VECTOR3X() : POGL_VECTOR3() {}
	POGL_VECTOR3X(const POGL_FLOAT _x, const POGL_FLOAT _y, const POGL_FLOAT _z) : POGL_VECTOR3(_x, _y, _z) {}
	POGL_VECTOR3X(const POGL_VECTOR3X& in) : POGL_VECTOR3(in) {}
	POGL_VECTOR3X(const POGL_VECTOR3& in) : POGL_VECTOR3(in) {}
	
	/*!
		\brief Retrieves a normalized version of this vector
	*/
	POGL_VECTOR3 GetNormalized() const;
	
	/*!
		\brief Normalize this 3D vector
	*/
	void Normalize();

	/*!
		\brief Retrieves a cross product based on this and the supplied 3D vector
	*/
	POGL_VECTOR3 CrossProduct(const POGL_VECTOR3& rhs) const;

	/*!
		\brief Is this 3D vector zero?
	*/
	bool IsZero() const;

	/*!
		\brief Is this 3D vector not zero?
	*/
	bool IsNotZero() const;

	/*!
		\brief Inverts this vector
	*/
	void Invert();

	/*!
		\brief Retrieves the inverted vector
	*/
	POGL_VECTOR3 GetInverted() const;

	/*!
	
	*/
	POGL_VECTOR3 Perp() const;

	/*
		\brief Retrieves the angle in degrees between this vector and the supplied vector
		
		\remark
				This vector and the supplied vector must be normalized before using this method
	*/
	POGL_FLOAT GetAngleDegree(const POGL_VECTOR3& v) const;

	/*
		\brief Retrieves the angle in radians between this vector and the supplied vector

		\remark
				This vector and the supplied vector must be normalized before using this method
	*/
	POGL_FLOAT GetAngleRadians(const POGL_VECTOR3& v) const;
};
_STATIC_ASSERT(sizeof(POGL_VECTOR3) == sizeof(POGL_VECTOR3X));

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
	\brief Transpose the supplied from matrix

	\param from
			The matrix we want to transpose
	\param _out_Mat4
			The 
*/
extern POGLAPI void POGLMat4Transpose(const POGL_MAT4& from, POGL_MAT4* _out_Mat4);

/*!
	\brief Calculates the inverse of the supplied matrix

	\param from
			The matrix we want to transpose
	\param _out_Mat4
			The output matrix.

	\return TRUE if the inverse is calculateable. FALSE if the supplied matrix is singular (i.e. does not have an inverse matrix)
*/
extern POGLAPI bool POGLMat4Inverse(const POGL_MAT4& from, POGL_MAT4* _out_Mat4);

/*!
	\brief Translate the supplied output matrix with the supplied vector

	\param v
	\param _out_Mat4
*/
extern POGLAPI void POGLMat4Translate(const POGL_VECTOR3& v, POGL_MAT4* _out_Mat4);

/*!
	\brief Retrieves a rotation matrix based on the angle and vector

	\param angle
			The angle in degrees
	\param v
	\param _out_Mat4
*/
extern POGLAPI void POGLMat4Rotate(POGL_FLOAT angle, const POGL_VECTOR3& v, POGL_MAT4* _out_Mat4);

/*!
	\brief Rotate the supplied output matrix with the supplied vector

	\param angle
			The angle in degrees
	\param v
	\param _out_Mat4
*/
extern POGLAPI void POGLMat4Rotate(POGL_FLOAT angle, const POGL_MAT4& mat4, const POGL_VECTOR3& v, POGL_MAT4* _out_Mat4);

/*!
	\brief Rotate the supplied output matrix with the supplied vector

	\param v
	\param _out_Mat4
*/
extern POGLAPI void POGLMat4Scale(const POGL_VECTOR3& v, POGL_MAT4* _out_Mat4);

#endif
