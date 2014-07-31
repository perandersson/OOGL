#pragma once
#include "POGLDeviceContext.hxx"

class Win32POGLDeviceContext : public POGLDeviceContext
{
public:
	Win32POGLDeviceContext(IPOGLDevice* device, HDC deviceContext);
	~Win32POGLDeviceContext();
	
	void Release();
	bool Initialize(Win32POGLDeviceContext* parentContext);
	Win32POGLDeviceContext* BindContextIfNeccessary();

	/*!
		\brief Retrieves the handle for this context
	*/
	inline HGLRC GetHandlePtr() const {
		return mRenderContext;
	}

private:
	void LoadExtensions();

private:
	HDC mDeviceContext;
	HGLRC mRenderContext;
	bool mBoundToThread;

	//
	// Extensions
	//

	PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
};
