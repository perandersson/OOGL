#include <gl/pogl.h>
#include <iostream>

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
	// Query vendor
	//

	auto vendor = device->GetVendor();
	switch (vendor) {
	case POGLVendor::AMD:
		std::cout << "Vendor: AMD" << std::endl;
		break;
	case POGLVendor::INTEL:
		std::cout << "Vendor: INTEL" << std::endl;
		break;
	case POGLVendor::NVIDIA:
		std::cout << "Vendor: NVIDIA" << std::endl;
		break;
	case POGLVendor::SOFTWARE:
		std::cout << "Vendor: SOFTWARE" << std::endl;
		break;
	case POGLVendor::UNKNOWN:
		std::cout << "Vendor: UNKNOWN" << std::endl;
		break;
	}

	//
	// Release the device when you are done with it
	//

	device->Release();

	// Quit the application
	return 0;
}
