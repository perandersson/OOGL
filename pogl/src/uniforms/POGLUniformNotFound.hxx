#pragma once
#include "config.hxx"
#include <gl/pogl.hxx>

class POGLUniformNotFound : public IPOGLUniform
{
public:
	POGLUniformNotFound();
	~POGLUniformNotFound();

	void SetInt32(POGL_INT32 a);
	void SetInt32(POGL_INT32 a, POGL_INT32 b);
	void SetInt32(POGL_INT32 a, POGL_INT32 b, POGL_INT32 c);
	void SetInt32(POGL_INT32 a, POGL_INT32 b, POGL_INT32 c, POGL_INT32 d);
	void SetInt32(POGL_INT32* ptr, POGL_UINT32 count);

	void SetUInt32(POGL_UINT32 a);
	void SetUInt32(POGL_UINT32 a, POGL_UINT32 b);
	void SetUInt32(POGL_UINT32 a, POGL_UINT32 b, POGL_UINT32 c);
	void SetUInt32(POGL_UINT32 a, POGL_UINT32 b, POGL_UINT32 c, POGL_UINT32 d);
	void SetUInt32(POGL_UINT32* ptr, POGL_UINT32 count);

	void SetFloat(POGL_FLOAT a);
	void SetFloat(POGL_FLOAT a, POGL_FLOAT b);
	void SetFloat(POGL_FLOAT a, POGL_FLOAT b, POGL_FLOAT c);
	void SetFloat(POGL_FLOAT a, POGL_FLOAT b, POGL_FLOAT c, POGL_FLOAT d);
	void SetFloat(POGL_FLOAT* ptr, POGL_UINT32 count);

	void SetDouble(POGL_DOUBLE a);
	void SetDouble(POGL_DOUBLE a, POGL_DOUBLE b);
	void SetDouble(POGL_DOUBLE a, POGL_DOUBLE b, POGL_DOUBLE c);
	void SetDouble(POGL_DOUBLE a, POGL_DOUBLE b, POGL_DOUBLE c, POGL_DOUBLE d);
	void SetDouble(POGL_DOUBLE* ptr, POGL_UINT32 count);

	void SetMatrix(const POGL_MAT4F& mat4);
	void SetMatrix(const POGL_MAT4D& mat4);

	void SetVector2I(const POGL_VECTOR2I& vec);
	void SetVector2F(const POGL_VECTOR2F& vec);
	void SetVector2D(const POGL_VECTOR2D& vec);

	void SetVector3F(const POGL_VECTOR3F& vec);
	void SetVector3D(const POGL_VECTOR3D& vec);

	void SetVector4F(const POGL_VECTOR4F& vec);
	void SetVector4D(const POGL_VECTOR4D& vec);
};