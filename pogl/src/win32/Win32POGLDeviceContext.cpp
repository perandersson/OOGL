#include "MemCheck.h"
#include "Win32POGLDeviceContext.h"
#include <vector>

Win32POGLDeviceContext::Win32POGLDeviceContext(IPOGLDevice* device, HDC deviceContext, HGLRC renderContext)
: POGLDeviceContext(device), mDeviceContext(deviceContext),
mRenderContext(renderContext), mBoundToThread(false),
// Extensions
wglCreateContextAttribsARB(nullptr), wglChoosePixelFormatARB(nullptr)
{

}

Win32POGLDeviceContext::~Win32POGLDeviceContext()
{
	if (mRenderContext != nullptr) {
		wglDeleteContext(mRenderContext);
		mRenderContext = nullptr;
	}
}

bool Win32POGLDeviceContext::Initialize(Win32POGLDeviceContext* parentContext)
{
	// Activate the legacy render context (which is supplied in the constructor) so 
	// that we can retrieve the necessary extensions
	wglMakeCurrent(mDeviceContext, mRenderContext);
	mRenderContext = nullptr;
	LoadExtensions();
	wglMakeCurrent(nullptr, nullptr);

	//
	// Verify that your graphics card support OpenGL 3.3
	//

	if (wglCreateContextAttribsARB == nullptr) {
		wglMakeCurrent(nullptr, nullptr);
		return false;
	}

	//
	// Set neccessary attributes so that we get an OpenGL 3.3 render context
	//

	std::vector<int> attributes;
	attributes.push_back(WGL_CONTEXT_MAJOR_VERSION_ARB); attributes.push_back(3);
	attributes.push_back(WGL_CONTEXT_MINOR_VERSION_ARB); attributes.push_back(3);
	attributes.push_back(WGL_CONTEXT_PROFILE_MASK_ARB); attributes.push_back(WGL_CONTEXT_CORE_PROFILE_BIT_ARB);
	if (BIT_ISSET(mDevice->GetDeviceInfo()->flags, POGLDeviceInfoFlags::DEBUG_MODE)) {
		attributes.push_back(WGL_CONTEXT_FLAGS_ARB); attributes.push_back(WGL_CONTEXT_DEBUG_BIT_ARB);
	}
	attributes.push_back(0); attributes.push_back(0);

	//
	// Create an OpenGL 3.3 render context
	//

	mRenderContext = (*wglCreateContextAttribsARB)(mDeviceContext, parentContext != nullptr ? parentContext->GetHandlePtr() : 0, &attributes[0]);
	if (mRenderContext == nullptr)
		return false;

	return true;
}

void Win32POGLDeviceContext::Bind()
{
	if (!mBoundToThread) {
		wglMakeCurrent(mDeviceContext, mRenderContext);
		mBoundToThread = true;

		// Make sure to load the extensions from the new library
		LoadExtensions();

		InitializeRenderState();
	}
}

void Win32POGLDeviceContext::Unbind()
{
	assert_with_message(mBoundToThread, "Why is this not bound to the current thread?");
	wglMakeCurrent(0, 0);
	mBoundToThread = false;
}

void Win32POGLDeviceContext::LoadExtensions()
{
	SET_EXTENSION_FUNC(PFNWGLCREATECONTEXTATTRIBSARBPROC, wglCreateContextAttribsARB);
	SET_EXTENSION_FUNC(PFNWGLCHOOSEPIXELFORMATARBPROC, wglChoosePixelFormatARB);

	POGLDeviceContext::LoadExtensions();
}

void* Win32POGLDeviceContext::GetProcAddress(const char* functionName)
{
	return wglGetProcAddress(functionName);
}
