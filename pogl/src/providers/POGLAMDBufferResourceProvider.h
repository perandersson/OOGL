#pragma once
#include "../IPOGLBufferResourceProvider.h"

class POGLRenderState;
class POGLAMDBufferResourceProvider : public IPOGLBufferResourceProvider
{
public:
	POGLAMDBufferResourceProvider();
	virtual ~POGLAMDBufferResourceProvider();

// IPOGLBufferResourceProvider
public:
	virtual IPOGLBufferResource* CreateBuffer(POGL_UINT32 memorySize, GLenum target, POGLBufferUsage::Enum bufferUsage);
};
