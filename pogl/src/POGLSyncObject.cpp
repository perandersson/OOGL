#include "MemCheck.h"
#include "POGLSyncObject.h"
#include "POGLDeviceContext.h"
#include "POGLDevice.h"
#include <atomic>

POGLSyncObject::POGLSyncObject(GLsync initSync, IPOGLDevice* device)
: mSync(initSync), mDevice(device)
{
}

POGLSyncObject::~POGLSyncObject()
{
	POGLDeviceContext* context = static_cast<POGLDeviceContext*>(mDevice->GetDeviceContext());
	if (mSync != nullptr) {
		std::lock_guard<std::recursive_mutex> wlock(mWriteLock);
		std::lock_guard<std::recursive_mutex> rlock(mReadLock);
		context->DeleteSync(mSync);
		mSync = nullptr;
	}
	context->Release();
}

void POGLSyncObject::WaitSyncDriver(POGLDeviceContext* context)
{
	{
		const GLenum error = glGetError();
		if (error != GL_NO_ERROR)
			THROW_EXCEPTION(POGLException, "Could not wait for driver sync. Reason: %d", error);
	}
	context->WaitSync(GetSyncObject(), 0, GL_TIMEOUT_IGNORED);
	//CHECK_GL("Could not wait for driver sync");
	{ 
		const GLenum error = glGetError(); 
		if (error != GL_NO_ERROR) 
			THROW_EXCEPTION(POGLException, "Could not wait for driver sync. Reason: %d", error); 
	}
}

void POGLSyncObject::WaitSyncClient(POGLDeviceContext* context)
{
	if (!WaitSyncClient(context, GL_TIMEOUT_IGNORED)) {
		THROW_EXCEPTION(POGLSyncException, "Waiting for synchronization failed");
	}
}

bool POGLSyncObject::WaitSyncClient(POGLDeviceContext* context, POGL_UINT64 timeout)
{
	std::lock_guard<std::recursive_mutex> lock(mReadLock);
	const GLenum result = context->ClientWaitSync(GetSyncObject(), 0, timeout);
	CHECK_GL("Could not wait for client sync");
	if (result == GL_WAIT_FAILED) {
		THROW_EXCEPTION(POGLSyncException, "Waiting for synchronization failed");
	}

	return result == GL_ALREADY_SIGNALED || result == GL_CONDITION_SATISFIED;
}

bool POGLSyncObject::WaitSyncClient(POGLDeviceContext* context, POGL_UINT64 timeout, IPOGLWaitSyncJob* job)
{
	std::lock_guard<std::recursive_mutex> lock(mReadLock);
	bool synchronized = true;
	POGL_UINT32 failCount = 0;
	GLsync syncObject = GetSyncObject();
	while (true) {
		const GLenum result = context->ClientWaitSync(syncObject, 0, timeout);

		// On error then throw exception
		if (result == GL_WAIT_FAILED) {
			THROW_EXCEPTION(POGLSyncException, "Waiting for synchronization failed");
		}

		// If okay then return
		if (result == GL_ALREADY_SIGNALED || result == GL_CONDITION_SATISFIED) {
			break;
		}

		if (result == GL_TIMEOUT_EXPIRED) {
			synchronized = job->Execute(context, ++failCount);
			if (!synchronized) {
				break;
			}
		}
	}

	CHECK_GL("Could not wait for client sync");
	return synchronized;
}

void POGLSyncObject::LockRead()
{
	mReadLock.lock();
}

void POGLSyncObject::UnlockRead()
{
	mReadLock.unlock();
}

void POGLSyncObject::LockWrite()
{
	mWriteLock.lock();
}

void POGLSyncObject::UnlockWrite()
{
	mWriteLock.unlock();
}

GLsync POGLSyncObject::GetSyncObject()
{
	std::lock_guard<std::recursive_mutex> lock(mReadLock);
	return mSync;
}

void POGLSyncObject::QueueFence(POGLDeviceContext* context)
{
	LockWrite();
	LockRead();
	GLsync sync = context->FenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
	glFlush();
	context->DeleteSync(mSync);
	mSync = sync;
	UnlockRead();
	UnlockWrite();
	CHECK_GL("Could not queue a new fence object");
}
