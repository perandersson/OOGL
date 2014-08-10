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

Win32POGLDevice::Win32POGLDevice()
: POGLDevice(), mRefCount(1), mReleasing(false),
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
		}
		mDeviceContexts.clear();
		mFreeDeviceContexts.clear();

		if (mLegacyRenderContext != nullptr) {
			wglDeleteContext(mLegacyRenderContext);
			mLegacyRenderContext = nullptr;
		}

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
		Win32POGLDeviceContext* context = GetOrCreateRenderContext();
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

void Win32POGLDevice::SwapBuffers()
{
	::SwapBuffers(mDeviceContext);

	CHECK_GL("Could not swap buffers");
}

bool Win32POGLDevice::Initialize(const POGL_DEVICE_INFO* info)
{
	static const BYTE PIXELTYPES[POGLPixelFormat::COUNT] = {
		PFD_TYPE_RGBA
	};

	mWindowHandle = (HWND)info->windowHandle;
	mDeviceContext = GetDC(mWindowHandle);
	if (mDeviceContext == nullptr)
		return false;

	PIXELFORMATDESCRIPTOR pfd = { 0 };
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PIXELTYPES[(POGL_UINT32)info->pixelFormat];
	pfd.cColorBits = (BYTE)info->colorBits;
	pfd.cDepthBits = (BYTE)info->depthBits;
	pfd.iLayerType = PFD_MAIN_PLANE;

	const int format = ChoosePixelFormat(mDeviceContext, &pfd);
	if (!format)
		return false;

	if (!SetPixelFormat(mDeviceContext, format, &pfd))
		return false;

	// Create a legacy render context, used so that we can load the neccessary extensions 
	mLegacyRenderContext = wglCreateContext(mDeviceContext);
	if (!mLegacyRenderContext)
		return false;

	return POGLDevice::Initialize(info);
}

Win32POGLDeviceContext* Win32POGLDevice::GetOrCreateRenderContext()
{
	std::lock_guard<std::recursive_mutex> lock(mDeviceContextsMutex);
	if (mFreeDeviceContexts.size() > 0) {
		auto it = (*mFreeDeviceContexts.begin());
		mFreeDeviceContexts.pop_front();
		return it;
	}

	//
	// Activate the legacy render context (which is supplied in the constructor) so 
	// that we can retrieve the necessary extensions
	//

	BOOL current = wglMakeCurrent(mDeviceContext, mLegacyRenderContext);
	if (current == FALSE) {
		const DWORD error = GetLastError();
		THROW_EXCEPTION(POGLException, "Could not bind the legacy render context. Reason: 0x%x", error);
	}

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

	HGLRC parentContext = mLegacyRenderContext;
	if (mDeviceContexts.empty()) {
		wglDeleteContext(mLegacyRenderContext);
		mLegacyRenderContext = (*wglCreateContextAttribsARB)(mDeviceContext, nullptr, &attributes[0]);
		wglMakeCurrent(nullptr, nullptr);
		wglMakeCurrent(mDeviceContext, mLegacyRenderContext);
		if (mLegacyRenderContext == nullptr) {
			const DWORD error = GetLastError();
			THROW_EXCEPTION(POGLException, "Failed to create an OpenGL 3.3 render context. Reason: 0x%x", error);
		}
		parentContext = mLegacyRenderContext;
	}
	
	HGLRC renderContext = (*wglCreateContextAttribsARB)(mDeviceContext, parentContext, &attributes[0]);
	wglMakeCurrent(nullptr, nullptr);
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

	Win32POGLDevice* device = new Win32POGLDevice();
	device->Initialize(info);
	return device;
}
