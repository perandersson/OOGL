#pragma once
#include "config.h"
#include <gl/pogl.h>
#include "POGLTextureResource.h"

class POGLTexture2D : public IPOGLTexture2D
{
public:
	POGLTexture2D(GLuint textureID, const POGL_SIZEI& size, POGLTextureFormat::Enum format, IPOGLDevice* device);
	~POGLTexture2D();

	void AddRef();
	void Release();
	IPOGLDevice* GetDevice();
	POGL_HANDLE GetHandlePtr(); 
	POGLTextureFormat::Enum GetTextureFormat() const;
	const POGL_SIZEI& GetSize() const;

private:
	POGL_UINT32 mRefCount;
	IPOGLDevice* mDevice;
	POGLTextureResource* mInternalResource;
	POGL_SIZEI mSize;
};
