#pragma once
#include "config.h"

class POGLAPI POGLDevice : public IPOGLDevice
{
public:
	POGLDevice(const POGL_DEVICE_INFO* info);
	virtual ~POGLDevice();

// IPOGLDevice
public:
	virtual const POGL_DEVICE_INFO* GetDeviceInfo() const;
	virtual POGLVendor::Enum GetVendor() const;

protected:
	POGL_DEVICE_INFO mDeviceInfo;
};
