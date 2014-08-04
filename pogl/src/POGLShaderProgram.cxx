#include "POGLShaderProgram.hxx"
#include "POGLDeviceContext.hxx"
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
	if (mShaderID != 0) {
		IPOGLDeviceContext* context = mDevice->GetDeviceContext();
		static_cast<POGLDeviceContext*>(context)->DeleteShader(mShaderID);
		context->Release();
		mShaderID = 0;
	}
}

void POGLShaderProgram::AddRef()
{
	mRefCount++;
}

void POGLShaderProgram::Release()
{
	if (--mRefCount == 0)
		delete this;
}

IPOGLDevice* POGLShaderProgram::GetDevice()
{
	return mDevice;
}

POGL_HANDLE POGLShaderProgram::GetHandlePtr()
{
	return this;
}

POGL_UINT32 POGLShaderProgram::GetUID() const
{
	return mUID;
}

GLuint POGLShaderProgram::GetShaderID() const
{
	return mShaderID;
}
