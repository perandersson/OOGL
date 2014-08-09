#pragma once
#include "POGLDevice.h"
#include "Win32POGLDeviceContext.h"
#include <list>
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
	
	/*!
		\brief Release the supplied render context and return it to the free RenderContext's list.
	*/
	void ReleaseRenderContext(Win32POGLDeviceContext* renderContext);

private:
	/*!
		\brief Retrieve a free render context. 

		This method creates a render context if no free render context is found

		\return A render context
	*/
	Win32POGLDeviceContext* GetOrCreateRenderContext();

private:
	POGL_UINT32 mRefCount;
	bool mReleasing;
	HWND mWindowHandle;
	HDC mDeviceContext;
	HGLRC mLegacyRenderContext;
	
	std::vector<Win32POGLDeviceContext*> mDeviceContexts;
	std::list<Win32POGLDeviceContext*> mFreeDeviceContexts;
	std::recursive_mutex mDeviceContextsMutex;
};
