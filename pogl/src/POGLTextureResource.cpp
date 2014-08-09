#include "MemCheck.h"
#include "POGLTextureResource.h"
#include "POGLDeviceContext.h"
#include "POGLSyncObject.h"
#include <atomic>
namespace {
	std::atomic<POGL_UINT32> uid;
	POGL_UINT32 GenTextureUID() {
		return ++uid;
	}
}

POGLTextureResource::POGLTextureResource(GLuint textureID, GLenum textureTarget, POGLTextureFormat::Enum format, POGLSyncObject* syncObject, IPOGLDevice* device)
: mRefCount(1), mUID(GenTextureUID()), mTextureID(textureID), mTextureTarget(textureTarget), mTextureFormat(format), mSyncObject(syncObject), mDevice(device)
{

}

POGLTextureResource::~POGLTextureResource()
{
}

void POGLTextureResource::AddRef()
{
	mRefCount++;
}

void POGLTextureResource::Release()
{
	if (--mRefCount == 0) {
		if (mTextureID != 0) {
			glDeleteTextures(1, &mTextureID);
			mTextureID = 0;
		}
		if (mSyncObject != nullptr) {
			delete mSyncObject;
			mSyncObject = nullptr;
		}
		delete this;
	}
}

void POGLTextureResource::WaitSyncDriver(IPOGLDeviceContext* context)
{
	mSyncObject->WaitSyncDriver(static_cast<POGLDeviceContext*>(context));
}

void POGLTextureResource::WaitSyncClient(IPOGLDeviceContext* context)
{
	mSyncObject->WaitSyncClient(static_cast<POGLDeviceContext*>(context));
}

bool POGLTextureResource::WaitSyncClient(IPOGLDeviceContext* context, POGL_UINT64 timeout)
{
	return mSyncObject->WaitSyncClient(static_cast<POGLDeviceContext*>(context), timeout);
}

bool POGLTextureResource::WaitSyncClient(IPOGLDeviceContext* context, POGL_UINT64 timeout, IPOGLWaitSyncJob* job)
{
	return mSyncObject->WaitSyncClient(static_cast<POGLDeviceContext*>(context), timeout, job);
}

POGL_UINT32 POGLTextureResource::GetUID() const
{
	return mUID;
}

GLuint POGLTextureResource::GetTextureID() const
{
	return mTextureID;
}

GLenum POGLTextureResource::GetTextureTarget() const
{
	return mTextureTarget;
}

POGLTextureFormat::Enum POGLTextureResource::GetTextureFormat() const
{
	return mTextureFormat;
}
