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
		std::lock_guard<std::recursive_mutex> lock(mSyncMutex);
		context->DeleteSync(mSync);
		mSync = nullptr;
	}
	context->Release();
}

void POGLSyncObject::WaitSyncDriver()
{
	std::lock_guard<std::recursive_mutex> lock(mGlobalMutex);
	POGLDeviceContext* context = static_cast<POGLDeviceContext*>(mDevice->GetDeviceContext());
	context->WaitSync(GetSyncObject(), 0, GL_TIMEOUT_IGNORED);
	context->Release();
	CHECK_GL("Could not wait for driver sync");
}

void POGLSyncObject::WaitSyncClient()
{
	if (!WaitSyncClient(GL_TIMEOUT_IGNORED)) {
		THROW_EXCEPTION(POGLSyncException, "Waiting for synchronization failed");
	}
}

bool POGLSyncObject::WaitSyncClient(POGL_UINT64 timeout)
{
	std::lock_guard<std::recursive_mutex> lock(mGlobalMutex);
	POGLDeviceContext* context = static_cast<POGLDeviceContext*>(mDevice->GetDeviceContext());
	const GLenum result = context->ClientWaitSync(GetSyncObject(), 0, timeout);
	context->Release();
	CHECK_GL("Could not wait for client sync");
	if (result == GL_WAIT_FAILED) {
		context->Release();
		THROW_EXCEPTION(POGLSyncException, "Waiting for synchronization failed");
	}

	return result == GL_ALREADY_SIGNALED || result == GL_CONDITION_SATISFIED;
}

bool POGLSyncObject::WaitSyncClient(POGL_UINT64 timeout, IPOGLWaitSyncJob* job)
{
	std::lock_guard<std::recursive_mutex> lock(mGlobalMutex);
	POGLDeviceContext* context = static_cast<POGLDeviceContext*>(mDevice->GetDeviceContext());
	bool synchronized = true;
	POGL_UINT32 failCount = 0;
	GLsync syncObject = GetSyncObject();
	while (true) {
		const GLenum result = context->ClientWaitSync(syncObject, 0, timeout);

		// On error then throw exception
		if (result == GL_WAIT_FAILED) {
			context->Release();
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

	context->Release();
	CHECK_GL("Could not wait for client sync");
	return synchronized;
}

void POGLSyncObject::Lock()
{
	mGlobalMutex.lock();
}

void POGLSyncObject::Unlock()
{
	mGlobalMutex.unlock();
}

GLsync POGLSyncObject::GetSyncObject()
{
	std::lock_guard<std::recursive_mutex> lock(mSyncMutex);
	return mSync;
}

void POGLSyncObject::QueueFence()
{
	std::lock_guard<std::recursive_mutex> global_lock(mGlobalMutex);
	POGLDeviceContext* context = static_cast<POGLDeviceContext*>(mDevice->GetDeviceContext());
	GLsync sync = context->FenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
	std::lock_guard<std::recursive_mutex> lock(mSyncMutex);
	context->DeleteSync(mSync);
	context->Release();
	mSync = sync;
}
