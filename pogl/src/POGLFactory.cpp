#include "MemCheck.h"
#include "POGLFactory.h"
#include "POGLEnum.h"

GLuint POGLFactory::GenSamplerID()
{
	GLuint id = 0;
	glGenSamplers(1, &id);

	const GLenum error = glGetError();
	if (id == 0 || error != GL_NO_ERROR)
		THROW_EXCEPTION(POGLResourceException, "Could not generate sampler ID");

	return id;
}

GLuint POGLFactory::GenBufferID()
{
	GLuint id = 0;
	glGenBuffers(1, &id);

	const GLenum error = glGetError();
	if (id == 0 || error != GL_NO_ERROR)
		THROW_EXCEPTION(POGLResourceException, "Could not generate buffer ID. Reason: 0x%x", error);

	return id;
}

GLuint POGLFactory::GenTextureID()
{
	GLuint id = 0;
	glGenTextures(1, &id);

	const GLenum error = glGetError();
	if (id == 0 || error != GL_NO_ERROR)
		THROW_EXCEPTION(POGLResourceException, "Could not generate texture ID. Reason: 0x%x", error);

	return id;
}

GLuint POGLFactory::GenVertexArrayObjectID(GLuint bufferID, const POGL_VERTEX_LAYOUT* layout)
{
	//
	// Generate a vertex array object
	//

	GLuint id = 0;
	glGenVertexArrays(1, &id);

	GLenum error = glGetError();
	if (id == 0 || error != GL_NO_ERROR)
		THROW_EXCEPTION(POGLResourceException, "Could not generate vertex array object ID. Reason: 0x%x", error);

	//
	// Bind the vertex array object and attach the buffer ID to it
	//

	glBindVertexArray(id);
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);

	//
	// Define how the vertex attributes are located
	//

	POGL_UINT32 offset = 0;
	for (POGL_UINT32 i = 0; i < MAX_VERTEX_LAYOUT_FIELD_SIZE; ++i) {
		const POGL_VERTEX_LAYOUT_FIELD& field = layout->fields[i];
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
			glVertexAttribIPointer(i, numElementsInField, POGLEnum::Convert(type), layout->vertexSize, OFFSET(offset));
			break;
		case POGLVertexType::FLOAT:
			glVertexAttribPointer(i, numElementsInField, POGLEnum::Convert(type), field.normalize ? GL_TRUE : GL_FALSE, layout->vertexSize, OFFSET(offset));
			break;
		case POGLVertexType::DOUBLE:
			glVertexAttribLPointer(i, numElementsInField, POGLEnum::Convert(type), layout->vertexSize, OFFSET(offset));
			break;
		}

		CHECK_GL("Could not set vertex attrib location for the vertex array object");
		offset += field.fieldSize;
	}

	return id;
}
