#include "MemCheck.h"
#include "Win32POGLDevice.h"
#include "Win32POGLDeviceContext.h"
#include <algorithm>

/* Thread local container for the device context */
THREAD_LOCAL Win32POGLDeviceContext* tDeviceContext = nullptr;

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
mWindowHandle(nullptr), mDeviceContext(nullptr)
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
		
		//
		// Destroy the render contexts created by this device. Destroy the main render context last
		//
		
		int size = mDeviceContexts.size();
		for (int i = size - 1; i >= 0; --i) {
			mDeviceContexts[i]->Destroy();
			auto it = std::find(mFreeDeviceContexts.begin(), mFreeDeviceContexts.end(), mDeviceContexts[i]);
			if (it != mFreeDeviceContexts.end())
				mFreeDeviceContexts.erase(it);
			if (i == 0) 
				mDeviceContexts[i]->Release();
			delete mDeviceContexts[i];
			mDeviceContexts[i] = nullptr;
		}
		mDeviceContexts.clear();
		mFreeDeviceContexts.clear();

		if (mDeviceContext != nullptr) {
			ReleaseDC(mWindowHandle, mDeviceContext);
			mDeviceContext = nullptr;
			mWindowHandle = nullptr;
		}

		delete this;
	}
}

IPOGLDeviceContext* Win32POGLDevice::GetDeviceContext()
{
	if (tDeviceContext == nullptr) {
		Win32POGLDeviceContext* context = TryGetRenderContext();
		tDeviceContext = context;
	}
	tDeviceContext->AddRef();
	return tDeviceContext;
}

void Win32POGLDevice::ReleaseRenderContext(Win32POGLDeviceContext* renderContext)
{
	std::lock_guard<std::recursive_mutex> lock(mDeviceContextsMutex);
	mFreeDeviceContexts.push_back(renderContext);
	tDeviceContext = nullptr;
}

void Win32POGLDevice::EndFrame()
{
	::SwapBuffers(mDeviceContext);

	CHECK_GL("Could not swap buffers");
}

bool Win32POGLDevice::Initialize()
{
	static const BYTE PIXELTYPES[POGLPixelFormat::COUNT] = {
		PFD_TYPE_RGBA
	};

	mWindowHandle = (HWND)mDeviceInfo.windowHandle;
	mDeviceContext = GetDC(mWindowHandle);
	if (mDeviceContext == nullptr)
		return false;

	PIXELFORMATDESCRIPTOR pfd = { 0 };
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PIXELTYPES[(POGL_UINT32)mDeviceInfo.pixelFormat];
	pfd.cColorBits = (BYTE)mDeviceInfo.colorBits;
	pfd.cDepthBits = (BYTE)mDeviceInfo.depthBits;
	pfd.iLayerType = PFD_MAIN_PLANE;

	const int format = ChoosePixelFormat(mDeviceContext, &pfd);
	if (!format)
		return false;

	if (!SetPixelFormat(mDeviceContext, format, &pfd))
		return false;

	// Create a legacy render context, used so that we can load the neccessary extensions 
	HGLRC legacyRenderContext = wglCreateContext(mDeviceContext);
	if (!legacyRenderContext)
		return false;

	BOOL current = wglMakeCurrent(mDeviceContext, legacyRenderContext);
	if (current == FALSE) {
		const DWORD error = GetLastError();
		THROW_EXCEPTION(POGLException, "Could not bind the legacy render context. Reason: 0x%x", error);
	}

	Win32POGLDeviceContext* mainRenderContext = CreateRenderContext(nullptr);
	mainRenderContext->AddRef();
	if (current == FALSE) {
		const DWORD error = GetLastError();
		THROW_EXCEPTION(POGLException, "Could not bind the new OpenGL 3.3 render context. Reason: 0x%x", error);
	}
	mFreeDeviceContexts.push_back(mainRenderContext);

	// Create render contexts that share resources with the main render context
	const POGL_UINT8 count = GetMaxRenderContexts();
	HGLRC parent = mainRenderContext->GetHGLRC();
	for (POGL_UINT8 i = 0; i < count - 1; ++i) {
		Win32POGLDeviceContext* ctx = CreateRenderContext(parent);
		mFreeDeviceContexts.push_back(ctx);
	}
	wglDeleteContext(legacyRenderContext);
	return POGLDevice::Initialize();
}

Win32POGLDeviceContext* Win32POGLDevice::TryGetRenderContext()
{
	std::lock_guard<std::recursive_mutex> lock(mDeviceContextsMutex);
	if (mFreeDeviceContexts.empty()) {
		THROW_EXCEPTION(POGLResourceException, "There are no more free render contexts available.\
											   \nThe render context count is now: %d. Increase the maximum amount of render contexts and try again", GetMaxRenderContexts());
	}

	auto it = (*mFreeDeviceContexts.begin());
	mFreeDeviceContexts.pop_front();
	return it;
}

Win32POGLDeviceContext* Win32POGLDevice::CreateRenderContext(HGLRC parent)
{
	//
	// Activate the legacy render context (which is supplied in the constructor) so 
	// that we can retrieve the necessary extensions
	//
	
	PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
	if (wglCreateContextAttribsARB == nullptr)
		THROW_EXCEPTION(POGLException, "Your computer does not support OpenGL 3.3. Make sure that you have the latest graphics-card drivers installed");

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

	HGLRC renderContext = (*wglCreateContextAttribsARB)(mDeviceContext, parent, &attributes[0]);
	if (renderContext == nullptr) {
		const DWORD error = GetLastError();
		THROW_EXCEPTION(POGLException, "Failed to create an OpenGL 3.3 render context. Reason: 0x%x", error);
	}

	Win32POGLDeviceContext* context = new Win32POGLDeviceContext(this, mDeviceContext, renderContext);
	mDeviceContexts.push_back(context);
	return context;
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
