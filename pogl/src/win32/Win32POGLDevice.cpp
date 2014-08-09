#include "MemCheck.h"
#include "Win32POGLDevice.h"
#include "Win32POGLDeviceContext.h"

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
mWindowHandle(nullptr), mDeviceContext(nullptr), mMainThreadDeviceContext(nullptr)
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
		size_t size = mDeviceContexts.size();
		for (size_t i = 0; i < size; ++i) {
			mDeviceContexts[i]->Release();
		}
		mDeviceContexts.clear();

		if (mMainThreadDeviceContext != nullptr) {
			mMainThreadDeviceContext->Release();
			mMainThreadDeviceContext = nullptr;
		}

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
		Win32POGLDeviceContext* context = new Win32POGLDeviceContext(this, mDeviceContext, mLegacyRenderContext);
		context->Initialize(mMainThreadDeviceContext);
		tDeviceContext = context;
		std::lock_guard<std::recursive_mutex> lock(mDeviceContextsMutex);
		mDeviceContexts.push_back(context);
		tDeviceContext = context;
	}
	tDeviceContext->Bind();
	tDeviceContext->AddRef();
	return tDeviceContext;
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

	mMainThreadDeviceContext = new Win32POGLDeviceContext(this, mDeviceContext, mLegacyRenderContext);
	mMainThreadDeviceContext->Initialize(nullptr);
	tDeviceContext = mMainThreadDeviceContext;
	return POGLDevice::Initialize(info);
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
