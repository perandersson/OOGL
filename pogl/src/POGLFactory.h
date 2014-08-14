#pragma once
#include "config.h"

class POGLFactory
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
};
