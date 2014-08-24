#include "MemCheck.h"
#include "Win32POGLRenderContext.h"
#include "Win32POGLDevice.h"
#include <vector>

Win32POGLRenderContext::Win32POGLRenderContext(POGLDevice* device, HDC deviceContext, HGLRC renderContext)
: POGLRenderContext(device), mRefCount(0), mDeviceContext(deviceContext), mRenderContext(renderContext)
{

}

Win32POGLRenderContext::~Win32POGLRenderContext()
{
	if (mRenderContext != nullptr) {
		wglDeleteContext(mRenderContext);
		mRenderContext = nullptr;
	}
}

void Win32POGLRenderContext::AddRef()
{
	if (++mRefCount == 1) {
		const BOOL current = wglMakeCurrent(mDeviceContext, mRenderContext);
		if (current == FALSE) {
			const DWORD error = GetLastError();
			THROW_EXCEPTION(POGLException, "Could not bind the OpenGL 3.3 render context. Reason: 0x%x", error);
		}
	}
}

void Win32POGLRenderContext::Release()
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
