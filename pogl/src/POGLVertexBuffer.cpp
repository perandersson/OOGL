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

POGLVertexBuffer::POGLVertexBuffer(GLuint bufferID, GLuint vaoID, const POGL_VERTEX_LAYOUT* layout, POGL_UINT32 numVertices, GLenum primitiveType, GLenum bufferUsage, IPOGLDevice* device)
: mRefCount(1), mUID(GenVertexBufferUID()), mVertexArrayObject(vaoID), mBufferID(bufferID), 
mLayout(layout), mNumVertices(numVertices), mPrimitiveType(primitiveType), mBufferUsage(bufferUsage), mDevice(device)
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

		POGLDeviceContext* context = static_cast<POGLDeviceContext*>(mDevice->GetDeviceContext());
		if (mBufferID != 0) {
			context->DeleteBuffer(mBufferID);
			mBufferID = 0;
		}
		if (mVertexArrayObject != 0) {
			context->BindVertexArray(0);
			context->DeleteVertexArray(mVertexArrayObject);
			mVertexArrayObject = 0;
		}
		context->Release();

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

POGL_UINT32 POGLVertexBuffer::GetNumVertices() const
{
	return mNumVertices;
}

void POGLVertexBuffer::Draw(POGLDeviceContext* context, POGL_UINT32 startIndex)
{
	glDrawArrays(mPrimitiveType, startIndex, mNumVertices);
}

void POGLVertexBuffer::Draw(POGLDeviceContext* context, POGL_UINT32 startIndex, POGL_UINT32 count)
{
	glDrawArrays(mPrimitiveType, startIndex, count);
}

void POGLVertexBuffer::Draw(POGLDeviceContext* context, POGLIndexBuffer* indexBuffer, POGL_UINT32 startIndex)
{
	assert_not_null(indexBuffer);
	indexBuffer->Draw(context, this, mPrimitiveType, startIndex);
}

void POGLVertexBuffer::Draw(POGLDeviceContext* context, POGLIndexBuffer* indexBuffer, POGL_UINT32 startIndex, POGL_UINT32 count)
{
	assert_not_null(indexBuffer);
	indexBuffer->Draw(context, this, mPrimitiveType, startIndex, count);
}
