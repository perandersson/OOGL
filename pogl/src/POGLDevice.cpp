#include "MemCheck.h"
#include "POGLDevice.h"

POGLDevice::POGLDevice(const POGL_DEVICE_INFO* info)
{
	memcpy(&mDeviceInfo, info, sizeof(mDeviceInfo));
}

POGLDevice::~POGLDevice()
{
}

const POGL_DEVICE_INFO* POGLDevice::GetDeviceInfo() const
{
	return &mDeviceInfo;
}

POGLVendor::Enum POGLDevice::GetVendor() const
{
	auto vendor = (char*)glGetString(GL_VENDOR);
	static const char* VENDOR_STRINGS[] = {
		"Microsoft",
		"ATI",
		"NVIDIA",
		"INTEL"
	};
	static const POGLVendor::Enum VENDORS[] = {
		POGLVendor::SOFTWARE,
		POGLVendor::AMD,
		POGLVendor::NVIDIA,
		POGLVendor::INTEL
	};
	const POGL_UINT32 count = sizeof(VENDORS) / sizeof(POGLVendor::Enum);
	for (POGL_UINT32 i = 0; i < count; ++i) {
		if (strncmp(vendor, VENDOR_STRINGS[i], strlen(VENDOR_STRINGS[i])) == 0) {
			return VENDORS[i];
		}
	}
	return POGLVendor::UNKNOWN;
}

//
// Other
//

POGL_SIZE& POGL_SIZE::operator = (const POGL_SIZE& rhs)
{
	width = rhs.width;
	height = rhs.height;
	return *this;
}

POGL_VECTOR2& POGL_VECTOR2::operator=(const POGL_VECTOR2& rhs)
{
	x = rhs.x;
	y = rhs.y;
	return *this;
}

POGL_VECTOR3& POGL_VECTOR3::operator = (const POGL_VECTOR3& rhs)
{
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;
	return *this;
}

POGL_VECTOR4& POGL_VECTOR4::operator = (const POGL_VECTOR4& rhs)
{
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;
	w = rhs.w;
	return *this;
}

POGL_RECT& POGL_RECT::operator = (const POGL_RECT& rhs)
{
	x = rhs.x;
	y = rhs.y;
	width = rhs.width;
	height = rhs.height;
	return *this;
}

POGL_POSITION_VERTEX& POGL_POSITION_VERTEX::operator=(const POGL_POSITION_VERTEX& rhs) 
{ 
	position = rhs.position; 
	return *this; 
}

POGL_POSITION_COLOR_VERTEX& POGL_POSITION_COLOR_VERTEX::operator=(const POGL_POSITION_COLOR_VERTEX& rhs) 
{ 
	position = rhs.position; 
	color = rhs.color;  
	return *this; 
}

POGL_POSITION_TEXCOORD_VERTEX& POGL_POSITION_TEXCOORD_VERTEX::operator=(const POGL_POSITION_TEXCOORD_VERTEX& rhs) 
{ 
	position = rhs.position; 
	texCoord = rhs.texCoord;  
	return *this;
}
