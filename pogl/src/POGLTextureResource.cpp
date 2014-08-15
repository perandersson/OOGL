#include "MemCheck.h"
#include "POGLTextureResource.h"

namespace {
	std::atomic<POGL_UINT32> uid;
	POGL_UINT32 GenTextureUID() {
		return ++uid;
	}
}

POGLTextureResource::POGLTextureResource(GLenum textureTarget, POGLTextureFormat::Enum format)
: mRefCount(1), mUID(0), mTextureID(0), mTextureTarget(textureTarget), mTextureFormat(format)
{

}

POGLTextureResource::~POGLTextureResource()
{
}

void POGLTextureResource::PostConstruct(GLuint textureID)
{
	mUID = GenTextureUID();
	mTextureID = textureID;
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
