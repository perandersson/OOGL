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

void* POGLVertexBuffer::Map(POGLResourceMapType::Enum e)
{
	if (e == POGLResourceMapType::WRITE)
		return glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

	THROW_NOT_IMPLEMENTED_EXCEPTION();
}

void* POGLVertexBuffer::Map(POGL_UINT32 offset, POGL_UINT32 length, POGLResourceMapType::Enum e)
{
	if (e == POGLResourceMapType::WRITE) {
		const POGL_UINT32 memorySize = mCount * mLayout->vertexSize;
		if (offset + length > memorySize)
			THROW_EXCEPTION(POGLStateException, "You cannot map with offset: %d and length: %d when the vertex buffer size is: %d", offset, length, memorySize);

		return glMapBufferRange(GL_ARRAY_BUFFER, offset, length, GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT);
	}

	THROW_NOT_IMPLEMENTED_EXCEPTION();
}

void POGLVertexBuffer::Unmap()
{
	glUnmapBuffer(GL_ARRAY_BUFFER);
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

void POGLVertexBuffer::PostConstruct(POGLRenderState* renderState)
{
	glGenBuffers(1, &mBufferID);
	GLenum error = glGetError();
	if (mBufferID == 0 || error != GL_NO_ERROR)
		THROW_EXCEPTION(POGLResourceException, "Could not generate buffer ID. Reason: 0x%x", error);

	glGenVertexArrays(1, &mVAOID);
	error = glGetError();
	if (mVAOID == 0 || error != GL_NO_ERROR)
		THROW_EXCEPTION(POGLResourceException, "Could not generate vertex array object ID. Reason: 0x%x", error);

	glBindVertexArray(mVAOID);
	glBindBuffer(GL_ARRAY_BUFFER, mBufferID);

	// Initialize the buffer size
	const POGL_UINT32 memorySize = mCount * mLayout->vertexSize;
	glBufferData(GL_ARRAY_BUFFER, memorySize, 0, mBufferUsage);

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
