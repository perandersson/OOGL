#pragma once
#include "config.h"
#include <vector>

class POGLFramebuffer : public IPOGLFramebuffer
{
public:
	POGLFramebuffer(GLuint framebufferID);
	virtual ~POGLFramebuffer();

// IPOGLInterface
public:
	virtual void AddRef();
	virtual void Release();
	
// IPOGLResource
public:
	virtual POGLResourceType::Enum GetResourceType() const;

// IPOGLFramebuffer
public:
	virtual IPOGLTexture* GetTexture(POGL_UINT32 idx);
	virtual IPOGLTexture* GetDepthStencilTexture();

private:
	std::atomic<POGL_UINT32> mRefCount;
	GLuint mFramebufferID;
	std::vector<IPOGLTexture*> mTextures;
	IPOGLTexture* mDepthStencilTexture;
};
