#pragma once
#include "IPOGLBufferResource.h"

/*!
	\brief Buffer resource provider
*/
class IPOGLBufferResourceProvider
{
public:
	virtual ~IPOGLBufferResourceProvider() {}

	/*!
		\brief Create a buffer resource
	*/
	virtual IPOGLBufferResource* CreateBuffer(POGL_UINT32 memorySize, GLenum target, POGLBufferUsage::Enum bufferUsage) = 0;

};
