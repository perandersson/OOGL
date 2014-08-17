#include "MemCheck.h"
#include "POGLFactory.h"
#include "POGLEnum.h"
#include "POGLTexture2D.h"
#include "POGLTextureResource.h"

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

GLuint POGLFactory::GenFramebufferID()
{
	GLuint id = 0;
	glGenFramebuffers(1, &id);

	const GLenum error = glGetError();
	if (id == 0 || error != GL_NO_ERROR)
		THROW_EXCEPTION(POGLResourceException, "Could not generate framebuffer ID. Reason: 0x%x", error);

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

GLuint POGLFactory::GenFramebufferObjectID(IPOGLTexture** textures, IPOGLTexture* depthStencilTexture)
{
	const GLuint framebufferID = GenFramebufferID();

	glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);
	CHECK_GL("Could not bind framebuffer");

	if (textures != nullptr) {
		POGL_UINT32 idx = 0;
		for (IPOGLTexture** ptr = textures; *ptr != nullptr; ++ptr) {
			IPOGLTexture* texture = *ptr;
			const POGLResourceType::Enum type = texture->GetType();
			if (type == POGLResourceType::TEXTURE2D) {
				POGLTexture2D* t2d = static_cast<POGLTexture2D*>(texture);
				POGLTextureResource* resource = t2d->GetResourcePtr();
				glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + idx, resource->GetTextureID(), 0);
				CHECK_GL("Could not attach framebuffer texture to frame buffer");
			}
			else {
				THROW_NOT_IMPLEMENTED_EXCEPTION();
			}
			idx++;
		}
	}
	if (depthStencilTexture != nullptr) {
		const POGLResourceType::Enum type = depthStencilTexture->GetType();
		if (type == POGLResourceType::TEXTURE2D) {
			POGLTexture2D* t2d = static_cast<POGLTexture2D*>(depthStencilTexture);
			POGLTextureResource* resource = t2d->GetResourcePtr();
			GLenum attachmentType = GL_DEPTH_ATTACHMENT;
			switch (resource->GetTextureFormat()) {
			case POGLTextureFormat::DEPTH24:
			case POGLTextureFormat::DEPTH32F:
				attachmentType = GL_DEPTH_ATTACHMENT;
				break;
			case POGLTextureFormat::DEPTH24_STENCIL8:
			case POGLTextureFormat::DEPTH32F_STENCIL8:
				attachmentType = GL_DEPTH_STENCIL_ATTACHMENT;
				break;
			default:
				THROW_EXCEPTION(POGLResourceException, "You cannot bind a non-depth-stencil texture as a depth buffer");
			}
			glFramebufferTexture(GL_FRAMEBUFFER, attachmentType, resource->GetTextureID(), 0);

		}
		else {
			THROW_NOT_IMPLEMENTED_EXCEPTION();
		}
	}

	//
	// Build framebuffer
	//

	const GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	switch (status)
	{
	case GL_FRAMEBUFFER_COMPLETE_EXT:
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
		THROW_EXCEPTION(POGLResourceException, "Framebuffer incomplete: Attachment is NOT complete.");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
		THROW_EXCEPTION(POGLResourceException, "Framebuffer incomplete: No image is attached to FBO.");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
		THROW_EXCEPTION(POGLResourceException, "Framebuffer incomplete: Attached images have different dimensions");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
		THROW_EXCEPTION(POGLResourceException, "Framebuffer incomplete: Color attached images have different internal formats");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
		THROW_EXCEPTION(POGLResourceException, "Framebuffer incomplete: Draw buffer");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
		THROW_EXCEPTION(POGLResourceException, "Framebuffer incomplete: Read buffer");
		break;
	case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
		THROW_EXCEPTION(POGLResourceException, "Unsupported by FBO implementation");
		break;
	default:
		THROW_EXCEPTION(POGLResourceException, "Unknow error");
		break;
	}

	return framebufferID;
}
