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

void POGLVertexBuffer::WaitSyncDriver(IPOGLDeviceContext* context)
{
	mSyncObject->WaitSyncDriver(static_cast<POGLDeviceContext*>(context));
}

void POGLVertexBuffer::WaitSyncClient(IPOGLDeviceContext* context)
{
	mSyncObject->WaitSyncClient(static_cast<POGLDeviceContext*>(context));
}

bool POGLVertexBuffer::WaitSyncClient(IPOGLDeviceContext* context, POGL_UINT64 timeout)
{
	return mSyncObject->WaitSyncClient(static_cast<POGLDeviceContext*>(context), timeout);
}

bool POGLVertexBuffer::WaitSyncClient(IPOGLDeviceContext* context, POGL_UINT64 timeout, IPOGLWaitSyncJob* job)
{
	return mSyncObject->WaitSyncClient(static_cast<POGLDeviceContext*>(context), timeout, job);
}


POGL_UINT32 POGLVertexBuffer::GetNumVertices() const
{
	return mNumVertices;
}

void POGLVertexBuffer::Draw(POGLDeviceContext* context, POGL_UINT32 startIndex)
{
	mSyncObject->WaitSyncDriver(context);
	glDrawArrays(mPrimitiveType, startIndex, mNumVertices);
}

void POGLVertexBuffer::Draw(POGLDeviceContext* context, POGL_UINT32 startIndex, POGL_UINT32 count)
{
	mSyncObject->WaitSyncDriver(context);
	glDrawArrays(mPrimitiveType, startIndex, count);
}

void POGLVertexBuffer::Draw(POGLDeviceContext* context, POGLIndexBuffer* indexBuffer, POGL_UINT32 startIndex)
{
	assert_not_null(indexBuffer);
	mSyncObject->WaitSyncDriver(context);
	indexBuffer->Draw(context, this, mPrimitiveType, startIndex);
}

void POGLVertexBuffer::Draw(POGLDeviceContext* context, POGLIndexBuffer* indexBuffer, POGL_UINT32 startIndex, POGL_UINT32 count)
{
	assert_not_null(indexBuffer);
	mSyncObject->WaitSyncDriver(context);
	indexBuffer->Draw(context, this, mPrimitiveType, startIndex, count);
}
