#include "MemCheck.h"
#include "POGLBufferResource.h"

POGLBufferResource::POGLBufferResource(POGL_UINT32 memorySize, GLenum target, GLenum bufferUsage)
: mRefCount(1), mBufferID(0), mMemorySize(memorySize), mTarget(target), mBufferUsage(bufferUsage)
{
}

POGLBufferResource::~POGLBufferResource()
{
}

void POGLBufferResource::AddRef()
{
	mRefCount++;
}

void POGLBufferResource::Release()
{
	if (--mRefCount == 0) {
		if (mBufferID != 0) {
			glDeleteBuffers(1, &mBufferID);
			mBufferID = 0;
		}
		delete this;
	}
}

void* POGLBufferResource::Map(POGLResourceMapType::Enum e)
{
	if (e == POGLResourceMapType::WRITE)
		return glMapBuffer(mTarget, GL_WRITE_ONLY);

	THROW_NOT_IMPLEMENTED_EXCEPTION();
}

void* POGLBufferResource::Map(POGL_UINT32 offset, POGL_UINT32 length, POGLResourceMapType::Enum e)
{
	if (e == POGLResourceMapType::WRITE) {
		if (offset + length > mMemorySize)
			THROW_EXCEPTION(POGLStateException, "You cannot map with offset: %d and length: %d when the buffer size is: %d", offset, length, mMemorySize);

		return glMapBufferRange(mTarget, offset, length, GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT);
	}

	THROW_NOT_IMPLEMENTED_EXCEPTION();
}

void POGLBufferResource::Unmap()
{
	glUnmapBuffer(mTarget);
}

void POGLBufferResource::PostConstruct(POGLRenderState* renderState)
{
	glGenBuffers(1, &mBufferID);
	const GLenum error = glGetError();
	if (mBufferID == 0 || error != GL_NO_ERROR)
		THROW_EXCEPTION(POGLResourceException, "Could not generate buffer ID. Reason: 0x%x", error);

	// Bind the buffer
	glBindBuffer(mTarget, mBufferID);

	// Initialize the buffer size
	glBufferData(mTarget, mMemorySize, 0, mBufferUsage);
}
