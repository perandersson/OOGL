#pragma once
#include "config.h"
#include <vector>

class POGLFramebuffer : public IPOGLFramebuffer
{
public:
	POGLFramebuffer(GLuint framebufferID, std::vector<IPOGLTexture*>& textures, IPOGLTexture* depthStencilTexture);
	virtual ~POGLFramebuffer();

	/*!
		\brief Retrieve a unique id for this resource
	*/
	inline POGL_UINT32 GetUID() const {
		return mUID;
	}

	/*!
		\brief Retrieves the OpenGL framebuffer resource ID
	*/
	inline GLuint GetFramebufferID() const {
		return mFramebufferID;
	}

	/*!
		\brief Retrieves the number of draw buffers 
	*/
	inline POGL_UINT32 GetNumDrawBuffers() {
		return mTextures.size();
	}

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
	POGL_UINT32 mUID;
	GLuint mFramebufferID;
	std::vector<IPOGLTexture*> mTextures;
	IPOGLTexture* mDepthStencilTexture;
};
