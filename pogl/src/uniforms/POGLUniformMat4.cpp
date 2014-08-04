#include "MemCheck.h"
#include "POGLUniformMat4.h"
#include "POGLDeviceContext.h"

POGLUniformMat4::POGLUniformMat4(const POGLEffect* effect, POGLRenderState* state, POGLDeviceContext* context, GLint componentID)
: POGLDefaultUniform(effect, state, context, componentID)
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
	mDeviceContext->UniformMatrix4dv(mComponentID, 1, GL_FALSE, mValue.vec);

	CHECK_GL("Could not assign mat4 uniform values");
}

void POGLUniformMat4::SetMatrix(const POGL_MAT4F& mat4)
{
	mValue._11 = (POGL_DOUBLE)mat4._11;
	mValue._12 = (POGL_DOUBLE)mat4._12;
	mValue._13 = (POGL_DOUBLE)mat4._13;
	mValue._14 = (POGL_DOUBLE)mat4._14;

	mValue._21 = (POGL_DOUBLE)mat4._21;
	mValue._22 = (POGL_DOUBLE)mat4._22;
	mValue._23 = (POGL_DOUBLE)mat4._23;
	mValue._24 = (POGL_DOUBLE)mat4._24;

	mValue._31 = (POGL_DOUBLE)mat4._31;
	mValue._32 = (POGL_DOUBLE)mat4._32;
	mValue._33 = (POGL_DOUBLE)mat4._33;
	mValue._34 = (POGL_DOUBLE)mat4._34;

	mValue._41 = (POGL_DOUBLE)mat4._41;
	mValue._42 = (POGL_DOUBLE)mat4._42;
	mValue._43 = (POGL_DOUBLE)mat4._43;
	mValue._44 = (POGL_DOUBLE)mat4._44;
}

void POGLUniformMat4::SetMatrix(const POGL_MAT4D& mat4)
{
	memcpy(&mValue, &mat4, sizeof(POGL_MAT4D));
}
