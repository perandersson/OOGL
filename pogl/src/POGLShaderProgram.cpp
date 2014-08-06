#include "MemCheck.h"
#include "POGLShaderProgram.h"
#include "POGLDeviceContext.h"
#include <atomic>

namespace {
	std::atomic<POGL_UINT32> uid;
	POGL_UINT32 GenShaderProgramUID() {
		return ++uid;
	}
}

POGLShaderProgram::POGLShaderProgram(GLuint shaderID, IPOGLDevice* device, POGLShaderProgramType::Enum type, GLsync initSync)
: mRefCount(1), mUID(GenShaderProgramUID()), mShaderID(shaderID), mDevice(device), mType(type), mSync(initSync)
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
			IPOGLDeviceContext* context = mDevice->GetDeviceContext();
			static_cast<POGLDeviceContext*>(context)->DeleteShader(mShaderID);
			context->Release();
			mShaderID = 0;
		}
		delete this;
	}
}

IPOGLDevice* POGLShaderProgram::GetDevice()
{
	return mDevice;
}

POGL_HANDLE POGLShaderProgram::GetHandlePtr()
{
	return this;
}

void POGLShaderProgram::WaitSyncDriver(POGLResourceFenceType::Enum e)
{
	static const POGL_UINT64 TIMEOUT_INFINITE = UINT64_MAX;
	WaitSyncDriver(e, TIMEOUT_INFINITE);
}

void POGLShaderProgram::WaitSyncDriver(POGLResourceFenceType::Enum e, POGL_UINT64 timeout)
{
	IPOGLDeviceContext* context = mDevice->GetDeviceContext();
	static_cast<POGLDeviceContext*>(context)->WaitSync(mSync, 0, timeout);
	context->Release();
}

void POGLShaderProgram::WaitSyncClient(POGLResourceFenceType::Enum e)
{
	static const POGL_UINT64 TIMEOUT_INFINITE = UINT64_MAX;
	WaitSyncClient(e, TIMEOUT_INFINITE);
}

bool POGLShaderProgram::WaitSyncClient(POGLResourceFenceType::Enum e, POGL_UINT64 timeout)
{
	POGLDeviceContext* context = static_cast<POGLDeviceContext*>(mDevice->GetDeviceContext());
	const GLenum syncResult = context->ClientWaitSync(mSync, 0, timeout);
	context->Release();
	if (syncResult == GL_WAIT_FAILED) {
		context->Release();
		THROW_EXCEPTION(POGLSyncException, "Waiting for synchronization failed");
	}

	return syncResult == GL_ALREADY_SIGNALED || syncResult == GL_CONDITION_SATISFIED;
}

bool POGLShaderProgram::WaitSyncClient(POGLResourceFenceType::Enum e, POGL_UINT64 timeout)
{
	POGLDeviceContext* context = static_cast<POGLDeviceContext*>(mDevice->GetDeviceContext());
	GLenum result = GL_TIMEOUT_EXPIRED;
	do {

	} while (true);

	while ((result = context->ClientWaitSync(mSync, 0, timeout) )
	const GLenum syncResult = context->ClientWaitSync(mSync, 0, timeout);
	context->Release();
	if (syncResult == GL_WAIT_FAILED) {
		context->Release();
		THROW_EXCEPTION(POGLSyncException, "Waiting for synchronization failed");
	}

	return syncResult == GL_ALREADY_SIGNALED || syncResult == GL_CONDITION_SATISFIED;
}

POGL_UINT32 POGLShaderProgram::GetUID() const
{
	return mUID;
}

GLuint POGLShaderProgram::GetShaderID() const
{
	return mShaderID;
}
