#include "POGLVertexBuffer.hxx"
#include "POGLDeviceContext.hxx"
#include "POGLIndexBuffer.hxx"
#include "POGLEnum.hxx"
#include <atomic>

namespace {
	std::atomic<POGL_UINT32> uid;
	POGL_UINT32 GenVertexBufferUID() {
		return ++uid;
	}
}

POGLVertexBuffer::POGLVertexBuffer(GLuint bufferID, const POGL_VERTEX_LAYOUT* layout, POGL_UINT32 numVertices, GLenum primitiveType, GLenum bufferUsage, IPOGLDevice* device)
: mRefCount(1), mUID(GenVertexBufferUID()), mBufferID(bufferID), mLayout(layout), mNumVertices(numVertices), mPrimitiveType(primitiveType), mBufferUsage(bufferUsage), mDevice(device)
{
}

POGLVertexBuffer::~POGLVertexBuffer()
{
	if (mBufferID != 0) {
		IPOGLDeviceContext* context = mDevice->GetContext();
		static_cast<POGLDeviceContext*>(context)->DeleteBuffer(mBufferID);
		context->Release();
		mBufferID = 0;
	}
}

void POGLVertexBuffer::AddRef()
{
	mRefCount++;
}

void POGLVertexBuffer::Release()
{
	if (--mRefCount == 0)
		delete this;
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

const POGL_VERTEX_LAYOUT* POGLVertexBuffer::GetLayout() const
{
	return mLayout;
}

POGL_UINT32 POGLVertexBuffer::GetNumVertices() const
{
	return mNumVertices;
}

POGL_UINT32 POGLVertexBuffer::GetUID() const
{
	return mUID;
}

GLuint POGLVertexBuffer::GetBufferID() const
{
	return mBufferID;
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
	Draw(indexBuffer, startIndex, indexBuffer->GetNumElements());
}

void POGLVertexBuffer::Draw(POGLIndexBuffer* indexBuffer, POGL_UINT32 startIndex, POGL_UINT32 count)
{
	assert_not_null(indexBuffer);

	const POGL_UINT32 numIndices = indexBuffer->GetNumElements();
	if (count > numIndices - startIndex) {
		count = numIndices - startIndex;
	}

	const GLvoid* indices = (void*)(startIndex * indexBuffer->GetTypeSize());
	glDrawElements(mPrimitiveType, count, GL_UNSIGNED_INT, indices);
}
