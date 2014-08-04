#include "POGLTextureHandle.hxx"

POGLTextureHandle::POGLTextureHandle(POGL_UINT32 _uid, GLuint _textureID, GLenum _textureTarget, const POGL_SIZEI& _size, GLenum _format)
: uid(uid), textureID(_textureID), textureTarget(_textureTarget), size(_size), textureFormat(_format)
{
}

POGLTextureHandle::~POGLTextureHandle()
{
	if (textureID != 0) {
		glDeleteTextures(1, &textureID);
		textureID = 0;
	}
}
