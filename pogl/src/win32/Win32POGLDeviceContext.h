#pragma once
#include "POGLDeviceContext.h"

class Win32POGLDeviceContext : public POGLDeviceContext
{
public:
	Win32POGLDeviceContext(IPOGLDevice* device, HDC deviceContext);
	~Win32POGLDeviceContext();
	
	void AddRef();
	void Release();
	bool Initialize(Win32POGLDeviceContext* parentContext);
	Win32POGLDeviceContext* BindContextIfNeccessary();

	/*!
		\brief Retrieves the handle for this context
	*/
	inline HGLRC GetHandlePtr() const {
		return mRenderContext;
	}

	void LoadExtensions();
	void* GetProcAddress(const char* functionName);

private:
	POGL_UINT32 mRefCount;
	HDC mDeviceContext;
	HGLRC mRenderContext;
	bool mBoundToThread;

	//
	// Extensions
	//

	PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
};
