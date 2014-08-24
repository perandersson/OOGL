#include "MemCheck.h"
#include "POGLIndexBuffer.h"
#include "POGLRenderState.h"

namespace {
	std::atomic<POGL_UINT32> uid;
	POGL_UINT32 GenIndexBufferUID() {
		return ++uid;
	}
}

POGLIndexBuffer::POGLIndexBuffer(POGL_UINT32 typeSize, POGL_UINT32 numIndices, GLenum elementType, GLenum bufferUsage)
: mRefCount(1), mUID(0), mBufferID(0), mTypeSize(typeSize), mNumIndices(numIndices), mElementType(elementType), mBufferUsage(bufferUsage)
{

}

POGLIndexBuffer::~POGLIndexBuffer()
{
}

void POGLIndexBuffer::PostConstruct(POGLRenderState* renderState)
{
	glGenBuffers(1, &mBufferID);
	const GLenum error = glGetError();
	if (mBufferID == 0 || error != GL_NO_ERROR)
		THROW_EXCEPTION(POGLResourceException, "Could not generate buffer ID. Reason: 0x%x", error);

	mUID = GenIndexBufferUID();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBufferID);
	renderState->ForceSetIndexBuffer(this);
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

POGLResourceType::Enum POGLIndexBuffer::GetType() const
{
	return POGLResourceType::INDEXBUFFER;
}

POGL_UINT32 POGLIndexBuffer::GetCount() const
{
	return mNumIndices;
}

void POGLIndexBuffer::Draw(GLenum primitiveType)
{
	glDrawElements(primitiveType, mNumIndices, mElementType, NULL);
}

void POGLIndexBuffer::Draw(GLenum primitiveType, POGL_UINT32 count)
{
	glDrawElements(primitiveType, count, mElementType, NULL);
}

void POGLIndexBuffer::Draw(GLenum primitiveType, POGL_UINT32 count, POGL_UINT32 offset)
{
	const GLvoid* indices = (const GLvoid*)(offset * mTypeSize);
	glDrawElements(primitiveType, count, mElementType, indices);
}
