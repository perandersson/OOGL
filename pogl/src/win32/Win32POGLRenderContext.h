#pragma once
#include "POGLRenderContext.h"

class POGLAPI Win32POGLRenderContext : public POGLRenderContext
{
public:
	Win32POGLRenderContext(IPOGLDevice* device, HDC deviceContext, HGLRC renderContext);
	~Win32POGLRenderContext();

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
