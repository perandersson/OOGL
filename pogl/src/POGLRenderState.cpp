#include "MemCheck.h"
#include "POGLRenderState.h"
#include "POGLDeviceContext.h"
#include "POGLProgramData.h"
#include "POGLVertexBuffer.h"
#include "POGLIndexBuffer.h"
#include "POGLEnum.h"
#include "POGLTextureResource.h"
#include "POGLSamplerObject.h"
#include "POGLFramebuffer.h"
#include "POGLProgram.h"

POGLRenderState::POGLRenderState(POGLDeviceContext* context)
: mRefCount(1), mDeviceContext(context), mProgram(nullptr), mProgramUID(0), mApplyCurrentProgramState(false),
mVertexBuffer(nullptr), mVertexBufferUID(0), mIndexBuffer(nullptr), mIndexBufferUID(0),
mDepthTest(false), mDepthFunc(POGLDepthFunc::DEFAULT), mDepthMask(true),
mColorMask(POGLColorMask::ALL), mStencilTest(false), mStencilMask(BIT_ALL), mSrcFactor(POGLSrcFactor::DEFAULT), mDstFactor(POGLDstFactor::DEFAULT), mBlending(false), 
mFrontFace(POGLFrontFace::DEFAULT), mCullFace(POGLCullFace::DEFAULT),
mViewport(0, 0, 0, 0),
mMaxActiveTextures(0), mNextActiveTexture(0), mActiveTextureIndex(0),
mFramebuffer(nullptr), mFramebufferUID(0)
{
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, (GLint*)&mMaxActiveTextures);
	mTextureUID.resize(mMaxActiveTextures, 0);
	mTextures.resize(mMaxActiveTextures, nullptr);
	mSamplerObjectUID.resize(mMaxActiveTextures, 0);
}

POGLRenderState::~POGLRenderState()
{
}

void POGLRenderState::AddRef()
{
	mRefCount++;
}

void POGLRenderState::Release()
{
	mApplyCurrentProgramState = true;
	if (--mRefCount == 0) {
		POGL_SAFE_RELEASE_UID(mProgram);
		POGL_SAFE_RELEASE_UID(mVertexBuffer);
		POGL_SAFE_RELEASE_UID(mIndexBuffer);
		POGL_SAFE_RELEASE_UID(mFramebuffer);

		auto size = mTextures.size();
		for (POGL_UINT32 i = 0; i < size; ++i) {
			POGL_SAFE_RELEASE(mTextures[i]);
			mTextureUID[i] = 0;
		}

		delete this;
	}
}


void POGLRenderState::Clear(POGL_UINT32 clearBits)
{
	if (clearBits == 0)
		return;

	GLenum clear = 0;
	if (BIT_ISSET(clearBits, POGLClearType::COLOR))
		clear = GL_COLOR_BUFFER_BIT;
	if (BIT_ISSET(clearBits, POGLClearType::DEPTH))
		clear |= GL_DEPTH_BUFFER_BIT;
	if (BIT_ISSET(clearBits, POGLClearType::STENCIL))
		clear |= GL_STENCIL_BUFFER_BIT;

	glClear(clear);
}

IPOGLUniform* POGLRenderState::FindUniformByName(const POGL_CHAR* name)
{
	return mProgram->FindStateUniformByName(name);
}

void POGLRenderState::SetFramebuffer(IPOGLFramebuffer* framebuffer)
{
	POGLFramebuffer* fb = static_cast<POGLFramebuffer*>(framebuffer);
	const POGL_UINT32 uid = framebuffer != nullptr ? fb->GetUID() : 0;
	if (mFramebufferUID == uid) {
		if (mFramebuffer == nullptr && fb != nullptr) {
			mFramebuffer = fb;
			mFramebuffer->AddRef();
		}
		return;
	}

	const GLuint framebufferID = framebuffer != nullptr ? fb->GetFramebufferID() : 0;
	glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);
	CHECK_GL("Could not bind framebuffer");
	if (mFramebuffer != nullptr) {
		mFramebuffer->Release();
	}
	mFramebuffer = fb;
	if (mFramebuffer != nullptr) {
		mFramebuffer->AddRef();
		POGL_UINT32 numDrawBuffers = mFramebuffer->GetNumDrawBuffers();
		if (numDrawBuffers > 0) {
			static const GLenum drawBuffers[8] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3,
				GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5, GL_COLOR_ATTACHMENT6, GL_COLOR_ATTACHMENT7 };
			glDrawBuffers(numDrawBuffers, drawBuffers);
			CHECK_GL("Could not set draw buffers");
		}
		else {
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
		}
	}
	mFramebufferUID = uid;
}

void POGLRenderState::Draw(IPOGLVertexBuffer* vertexBuffer)
{
	assert_not_null(vertexBuffer);
	Draw(vertexBuffer, nullptr, 0);
}

void POGLRenderState::Draw(IPOGLVertexBuffer* vertexBuffer, IPOGLIndexBuffer* indexBuffer)
{
	assert_not_null(vertexBuffer);
	Draw(vertexBuffer, indexBuffer, 0);
}

void POGLRenderState::Draw(IPOGLVertexBuffer* vertexBuffer, IPOGLIndexBuffer* indexBuffer, POGL_UINT32 startIndex)
{
	if (vertexBuffer == nullptr)
		THROW_EXCEPTION(POGLStateException, "You are not allowed to draw a non-existing vertex buffer");

	POGLVertexBuffer* vbo = static_cast<POGLVertexBuffer*>(vertexBuffer);
	POGLIndexBuffer* ibo = static_cast<POGLIndexBuffer*>(indexBuffer);
	BindBuffers(vbo, ibo);

	if (ibo == nullptr)
		vbo->Draw(startIndex);
	else
		vbo->Draw(ibo, startIndex);

	CHECK_GL("Cannot draw vertex- and index buffer");
}

void POGLRenderState::Draw(IPOGLVertexBuffer* vertexBuffer, IPOGLIndexBuffer* indexBuffer, POGL_UINT32 startIndex, POGL_UINT32 count)
{
	if (vertexBuffer == nullptr)
		THROW_EXCEPTION(POGLStateException, "You are not allowed to draw a non-existing vertex buffer");

	POGLVertexBuffer* vbo = static_cast<POGLVertexBuffer*>(vertexBuffer);
	POGLIndexBuffer* ibo = static_cast<POGLIndexBuffer*>(indexBuffer);
	BindBuffers(vbo, ibo);

	if (ibo == nullptr)
		vbo->Draw(startIndex, count);
	else
		vbo->Draw(ibo, startIndex, count);

	CHECK_GL("Cannot draw vertex- and index buffer");
}

void POGLRenderState::SetDepthTest(bool b)
{
	if (b == mDepthTest)
		return;

	if (b)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
	mDepthTest = b;

	CHECK_GL("Cannot enable/disable depth test");
}

void POGLRenderState::SetDepthFunc(POGLDepthFunc::Enum depthFunc)
{
	if (mDepthFunc == depthFunc)
		return;

	glDepthFunc(POGLEnum::Convert(depthFunc));
	mDepthFunc = depthFunc;

	CHECK_GL("Could not set the depth function used when render faces on the screen");
}

void POGLRenderState::SetDepthMask(bool b)
{
	if (mDepthMask == b)
		return;

	glDepthMask((GLboolean)b);
	mDepthMask = b;

	CHECK_GL("Could not set the depth mask used when render faces on the screen");
}

void POGLRenderState::SetColorMask(POGL_UINT8 mask)
{
	if (mColorMask == mask) {
		return;
	}

	const GLboolean r = BIT_ISSET(mask, POGLColorMask::RED);
	const GLboolean g = BIT_ISSET(mask, POGLColorMask::GREEN);
	const GLboolean b = BIT_ISSET(mask, POGLColorMask::BLUE);
	const GLboolean a = BIT_ISSET(mask, POGLColorMask::ALPHA);

	glColorMask(r, g, b, a);
	mColorMask = mask;

	CHECK_GL("Could not set the color mask used when render faces on the screen");
}

void POGLRenderState::SetStencilTest(bool b)
{
	if (mStencilTest == b)
		return;

	if (b)
		glEnable(GL_STENCIL_TEST);
	else
		glDisable(GL_STENCIL_TEST);
	mStencilTest = b;

	CHECK_GL("Cannot enable/disable stencil test");
}

void POGLRenderState::SetStencilMask(POGL_UINT32 mask)
{
	if (mStencilMask == mask)
		return;

	glStencilMask((GLuint)mask);
	mStencilMask = mask;

	CHECK_GL("Cannot set new stencil mask");
}

void POGLRenderState::SetBlendFunc(POGLSrcFactor::Enum sfactor, POGLDstFactor::Enum dfactor)
{
	if (mSrcFactor == sfactor && mDstFactor == dfactor) {
		return;
	}

	const GLenum src = POGLEnum::Convert(sfactor);
	const GLenum dest = POGLEnum::Convert(dfactor);
	glBlendFunc(src, dest);

	CHECK_GL("Cannot set blend functions");
}

void POGLRenderState::SetBlend(bool b)
{
	if (mBlending == b)
		return;

	if (b)
		glEnable(GL_BLEND);
	else
		glDisable(GL_BLEND);
	mBlending = b;

	CHECK_GL("Cannot enable/disable blendng");
}

void POGLRenderState::SetFrontFace(POGLFrontFace::Enum e)
{
	if (mFrontFace == e)
		return;

	glFrontFace(POGLEnum::Convert(e));
	mFrontFace = e;

	CHECK_GL("Cannot change the front faces using when render vertices onto the screen");
}

void POGLRenderState::SetCullFace(POGLCullFace::Enum e)
{
	if (mCullFace == e)
		return;

	if (e == POGLCullFace::DISABLED)
		glDisable(GL_CULL_FACE);
	else
	{
		glEnable(GL_CULL_FACE);
		glCullFace(POGLEnum::Convert(e));
	}
	mCullFace = e;

	CHECK_GL("Could not change the cull faces used when render faces on the screen");
}

void POGLRenderState::SetViewport(const POGL_RECT& viewport)
{
	if (mViewport.x == viewport.x && mViewport.y == viewport.y && mViewport.width == viewport.width && mViewport.height == viewport.height)
		return;

	glViewport(viewport.x, viewport.y, viewport.width, viewport.height);
	mViewport = viewport;
	CHECK_GL("Could not set viewport");
}

void POGLRenderState::Apply(IPOGLProgram* program)
{
	if (program == nullptr)
		THROW_EXCEPTION(POGLResourceException, "You are not allowed to apply a non-existing program");

	// Bind the program if neccessary
	BindProgram(static_cast<POGLProgram*>(program));

	// Apply the global uniform values
	mProgram->ApplyGlobalUniforms();

	// Retrieve the program data
	POGLProgramData data;
	mProgram->CopyProgramData(&data);

	//
	// Update the render state with the (potentially) new properties
	//

	SetDepthTest(data.depthTest);
	SetDepthFunc(data.depthFunc);
	SetDepthMask(data.depthMask);
	SetColorMask(data.colorMask);
	SetStencilTest(data.stencilTest);
	SetStencilMask(data.stencilMask);
	SetBlend(data.blending);
	SetBlendFunc(data.srcFactor, data.dstFactor);
	SetFrontFace(data.frontFace);
	SetCullFace(data.cullFace);
}

void POGLRenderState::BindSamplerObject(POGLSamplerObject* samplerObject, POGL_UINT32 idx)
{
	assert_not_null(samplerObject);
	const POGL_UINT32 uid = samplerObject != nullptr ? samplerObject->GetUID() : 0;
	if (mSamplerObjectUID[idx] == uid)
		return;

	const GLuint samplerID = samplerObject != nullptr ? samplerObject->GetSamplerID() : 0;
	glBindSampler(idx, samplerID);
	mSamplerObjectUID[idx] = uid;
	CHECK_GL("Cannot bind sampler ID");
}

void POGLRenderState::BindProgram(POGLProgram* program)
{
	const POGL_UINT32 uid = program->GetUID();
	if (uid == mProgramUID) {
		return;
	}

	if (mProgram != nullptr)
		mProgram->Release();

	mProgram = program;
	mProgram->AddRef();
	mProgramUID = uid;
	glUseProgram(mProgram->GetProgramID());
	mApplyCurrentProgramState = true;

	CHECK_GL("Could not bind the supplied program");
}

void POGLRenderState::BindBuffers(POGLVertexBuffer* vertexBuffer, POGLIndexBuffer* indexBuffer)
{
	if (mApplyCurrentProgramState) {
		mProgram->ApplyStateUniforms();
		mApplyCurrentProgramState = false;
	}

	BindVertexBuffer(vertexBuffer);
	BindIndexBuffer(indexBuffer);
}

void POGLRenderState::BindVertexBuffer(POGLVertexBuffer* buffer)
{
	const POGL_UINT32 uid = buffer != nullptr ? buffer->GetUID() : 0;
	if (mVertexBufferUID == uid) {
		return;
	}

	if (mVertexBuffer != nullptr)
		mVertexBuffer->Release();
	mVertexBuffer = buffer;
	if (mVertexBuffer != nullptr)
		mVertexBuffer->AddRef();

	const GLuint vaoID = mVertexBuffer != nullptr ? mVertexBuffer->GetVAOID() : 0;
	glBindVertexArray(vaoID);
	mVertexBufferUID = uid;

	CHECK_GL("Could not bind the supplied vertex array object");
}

void POGLRenderState::BindIndexBuffer(POGLIndexBuffer* buffer)
{
	const POGL_UINT32 uid = buffer != nullptr ? buffer->GetUID() : 0;
	if (mIndexBufferUID == uid) {
		return;
	}

	if (mIndexBuffer != nullptr)
		mIndexBuffer->Release();
	mIndexBuffer = buffer;
	if (mIndexBuffer != nullptr)
		mIndexBuffer->AddRef();

	const GLuint bufferID = buffer != nullptr ? buffer->GetBufferID() : 0;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);
	mIndexBufferUID = uid;

	CHECK_GL("Could not bind the supplied index buffer");
}

void POGLRenderState::BindTextureResource(POGLTextureResource* resource, POGL_UINT32 idx)
{
	if (idx >= mMaxActiveTextures) {
		THROW_EXCEPTION(POGLStateException,
			"This computer does not support %d consecutive textures. The maximum amount of texture bindable at the same time is %d", idx, mMaxActiveTextures);
	}

	// Check if the supplied texture is already bound to this context
	const POGL_UINT32 uid = resource != nullptr ? resource->GetUID() : 0;
	if (mTextureUID[idx] == uid)
		return;

	if (mActiveTextureIndex != idx) {
		glActiveTexture(GL_TEXTURE0 + idx);
		mActiveTextureIndex = idx;
		CHECK_GL("Could not set active texture index");
	}

	// Bind supplied texture
	const GLuint textureID = resource != nullptr ? resource->GetTextureID() : 0;
	const GLenum textureTarget = resource != nullptr ? resource->GetTextureTarget() : mTextures[idx]->GetTextureTarget();
	glBindTexture(textureTarget, textureID);

	// Release the previous bound texture if it exists
	if (mTextures[idx] != nullptr)
		mTextures[idx]->Release();

	// Save the bound texture
	mTextureUID[idx] = uid;
	mTextures[idx] = resource;
	if (mTextures[idx] != nullptr)
		mTextures[idx]->AddRef();

	CHECK_GL("Could not bind texture");
}

void POGLRenderState::SetTextureResource(POGLTextureResource* texture)
{
	// Release the previous bound texture if it exists
	if (mTextures[mActiveTextureIndex] != nullptr)
		mTextures[mActiveTextureIndex]->Release();

	// Set the new resource
	mTextureUID[mActiveTextureIndex] = texture->GetUID();
	mTextures[mActiveTextureIndex] = texture;
	texture->AddRef();
}

void POGLRenderState::SetVertexBuffer(POGLVertexBuffer* vertexBuffer)
{
	if (mVertexBuffer != nullptr)
		mVertexBuffer->Release();

	mVertexBuffer = vertexBuffer;
	mVertexBuffer->AddRef();
	mVertexBufferUID = vertexBuffer->GetUID();
}

void POGLRenderState::SetIndexBuffer(POGLIndexBuffer* indexBuffer)
{
	if (mIndexBuffer != nullptr)
		mIndexBuffer->Release();

	mIndexBuffer = indexBuffer;
	mIndexBuffer->AddRef();
	mIndexBufferUID = indexBuffer->GetUID();
}

void POGLRenderState::SetFramebuffer(POGLFramebuffer* framebuffer)
{
	if (mFramebuffer != nullptr)
		mFramebuffer->Release();

	mFramebuffer = framebuffer;
	mFramebuffer->AddRef();
	mFramebufferUID = framebuffer->GetUID();

	POGL_UINT32 numDrawBuffers = mFramebuffer->GetNumDrawBuffers();
	if (numDrawBuffers > 0) {
		static const GLenum drawBuffers[8] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3,
			GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5, GL_COLOR_ATTACHMENT6, GL_COLOR_ATTACHMENT7 };
		glDrawBuffers(numDrawBuffers, drawBuffers);
		CHECK_GL("Could not set draw buffers");
	}
}

POGL_UINT32 POGLRenderState::NextActiveTexture()
{
	const POGL_UINT32 textureIndex = mNextActiveTexture++;
	if (mNextActiveTexture >= mMaxActiveTextures)
		mNextActiveTexture = 0;
	return textureIndex;
}
