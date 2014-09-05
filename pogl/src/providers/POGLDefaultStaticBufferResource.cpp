#include "MemCheck.h"
#include "POGLDefaultStaticBufferResource.h"

POGLDefaultStaticBufferResource::POGLDefaultStaticBufferResource(POGL_UINT32 memorySize, GLenum target)
: mRefCount(1), mBufferID(0), mMemorySize(memorySize), mTarget(target), mOffsetCounter(0)
{
}

POGLDefaultStaticBufferResource::~POGLDefaultStaticBufferResource()
{
}

void POGLDefaultStaticBufferResource::AddRef()
{
	mRefCount++;
}

void POGLDefaultStaticBufferResource::Release()
{
	if (--mRefCount == 0) {
		if (mBufferID != 0) {
			glDeleteBuffers(1, &mBufferID);
			mBufferID = 0;
		}
		delete this;
	}
}

void* POGLDefaultStaticBufferResource::Map(POGLResourceMapType::Enum e)
{
	GLenum access = GL_WRITE_ONLY;
	if (e == POGLResourceMapType::READ)
		access = GL_READ_ONLY;

	return glMapBuffer(mTarget, access);
}

void* POGLDefaultStaticBufferResource::Map(POGL_UINT32 offset, POGL_UINT32 length, POGLResourceMapType::Enum e)
{
	if (offset + length > mMemorySize)
		THROW_EXCEPTION(POGLStateException, "You cannot map with offset: %d and length: %d when the buffer size is: %d", offset, length, mMemorySize);

	GLenum access = 0;
	if (e == POGLResourceMapType::READ)
		access |= GL_MAP_READ_BIT;
	else if (e == POGLResourceMapType::WRITE)
		access |= GL_MAP_WRITE_BIT;

	return glMapBufferRange(mTarget, offset, length, access);
}

void POGLDefaultStaticBufferResource::Unmap()
{
	glUnmapBuffer(mTarget);
}

void POGLDefaultStaticBufferResource::Lock()
{

}

void POGLDefaultStaticBufferResource::Unlock()
{

}

GLuint POGLDefaultStaticBufferResource::PostConstruct(POGLRenderState* renderState)
{
	glGenBuffers(1, &mBufferID);
	const GLenum error = glGetError();
	if (mBufferID == 0 || error != GL_NO_ERROR)
		THROW_EXCEPTION(POGLResourceException, "Could not generate buffer ID. Reason: 0x%x", error);

	// Bind the buffer
	glBindBuffer(mTarget, mBufferID);
	CHECK_GL("Could not bind buffer");

	// Initialize the buffer size
	glBufferData(mTarget, mMemorySize, 0, GL_STATIC_DRAW);
	CHECK_GL("Could not initialize buffer data");

	return mBufferID;
}
