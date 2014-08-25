#include "MemCheck.h"
#include "POGLFramebuffer.h"
#include "POGLTexture2D.h"
#include "POGLTextureResource.h"

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

void POGLFramebuffer::PostConstruct()
{
	glGenFramebuffers(1, &mFramebufferID);

	const GLenum error = glGetError();
	if (mFramebufferID == 0 || error != GL_NO_ERROR)
		THROW_EXCEPTION(POGLResourceException, "Could not generate framebuffer ID. Reason: 0x%x", error);

	glBindFramebuffer(GL_FRAMEBUFFER, mFramebufferID);
	CHECK_GL("Could not bind framebuffer");

	const POGL_UINT32 numTextures = mTextures.size();
	for (POGL_UINT32 i = 0; i < numTextures; ++i) {
		IPOGLTexture* texture = mTextures[i];
		const POGLResourceType::Enum type = texture->GetType();
		if (type == POGLResourceType::TEXTURE2D) {
			POGLTexture2D* t2d = static_cast<POGLTexture2D*>(texture);
			POGLTextureResource* resource = t2d->GetResourcePtr();
			glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, resource->GetTextureID(), 0);
			CHECK_GL("Could not attach framebuffer texture to frame buffer");
		}
		else {
			THROW_NOT_IMPLEMENTED_EXCEPTION();
		}
	}

	if (mDepthStencilTexture != nullptr) {
		const POGLResourceType::Enum type = mDepthStencilTexture->GetType();
		if (type == POGLResourceType::TEXTURE2D) {
			POGLTexture2D* t2d = static_cast<POGLTexture2D*>(mDepthStencilTexture);
			POGLTextureResource* resource = t2d->GetResourcePtr();
			GLenum attachmentType = GL_DEPTH_ATTACHMENT;
			switch (resource->GetTextureFormat()) {
			case POGLTextureFormat::DEPTH24:
			case POGLTextureFormat::DEPTH32F:
				attachmentType = GL_DEPTH_ATTACHMENT;
				break;
			case POGLTextureFormat::DEPTH24_STENCIL8:
			case POGLTextureFormat::DEPTH32F_STENCIL8:
				attachmentType = GL_DEPTH_STENCIL_ATTACHMENT;
				break;
			default:
				THROW_EXCEPTION(POGLResourceException, "You cannot bind a non-depth-stencil texture as a depth buffer");
			}
			glFramebufferTexture(GL_FRAMEBUFFER, attachmentType, resource->GetTextureID(), 0);

		}
		else {
			THROW_NOT_IMPLEMENTED_EXCEPTION();
		}
	}

	//
	// Build framebuffer
	//

	const GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	switch (status)
	{
	case GL_FRAMEBUFFER_COMPLETE_EXT:
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
		THROW_EXCEPTION(POGLResourceException, "Framebuffer incomplete: Attachment is NOT complete.");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
		THROW_EXCEPTION(POGLResourceException, "Framebuffer incomplete: No image is attached to FBO.");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
		THROW_EXCEPTION(POGLResourceException, "Framebuffer incomplete: Attached images have different dimensions");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
		THROW_EXCEPTION(POGLResourceException, "Framebuffer incomplete: Color attached images have different internal formats");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
		THROW_EXCEPTION(POGLResourceException, "Framebuffer incomplete: Draw buffer");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
		THROW_EXCEPTION(POGLResourceException, "Framebuffer incomplete: Read buffer");
		break;
	case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
		THROW_EXCEPTION(POGLResourceException, "Unsupported by FBO implementation");
		break;
	default:
		THROW_EXCEPTION(POGLResourceException, "Unknow error");
		break;
	}

	mUID = GenFramebufferID();
}

