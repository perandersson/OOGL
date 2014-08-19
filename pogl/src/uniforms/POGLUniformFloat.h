#pragma once
#include "POGLDefaultUniform.h"
#include <mutex>

class POGLUniformFloat : public POGLDefaultUniform
{
public:
	POGLUniformFloat(POGL_UINT32 programUID, POGLRenderState* state, GLint componentID);
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

	void SetVector2(const POGL_VECTOR2& vec);
	void SetVector3(const POGL_VECTOR3& vec);
	void SetVector4(const POGL_VECTOR4& vec);

private:
	POGL_INT32 mCount;
	POGL_FLOAT mValues[4];
	POGL_FLOAT mValuesSet[4];
};
