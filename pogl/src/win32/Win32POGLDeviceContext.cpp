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
		wglDeleteContext(mRenderContext);
		mRenderContext = nullptr;
	}
}

void Win32POGLDeviceContext::AddRef()
{
	if (++mRefCount == 1) {
		wglMakeCurrent(mDeviceContext, mRenderContext);
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
		wglMakeCurrent(nullptr, nullptr);
		static_cast<Win32POGLDevice*>(mDevice)->ReleaseRenderContext(this);
	}
}

void* Win32POGLDeviceContext::GetProcAddress(const char* functionName)
{
	return wglGetProcAddress(functionName);
}
