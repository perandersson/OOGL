#pragma once
#include "config.h"

class POGLTextureResource : public IPOGLInterface
{
public:
	POGLTextureResource(GLenum textureTarget, POGLTextureFormat::Enum format);
	virtual ~POGLTextureResource();
	
	/*!
		\brief Method called when the texture is completed in it's construction

		\param textureID
	*/
	void PostConstruct(GLuint textureID);

	/*!
		\brief Retrieves the unique ID for this resource.
	*/
	inline POGL_UINT32 GetUID() const {
		return mUID;
	}

	/*!
		\brief Retrieves the internal OpenGL texture ID for this object
	*/
	inline GLuint GetTextureID() const {
		return mTextureID;
	}

	/*!
		\brief Retrieves the texture target for this texture.
	*/
	inline GLenum GetTextureTarget() const {
		return mTextureTarget;
	}

	/*!
		\brief Retrieves the format for this texture
	*/
	inline POGLTextureFormat::Enum GetTextureFormat() const {
		return mTextureFormat;
	}

// IPOGLInterface
public:
	virtual void AddRef();
	virtual void Release();
	
private:
	REF_COUNTER mRefCount;
	POGL_UINT32 mUID;
	GLuint mTextureID;
	GLenum mTextureTarget;
	POGLTextureFormat::Enum mTextureFormat;
};
