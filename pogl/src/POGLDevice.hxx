#pragma once
#include "config.hxx"
#include <gl/pogl.hxx>

class POGLDevice : public IPOGLDevice
{
public:
	POGLDevice();
	~POGLDevice();

	void AddRef();
	void Release();
	const POGL_DEVICE_INFO* GetDeviceInfo();

	/*!
		\brief Initialize the windows implementation of the IPOLDevice interface

		\param info
	*/
	virtual bool Initialize(POGL_DEVICE_INFO* info);

private:
	POGL_UINT32 mRefCount;
	POGL_DEVICE_INFO mDeviceInfo;
};
