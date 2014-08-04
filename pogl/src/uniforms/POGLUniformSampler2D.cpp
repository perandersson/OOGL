#include "MemCheck.h"
#include "POGLUniformSampler2D.h"
#include "POGLDeviceContext.h"
#include "POGLRenderState.h"
#include "POGLSamplerObject.h"
#include "POGLTextureHandle.h"
#include "POGLEnum.h"

POGLUniformSampler2D::POGLUniformSampler2D(const POGLEffect* effect, POGLRenderState* state, POGLDeviceContext* context, GLint componentID, GLuint activeTexture, POGLSamplerObject* samplerObject)
: POGLDefaultUniform(effect, state, context, componentID),
mTextureHandle(nullptr), mTextureUID(0), mActiveTexture(activeTexture), mSamplerObject(samplerObject)
{
	assert_not_null(mSamplerObject);
}

POGLUniformSampler2D::~POGLUniformSampler2D()
{
	if (mSamplerObject != nullptr) {
		delete mSamplerObject;
		mSamplerObject = nullptr;
	}
}

void POGLUniformSampler2D::Apply()
{
	if (mTextureHandle == nullptr) {
		return;
	}

	mRenderState->BindTextureHandle(mTextureHandle, mActiveTexture);
	mRenderState->BindSamplerObject(mSamplerObject, mActiveTexture);
	mDeviceContext->Uniform1i(mComponentID, mActiveTexture);

	CHECK_GL("Could not assign sampler2D uniform values");
}

void POGLUniformSampler2D::SetTexture(IPOGLTexture* texture)
{
	if (texture == nullptr)
		SetTextureHandle(nullptr);
	else
		SetTextureHandle(reinterpret_cast<POGLTextureHandle*>(texture->GetHandlePtr()));
}

void POGLUniformSampler2D::SetMinFilter(POGLMinFilter::Enum minFilter)
{
	const GLuint samplerID = mSamplerObject->GetSamplerID();
	mDeviceContext->SamplerParameteri(samplerID, GL_TEXTURE_MIN_FILTER, POGLEnum::Convert(minFilter));
}

void POGLUniformSampler2D::SetMagFilter(POGLMagFilter::Enum magFilter)
{
	const GLuint samplerID = mSamplerObject->GetSamplerID();
	mDeviceContext->SamplerParameteri(samplerID, GL_TEXTURE_MAG_FILTER, POGLEnum::Convert(magFilter));
}

void POGLUniformSampler2D::SetTextureWrap(POGLTextureWrap::Enum s, POGLTextureWrap::Enum t)
{
	const GLuint samplerID = mSamplerObject->GetSamplerID();
	mDeviceContext->SamplerParameteri(samplerID, GL_TEXTURE_WRAP_S, POGLEnum::Convert(s));
	mDeviceContext->SamplerParameteri(samplerID, GL_TEXTURE_WRAP_T, POGLEnum::Convert(t));
}

void POGLUniformSampler2D::SetCompareFunc(POGLCompareFunc::Enum compareFunc)
{
	const GLuint samplerID = mSamplerObject->GetSamplerID();
	mDeviceContext->SamplerParameteri(samplerID, GL_TEXTURE_COMPARE_FUNC, POGLEnum::Convert(compareFunc));
}

void POGLUniformSampler2D::SetCompareMode(POGLCompareMode::Enum compareMode)
{
	const GLuint samplerID = mSamplerObject->GetSamplerID();
	mDeviceContext->SamplerParameteri(samplerID, GL_TEXTURE_COMPARE_MODE, POGLEnum::Convert(compareMode));
}

void POGLUniformSampler2D::SetTextureHandle(POGLTextureHandle* textureHandle)
{
	const POGL_UINT32 uid = textureHandle != nullptr ? textureHandle->uid : 0;
	if (mTextureUID == uid)
		return;

	mTextureUID = uid;
	mTextureHandle = textureHandle;

	if (IsEffectActive())
		POGLUniformSampler2D::Apply();
}
