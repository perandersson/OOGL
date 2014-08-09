#pragma once
#include "config.h"
#include <gl/pogl.h>

class POGLDevice : public IPOGLDevice
{
public:
	POGLDevice();
	~POGLDevice();

	const POGL_DEVICE_INFO* GetDeviceInfo();

	/*!
		\brief Initialize the windows implementation of the IPOLDevice interface

		\param info
	*/
	virtual bool Initialize(const POGL_DEVICE_INFO* info);

protected:
	POGL_DEVICE_INFO mDeviceInfo;
};
