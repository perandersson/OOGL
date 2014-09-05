#pragma once
#include "config.h"

class POGLRenderState;

class IPOGLBufferResource : public IPOGLInterface
{
public:

	/*!
		\brief Set buffer-, count and vertex array object after the construction is complete.

		This method is called if this object is created in another thread and we want, after the command queue has been executed, to put the
		values into this buffer.

		\return The bufferID associated with this resource object
	*/
	virtual GLuint PostConstruct(POGLRenderState* renderState) = 0;

	/*!
		\brief Retrieves a memory location connected to this buffer

		\param e
				What we are doing with the buffer
	*/
	virtual void* Map(POGLResourceMapType::Enum e) = 0;
	
	/*!
		\brief Retrieves a memory location connected to this buffer

		\param offset
		\param length
		\param e
				What we are doing with the buffer
	*/
	virtual void* Map(POGL_UINT32 offset, POGL_UINT32 length, POGLResourceMapType::Enum e) = 0;

	/*!
		\brief Unmaps this buffer
	*/
	virtual void Unmap() = 0;

	virtual void Lock() = 0;

	virtual void Lock(POGL_UINT32 offset, POGL_UINT32 length) = 0;

	virtual void Unlock() = 0;
};
