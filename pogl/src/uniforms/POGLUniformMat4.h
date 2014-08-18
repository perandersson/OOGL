#pragma once
#include "POGLDefaultUniform.h"
#include <mutex>

class POGLUniformMat4 : public POGLDefaultUniform
{
public:
	POGLUniformMat4(POGL_UINT32 programUID, POGLRenderState* state, GLint componentID);
	~POGLUniformMat4();

	void Apply();

	void SetMatrix(const POGL_MAT4& mat4);

private:
	POGL_MAT4 mValue;
	std::mutex mMutex;
};
