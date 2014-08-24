#pragma once
#include "config.h"
#include "IPOGLBufferResourceProvider.h"

class POGLAPI POGLDevice : public IPOGLDevice
{
public:
	POGLDevice(const POGL_DEVICE_INFO* info);
	virtual ~POGLDevice();

	/*!
		\brief Retrieves the buffer resource provider

		\return
	*/
	virtual IPOGLBufferResourceProvider* GetBufferResourceProvider() = 0;

// IPOGLDevice
public:
	virtual const POGL_DEVICE_INFO* GetDeviceInfo() const;
	virtual POGLVendor::Enum GetVendor() const;

protected:
	POGL_DEVICE_INFO mDeviceInfo;
};
