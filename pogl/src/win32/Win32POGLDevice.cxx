#include "Win32POGLDevice.hxx"
#include "Win32POGLDeviceContext.hxx"

/* Thread local container for the device context */
THREAD_LOCAL Win32POGLDeviceContext* tDeviceContext = nullptr;

Win32POGLDevice::Win32POGLDevice()
: POGLDevice(),
mWindowHandle(nullptr), mDeviceContext(nullptr), mMainThreadDeviceContext(nullptr)
{

}

Win32POGLDevice::~Win32POGLDevice()
{
	// Release the rest of the device contexts
	size_t size = mDeviceContexts.size();
	for (size_t i = 0; i < size; ++i) {
		mDeviceContexts[i]->Release();
	}
	mDeviceContexts.clear();

	// Release the main thread device
	mMainThreadDeviceContext->Release();

	if (mDeviceContext != nullptr) {
		ReleaseDC(mWindowHandle, mDeviceContext);
		mDeviceContext = nullptr;
		mWindowHandle = nullptr;
	}
}

IPOGLDeviceContext* Win32POGLDevice::GetContext()
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

	const GLenum error = glGetError();
	if (error != GL_NO_ERROR)
		THROW_EXCEPTION(POGLException, "Could not swap buffers");
}

bool Win32POGLDevice::Initialize(POGL_DEVICE_INFO* info)
{
	static const BYTE PIXELTYPES[POGLPixelFormat::COUNT] = {
		PFD_TYPE_RGBA
	};

	mWindowHandle = (HWND)info->window;
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

IPOGLDevice* POGLCreateDevice(POGL_DEVICE_INFO* info)
{
	Win32POGLDevice* device = new Win32POGLDevice();
	device->Initialize(info);
	return device;
}
