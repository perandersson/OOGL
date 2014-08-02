#pragma once
#include "POGLDefaultUniform.hxx"

class POGLUniformMat4 : public POGLDefaultUniform
{
public:
	POGLUniformMat4(const POGLEffect* effect, POGLRenderState* state, POGLDeviceContext* context, GLint componentID);
	~POGLUniformMat4();

	void Apply();

	void SetMatrix(const POGL_MAT4F& mat4);
	void SetMatrix(const POGL_MAT4D& mat4);

private:
	POGL_MAT4D mValue;
};
