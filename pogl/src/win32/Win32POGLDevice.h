#pragma once
#include "POGLDevice.h"
#include "Win32POGLDeviceContext.h"
#include <vector>
#include <mutex>

class Win32POGLDevice : public POGLDevice
{
public:
	Win32POGLDevice();
	~Win32POGLDevice();

	void AddRef();
	void Release();
	IPOGLDeviceContext* GetDeviceContext();
	void SwapBuffers();
	bool Initialize(const POGL_DEVICE_INFO* info);

private:
	POGL_UINT32 mRefCount;
	bool mReleasing;
	HWND mWindowHandle;
	HDC mDeviceContext;
	HGLRC mLegacyRenderContext;
	Win32POGLDeviceContext* mMainThreadDeviceContext;

	std::vector<IPOGLDeviceContext*> mDeviceContexts;
	std::recursive_mutex mDeviceContextsMutex;
};
