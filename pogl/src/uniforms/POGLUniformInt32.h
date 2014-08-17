#pragma once
#include "POGLDefaultUniform.h"

class POGLUniformInt32 : public POGLDefaultUniform
{
public:
	POGLUniformInt32(const POGLProgram* program, POGLRenderState* state, POGLDeviceContext* context, GLint componentID);
	~POGLUniformInt32();

	void Apply();

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

private:
	POGL_INT32 mCount;
	POGL_INT32 mValues[4];
	POGL_INT32 mValuesSet[4];
};
