#pragma once
#include "config.h"

class POGLAPI POGLFactory
{
public:
	/*!
		\brief Generate a new sampler ID
	*/
	static GLuint GenSamplerID();

	/*!
		\brief Generate a new bufferID
	*/
	static GLuint GenBufferID();

	/*!
		\brief Generate a new texture ID
	*/
	static GLuint GenTextureID();

	/*!
		\brief Generate a new framebuffer ID
	*/
	static GLuint GenFramebufferID();

	/*!
		\brief Generate a vertex array object ID for the supplied buffer and layout

		\param bufferID
		\param layout
		\return
	*/
	static GLuint GenVertexArrayObjectID(GLuint bufferID, const POGL_VERTEX_LAYOUT* layout);

	/*!
		\brief Generates a framebuffer ID based on the supplied parameters
	*/
	static GLuint GenFramebufferObjectID(IPOGLTexture** textures, POGL_UINT32 count, IPOGLTexture* depthStencilTexture);

	/*!
		\brief Creates a shader OpenGL ID based on the supplied parameters
	*/
	static GLuint CreateShader(const POGL_CHAR* memory, POGL_UINT32 size, POGLShaderType::Enum type);

	/*!
		\brief Creates a program ID based on the supplied shaders
	*/
	static GLuint CreateProgram(IPOGLShader** shaders, POGL_UINT32 count);
};
