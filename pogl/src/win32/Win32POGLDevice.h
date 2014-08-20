#pragma once
#include "POGLDevice.h"
#include "Win32POGLRenderContext.h"
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
	virtual IPOGLRenderContext* GetRenderContext();
	virtual IPOGLDeferredRenderContext* CreateDeferredRenderContext();
	virtual void EndFrame();
	
private:
	/*!
		\brief Creates a new render OpenGL 3.3 RenderContext
	*/
	Win32POGLRenderContext* CreateRenderContext();

private:
	REF_COUNTER mRefCount;
	bool mReleasing;
	HWND mHWND;
	HDC mDC;
	
	Win32POGLRenderContext* mRenderContext;
};
