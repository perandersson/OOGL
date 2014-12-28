#include "MemCheck.h"
#include "POGLAMDBufferResourceProvider.h"
#include "POGLAMDBufferResource.h"
#include "POGLDefaultBufferResource.h"
#include "POGLEnum.h"

POGLAMDBufferResourceProvider::POGLAMDBufferResourceProvider()
: POGLDefaultBufferResourceProvider()
{
}

POGLAMDBufferResourceProvider::~POGLAMDBufferResourceProvider()
{
}

IPOGLBufferResource* POGLAMDBufferResourceProvider::CreateBuffer(POGL_UINT32 memorySize, GLenum target, POGLBufferUsage::Enum bufferUsage)
{
	if (bufferUsage == POGLBufferUsage::STREAM)
		return new POGLAMDBufferResource(memorySize, target, bufferUsage);

	return POGLDefaultBufferResourceProvider::CreateBuffer(memorySize, target, bufferUsage);
}
