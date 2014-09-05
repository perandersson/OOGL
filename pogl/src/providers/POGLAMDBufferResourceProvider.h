#pragma once
#include "../IPOGLBufferResourceProvider.h"
#include "POGLDefaultBufferResourceProvider.h"

class POGLRenderState;
class POGLAMDBufferResourceProvider : public POGLDefaultBufferResourceProvider
{
public:
	POGLAMDBufferResourceProvider();
	virtual ~POGLAMDBufferResourceProvider();

// IPOGLBufferResourceProvider
public:
	virtual IPOGLBufferResource* CreateBuffer(POGL_UINT32 memorySize, GLenum target, POGLBufferUsage::Enum bufferUsage);
};
