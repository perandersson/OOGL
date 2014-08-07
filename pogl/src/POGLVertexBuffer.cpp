#include "MemCheck.h"
#include "POGLVertexBuffer.h"
#include "POGLDeviceContext.h"
#include "POGLIndexBuffer.h"
#include "POGLEnum.h"
#include "POGLSyncObject.h"
#include "POGLRenderState.h"
#include <atomic>

namespace {
	std::atomic<POGL_UINT32> uid;
	POGL_UINT32 GenVertexBufferUID() {
		return ++uid;
	}
}

POGLVertexBuffer::POGLVertexBuffer(GLuint bufferID, const POGL_VERTEX_LAYOUT* layout, POGL_UINT32 numVertices, GLenum primitiveType, GLenum bufferUsage, POGLSyncObject* syncObject, IPOGLDevice* device)
: mRefCount(1), mUID(GenVertexBufferUID()), mBufferID(bufferID), mLayout(layout), mNumVertices(numVertices), mPrimitiveType(primitiveType), mBufferUsage(bufferUsage), mSyncObject(syncObject), mDevice(device)
{
}

POGLVertexBuffer::~POGLVertexBuffer()
{
}

void POGLVertexBuffer::AddRef()
{
	mRefCount++;
}

void POGLVertexBuffer::Release()
{
	if (--mRefCount == 0) {
		if (mBufferID != 0) {
			POGLDeviceContext* context = static_cast<POGLDeviceContext*>(mDevice->GetDeviceContext());
			context->DeleteBuffer(mBufferID);
			context->Release();
			mBufferID = 0;
		}
		if (mSyncObject != nullptr) {
			delete mSyncObject;
			mSyncObject = nullptr;
		}
		delete this;
	}
}

IPOGLDevice* POGLVertexBuffer::GetDevice()
{
	mDevice->AddRef();
	return mDevice;
}

POGL_HANDLE POGLVertexBuffer::GetHandlePtr()
{
	return this;
}

void POGLVertexBuffer::WaitSyncDriver()
{
	mSyncObject->WaitSyncDriver();
}

void POGLVertexBuffer::WaitSyncClient()
{
	mSyncObject->WaitSyncClient();
}

bool POGLVertexBuffer::WaitSyncClient(POGL_UINT64 timeout)
{
	return mSyncObject->WaitSyncClient(timeout);
}

bool POGLVertexBuffer::WaitSyncClient(POGL_UINT64 timeout, IPOGLWaitSyncJob* job)
{
	return mSyncObject->WaitSyncClient(timeout, job);
}

POGLResourceType::Enum POGLVertexBuffer::GetResourceType() const
{
	return POGLResourceType::VERTEX_BUFFER;
}

POGL_UINT32 POGLVertexBuffer::GetNumVertices() const
{
	return mNumVertices;
}

void POGLVertexBuffer::Draw(POGL_UINT32 startIndex)
{
	Draw(startIndex, mNumVertices);
}

void POGLVertexBuffer::Draw(POGL_UINT32 startIndex, POGL_UINT32 count)
{
	if (count > mNumVertices - startIndex) {
		count = mNumVertices - startIndex;
	}

	glDrawArrays(mPrimitiveType, startIndex, count);
}

void POGLVertexBuffer::Draw(POGLIndexBuffer* indexBuffer, POGL_UINT32 startIndex)
{
	assert_not_null(indexBuffer);

	const POGL_UINT32 numIndices = indexBuffer->GetNumElements();
	const GLvoid* indices = (const GLvoid*)(startIndex * indexBuffer->GetTypeSize());
	glDrawElements(mPrimitiveType, numIndices, indexBuffer->GetType(), indices);
}

void POGLVertexBuffer::Draw(POGLIndexBuffer* indexBuffer, POGL_UINT32 startIndex, POGL_UINT32 count)
{
	assert_not_null(indexBuffer);

	const POGL_UINT32 numIndices = indexBuffer->GetNumElements();
	if (count > numIndices - startIndex) {
		count = numIndices - startIndex;
	}

	const GLvoid* indices = (const GLvoid*)(startIndex * indexBuffer->GetTypeSize());
	glDrawElements(mPrimitiveType, count, indexBuffer->GetType(), indices);
}

void* POGLVertexBuffer::Map(POGLDeviceContext* context, POGLRenderState* state, GLenum access)
{
	mSyncObject->Lock();
	mSyncObject->WaitSyncClient();
	state->BindVertexBuffer(this);
	return context->MapBuffer(GL_ARRAY_BUFFER, access);
}

void* POGLVertexBuffer::MapRange(POGLDeviceContext* context, POGLRenderState* state, POGL_UINT32 offset, POGL_UINT32 length, GLbitfield access)
{
	mSyncObject->Lock();
	mSyncObject->WaitSyncClient();
	state->BindVertexBuffer(this);
	return context->MapBufferRange(GL_ARRAY_BUFFER, offset, length, access);
}

void POGLVertexBuffer::Unmap(POGLDeviceContext* context, POGLRenderState* state)
{
	context->UnmapBuffer(GL_ARRAY_BUFFER);
	mSyncObject->QueueFence();
	mSyncObject->Unlock();
}
