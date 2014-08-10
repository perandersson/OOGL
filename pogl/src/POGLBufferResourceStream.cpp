#include "MemCheck.h"
#include "POGLBufferResourceStream.h"
#include "POGLDeviceContext.h"
#include "POGLEnum.h"
#include "POGLSyncObject.h"

POGLBufferResourceStream::POGLBufferResourceStream(POGLDeviceContext* context)
: POGLResourceStream(context), mResource(nullptr), mSyncObject(nullptr), mTarget(0), mUsage(0), mMapping(false), mStreamType(POGLResourceStreamType::WRITE)
{
}

POGLBufferResourceStream::~POGLBufferResourceStream()
{
}

void POGLBufferResourceStream::Open(IPOGLResource* resource, POGLSyncObject* syncObject, GLenum target, GLenum usage, POGLResourceStreamType::Enum streamType)
{
	mResource = resource;
	mResource->AddRef();
	mSyncObject = syncObject;
	mTarget = target;
	mUsage = usage;
	mStreamType = streamType;
	mMapping = false;
}

void POGLBufferResourceStream::Update(POGL_UINT32 size, const void* data)
{
	if (mStreamType != POGLResourceStreamType::WRITE)
		THROW_EXCEPTION(POGLStreamException, "The stream is read-only");

	if (mUsage == GL_STATIC_DRAW || mUsage == GL_STATIC_READ || mUsage == GL_STATIC_COPY)
		THROW_EXCEPTION(POGLStreamException, "You are not allowed to update static buffers");

	// Make sure no one is writing to this buffer before we continue
	mSyncObject->LockWrite();

	// 1. Orphan the old buffer. The old vertex buffer data will automatically be destroyed when it's no longer inside any OpenGL queue
	mDeviceContext->BufferData(mTarget, size, NULL, mUsage);

	// 2. Copy the new data to the new buffer
	void* ptr = mDeviceContext->MapBuffer(mTarget, POGLEnum::ConvertForMapBuffer(mStreamType));
	memcpy(ptr, data, size);
	mSyncObject->QueueFence(mDeviceContext);
}

void POGLBufferResourceStream::UpdateRange(POGL_UINT32 offset, POGL_UINT32 size, const void* data)
{
	if (mStreamType != POGLResourceStreamType::WRITE)
		THROW_EXCEPTION(POGLStreamException, "The stream is read-only");

	if (mUsage == GL_STATIC_DRAW || mUsage == GL_STATIC_READ || mUsage == GL_STATIC_COPY)
		THROW_EXCEPTION(POGLStreamException, "You are not allowed to partially update static buffers");

	// Verify that you can perform this type of update
	if (mStreamType == POGLResourceStreamType::WRITE) {
		if (mUsage == GL_STATIC_DRAW)
			THROW_EXCEPTION(POGLStreamException, "You are not allowed to update ");
	}

	// Make sure no one is writing to this buffer before we continue on the CPU/client side
	mSyncObject->LockWrite();
	mSyncObject->WaitSyncClient(mDeviceContext);

	// 2. Copy the new data to the buffer
	void* ptr = mDeviceContext->MapBufferRange(mTarget, offset, size, POGLEnum::ConvertForMapBufferRange(mStreamType));
	memcpy(ptr, data, size);
	mSyncObject->QueueFence(mDeviceContext);
}

void* POGLBufferResourceStream::Map()
{
	if (mStreamType == POGLResourceStreamType::WRITE) {
		if (mUsage == GL_STATIC_DRAW || mUsage == GL_STATIC_READ || mUsage == GL_STATIC_COPY)
			THROW_EXCEPTION(POGLStreamException, "You are not allowed to map static buffers when writing");
	}

	// Make sure no one is writing to this buffer before we continue on the CPU/client side
	mSyncObject->LockWrite();

	// Wait until the GL server is complete with writing to the buffer from some other context
	mSyncObject->WaitSyncClient(mDeviceContext);

	// Create fence object on mapping when closing
	mMapping = true;

	return mDeviceContext->MapBuffer(mTarget, POGLEnum::ConvertForMapBuffer(mStreamType));
}

void* POGLBufferResourceStream::Map(POGL_UINT32 offset, POGL_UINT32 size)
{
	if (mStreamType == POGLResourceStreamType::WRITE) {
		if (mUsage == GL_STATIC_DRAW || mUsage == GL_STATIC_READ || mUsage == GL_STATIC_COPY)
			THROW_EXCEPTION(POGLStreamException, "You are not allowed to map static buffers when writing");
	}

	// Make sure no one is writing to this buffer before we continue on the CPU/client side
	mSyncObject->LockWrite();

	// Wait until the GL server is complete with writing to the buffer from some other context
	mSyncObject->WaitSyncClient(mDeviceContext);

	// Create fence object on mapping when closing
	mMapping = true;

	return mDeviceContext->MapBufferRange(mTarget, offset, size, POGLEnum::ConvertForMapBufferRange(mStreamType) | GL_MAP_UNSYNCHRONIZED_BIT);
}

void POGLBufferResourceStream::Close()
{
	GLboolean unmapped = mDeviceContext->UnmapBuffer(mTarget);
	if (unmapped == GL_FALSE) {
		const GLenum error = glGetError();
		mSyncObject->UnlockWrite();
		mResource->Release();
		THROW_EXCEPTION(POGLStreamException, "Could not unmap stream. Reason: 0x%x", error);
	}

	if (mMapping) {
		mSyncObject->QueueFence(mDeviceContext);
		mMapping = false;
	}
	else
		glFlush();

	mSyncObject->UnlockWrite();
	mResource->Release();
	mResource = nullptr;
}

