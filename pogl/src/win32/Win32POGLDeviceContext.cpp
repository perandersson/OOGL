#include "MemCheck.h"
#include "Win32POGLDeviceContext.h"
#include "Win32POGLDevice.h"
#include <vector>

Win32POGLDeviceContext::Win32POGLDeviceContext(IPOGLDevice* device, HDC deviceContext, HGLRC renderContext)
: POGLDeviceContext(device), mRefCount(0), mDeviceContext(deviceContext), mRenderContext(renderContext), mInitialized(false)
{

}

Win32POGLDeviceContext::~Win32POGLDeviceContext()
{
	if (mRenderContext != nullptr) {
		wglMakeCurrent(mDeviceContext, mRenderContext);
		wglDeleteContext(mRenderContext);
		mRenderContext = nullptr;
	}
}

void Win32POGLDeviceContext::AddRef()
{
	if (++mRefCount == 1) {
		const BOOL current = wglMakeCurrent(mDeviceContext, mRenderContext);
		if (current == FALSE) {
			const DWORD error = GetLastError();
			THROW_EXCEPTION(POGLException, "Could not bind the OpenGL 3.3 render context. Reason: 0x%x", error);
		}
		if (!mInitialized) {
			mInitialized = true;
			LoadExtensions();
			InitializeRenderState();
		}
	}
}

void Win32POGLDeviceContext::Release()
{
	assert_with_message(mRefCount > 0, "You are calling Release more often than to AddRef");

	if (--mRefCount == 0) {
		const BOOL current = wglMakeCurrent(nullptr, nullptr);
		if (current == FALSE) {
			const DWORD error = GetLastError();
			THROW_EXCEPTION(POGLException, "Could not unbind the OpenGL 3.3 render context. Reason: 0x%x", error);
		}
		static_cast<Win32POGLDevice*>(mDevice)->ReleaseRenderContext(this);
	}
}

void* Win32POGLDeviceContext::GetProcAddress(const char* functionName)
{
	return wglGetProcAddress(functionName);
}
