#pragma once
#include "config.h"

class POGLRenderState;
class POGLBufferResource : public IPOGLInterface
{
public:
	POGLBufferResource(POGL_UINT32 memorySize, GLenum target, GLenum bufferUsage);
	~POGLBufferResource();
	
	/*!
		\brief Set buffer-, count and vertex array object after the construction is complete.

		This method is called if this object is created in another thread and we want, after the command queue has been executed, to put the
		values into this buffer.
	*/
	void PostConstruct(POGLRenderState* renderState);

	/*!
	
	*/
	inline GLuint GetBufferID() const {
		return mBufferID;
	}

	void* Map(POGLResourceMapType::Enum e);
	void* Map(POGL_UINT32 offset, POGL_UINT32 length, POGLResourceMapType::Enum e);
	void Unmap();

// IPOGLInterface
public:
	virtual void AddRef();
	virtual void Release();

private:
	REF_COUNTER mRefCount;
	GLuint mBufferID;
	POGL_UINT32 mMemorySize;
	GLenum mTarget;
	GLenum mBufferUsage;
};
