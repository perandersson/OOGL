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
: mRefCount(1), mUID(0), mBufferID(bufferID), mCount(count), mVAOID(vaoID), mLayout(layout), mPrimitiveType(primitiveType), mBufferUsage(bufferUsage)
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

void POGLVertexBuffer::PostConstruct(GLuint bufferID)
{
	mBufferID = bufferID;
	mUID = GenVertexBufferUID();

	//
	// Generate a vertex array object
	//

	glGenVertexArrays(1, &mVAOID);
	glBindVertexArray(mVAOID);
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);


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

		static const POGL_UINT32 TYPE_SIZE[POGLVertexType::COUNT] = {
			sizeof(POGL_INT8),
			sizeof(POGL_UINT8),
			sizeof(POGL_INT16),
			sizeof(POGL_UINT16),
			sizeof(POGL_INT32),
			sizeof(POGL_UINT32),
			sizeof(POGL_FLOAT),
			sizeof(POGL_DOUBLE)
		};

		const GLint numElementsInField = field.fieldSize / TYPE_SIZE[(POGL_UINT32)field.type];
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

	const GLenum error = glGetError();
	if (error != GL_NO_ERROR)
		THROW_EXCEPTION(POGLResourceException, "Failed to create a vertex buffer. Reason: 0x%x", error);
}
