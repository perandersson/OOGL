#include "MemCheck.h"
#include "POGLVertexBuffer.h"
#include "POGLIndexBuffer.h"
#include "POGLEnum.h"

namespace {
	std::atomic<POGL_UINT32> uid;
	POGL_UINT32 GenVertexBufferUID() {
		return ++uid;
	}
}

POGLVertexBuffer::POGLVertexBuffer(GLuint bufferID, POGL_UINT32 count, GLuint vaoID, const POGL_VERTEX_LAYOUT* layout, GLenum primitiveType, GLenum bufferUsage)
: mRefCount(1), mUID(GenVertexBufferUID()), mBufferID(bufferID), mCount(count), mVAOID(vaoID), mLayout(layout), mPrimitiveType(primitiveType), mBufferUsage(bufferUsage)
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
			glDeleteBuffers(1, &mBufferID);
			mBufferID = 0;
		}
		if (mVAOID != 0) {
			glDeleteVertexArrays(1, &mVAOID);
			mVAOID = 0;
		}
		delete this;
	}
}

POGLResourceType::Enum POGLVertexBuffer::GetResourceType() const
{
	return POGLResourceType::VERTEXBUFFER;
}

const POGL_VERTEX_LAYOUT* POGLVertexBuffer::GetLayout() const
{
	return mLayout;
}

POGL_UINT32 POGLVertexBuffer::GetCount() const
{
	return mCount;
}

void POGLVertexBuffer::Draw(POGL_UINT32 startIndex)
{
	glDrawArrays(mPrimitiveType, startIndex, mCount);
}

void POGLVertexBuffer::Draw(POGL_UINT32 startIndex, POGL_UINT32 count)
{
	glDrawArrays(mPrimitiveType, startIndex, count);
}

void POGLVertexBuffer::Draw(POGLIndexBuffer* indexBuffer, POGL_UINT32 startIndex)
{
	assert_not_null(indexBuffer);
	indexBuffer->Draw(this, mPrimitiveType, startIndex);
}

void POGLVertexBuffer::Draw(POGLIndexBuffer* indexBuffer, POGL_UINT32 startIndex, POGL_UINT32 count)
{
	assert_not_null(indexBuffer);
	indexBuffer->Draw(this, mPrimitiveType, startIndex, count);
}

void POGLVertexBuffer::PostConstruct(GLuint bufferID, GLuint vaoID)
{
	mBufferID = bufferID;
	mVAOID = vaoID;
}
