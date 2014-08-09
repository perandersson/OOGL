#pragma once
#include "config.h"
#include <gl/pogl.h>
#include <mutex>

class POGLDeviceContext;
class POGLSyncObject
{
public:
	POGLSyncObject(GLsync initSync, IPOGLDevice* device);
	~POGLSyncObject();

	void WaitSyncDriver(POGLDeviceContext* context);
	void WaitSyncClient(POGLDeviceContext* context);
	bool WaitSyncClient(POGLDeviceContext* context, POGL_UINT64 timeout);
	bool WaitSyncClient(POGLDeviceContext* context, POGL_UINT64 timeout, IPOGLWaitSyncJob* job);

	/*!
		\brief Lock CPU access to this instance from other threads
	*/
	void LockRead();

	/*!
		\brief Unlocks CPU access to this instance from other threads
	*/
	void UnlockRead();
	
	/*!
		\brief Locks write access to this object
	*/
	void LockWrite();

	/*!
		\brief Unlocks write access to this object
	*/
	void UnlockWrite();
	
	/*!
		\brief Queue a new fence object
	*/
	void QueueFence(POGLDeviceContext* context);

private:
	/*!
		\brief Retrieves the sync object for this resource
	*/
	GLsync GetSyncObject();

private:
	std::recursive_mutex mReadLock;
	std::recursive_mutex mWriteLock;
	GLsync mSync;
	IPOGLDevice* mDevice;
};
