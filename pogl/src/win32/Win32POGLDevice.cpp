#include "MemCheck.h"
#include "Win32POGLDevice.h"
#include "POGLDeferredRenderContext.h"
#include "providers/POGLDefaultBufferResourceProvider.h"
#include "providers/POGLAMDBufferResourceProvider.h"
#include <algorithm>

/* Memory Leak Detection */
int gPOGDebugFlag;

void POGLEnableMemoryLeakDetection()
{
#ifdef _DEBUG
	gPOGDebugFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	_CrtSetDbgFlag(gPOGDebugFlag | _CRTDBG_LEAK_CHECK_DF);
#endif
}

/////////////////////////////////////

Win32POGLDevice::Win32POGLDevice(const POGL_DEVICE_INFO* info)
: POGLDevice(info), mRefCount(1), mReleasing(false),
mHWND(nullptr), mDC(nullptr), mRenderContext(nullptr), mBufferResourceProvider(nullptr)
{

}

Win32POGLDevice::~Win32POGLDevice()
{
}

void Win32POGLDevice::AddRef()
{
	mRefCount++;
}

void Win32POGLDevice::Release()
{
	if (--mRefCount == 0 && !mReleasing) {
		mReleasing = true;
				
		if (mRenderContext != nullptr) {
			mRenderContext->Destroy();
			mRenderContext->Release();
			mRenderContext = nullptr;
		}
		
		if (mBufferResourceProvider != nullptr) {
			delete mBufferResourceProvider;
			mBufferResourceProvider = nullptr;
		}

		if (mDC != nullptr && mHWND != nullptr) {
			ReleaseDC(mHWND, mDC);
			mDC = nullptr;
			mHWND = nullptr;
		}

		delete this;
	}
}

IPOGLRenderContext* Win32POGLDevice::GetRenderContext()
{
	mRenderContext->AddRef();
	return mRenderContext;
}

IPOGLDeferredRenderContext* Win32POGLDevice::CreateDeferredRenderContext()
{
	POGLDeferredRenderContext* context = new POGLDeferredRenderContext(this);
	return context;
}

void Win32POGLDevice::EndFrame()
{
	if (!SwapBuffers(mDC)) {
		const DWORD error = GetLastError();
		THROW_EXCEPTION(POGLException, "Could not swap buffers. Reason: 0x%x", error);
	}

	CHECK_GL("Could not swap buffers");
}

void Win32POGLDevice::Initialize()
{
	static const BYTE PIXELTYPES[POGLPixelFormat::COUNT] = {
		PFD_TYPE_RGBA
	};

	mHWND = (HWND)mDeviceInfo.windowHandle;
	mDC = GetDC(mHWND);
	if (mDC == nullptr) {
		const DWORD error = GetLastError();
		THROW_EXCEPTION(POGLInitializationException, "Could not retrieve windows device context handle. Reason: 0x%x", error);		
	}

	PIXELFORMATDESCRIPTOR pfd = { 0 };
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PIXELTYPES[(POGL_UINT32)mDeviceInfo.pixelFormat];
	pfd.cColorBits = (BYTE)mDeviceInfo.colorBits;
	pfd.cDepthBits = (BYTE)mDeviceInfo.depthBits;
	pfd.iLayerType = PFD_MAIN_PLANE;

	const int format = ChoosePixelFormat(mDC, &pfd);
	if (!format) {
		const DWORD error = GetLastError();
		ReleaseDC(mHWND, mDC); mDC = nullptr; mHWND = nullptr;
		THROW_EXCEPTION(POGLInitializationException, "Could not choose pixel format. Reason: 0x%x", error);
	}

	if (!SetPixelFormat(mDC, format, &pfd)) {
		const DWORD error = GetLastError();
		ReleaseDC(mHWND, mDC); mDC = nullptr; mHWND = nullptr;
		THROW_EXCEPTION(POGLInitializationException, "Could not set pixel format. Reason: 0x%x", error);
	}

	// Create a legacy render context, used so that we can load the neccessary extensions 
	HGLRC legacyRenderContext = wglCreateContext(mDC);
	if (!legacyRenderContext) {
		const DWORD error = GetLastError();
		ReleaseDC(mHWND, mDC); mDC = nullptr; mHWND = nullptr;
		THROW_EXCEPTION(POGLInitializationException, "Could not create legacy OpenGL render context. Reason: 0x%x", error);
	}

	// Make the legacy render context current so that we can load the neccessary OpenGL extensions
	if (!wglMakeCurrent(mDC, legacyRenderContext)) {
		const DWORD error = GetLastError();
		wglDeleteContext(legacyRenderContext);
		ReleaseDC(mHWND, mDC); mDC = nullptr; mHWND = nullptr;
		THROW_EXCEPTION(POGLInitializationException, "Could not bind the legacy render context. Reason: 0x%x", error);
	}

	// Load extensions for the Legacy OpenGL RenderContext
	if (!POGLLoadExtensions()) {
		wglDeleteContext(legacyRenderContext);
		ReleaseDC(mHWND, mDC); mDC = nullptr; mHWND = nullptr;
		THROW_EXCEPTION(POGLInitializationException, "Could not load OpenGL extensions");
	}

	// Verify OpenGL 3.3
	if (wglCreateContextAttribsARB == nullptr) {
		ReleaseDC(mHWND, mDC); mDC = nullptr; mHWND = nullptr;
		THROW_EXCEPTION(POGLException, "Your computer does not support OpenGL 3.3. Make sure that you have the latest graphics-card drivers installed");
	}

	// Create the OpenGL 3.3 render context
	mRenderContext = CreateRenderContext();

	// Legacy Render context is no longer needed
	wglDeleteContext(legacyRenderContext);

	// Ensure that we have the reference for it
	mRenderContext->AddRef();

	// Load extensions for the OpenGL 3.3 RenderContext
	if (!POGLLoadExtensions()) {
		mRenderContext->Release();
		ReleaseDC(mHWND, mDC); mDC = nullptr; mHWND = nullptr;
		THROW_EXCEPTION(POGLInitializationException, "Could not load OpenGL extensions");
	}

	// Prepare the resource providers
	bool amdPinnedMemory = POGLExtensionAvailable(POGL_TOCHAR("GL_AMD_pinned_memory"));
	if (amdPinnedMemory) {
		mBufferResourceProvider = new POGLAMDBufferResourceProvider();
	}
	else {
		mBufferResourceProvider = new POGLDefaultBufferResourceProvider();
	}
	mRenderContext->InitializeRenderState();
}


IPOGLBufferResourceProvider* Win32POGLDevice::GetBufferResourceProvider()
{
	return mBufferResourceProvider;
}

Win32POGLRenderContext* Win32POGLDevice::CreateRenderContext()
{
	//
	// Set neccessary attributes so that we get an OpenGL 3.3 render context
	//

	std::vector<int> attributes;
	attributes.push_back(WGL_CONTEXT_MAJOR_VERSION_ARB); attributes.push_back(3);
	attributes.push_back(WGL_CONTEXT_MINOR_VERSION_ARB); attributes.push_back(3);
	attributes.push_back(WGL_CONTEXT_PROFILE_MASK_ARB); attributes.push_back(WGL_CONTEXT_CORE_PROFILE_BIT_ARB);
	if (BIT_ISSET(mDeviceInfo.flags, POGLDeviceInfoFlags::DEBUG_MODE)) {
		attributes.push_back(WGL_CONTEXT_FLAGS_ARB); attributes.push_back(WGL_CONTEXT_DEBUG_BIT_ARB);
	}
	attributes.push_back(0); attributes.push_back(0);

	//
	// Create an OpenGL 3.3 render context
	//

	HGLRC renderContext = wglCreateContextAttribsARB(mDC, nullptr, &attributes[0]);
	if (renderContext == nullptr) {
		const DWORD error = GetLastError();
		THROW_EXCEPTION(POGLException, "Failed to create an OpenGL 3.3 render context. Reason: 0x%x", error);
	}

	return new Win32POGLRenderContext(this, mDC, renderContext);
}

//
// Win32 variant of the IPOGLDevice interface
//

IPOGLDevice* POGLCreateDevice(const POGL_DEVICE_INFO* info)
{
	POGLEnableMemoryLeakDetection();

	Win32POGLDevice* device = new Win32POGLDevice(info);
	device->Initialize();
	return device;
}
