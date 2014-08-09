#pragma once
#include "POGLDeviceContext.h"

class Win32POGLDeviceContext : public POGLDeviceContext
{
public:
	Win32POGLDeviceContext(IPOGLDevice* device, HDC deviceContext, HGLRC legacyRenderContext);
	~Win32POGLDeviceContext();

	void Bind();
	void Unbind();
	void Initialize(Win32POGLDeviceContext* parentContext);

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
	HGLRC mLegacyRenderContext;
	HGLRC mRenderContext;
	bool mBoundToThread;

	//
	// Extensions
	//

	PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
	PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
};
