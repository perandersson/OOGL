#include "MemCheck.h"
#include "POGLUniformMat4.h"
#include "POGLDeviceContext.h"

POGLUniformMat4::POGLUniformMat4(POGL_UINT32 programUID, POGLRenderState* state, GLint componentID)
: POGLDefaultUniform(programUID, state, componentID)
{
	memset(&mValue, 0, sizeof(mValue));
	mValue._11 = 1.0;
	mValue._22 = 1.0;
	mValue._33 = 1.0;
	mValue._44 = 1.0;
}

POGLUniformMat4::~POGLUniformMat4()
{
}

void POGLUniformMat4::Apply()
{
	glUniformMatrix4fv(mComponentID, 1, GL_FALSE, mValue.vec);

	CHECK_GL("Could not assign mat4 uniform values");
}

void POGLUniformMat4::SetMatrix(const POGL_MAT4& mat4)
{
	std::lock_guard<std::mutex> lock(mMutex);

	mValue._11 = mat4._11;
	mValue._12 = mat4._12;
	mValue._13 = mat4._13;
	mValue._14 = mat4._14;

	mValue._21 = mat4._21;
	mValue._22 = mat4._22;
	mValue._23 = mat4._23;
	mValue._24 = mat4._24;

	mValue._31 = mat4._31;
	mValue._32 = mat4._32;
	mValue._33 = mat4._33;
	mValue._34 = mat4._34;

	mValue._41 = mat4._41;
	mValue._42 = mat4._42;
	mValue._43 = mat4._43;
	mValue._44 = mat4._44;

	if (IsProgramActive())
		POGLUniformMat4::Apply();
}
