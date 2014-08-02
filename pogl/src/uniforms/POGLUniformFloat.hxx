#pragma once
#include "POGLDefaultUniform.hxx"

class POGLUniformFloat : public POGLDefaultUniform
{
public:
	POGLUniformFloat(const POGLEffect* effect, POGLRenderState* state, POGLDeviceContext* context, GLint componentID);
	~POGLUniformFloat();

	void Apply();

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

	void SetVector2F(const POGL_VECTOR2F& vec);
	void SetVector2D(const POGL_VECTOR2D& vec);

	void SetVector3F(const POGL_VECTOR3F& vec);
	void SetVector3D(const POGL_VECTOR3D& vec);

	void SetVector4F(const POGL_VECTOR4F& vec);
	void SetVector4D(const POGL_VECTOR4D& vec);

private:
	POGL_INT32 mCount;
	POGL_FLOAT mValues[4];
	POGL_FLOAT mValuesSet[4];
};
