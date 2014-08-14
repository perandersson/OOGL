#pragma once
#include "config.h"

class POGLDevice : public IPOGLDevice
{
public:
	POGLDevice(const POGL_DEVICE_INFO* info);
	~POGLDevice();

	/*!
		\brief Retrieves the device information
	*/
	const POGL_DEVICE_INFO* GetDeviceInfo();

protected:
	POGL_DEVICE_INFO mDeviceInfo;
};
