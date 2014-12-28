#include "MemCheck.h"
#include "POGLDefaultBufferResource.h"
#include "POGLEnum.h"

POGLDefaultBufferResource::POGLDefaultBufferResource(POGL_UINT32 memorySize, GLenum target, POGLBufferUsage::Enum bufferUsage)
: mRefCount(1), mBufferID(0), mMemorySize(memorySize), mTarget(target), mBufferUsage(bufferUsage)
{
}

POGLDefaultBufferResource::~POGLDefaultBufferResource()
{
}

void POGLDefaultBufferResource::AddRef()
{
	mRefCount++;
}

void POGLDefaultBufferResource::Release()
{
	if (--mRefCount == 0) {
		if (mBufferID != 0) {
			glDeleteBuffers(1, &mBufferID);
			mBufferID = 0;
		}
		delete this;
	}
}

void* POGLDefaultBufferResource::Map(POGLResourceMapType::Enum e)
{
	GLenum access = GL_WRITE_ONLY;
	if (e == POGLResourceMapType::READ)
		access = GL_READ_ONLY;

	if (mBufferUsage == POGLBufferUsage::IMMUTABLE) {
		glBufferData(mTarget, mMemorySize, 0, POGLEnum::Convert(mBufferUsage));
		CHECK_GL("Could not initialize buffer data");
	}
	else {
		mLock.PrepareFence();
	}
	return glMapBuffer(mTarget, access);
}

void* POGLDefaultBufferResource::Map(POGL_UINT32 offset, POGL_UINT32 length, POGLResourceMapType::Enum e)
{
	if (offset + length > mMemorySize)
		THROW_EXCEPTION(POGLStateException, "You cannot map with offset: %d and length: %d when the buffer size is: %d", offset, length, mMemorySize);
	
	GLenum access = GL_MAP_UNSYNCHRONIZED_BIT;
	if (e == POGLResourceMapType::READ)
		access |= GL_MAP_READ_BIT;
	else if (e == POGLResourceMapType::WRITE)
		access |= GL_MAP_WRITE_BIT;

	if (mBufferUsage != POGLBufferUsage::IMMUTABLE) {
		mLock.PrepareFence(offset, length);
	}

	return glMapBufferRange(mTarget, offset, length, access);
}

void POGLDefaultBufferResource::Unmap()
{
	glUnmapBuffer(mTarget);

	if (mBufferUsage != POGLBufferUsage::IMMUTABLE) {
		mLock.AddFences();
	}
}

void POGLDefaultBufferResource::Lock()
{
	if (mBufferUsage != POGLBufferUsage::IMMUTABLE) {
		mLock.WaitAndClear();
	}
}

void POGLDefaultBufferResource::Lock(POGL_UINT32 offset, POGL_UINT32 length)
{
	if (mBufferUsage != POGLBufferUsage::IMMUTABLE) {
		mLock.WaitAndClear(offset, length);
	}
}

void POGLDefaultBufferResource::Unlock()
{

}

GLuint POGLDefaultBufferResource::PostConstruct(POGLRenderState* renderState)
{
	glGenBuffers(1, &mBufferID);
	const GLenum error = glGetError();
	if (mBufferID == 0 || error != GL_NO_ERROR)
		THROW_EXCEPTION(POGLResourceException, "Could not generate buffer ID. Reason: 0x%x", error);

	// Bind the buffer
	glBindBuffer(mTarget, mBufferID);
	CHECK_GL("Could not bind buffer");

	// Initialize the buffer size
	glBufferData(mTarget, mMemorySize, 0, POGLEnum::Convert(mBufferUsage));
	CHECK_GL("Could not initialize buffer data");

	return mBufferID;
}
