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

void POGLDisableMemoryLeakDetection()
{
#ifdef _DEBUG
	_CrtSetDbgFlag(gPOGDebugFlag);
#endif
}

/////////////////////////////////////

Win32POGLDevice::Win32POGLDevice()
: POGLDevice(), mRefCount(1),
mWindowHandle(nullptr), mDeviceContext(nullptr), mMainThreadDeviceContext(nullptr)
{

}

Win32POGLDevice::~Win32POGLDevice()
{
	size_t size = mDeviceContexts.size();
	for (size_t i = 0; i < size; ++i) {
		delete mDeviceContexts[i];
	}
	mDeviceContexts.clear();

	if (mMainThreadDeviceContext != nullptr) {
		delete mMainThreadDeviceContext;
		mMainThreadDeviceContext = nullptr;
	}

	if (mDeviceContext != nullptr) {
		ReleaseDC(mWindowHandle, mDeviceContext);
		mDeviceContext = nullptr;
		mWindowHandle = nullptr;
	}
}

void Win32POGLDevice::AddRef()
{
	mRefCount++;
}

void Win32POGLDevice::Release()
{
	if (--mRefCount == 0) {
		delete this;
		POGLDisableMemoryLeakDetection();
	}
}

IPOGLDeviceContext* Win32POGLDevice::GetDeviceContext()
{
	if (tDeviceContext == nullptr) {
		Win32POGLDeviceContext* context = new Win32POGLDeviceContext(this, mDeviceContext);
		if (!context->Initialize(mMainThreadDeviceContext)) {
			delete context;
			THROW_EXCEPTION(POGLException, "Could not create a new Device context");
		}
		tDeviceContext = context;
		std::lock_guard<std::recursive_mutex> lock(mDeviceContextsMutex);
		mDeviceContexts.push_back(context);
		tDeviceContext = context;
	}
	return tDeviceContext->BindContextIfNeccessary();
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

	mMainThreadDeviceContext = new Win32POGLDeviceContext(this, mDeviceContext);
	if (!mMainThreadDeviceContext->Initialize(nullptr)) {
		delete mMainThreadDeviceContext;
		mMainThreadDeviceContext = nullptr;
		return false;
	}
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
