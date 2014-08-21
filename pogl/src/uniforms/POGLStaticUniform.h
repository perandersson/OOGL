#pragma once
#include "config.h"

class POGLDefaultUniform;
class POGLStaticUniform : public IPOGLUniform, public IPOGLSamplerState
{
public:
	POGLStaticUniform();
	virtual ~POGLStaticUniform();

	/*!
		\brief Associate this uniform with the supplied uniform

		\param uniform
	*/
	void SetAssociatedUniform(POGLDefaultUniform* uniform);

	/*!
		\brief Apply this uniform.

		This will set the associated uniform value with the ones specified in this uniform
	*/
	void Apply();

public:
	void SetInt32(POGL_INT32 a);
	void SetInt32(POGL_INT32 a, POGL_INT32 b);
	void SetInt32(POGL_INT32 a, POGL_INT32 b, POGL_INT32 c);
	void SetInt32(POGL_INT32 a, POGL_INT32 b, POGL_INT32 c, POGL_INT32 d);
	void SetInt32(POGL_INT32* ptr, POGL_UINT32 count);

	void SetUInt32(POGL_UINT32 a);
	void SetUInt32(POGL_UINT32 a, POGL_UINT32 b);
	void SetUInt32(POGL_UINT32 a, POGL_UINT32 b, POGL_UINT32 c);
	void SetUInt32(POGL_UINT32 a, POGL_UINT32 b, POGL_UINT32 c, POGL_UINT32 d);
	void SetUInt32(POGL_UINT32* ptr, POGL_UINT32 count);

	void SetFloat(POGL_FLOAT a);
	void SetFloat(POGL_FLOAT a, POGL_FLOAT b);
	void SetFloat(POGL_FLOAT a, POGL_FLOAT b, POGL_FLOAT c);
	void SetFloat(POGL_FLOAT a, POGL_FLOAT b, POGL_FLOAT c, POGL_FLOAT d);
	void SetFloat(POGL_FLOAT* ptr, POGL_UINT32 count);

	void SetDouble(POGL_DOUBLE a);
	void SetDouble(POGL_DOUBLE a, POGL_DOUBLE b);
	void SetDouble(POGL_DOUBLE a, POGL_DOUBLE b, POGL_DOUBLE c);
	void SetDouble(POGL_DOUBLE a, POGL_DOUBLE b, POGL_DOUBLE c, POGL_DOUBLE d);
	void SetDouble(POGL_DOUBLE* ptr, POGL_UINT32 count);

	void SetMatrix(const POGL_MAT4& mat4);

	void SetVector2(const POGL_VECTOR2& vec);
	void SetVector3(const POGL_VECTOR3& vec);
	void SetVector4(const POGL_VECTOR4& vec);

	IPOGLSamplerState* GetSamplerState();

	void SetTexture(IPOGLTexture* texture);

	void SetMinFilter(POGLMinFilter::Enum minFilter);
	void SetMagFilter(POGLMagFilter::Enum magFilter);
	void SetTextureWrap(POGLTextureWrap::Enum s, POGLTextureWrap::Enum t);
	void SetTextureWrap(POGLTextureWrap::Enum s, POGLTextureWrap::Enum t, POGLTextureWrap::Enum r);
	void SetCompareFunc(POGLCompareFunc::Enum compareFunc);
	void SetCompareMode(POGLCompareMode::Enum compareMode);

private:
	POGLDefaultUniform* mAssociatedUniform;
	GLenum mType;

	POGL_FLOAT mFloats[4];
	POGL_DOUBLE mDoubles[4];
	POGL_UINT32 mInts[4];
	POGL_MAT4 mValue;
	IPOGLTexture* mTexture;

	POGLMinFilter::Enum mMinFilter;
	POGLMagFilter::Enum mMagFilter;
	POGLTextureWrap::Enum mWraps[3];
	POGLCompareFunc::Enum mCompareFunc;
	POGLCompareMode::Enum mCompareMode;

	std::mutex mMutex;
};
