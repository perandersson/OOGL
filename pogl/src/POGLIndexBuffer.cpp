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

	// Initialize the buffer size
	const POGL_UINT32 memorySize = mTypeSize * mNumIndices;
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, memorySize, 0, mBufferUsage);

	// Ensure that the index buffer is bound
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

void* POGLIndexBuffer::Map(POGLResourceMapType::Enum e)
{
	if (e == POGLResourceMapType::WRITE)
		return glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);

	THROW_NOT_IMPLEMENTED_EXCEPTION();
}

void* POGLIndexBuffer::Map(POGL_UINT32 offset, POGL_UINT32 length, POGLResourceMapType::Enum e)
{
	if (e == POGLResourceMapType::WRITE) {
		const POGL_UINT32 memorySize = mTypeSize * mNumIndices;
		if (offset + length > memorySize)
			THROW_EXCEPTION(POGLStateException, "You cannot map with offset: %d and length: %d when the index buffer size is: %d", offset, length, memorySize);

		return glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, offset, length, GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT);
	}

	THROW_NOT_IMPLEMENTED_EXCEPTION();
}

void POGLIndexBuffer::Unmap()
{
	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
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
