#pragma once
#include "config.h"
#include <gl/pogl.h>

class POGLDevice : public IPOGLDevice
{
public:
	POGLDevice(const POGL_DEVICE_INFO* info);
	~POGLDevice();

	/*!
		\brief Retrieves the device information
	*/
	const POGL_DEVICE_INFO* GetDeviceInfo();

// IPOGLDevice
public:
	virtual IPOGLDeferredDeviceContext* CreateDeferredDeviceContext();

protected:
	POGL_DEVICE_INFO mDeviceInfo;
};
