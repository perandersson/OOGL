#include "MemCheck.h"
#include "POGLTexture2D.h"
#include "POGLDeviceContext.h"
#include <atomic>
namespace {
	std::atomic<POGL_UINT32> uid;
	POGL_UINT32 GenTextureUID() {
		return ++uid;
	}
}

POGLTexture2D::POGLTexture2D(GLuint textureID, const POGL_SIZEI& size, POGLTextureFormat::Enum format, IPOGLDevice* device)
: mRefCount(1), mDevice(device), mInternalResource(nullptr), mSize(size)
{
	mInternalResource = new POGLTextureResource(textureID, GL_TEXTURE_2D, format, device);
}

POGLTexture2D::~POGLTexture2D()
{
}

void POGLTexture2D::AddRef()
{
	mRefCount++;
}

void POGLTexture2D::Release()
{
	if (--mRefCount == 0) {
		if (mInternalResource == nullptr) {
			mInternalResource->Release();
			mInternalResource = nullptr;
		}
		delete this;
	}
}

IPOGLDevice* POGLTexture2D::GetDevice()
{
	mDevice->AddRef();
	return mDevice;
}

POGL_HANDLE POGLTexture2D::GetHandlePtr()
{
	mInternalResource->AddRef();
	return mInternalResource;
}

POGLTextureFormat::Enum POGLTexture2D::GetTextureFormat() const
{
	return mInternalResource->GetTextureFormat();
}

const POGL_SIZEI& POGLTexture2D::GetSize() const
{
	return mSize;
}
