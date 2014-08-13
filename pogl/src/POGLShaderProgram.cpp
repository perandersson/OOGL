#include "MemCheck.h"
#include "POGLShaderProgram.h"

namespace {
	std::atomic<POGL_UINT32> uid;
	POGL_UINT32 GenShaderProgramUID() {
		return ++uid;
	}
}

POGLShaderProgram::POGLShaderProgram(GLuint shaderID, POGLShaderProgramType::Enum type)
: mRefCount(1), mUID(GenShaderProgramUID()), mShaderID(shaderID), mType(type)
{
}

POGLShaderProgram::~POGLShaderProgram()
{
}

void POGLShaderProgram::AddRef()
{
	mRefCount++;
}

void POGLShaderProgram::Release()
{
	if (--mRefCount == 0) {
		if (mShaderID != 0) {
			glDeleteShader(mShaderID);
			mShaderID = 0;
		}
		delete this;
	}
}

POGLResourceType::Enum POGLShaderProgram::GetResourceType() const
{
	return POGLResourceType::SHADER;
}
