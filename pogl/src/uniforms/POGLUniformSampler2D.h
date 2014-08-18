#pragma once
#include "POGLDefaultUniform.h"
#include <mutex>

class POGLSamplerObject;
class POGLTextureResource;
class POGLUniformSampler2D : public POGLDefaultUniform
{
public:
	POGLUniformSampler2D(POGL_UINT32 programUID, POGLRenderState* state, GLint componentID, GLuint activeTexture, POGLSamplerObject* samplerObject);
	~POGLUniformSampler2D();

	void Apply();

	void SetTexture(IPOGLTexture* texture);

	void SetMinFilter(POGLMinFilter::Enum minFilter);
	void SetMagFilter(POGLMagFilter::Enum magFilter);
	void SetTextureWrap(POGLTextureWrap::Enum s, POGLTextureWrap::Enum t);
	void SetCompareFunc(POGLCompareFunc::Enum compareFunc);
	void SetCompareMode(POGLCompareMode::Enum compareMode);

	/*!
		\brief
	*/
	void SetTextureResource(POGLTextureResource* texture);

private:
	POGLTextureResource* mTextureResource;
	POGL_UINT32 mTextureUID;
	GLuint mActiveTexture;
	POGLSamplerObject* mSamplerObject;
	std::mutex mMutex;
};
