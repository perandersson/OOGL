#include "MemCheck.h"
#include "POGLTexture2D.h"
namespace {
	std::atomic<POGL_UINT32> uid;
	POGL_UINT32 GenTextureUID() {
		return ++uid;
	}
}

POGLTexture2D::POGLTexture2D(GLuint textureID, const POGL_SIZEI& size, POGLTextureFormat::Enum format)
: mRefCount(1), mResourcePtr(nullptr), mSize(size)
{
	mResourcePtr = new POGLTextureResource(textureID, GL_TEXTURE_2D, format);
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
		if (mResourcePtr != nullptr) {
			mResourcePtr->Release();
			mResourcePtr = nullptr;
		}
		delete this;
	}
}

POGLResourceType::Enum POGLTexture2D::GetResourceType() const
{
	return POGLResourceType::TEXTURE2D;
}

POGLTextureFormat::Enum POGLTexture2D::GetTextureFormat() const
{
	return mResourcePtr->GetTextureFormat();
}

const POGL_SIZEI& POGLTexture2D::GetSize() const
{
	return mSize;
}
