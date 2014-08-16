#pragma once
#include "config.h"
#include <gl/pogl.h>
#include "POGLTextureResource.h"

class POGLTexture2D : public IPOGLTexture2D
{
public:
	POGLTexture2D(const POGL_SIZE& size, POGLTextureFormat::Enum format);
	virtual ~POGLTexture2D();

	/*!
		\brief 
	*/
	inline POGLTextureResource* GetResourcePtr() const {
		return mResourcePtr;
	}

	/*!
		\brief Method called when the texture is completed in it's construction

		\param textureID
	*/
	void PostConstruct(GLuint textureID);

	/*!
		\brief Set a new size for this texture
	*/
	void SetSize(const POGL_SIZE& newSize);

// IPOGLInterface
public:
	virtual void AddRef();
	virtual void Release();
	
// IPOGLResource
public:
	virtual POGLResourceType::Enum GetResourceType() const;

// IPOGLTexture
public:
	virtual POGLTextureFormat::Enum GetTextureFormat() const;

// IPOGLTexture2D
public:
	virtual const POGL_SIZE& GetSize() const;

private:
	REF_COUNTER mRefCount;
	POGLTextureResource* mResourcePtr;
	POGL_SIZE mSize;
};
