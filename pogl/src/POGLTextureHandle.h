#pragma once
#include "config.h"
#include <gl/pogl.h>

struct POGLTextureHandle
{
	POGL_UINT32 uid;
	GLuint textureID;
	GLenum textureTarget; // GL_TEXTURE_2D, .....
	POGL_SIZEI size;
	GLenum textureFormat;

	POGLTextureHandle(POGL_UINT32 _uid, GLuint _textureID, GLenum _textureTarget, const POGL_SIZEI& _size, GLenum _format);
	~POGLTextureHandle();
};
