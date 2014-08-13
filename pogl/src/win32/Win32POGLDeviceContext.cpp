#include "MemCheck.h"
#include "Win32POGLDeviceContext.h"
#include "Win32POGLDevice.h"
#include <vector>

Win32POGLDeviceContext::Win32POGLDeviceContext(IPOGLDevice* device, HDC deviceContext, HGLRC renderContext)
: POGLDeviceContext(device), mRefCount(0), mDeviceContext(deviceContext), mRenderContext(renderContext)
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
		const BOOL current = wglMakeCurrent(mDeviceContext, mRenderContext);
		if (current == FALSE) {
			const DWORD error = GetLastError();
			THROW_EXCEPTION(POGLException, "Could not bind the OpenGL 3.3 render context. Reason: 0x%x", error);
		}
	}
}

void Win32POGLDeviceContext::Release()
{
	assert_with_message(mRefCount > 0, "You are calling Release more often than to AddRef");

	if (--mRefCount == 0) {
		if (!wglMakeCurrent(nullptr, nullptr)) {
			const DWORD error = GetLastError();
			THROW_EXCEPTION(POGLException, "Could not unbind the OpenGL 3.3 render context. Reason: 0x%x", error);
		}

		delete this;
	}
}
