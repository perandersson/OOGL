#pragma once
#include "POGLDeviceContext.h"

class POGLAPI Win32POGLDeviceContext : public POGLDeviceContext
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

// IPOGLInterface
public:
	virtual void AddRef();
	virtual void Release();

private:
	REF_COUNTER mRefCount;
	HDC mDeviceContext;
	HGLRC mRenderContext;
};
