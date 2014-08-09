#pragma once
#include "config.h"
#include <gl/pogl.h>

class POGLDeviceContext;
class POGLResourceStream : public IPOGLResourceStream
{
public:
	POGLResourceStream(POGLDeviceContext* context);
	~POGLResourceStream();

protected:
	POGLDeviceContext* mDeviceContext;
};
