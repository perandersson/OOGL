#pragma once
#include "POGLDevice.h"
#include "Win32POGLDeviceContext.h"
#include <list>
#include <vector>
#include <mutex>

class POGLAPI Win32POGLDevice : public POGLDevice
{
public:
	Win32POGLDevice(const POGL_DEVICE_INFO* info);
	~Win32POGLDevice();

	/*!
		Initializes this device
	*/
	void Initialize();
	
// IPOGLInterface
public:
	virtual void AddRef();
	virtual void Release();

// IPOGLDevice
public:
	virtual IPOGLDeviceContext* GetDeviceContext();
	virtual IPOGLDeferredDeviceContext* CreateDeferredDeviceContext();
	virtual void EndFrame();
	
private:
	/*!
		\brief Creates a new render OpenGL 3.3 RenderContext
	*/
	Win32POGLDeviceContext* CreateRenderContext();

private:
	REF_COUNTER mRefCount;
	bool mReleasing;
	HWND mHWND;
	HDC mDC;
	
	Win32POGLDeviceContext* mDeviceContext;
};
