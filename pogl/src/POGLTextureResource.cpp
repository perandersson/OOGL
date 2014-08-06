#include "MemCheck.h"
#include "POGLTextureResource.h"
#include <atomic>
namespace {
	std::atomic<POGL_UINT32> uid;
	POGL_UINT32 GenTextureUID() {
		return ++uid;
	}
}

POGLTextureResource::POGLTextureResource(GLuint textureID, GLenum textureTarget, POGLTextureFormat::Enum format, IPOGLDevice* device)
: mRefCount(1), mUID(GenTextureUID()), mTextureID(textureID), mTextureTarget(textureTarget), mTextureFormat(format), mDevice(device)
{

}

POGLTextureResource::~POGLTextureResource()
{
}

void POGLTextureResource::AddRef()
{
	mRefCount++;
}

void POGLTextureResource::Release()
{
	if (--mRefCount == 0) {
		if (mTextureID != 0) {
			glDeleteTextures(1, &mTextureID);
			mTextureID = 0;
		}
		delete this;
	}
}

POGL_UINT32 POGLTextureResource::GetUID() const
{
	return mUID;
}

GLuint POGLTextureResource::GetTextureID() const
{
	return mTextureID;
}

GLenum POGLTextureResource::GetTextureTarget() const
{
	return mTextureTarget;
}

POGLTextureFormat::Enum POGLTextureResource::GetTextureFormat() const
{
	return mTextureFormat;
}