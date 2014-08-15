#include <gl/pogl.h>

int main()
{
	//
	// Prepare the neccessary information needed by POGL to create a valid device
	//

	POGL_DEVICE_INFO deviceInfo = { 0 };
#ifdef _DEBUG
	deviceInfo.flags = POGLDeviceInfoFlags::DEBUG_MODE;
#else
	deviceInfo.flags = 0;
#endif
	deviceInfo.windowHandle = nullptr;
	deviceInfo.colorBits = 32;
	deviceInfo.depthBits = 16;
	deviceInfo.pixelFormat = POGLPixelFormat::R8G8B8A8;

	//
	// Create the device
	//

	IPOGLDevice* device = POGLCreateDevice(&deviceInfo);

	//
	// Release the device when you are done with it
	//

	device->Release();

	// Quit the application
	return 0;
}
