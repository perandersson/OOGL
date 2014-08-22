#include "MemCheck.h"
#include "POGLVertexBuffer.h"
#include "POGLIndexBuffer.h"

namespace {
	std::atomic<POGL_UINT32> uid;
	POGL_UINT32 GenVertexBufferUID() {
		return ++uid;
	}
}

POGLVertexBuffer::POGLVertexBuffer(POGL_UINT32 count, const POGL_VERTEX_LAYOUT* layout, GLenum primitiveType, GLenum bufferUsage)
: mRefCount(1), mUID(0), mBufferID(0), mCount(count), mVAOID(0), mLayout(layout), mPrimitiveType(primitiveType), mBufferUsage(bufferUsage)
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

POGLResourceType::Enum POGLVertexBuffer::GetType() const
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

void POGLVertexBuffer::Draw()
{
	glDrawArrays(mPrimitiveType, 0, mCount);
}

void POGLVertexBuffer::Draw(POGL_UINT32 count)
{
	glDrawArrays(mPrimitiveType, 0, count);
}

void POGLVertexBuffer::Draw(POGL_UINT32 count, POGL_UINT32 offset)
{
	glDrawArrays(mPrimitiveType, offset, count);
}

void POGLVertexBuffer::PostConstruct(GLuint bufferID, GLuint vaoID)
{
	mBufferID = bufferID;
	mVAOID = vaoID;
	mUID = GenVertexBufferUID();
}
