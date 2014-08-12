#include "MemCheck.h"
#include "POGLShaderProgram.h"
#include "POGLDeviceContext.h"
#include "POGLSyncObject.h"
#include <atomic>

namespace {
	std::atomic<POGL_UINT32> uid;
	POGL_UINT32 GenShaderProgramUID() {
		return ++uid;
	}
}

POGLShaderProgram::POGLShaderProgram(GLuint shaderID, IPOGLDevice* device, POGLShaderProgramType::Enum type)
: mRefCount(1), mUID(GenShaderProgramUID()), mShaderID(shaderID), mDevice(device), mType(type)
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
		POGLDeviceContext* context = static_cast<POGLDeviceContext*>(mDevice->GetDeviceContext());
		if (mShaderID != 0) {
			context->DeleteShader(mShaderID);
			mShaderID = 0;
		}
		context->Release();
		delete this;
	}
}

IPOGLDevice* POGLShaderProgram::GetDevice()
{
	mDevice->AddRef();
	return mDevice;
}

POGL_HANDLE POGLShaderProgram::GetHandlePtr()
{
	return nullptr;
}

POGL_UINT32 POGLShaderProgram::GetUID() const
{
	return mUID;
}

GLuint POGLShaderProgram::GetShaderID() const
{
	return mShaderID;
}
