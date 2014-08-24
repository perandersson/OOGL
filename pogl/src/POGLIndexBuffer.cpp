#include "MemCheck.h"
#include "POGLIndexBuffer.h"
#include "POGLRenderState.h"
#include "providers/POGLDefaultBufferResource.h"

namespace {
	std::atomic<POGL_UINT32> uid;
	POGL_UINT32 GenIndexBufferUID() {
		return ++uid;
	}
}

POGLIndexBuffer::POGLIndexBuffer(POGL_UINT32 typeSize, POGL_UINT32 numIndices, GLenum elementType, POGLBufferUsage::Enum bufferUsage, IPOGLBufferResourceProvider* provider)
: mRefCount(1), mUID(0), mTypeSize(typeSize), mNumIndices(numIndices), mElementType(elementType), mBufferID(0), mBufferResource(nullptr)
{
	const POGL_UINT32 memorySize = typeSize * numIndices;
	mBufferResource = provider->CreateBuffer(memorySize, GL_ELEMENT_ARRAY_BUFFER, bufferUsage);
}

POGLIndexBuffer::~POGLIndexBuffer()
{
}

void POGLIndexBuffer::PostConstruct(POGLRenderState* renderState)
{
	mBufferID = mBufferResource->PostConstruct(renderState);
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
		if (mBufferResource != nullptr) {
			mBufferResource->Release();
			mBufferResource = nullptr;
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
	return mBufferResource->Map(e);
}

void* POGLIndexBuffer::Map(POGL_UINT32 offset, POGL_UINT32 length, POGLResourceMapType::Enum e)
{
	return mBufferResource->Map(offset, length, e);
}

void POGLIndexBuffer::Unmap()
{
	return mBufferResource->Unmap();
}

void POGLIndexBuffer::DrawIndexed(GLenum primitiveType)
{
	mBufferResource->Lock();
	glDrawElements(primitiveType, mNumIndices, mElementType, NULL);
	mBufferResource->Unlock();
}

void POGLIndexBuffer::DrawIndexed(GLenum primitiveType, POGL_UINT32 count)
{
	mBufferResource->Lock();
	glDrawElements(primitiveType, count, mElementType, NULL);
	mBufferResource->Unlock();
}

void POGLIndexBuffer::DrawIndexed(GLenum primitiveType, POGL_UINT32 count, POGL_UINT32 offset)
{
	mBufferResource->Lock();
	const GLvoid* indices = (const GLvoid*)(offset * mTypeSize);
	glDrawElements(primitiveType, count, mElementType, indices);
	mBufferResource->Unlock();
}
