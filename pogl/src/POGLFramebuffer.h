#pragma once
#include "config.h"
#include <vector>

class POGLFramebuffer : public IPOGLFramebuffer
{
public:
	POGLFramebuffer(std::vector<IPOGLTexture*>& textures, IPOGLTexture* depthStencilTexture);
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

	/*!
		\brief Post-construct this framebuffer
	*/
	void PostConstruct(GLuint framebufferID);

// IPOGLInterface
public:
	virtual void AddRef();
	virtual void Release();
	
// IPOGLFramebuffer
public:
	virtual IPOGLTexture* GetTexture(POGL_UINT32 idx);
	virtual IPOGLTexture* GetDepthStencilTexture();

private:
	REF_COUNTER mRefCount;
	POGL_UID mUID;
	GLuint mFramebufferID;
	std::vector<IPOGLTexture*> mTextures;
	IPOGLTexture* mDepthStencilTexture;
};
