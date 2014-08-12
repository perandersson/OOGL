#include "MemCheck.h"
#include "POGLIndexBuffer.h"
#include "POGLDeviceContext.h"
#include "POGLSyncObject.h"
#include "POGLRenderState.h"
#include <atomic>
namespace {
	std::atomic<POGL_UINT32> uid;
	POGL_UINT32 GenIndexBufferUID() {
		return ++uid;
	}
}

POGLIndexBuffer::POGLIndexBuffer(GLuint bufferID, POGL_UINT32 typeSize, POGL_UINT32 numIndices, GLenum type, GLenum bufferUsage, IPOGLDevice* device)
: mRefCount(1), mUID(GenIndexBufferUID()), mBufferID(bufferID), mTypeSize(typeSize), mNumIndices(numIndices), mType(type), mBufferUsage(bufferUsage), mDevice(device)
{

}

POGLIndexBuffer::~POGLIndexBuffer()
{
}

void POGLIndexBuffer::AddRef()
{
	mRefCount++;
}

void POGLIndexBuffer::Release()
{
	if (--mRefCount == 0) {
		if (mBufferID != 0) {
			POGLDeviceContext* context = static_cast<POGLDeviceContext*>(mDevice->GetDeviceContext());
			context->DeleteBuffer(mBufferID);
			context->Release();
			mBufferID = 0;
		}
		delete this;
	}
}

IPOGLDevice* POGLIndexBuffer::GetDevice()
{
	mDevice->AddRef();
	return mDevice;
}

POGL_HANDLE POGLIndexBuffer::GetHandlePtr()
{
	return this;
}
//
//void POGLIndexBuffer::WaitSyncDriver(IPOGLDeviceContext* context)
//{
//	mSyncObject->WaitSyncDriver(static_cast<POGLDeviceContext*>(context));
//}
//
//void POGLIndexBuffer::WaitSyncClient(IPOGLDeviceContext* context)
//{
//	mSyncObject->WaitSyncClient(static_cast<POGLDeviceContext*>(context));
//}
//
//bool POGLIndexBuffer::WaitSyncClient(IPOGLDeviceContext* context, POGL_UINT64 timeout)
//{
//	return mSyncObject->WaitSyncClient(static_cast<POGLDeviceContext*>(context), timeout);
//}
//
//bool POGLIndexBuffer::WaitSyncClient(IPOGLDeviceContext* context, POGL_UINT64 timeout, IPOGLWaitSyncJob* job)
//{
//	return mSyncObject->WaitSyncClient(static_cast<POGLDeviceContext*>(context), timeout, job);
//}

POGL_UINT32 POGLIndexBuffer::GetNumElements() const
{
	return mNumIndices;
}

void POGLIndexBuffer::Draw(POGLDeviceContext* context, POGLVertexBuffer* vertexBuffer, GLenum primitiveType, POGL_UINT32 startIndex)
{
//	mSyncObject->WaitSyncDriver(context);
	const GLvoid* indices = (const GLvoid*)(startIndex * mTypeSize);
	glDrawElements(primitiveType, mNumIndices, mType, indices);
}

void POGLIndexBuffer::Draw(POGLDeviceContext* context, POGLVertexBuffer* vertexBuffer, GLenum primitiveType, POGL_UINT32 startIndex, POGL_UINT32 count)
{
//	mSyncObject->WaitSyncDriver(context);
	const GLvoid* indices = (const GLvoid*)(startIndex * mTypeSize);
	glDrawElements(primitiveType, count, mType, indices);
}
