#pragma once
#include "POGLDeviceContext.h"

class Win32POGLDeviceContext : public POGLDeviceContext
{
public:
	Win32POGLDeviceContext(IPOGLDevice* device, HDC deviceContext, HGLRC renderContext);
	~Win32POGLDeviceContext();

	/*!
		\brief Retrieves the windows specific render context
	*/
	inline HGLRC GetHGLRC() {
		return mRenderContext;
	}

	void AddRef();
	void Release();

	void* GetProcAddress(const char* functionName);

private:
	POGL_UINT32 mRefCount;
	HDC mDeviceContext;
	HGLRC mRenderContext;
};
