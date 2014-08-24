#pragma once
#include "../IPOGLBufferResourceProvider.h"

class POGLRenderState;
class POGLDefaultBufferResourceProvider : public IPOGLBufferResourceProvider
{
public:
	POGLDefaultBufferResourceProvider();
	virtual ~POGLDefaultBufferResourceProvider();

// IPOGLBufferResourceProvider
public:
	virtual IPOGLBufferResource* CreateBuffer(POGL_UINT32 memorySize, GLenum target, POGLBufferUsage::Enum bufferUsage);
};
