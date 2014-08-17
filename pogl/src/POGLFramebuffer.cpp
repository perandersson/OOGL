#include "MemCheck.h"
#include "POGLFramebuffer.h"
namespace {
	std::atomic<POGL_UINT32> uid;
	POGL_UINT32 GenFramebufferID() {
		return ++uid;
	}
}

POGLFramebuffer::POGLFramebuffer(std::vector<IPOGLTexture*>& textures, IPOGLTexture* depthStencilTexture)
: mRefCount(1), mUID(0), mFramebufferID(0), mTextures(textures), mDepthStencilTexture(depthStencilTexture)
{
	for (POGL_UINT32 i = 0; i < mTextures.size(); ++i) {
		mTextures[i]->AddRef();
	}
	if (mDepthStencilTexture != nullptr)
		mDepthStencilTexture->AddRef();
}

POGLFramebuffer::~POGLFramebuffer()
{
}

void POGLFramebuffer::AddRef()
{
	mRefCount++;
}

void POGLFramebuffer::Release()
{
	if (--mRefCount == 0) {
		if (mFramebufferID != 0) {
			glDeleteFramebuffers(1, &mFramebufferID);
			mFramebufferID = 0;
		}

		POGL_SAFE_RELEASE(mDepthStencilTexture);
		POGL_UINT32 size = mTextures.size();
		for (POGL_UINT32 i = 0; i < size; ++i) {
			POGL_SAFE_RELEASE(mTextures[i]);
		}
		delete this;
	}
}

IPOGLTexture* POGLFramebuffer::GetTexture(POGL_UINT32 idx)
{
	if (mTextures.size() <= idx)
		THROW_EXCEPTION(POGLResourceException, "There is no texture bound at index: %d", idx);

	IPOGLTexture* texture = mTextures[idx];
	texture->AddRef();
	return texture;
}

IPOGLTexture* POGLFramebuffer::GetDepthStencilTexture()
{
	if (mDepthStencilTexture != nullptr) {
		mDepthStencilTexture->AddRef();
		return mDepthStencilTexture;
	}
	return nullptr;
}

void POGLFramebuffer::PostConstruct(GLuint framebufferID)
{
	mFramebufferID = framebufferID;
	mUID = GenFramebufferID();
}

