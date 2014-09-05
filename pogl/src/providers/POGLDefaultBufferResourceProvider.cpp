#include "MemCheck.h"
#include "POGLDefaultBufferResourceProvider.h"
#include "POGLDefaultBufferResource.h"
#include "POGLDefaultStaticBufferResource.h"
#include "POGLEnum.h"

POGLDefaultBufferResourceProvider::POGLDefaultBufferResourceProvider()
{
}

POGLDefaultBufferResourceProvider::~POGLDefaultBufferResourceProvider()
{
}

IPOGLBufferResource* POGLDefaultBufferResourceProvider::CreateBuffer(POGL_UINT32 memorySize, GLenum target, POGLBufferUsage::Enum bufferUsage)
{
	if (bufferUsage == POGLBufferUsage::STATIC)
		return new POGLDefaultStaticBufferResource(memorySize, target);

	return new POGLDefaultBufferResource(memorySize, target, POGLEnum::Convert(bufferUsage));
}
