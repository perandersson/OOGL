#include "MemCheck.h"
#include "POGLAMDBufferResourceProvider.h"
#include "POGLAMDBufferResource.h"
#include "POGLDefaultBufferResource.h"
#include "POGLEnum.h"

POGLAMDBufferResourceProvider::POGLAMDBufferResourceProvider()
{
}

POGLAMDBufferResourceProvider::~POGLAMDBufferResourceProvider()
{
}

IPOGLBufferResource* POGLAMDBufferResourceProvider::CreateBuffer(POGL_UINT32 memorySize, GLenum target, POGLBufferUsage::Enum bufferUsage)
{
	if (bufferUsage == POGLBufferUsage::DYNAMIC)
		return new POGLAMDBufferResource(memorySize, target, bufferUsage);

	return new POGLDefaultBufferResource(memorySize, target, POGLEnum::Convert(bufferUsage));
}
