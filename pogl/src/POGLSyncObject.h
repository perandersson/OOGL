#pragma once
#include "config.h"
#include <gl/pogl.h>
#include <mutex>

class POGLSyncObject
{
public:
	POGLSyncObject(GLsync initSync, IPOGLDevice* device);
	~POGLSyncObject();

	void WaitSyncDriver();
	void WaitSyncClient();
	bool WaitSyncClient(POGL_UINT64 timeout);
	bool WaitSyncClient(POGL_UINT64 timeout, IPOGLWaitSyncJob* job);

	/*!
		\brief Lock
	*/
	void Lock();

	/*!
	
	*/
	void Unlock();
	
	/*!
		\brief Queue a new fence object
	*/
	void QueueFence();

private:
	/*!
		\brief Retrieves the sync object for this resource
	*/
	GLsync GetSyncObject();

private:
	std::recursive_mutex mSyncMutex;
	std::recursive_mutex mGlobalMutex;
	GLsync mSync;
	IPOGLDevice* mDevice;
};
