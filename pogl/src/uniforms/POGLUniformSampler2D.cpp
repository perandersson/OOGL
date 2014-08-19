#include "MemCheck.h"
#include "POGLUniformSampler2D.h"
#include "POGLDeviceContext.h"
#include "POGLRenderState.h"
#include "POGLSamplerObject.h"
#include "POGLTexture2D.h"
#include "POGLEnum.h"

POGLUniformSampler2D::POGLUniformSampler2D(POGL_UINT32 programUID, POGLRenderState* state, GLint componentID, GLuint activeTexture, POGLSamplerObject* samplerObject, GLenum uniformType)
: POGLDefaultUniform(programUID, state, componentID, uniformType),
mTextureResource(nullptr), mTextureUID(0), mActiveTexture(activeTexture), 
mSamplerObject(samplerObject), mMinFilter(POGLMinFilter::DEFAULT), mMagFilter(POGLMagFilter::DEFAULT), mCompareFunc(POGLCompareFunc::DEFAULT), mCompareMode(POGLCompareMode::DEFAULT)
{
	assert_not_null(mSamplerObject);
	mWrap[0] = mWrap[1] = POGLTextureWrap::DEFAULT;
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
		auto type = texture->GetType();
		if (type == POGLResourceType::TEXTURE2D)
			SetTextureResource(reinterpret_cast<POGLTexture2D*>(texture)->GetResourcePtr());
	}
}

void POGLUniformSampler2D::SetMinFilter(POGLMinFilter::Enum minFilter)
{
	if (mMinFilter == minFilter)
		return;

	const GLuint samplerID = mSamplerObject->GetSamplerID();
	glSamplerParameteri(samplerID, GL_TEXTURE_MIN_FILTER, POGLEnum::Convert(minFilter));
	mMinFilter = minFilter;
}

void POGLUniformSampler2D::SetMagFilter(POGLMagFilter::Enum magFilter)
{
	if (mMagFilter == magFilter)
		return;

	const GLuint samplerID = mSamplerObject->GetSamplerID();
	glSamplerParameteri(samplerID, GL_TEXTURE_MAG_FILTER, POGLEnum::Convert(magFilter));
	mMagFilter = magFilter;
}

void POGLUniformSampler2D::SetTextureWrap(POGLTextureWrap::Enum s, POGLTextureWrap::Enum t)
{
	if (mWrap[0] == s && mWrap[1] == t)
		return;

	const GLuint samplerID = mSamplerObject->GetSamplerID();
	glSamplerParameteri(samplerID, GL_TEXTURE_WRAP_S, POGLEnum::Convert(s));
	glSamplerParameteri(samplerID, GL_TEXTURE_WRAP_T, POGLEnum::Convert(t));

	mWrap[0] = s;
	mWrap[1] = t;
}

void POGLUniformSampler2D::SetCompareFunc(POGLCompareFunc::Enum compareFunc)
{
	if (mCompareFunc == compareFunc)
		return;

	const GLuint samplerID = mSamplerObject->GetSamplerID();
	glSamplerParameteri(samplerID, GL_TEXTURE_COMPARE_FUNC, POGLEnum::Convert(compareFunc));

	mCompareFunc = compareFunc;
}

void POGLUniformSampler2D::SetCompareMode(POGLCompareMode::Enum compareMode)
{
	if (mCompareMode == compareMode)
		return;

	const GLuint samplerID = mSamplerObject->GetSamplerID();
	glSamplerParameteri(samplerID, GL_TEXTURE_COMPARE_MODE, POGLEnum::Convert(compareMode));

	mCompareMode = compareMode;
}

void POGLUniformSampler2D::SetTextureResource(POGLTextureResource* texture)
{
	const POGL_UINT32 uid = texture != nullptr ? texture->GetUID() : 0;
	mTextureUID = uid;
	if (mTextureResource != nullptr)
		mTextureResource->Release();
	mTextureResource = texture;
	if (mTextureResource != nullptr)
		mTextureResource->AddRef();

	if (IsProgramActive())
		POGLUniformSampler2D::Apply();
}
