#pragma once
#include "config.h"
#include <gl/pogl.h>
#include "POGLTextureResource.h"

class POGLTexture2D : public IPOGLTexture2D
{
public:
	POGLTexture2D(GLuint textureID, const POGL_SIZEI& size, POGLTextureFormat::Enum format);
	~POGLTexture2D();

	/*!
		\brief 
	*/
	inline POGLTextureResource* GetResourcePtr() const {
		return mResourcePtr;
	}

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
	const POGL_SIZEI& GetSize() const;

private:
	POGL_UINT32 mRefCount;
	POGLTextureResource* mResourcePtr;
	POGL_SIZEI mSize;
};
