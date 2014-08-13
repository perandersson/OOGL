#include "MemCheck.h"
#include "POGLIndexBuffer.h"

namespace {
	std::atomic<POGL_UINT32> uid;
	POGL_UINT32 GenIndexBufferUID() {
		return ++uid;
	}
}

POGLIndexBuffer::POGLIndexBuffer(GLuint bufferID, POGL_UINT32 typeSize, POGL_UINT32 numIndices, GLenum type, GLenum bufferUsage)
: mRefCount(1), mUID(GenIndexBufferUID()), mBufferID(bufferID), mTypeSize(typeSize), mNumIndices(numIndices), mType(type), mBufferUsage(bufferUsage)
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
			glDeleteBuffers(1, &mBufferID);
			mBufferID = 0;
		}
		delete this;
	}
}

POGLResourceType::Enum POGLIndexBuffer::GetResourceType() const
{
	return POGLResourceType::INDEXBUFFER;
}

POGL_UINT32 POGLIndexBuffer::GetCount() const
{
	return mNumIndices;
}

void POGLIndexBuffer::Draw(POGLVertexBuffer* vertexBuffer, GLenum primitiveType, POGL_UINT32 startIndex)
{
	const GLvoid* indices = (const GLvoid*)(startIndex * mTypeSize);
	glDrawElements(primitiveType, mNumIndices, mType, indices);
}

void POGLIndexBuffer::Draw(POGLVertexBuffer* vertexBuffer, GLenum primitiveType, POGL_UINT32 startIndex, POGL_UINT32 count)
{
	const GLvoid* indices = (const GLvoid*)(startIndex * mTypeSize);
	glDrawElements(primitiveType, count, mType, indices);
}
