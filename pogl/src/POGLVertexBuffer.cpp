#include "MemCheck.h"
#include "POGLVertexBuffer.h"
#include "POGLIndexBuffer.h"
#include "POGLFactory.h"
#include "POGLRenderState.h"
#include "POGLEnum.h"

namespace {
	std::atomic<POGL_UINT32> uid;
	POGL_UINT32 GenVertexBufferUID() {
		return ++uid;
	}
}

POGLVertexBuffer::POGLVertexBuffer(POGL_UINT32 count, const POGL_VERTEX_LAYOUT* layout, GLenum primitiveType, POGLBufferUsage::Enum bufferUsage, IPOGLBufferResourceProvider* provider)
: mRefCount(1), mUID(0), mBufferID(0), mCount(count), mVAOID(0), mLayout(layout), mPrimitiveType(primitiveType), mBufferResource(nullptr)
{
	const POGL_UINT32 memorySize = count * layout->vertexSize;
	mBufferResource = provider->CreateBuffer(memorySize, GL_ARRAY_BUFFER, bufferUsage);

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
		if (mBufferResource != nullptr) {
			mBufferResource->Release();
			mBufferResource = nullptr;
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

POGL_UINT32 POGLVertexBuffer::GetMemorySize() const
{
	return mCount * mLayout->vertexSize;
}

void* POGLVertexBuffer::Map(POGLResourceMapType::Enum e)
{
	return mBufferResource->Map(e);
}

void* POGLVertexBuffer::Map(POGL_UINT32 offset, POGL_UINT32 length, POGLResourceMapType::Enum e)
{
	return mBufferResource->Map(offset, length, e);
}

void POGLVertexBuffer::Unmap()
{
	return mBufferResource->Unmap();
}

void POGLVertexBuffer::Draw()
{
	mBufferResource->Lock();
	glDrawArrays(mPrimitiveType, 0, mCount);
	mBufferResource->Unlock();
}

void POGLVertexBuffer::Draw(POGL_UINT32 count)
{
	mBufferResource->Lock(0, count * mLayout->vertexSize);
	glDrawArrays(mPrimitiveType, 0, count);
	mBufferResource->Unlock();
}

void POGLVertexBuffer::Draw(POGL_UINT32 count, POGL_UINT32 offset)
{
	mBufferResource->Lock(offset * mLayout->vertexSize, count * mLayout->vertexSize);
	glDrawArrays(mPrimitiveType, offset, count);
	mBufferResource->Unlock();
}

void POGLVertexBuffer::DrawIndexed(POGLIndexBuffer* indexBuffer)
{
	mBufferResource->Lock();
	indexBuffer->DrawIndexed(mPrimitiveType);
	mBufferResource->Unlock();
}

void POGLVertexBuffer::DrawIndexed(POGLIndexBuffer* indexBuffer, POGL_UINT32 count)
{
	mBufferResource->Lock();
	indexBuffer->DrawIndexed(mPrimitiveType, count);
	mBufferResource->Unlock();
}

void POGLVertexBuffer::DrawIndexed(POGLIndexBuffer* indexBuffer, POGL_UINT32 count, POGL_UINT32 offset)
{
	mBufferResource->Lock();
	indexBuffer->DrawIndexed(mPrimitiveType, count, offset);
	mBufferResource->Unlock();
}

void POGLVertexBuffer::PostConstruct(POGLRenderState* renderState)
{
	glGenVertexArrays(1, &mVAOID);
	const GLenum error = glGetError();
	if (mVAOID == 0 || error != GL_NO_ERROR)
		THROW_EXCEPTION(POGLResourceException, "Could not generate vertex array object ID. Reason: 0x%x", error);

	glBindVertexArray(mVAOID);
	mBufferResource->PostConstruct(renderState);

	//
	// Define how the vertex attributes are located
	//

	POGL_UINT32 offset = 0;
	for (POGL_UINT32 i = 0; i < MAX_VERTEX_LAYOUT_FIELD_SIZE; ++i) {
		const POGL_VERTEX_LAYOUT_FIELD& field = mLayout->fields[i];
		if (field.fieldSize == 0) {
			continue;
		}

		// Enable vertex attribute location if neccessary
		glEnableVertexAttribArray(i);
		CHECK_GL("Could not enable vertex attrib location for the vertex array object");

		const POGL_UINT32 typeSize = POGLEnum::VertexTypeSize(field.type);
		const GLint numElementsInField = field.fieldSize / typeSize;
		const auto type = field.type;
		switch (type) {
		case POGLVertexType::BYTE:
		case POGLVertexType::UNSIGNED_BYTE:
		case POGLVertexType::SHORT:
		case POGLVertexType::UNSIGNED_SHORT:
		case POGLVertexType::INT:
		case POGLVertexType::UNSIGNED_INT:
			glVertexAttribIPointer(i, numElementsInField, POGLEnum::Convert(type), mLayout->vertexSize, OFFSET(offset));
			break;
		case POGLVertexType::FLOAT:
			glVertexAttribPointer(i, numElementsInField, POGLEnum::Convert(type), field.normalize ? GL_TRUE : GL_FALSE, mLayout->vertexSize, OFFSET(offset));
			break;
		case POGLVertexType::DOUBLE:
			glVertexAttribLPointer(i, numElementsInField, POGLEnum::Convert(type), mLayout->vertexSize, OFFSET(offset));
			break;
		}

		CHECK_GL("Could not set vertex attrib location for the vertex array object");
		offset += field.fieldSize;
	}

	mUID = GenVertexBufferUID();

	// Ensure that the vertex buffer is bound
	renderState->ForceSetVertexBuffer(this);
}
