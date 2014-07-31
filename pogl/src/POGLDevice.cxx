#include "POGLDevice.hxx"

POGLDevice::POGLDevice()
: mRefCount(1)
{

}

POGLDevice::~POGLDevice()
{
}

void POGLDevice::AddRef()
{
	mRefCount++;
}

void POGLDevice::Release()
{
	if (--mRefCount == 0)
		delete this;
}

const POGL_DEVICE_INFO* POGLDevice::GetDeviceInfo()
{
	return &mDeviceInfo;
}

bool POGLDevice::Initialize(POGL_DEVICE_INFO* info)
{
	memcpy(&mDeviceInfo, info, sizeof(mDeviceInfo));
	return true;
}
