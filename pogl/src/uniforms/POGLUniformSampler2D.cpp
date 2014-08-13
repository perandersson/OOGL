#include "MemCheck.h"
#include "POGLUniformSampler2D.h"
#include "POGLDeviceContext.h"
#include "POGLRenderState.h"
#include "POGLSamplerObject.h"
#include "POGLTexture2D.h"
#include "POGLEnum.h"

POGLUniformSampler2D::POGLUniformSampler2D(const POGLEffect* effect, POGLRenderState* state, POGLDeviceContext* context, GLint componentID, GLuint activeTexture, POGLSamplerObject* samplerObject)
: POGLDefaultUniform(effect, state, context, componentID),
mTextureResource(nullptr), mTextureUID(0), mActiveTexture(activeTexture), mSamplerObject(samplerObject)
{
	assert_not_null(mSamplerObject);
}

POGLUniformSampler2D::~POGLUniformSampler2D()
{
	if (mSamplerObject != nullptr) {
		delete mSamplerObject;
		mSamplerObject = nullptr;
	}

	if (mTextureResource != nullptr) {
		mTextureResource->Release();
		mTextureResource = nullptr;
	}
}

void POGLUniformSampler2D::Apply()
{
	if (mTextureResource == nullptr) {
		return;
	}

	mRenderState->BindTextureResource(mTextureResource, mActiveTexture);
	mRenderState->BindSamplerObject(mSamplerObject, mActiveTexture);
	glUniform1i(mComponentID, mActiveTexture);

	CHECK_GL("Could not assign sampler2D uniform values");
}

void POGLUniformSampler2D::SetTexture(IPOGLTexture* texture)
{
	if (texture == nullptr)
		SetTextureResource(nullptr);
	else {
		auto type = texture->GetResourceType();
		if (type == POGLResourceType::TEXTURE2D)
			SetTextureResource(reinterpret_cast<POGLTexture2D*>(texture)->GetResourcePtr());
	}
}

void POGLUniformSampler2D::SetMinFilter(POGLMinFilter::Enum minFilter)
{
	const GLuint samplerID = mSamplerObject->GetSamplerID();
	glSamplerParameteri(samplerID, GL_TEXTURE_MIN_FILTER, POGLEnum::Convert(minFilter));
}

void POGLUniformSampler2D::SetMagFilter(POGLMagFilter::Enum magFilter)
{
	const GLuint samplerID = mSamplerObject->GetSamplerID();
	glSamplerParameteri(samplerID, GL_TEXTURE_MAG_FILTER, POGLEnum::Convert(magFilter));
}

void POGLUniformSampler2D::SetTextureWrap(POGLTextureWrap::Enum s, POGLTextureWrap::Enum t)
{
	const GLuint samplerID = mSamplerObject->GetSamplerID();
	glSamplerParameteri(samplerID, GL_TEXTURE_WRAP_S, POGLEnum::Convert(s));
	glSamplerParameteri(samplerID, GL_TEXTURE_WRAP_T, POGLEnum::Convert(t));
}

void POGLUniformSampler2D::SetCompareFunc(POGLCompareFunc::Enum compareFunc)
{
	const GLuint samplerID = mSamplerObject->GetSamplerID();
	glSamplerParameteri(samplerID, GL_TEXTURE_COMPARE_FUNC, POGLEnum::Convert(compareFunc));
}

void POGLUniformSampler2D::SetCompareMode(POGLCompareMode::Enum compareMode)
{
	const GLuint samplerID = mSamplerObject->GetSamplerID();
	glSamplerParameteri(samplerID, GL_TEXTURE_COMPARE_MODE, POGLEnum::Convert(compareMode));
}

void POGLUniformSampler2D::SetTextureResource(POGLTextureResource* texture)
{
	const POGL_UINT32 uid = texture != nullptr ? texture->GetUID() : 0;
	if (mTextureUID == uid)
		return;

	mTextureUID = uid;
	if (mTextureResource != nullptr)
		mTextureResource->Release();
	mTextureResource = texture;
	if (mTextureResource != nullptr)
		mTextureResource->AddRef();

	if (IsEffectActive())
		POGLUniformSampler2D::Apply();
}
