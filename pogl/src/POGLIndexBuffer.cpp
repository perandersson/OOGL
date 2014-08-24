#include "MemCheck.h"
#include "POGLIndexBuffer.h"
#include "POGLRenderState.h"
#include "POGLBufferResource.h"

namespace {
	std::atomic<POGL_UINT32> uid;
	POGL_UINT32 GenIndexBufferUID() {
		return ++uid;
	}
}

POGLIndexBuffer::POGLIndexBuffer(POGL_UINT32 typeSize, POGL_UINT32 numIndices, GLenum elementType, GLenum bufferUsage)
: mRefCount(1), mUID(0), mTypeSize(typeSize), mNumIndices(numIndices), mElementType(elementType), mBufferUsage(bufferUsage), mResourcePtr(nullptr)
{
	const POGL_UINT32 memorySize = typeSize * numIndices;
	mResourcePtr = new POGLBufferResource(memorySize, GL_ELEMENT_ARRAY_BUFFER, bufferUsage);
}

POGLIndexBuffer::~POGLIndexBuffer()
{
}

void POGLIndexBuffer::PostConstruct(POGLRenderState* renderState)
{
	mResourcePtr->PostConstruct(renderState);
	mUID = GenIndexBufferUID();

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
		if (mResourcePtr != nullptr) {
			mResourcePtr->Release();
			mResourcePtr = nullptr;
		}
		delete this;
	}
}

GLuint POGLIndexBuffer::GetBufferID() const
{
	return mResourcePtr->GetBufferID();
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
	return mResourcePtr->Map(e);
}

void* POGLIndexBuffer::Map(POGL_UINT32 offset, POGL_UINT32 length, POGLResourceMapType::Enum e)
{
	return mResourcePtr->Map(offset, length, e);
}

void POGLIndexBuffer::Unmap()
{
	return mResourcePtr->Unmap();
}

void POGLIndexBuffer::DrawIndexed(GLenum primitiveType)
{
	glDrawElements(primitiveType, mNumIndices, mElementType, NULL);
}

void POGLIndexBuffer::DrawIndexed(GLenum primitiveType, POGL_UINT32 count)
{
	glDrawElements(primitiveType, count, mElementType, NULL);
}

void POGLIndexBuffer::DrawIndexed(GLenum primitiveType, POGL_UINT32 count, POGL_UINT32 offset)
{
	const GLvoid* indices = (const GLvoid*)(offset * mTypeSize);
	glDrawElements(primitiveType, count, mElementType, indices);
}
