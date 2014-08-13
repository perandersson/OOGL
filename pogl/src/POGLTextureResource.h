#pragma once
#include "config.h"
#include <gl/pogl.h>

class POGLTextureResource : public IPOGLInterface
{
public:
	POGLTextureResource(GLuint textureID, GLenum textureTarget, POGLTextureFormat::Enum format);
	virtual ~POGLTextureResource();

	void AddRef();
	void Release();

	POGL_UINT32 GetUID() const;
	GLuint GetTextureID() const;
	GLenum GetTextureTarget() const;
	POGLTextureFormat::Enum GetTextureFormat() const;

private:
	POGL_UINT32 mRefCount;
	POGL_UINT32 mUID;
	GLuint mTextureID;
	GLenum mTextureTarget;
	POGLTextureFormat::Enum mTextureFormat;
};
