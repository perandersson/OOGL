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

	/*!
		\brief Initialize the windows implementation of the IPOLDevice interface

		\param info
	*/
	virtual bool Initialize();
	
	/*!
		\brief Retrieves the maximum amount of render contexts
	*/
	POGL_UINT8 GetMaxRenderContexts() const;

protected:
	POGL_DEVICE_INFO mDeviceInfo;
};
