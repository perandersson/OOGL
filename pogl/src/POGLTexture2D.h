#pragma once
#include "config.h"
#include <gl/pogl.h>
#include "POGLTextureResource.h"

class POGLTexture2D : public IPOGLTexture2D
{
public:
	POGLTexture2D(GLuint textureID, const POGL_SIZEI& size, POGLTextureFormat::Enum format, IPOGLDevice* device);
	~POGLTexture2D();

// IPOGLInterface
public:
	void AddRef();
	void Release();
	
// IPOGLResource
public:
	IPOGLDevice* GetDevice();
	POGL_HANDLE GetHandlePtr();

// IPOGLTexture
public:
	POGLTextureFormat::Enum GetTextureFormat() const;

// IPOGLTexture2D
public:
	const POGL_SIZEI& GetSize() const;

private:
	POGL_UINT32 mRefCount;
	IPOGLDevice* mDevice;
	POGLTextureResource* mInternalResource;
	POGL_SIZEI mSize;
};
