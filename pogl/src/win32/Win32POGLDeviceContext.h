#pragma once
#include "POGLDeviceContext.h"

class Win32POGLDeviceContext : public POGLDeviceContext
{
public:
	Win32POGLDeviceContext(IPOGLDevice* device, HDC deviceContext, HGLRC renderContext);
	~Win32POGLDeviceContext();

	void Bind();
	void Unbind();
	bool Initialize(Win32POGLDeviceContext* parentContext);

	/*!
		\brief Retrieves the handle for this context
	*/
	inline HGLRC GetHandlePtr() const {
		return mRenderContext;
	}

	void LoadExtensions();
	void* GetProcAddress(const char* functionName);

private:
	HDC mDeviceContext;
	HGLRC mRenderContext;
	bool mBoundToThread;

	//
	// Extensions
	//

	PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
	PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
};
