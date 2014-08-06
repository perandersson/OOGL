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

POGLShaderProgram::POGLShaderProgram(GLuint shaderID, IPOGLDevice* device, POGLShaderProgramType::Enum type, POGLSyncObject* syncObject)
: mRefCount(1), mUID(GenShaderProgramUID()), mShaderID(shaderID), mDevice(device), mType(type), mSyncObject(syncObject)
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
		if (mSyncObject != nullptr) {
			delete mSyncObject;
			mSyncObject = nullptr;
		}
		context->Release();
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

void POGLShaderProgram::WaitSyncDriver()
{
	mSyncObject->WaitSyncDriver();
}

void POGLShaderProgram::WaitSyncClient()
{
	mSyncObject->WaitSyncClient();
}

bool POGLShaderProgram::WaitSyncClient(POGL_UINT64 timeout)
{
	return mSyncObject->WaitSyncClient(timeout);
}

bool POGLShaderProgram::WaitSyncClient(POGL_UINT64 timeout, IPOGLWaitSyncJob* job)
{
	return mSyncObject->WaitSyncClient(timeout, job);
}

POGL_UINT32 POGLShaderProgram::GetUID() const
{
	return mUID;
}

GLuint POGLShaderProgram::GetShaderID() const
{
	return mShaderID;
}
