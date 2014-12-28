#include "MemCheck.h"
#include "POGLAMDBufferResource.h"
#include "../POGLEnum.h"

POGLAMDBufferResource::POGLAMDBufferResource(POGL_UINT32 memorySize, GLenum target, POGLBufferUsage::Enum bufferUsage)
: mRefCount(1), mBufferID(0), mMemorySize(memorySize), mTarget(target), mBufferUsage(bufferUsage), mPinnedMemory(nullptr)
{
	mPinnedMemory = (char*)malloc(memorySize);
}

POGLAMDBufferResource::~POGLAMDBufferResource()
{
}

void POGLAMDBufferResource::AddRef()
{
	mRefCount++;
}

void POGLAMDBufferResource::Release()
{
	if (--mRefCount == 0) {
		if (mBufferID != 0) {
			glDeleteBuffers(1, &mBufferID);
			mBufferID = 0;
		}
		if (mPinnedMemory != nullptr) {
			free(mPinnedMemory);
			mPinnedMemory = nullptr;
		}
		delete this;
	}
}

void* POGLAMDBufferResource::Map(POGLResourceMapType::Enum e)
{
	mLock.WaitClientAndClear();
	return mPinnedMemory;
}

void* POGLAMDBufferResource::Map(POGL_UINT32 offset, POGL_UINT32 length, POGLResourceMapType::Enum e)
{
	if (offset + length > mMemorySize)
		THROW_EXCEPTION(POGLStateException, "You cannot map with offset: %d and length: %d when the buffer size is: %d", offset, length, mMemorySize);

	mLock.WaitClientAndClear(offset, length);
	return mPinnedMemory + offset;
}

void POGLAMDBufferResource::Unmap()
{
}

void POGLAMDBufferResource::Lock()
{
	mLock.PrepareFence();
}

void POGLAMDBufferResource::Lock(POGL_UINT32 offset, POGL_UINT32 length)
{
	mLock.PrepareFence(offset, length);
}

void POGLAMDBufferResource::Unlock()
{
	mLock.AddFences();
}

GLuint POGLAMDBufferResource::PostConstruct(POGLRenderState* renderState)
{
	glGenBuffers(1, &mBufferID);
	const GLenum error = glGetError();
	if (mBufferID == 0 || error != GL_NO_ERROR)
		THROW_EXCEPTION(POGLResourceException, "Could not generate buffer ID. Reason: 0x%x", error);

	const GLenum usage = POGLEnum::Convert(mBufferUsage);

	glBindBuffer(GL_EXTERNAL_VIRTUAL_MEMORY_BUFFER_AMD, mBufferID);
	glBufferData(GL_EXTERNAL_VIRTUAL_MEMORY_BUFFER_AMD, mMemorySize, mPinnedMemory, usage);
	//glBindBuffer(GL_EXTERNAL_VIRTUAL_MEMORY_BUFFER_AMD, 0);
	CHECK_GL("Could not initialize buffer data");

	// Bind the buffer
	glBindBuffer(mTarget, mBufferID);
	CHECK_GL("Could not bind buffer");

	// Lock and wait
	GLsync lock = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
	glClientWaitSync(lock, GL_SYNC_FLUSH_COMMANDS_BIT, ~0ull);
	glDeleteSync(lock);

	return mBufferID;
}
