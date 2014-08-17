#pragma once
#include "POGLDefaultUniform.h"

class POGLUniformMat4 : public POGLDefaultUniform
{
public:
	POGLUniformMat4(const POGLProgram* program, POGLRenderState* state, POGLDeviceContext* context, GLint componentID);
	~POGLUniformMat4();

	void Apply();

	void SetMatrix(const POGL_MAT4& mat4);

private:
	POGL_MAT4 mValue;
};
