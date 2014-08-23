#include "MemCheck.h"
#include "POGLStaticUniform.h"
#include "POGLDefaultUniform.h"

POGLStaticUniform::POGLStaticUniform()
: mAssociatedUniform(nullptr), mType(0), mTexture(nullptr), mMinFilter(POGLMinFilter::DEFAULT), mMagFilter(POGLMagFilter::DEFAULT),
mCompareFunc(POGLCompareFunc::DEFAULT), mCompareMode(POGLCompareMode::DEFAULT)
{
	mFloats[0] = mFloats[1] = mFloats[2] = mFloats[3] = 0.0f;
	mDoubles[0] = mDoubles[1] = mDoubles[2] = mDoubles[3] = 0.0;
	mInts[0] = mInts[1] = mInts[2] = mInts[3] = 0;
	mWraps[0] = mWraps[1] = mWraps[2] = POGLTextureWrap::DEFAULT;

	memset(&mValue, 0, sizeof(mValue));
	mValue._11 = 1.0;
	mValue._22 = 1.0;
	mValue._33 = 1.0;
	mValue._44 = 1.0;
}

POGLStaticUniform::~POGLStaticUniform()
{
	if (mTexture != nullptr) {
		mTexture->Release();
		mTexture = nullptr;
	}
}

void POGLStaticUniform::SetAssociatedUniform(POGLDefaultUniform* uniform)
{
	mAssociatedUniform = uniform;
	mType = uniform->GetUniformType();
}

void POGLStaticUniform::Apply()
{
	std::lock_guard<std::mutex> lock(mMutex);

	switch (mType)
	{
	case GL_FLOAT:
		mAssociatedUniform->SetFloat(mFloats[0]);
		break;
	case GL_FLOAT_VEC2:
		mAssociatedUniform->SetFloat(mFloats[0], mFloats[1]);
		break;
	case GL_FLOAT_VEC3:
		mAssociatedUniform->SetFloat(mFloats[0], mFloats[1], mFloats[2]);
		break;
	case GL_FLOAT_VEC4:
		mAssociatedUniform->SetFloat(mFloats[0], mFloats[1], mFloats[2], mFloats[3]);
		break;
	case GL_DOUBLE:
		mAssociatedUniform->SetDouble(mDoubles[0]);
		break;
	case GL_DOUBLE_VEC2:
		mAssociatedUniform->SetDouble(mDoubles[0], mDoubles[1]);
		break;
	case GL_DOUBLE_VEC3:
		mAssociatedUniform->SetDouble(mDoubles[0], mDoubles[1], mDoubles[2]);
		break;
	case GL_DOUBLE_VEC4:
		mAssociatedUniform->SetDouble(mDoubles[0], mDoubles[1], mDoubles[2], mDoubles[3]);
		break;
	case GL_INT:
		mAssociatedUniform->SetInt32((POGL_INT32)mInts[0]);
		break;
	case GL_INT_VEC2:
		mAssociatedUniform->SetInt32((POGL_INT32)mInts[0], (POGL_INT32)mInts[1]);
		break;
	case GL_INT_VEC3:
		mAssociatedUniform->SetInt32((POGL_INT32)mInts[0], (POGL_INT32)mInts[1], (POGL_INT32)mInts[2]);
		break;
	case GL_INT_VEC4:
		mAssociatedUniform->SetInt32((POGL_INT32)mInts[0], (POGL_INT32)mInts[1], (POGL_INT32)mInts[2], (POGL_INT32)mInts[3]);
		break;
	case GL_UNSIGNED_INT:
		mAssociatedUniform->SetUInt32(mInts[0]);
		break;
	case GL_UNSIGNED_INT_VEC2:
		mAssociatedUniform->SetUInt32(mInts[0], mInts[1]);
		break;
	case GL_UNSIGNED_INT_VEC3:
		mAssociatedUniform->SetUInt32(mInts[0], mInts[1], mInts[2]);
		break;
	case GL_UNSIGNED_INT_VEC4:
		mAssociatedUniform->SetUInt32(mInts[0], mInts[1], mInts[2], mInts[3]);
		break;
	case GL_FLOAT_MAT4:
		mAssociatedUniform->SetMatrix(mValue);
		break;
	case GL_SAMPLER_2D:
		if (mTexture != nullptr) {
			mAssociatedUniform->SetTexture(mTexture);
			mAssociatedUniform->SetMinFilter(mMinFilter);
			mAssociatedUniform->SetMagFilter(mMagFilter);
			mAssociatedUniform->SetTextureWrap(mWraps[0], mWraps[1]);
			mAssociatedUniform->SetCompareFunc(mCompareFunc);
			mAssociatedUniform->SetCompareMode(mCompareMode);
		}
		break;
	};
}

void POGLStaticUniform::SetInt32(POGL_INT32 a)
{
	std::lock_guard<std::mutex> lock(mMutex);

	mInts[0] = a;
}

void POGLStaticUniform::SetInt32(POGL_INT32 a, POGL_INT32 b)
{
	std::lock_guard<std::mutex> lock(mMutex);

	mInts[0] = a;
	mInts[1] = b;
}

void POGLStaticUniform::SetInt32(POGL_INT32 a, POGL_INT32 b, POGL_INT32 c)
{
	std::lock_guard<std::mutex> lock(mMutex);

	mInts[0] = a;
	mInts[1] = b;
	mInts[2] = c;
}

void POGLStaticUniform::SetInt32(POGL_INT32 a, POGL_INT32 b, POGL_INT32 c, POGL_INT32 d)
{
	std::lock_guard<std::mutex> lock(mMutex);

	mInts[0] = a;
	mInts[1] = b;
	mInts[2] = c;
	mInts[3] = d;

}

void POGLStaticUniform::SetInt32(POGL_INT32* ptr, POGL_UINT32 count)
{
	std::lock_guard<std::mutex> lock(mMutex);

	const POGL_UINT32 clampedCount = count > 4 ? 4 : count;
	for (POGL_UINT32 i = 0; i < clampedCount; ++i)
		mInts[i] = (POGL_UINT32)ptr[i];
}

void POGLStaticUniform::SetUInt32(POGL_UINT32 a)
{
	std::lock_guard<std::mutex> lock(mMutex);

	mInts[0] = a;
}

void POGLStaticUniform::SetUInt32(POGL_UINT32 a, POGL_UINT32 b)
{
	std::lock_guard<std::mutex> lock(mMutex);

	mInts[0] = a;
	mInts[1] = b;
}

void POGLStaticUniform::SetUInt32(POGL_UINT32 a, POGL_UINT32 b, POGL_UINT32 c)
{
	std::lock_guard<std::mutex> lock(mMutex);

	mInts[0] = a;
	mInts[1] = b;
	mInts[2] = c;
}

void POGLStaticUniform::SetUInt32(POGL_UINT32 a, POGL_UINT32 b, POGL_UINT32 c, POGL_UINT32 d)
{
	std::lock_guard<std::mutex> lock(mMutex);

	mInts[0] = a;
	mInts[1] = b;
	mInts[2] = c;
	mInts[3] = d;
}

void POGLStaticUniform::SetUInt32(POGL_UINT32* ptr, POGL_UINT32 count)
{
	std::lock_guard<std::mutex> lock(mMutex);

	const POGL_UINT32 clampedCount = count > 4 ? 4 : count;
	for (POGL_UINT32 i = 0; i < clampedCount; ++i)
		mInts[i] = ptr[i];
}

void POGLStaticUniform::SetFloat(POGL_FLOAT a)
{
	std::lock_guard<std::mutex> lock(mMutex);

	mFloats[0] = a;
}

void POGLStaticUniform::SetFloat(POGL_FLOAT a, POGL_FLOAT b)
{
	std::lock_guard<std::mutex> lock(mMutex);

	mFloats[0] = a;
	mFloats[1] = b;
}

void POGLStaticUniform::SetFloat(POGL_FLOAT a, POGL_FLOAT b, POGL_FLOAT c)
{
	std::lock_guard<std::mutex> lock(mMutex);

	mFloats[0] = a;
	mFloats[1] = b;
	mFloats[2] = c;
}

void POGLStaticUniform::SetFloat(POGL_FLOAT a, POGL_FLOAT b, POGL_FLOAT c, POGL_FLOAT d)
{
	std::lock_guard<std::mutex> lock(mMutex);

	mFloats[0] = a;
	mFloats[1] = b;
	mFloats[2] = c;
	mFloats[3] = d;
}

void POGLStaticUniform::SetFloat(POGL_FLOAT* ptr, POGL_UINT32 count)
{
	std::lock_guard<std::mutex> lock(mMutex);

	const POGL_UINT32 clampedCount = count > 4 ? 4 : count;
	for (POGL_UINT32 i = 0; i < clampedCount; ++i)
		mFloats[i] = ptr[i];
}

void POGLStaticUniform::SetDouble(POGL_DOUBLE a)
{
	std::lock_guard<std::mutex> lock(mMutex);

	mDoubles[0] = a;
}

void POGLStaticUniform::SetDouble(POGL_DOUBLE a, POGL_DOUBLE b)
{
	std::lock_guard<std::mutex> lock(mMutex);

	mDoubles[0] = a;
	mDoubles[1] = b;
}

void POGLStaticUniform::SetDouble(POGL_DOUBLE a, POGL_DOUBLE b, POGL_DOUBLE c)
{
	std::lock_guard<std::mutex> lock(mMutex);

	mDoubles[0] = a;
	mDoubles[1] = b;
	mDoubles[2] = c;
}

void POGLStaticUniform::SetDouble(POGL_DOUBLE a, POGL_DOUBLE b, POGL_DOUBLE c, POGL_DOUBLE d)
{
	std::lock_guard<std::mutex> lock(mMutex);

	mDoubles[0] = a;
	mDoubles[1] = b;
	mDoubles[2] = c;
	mDoubles[3] = d;
}

void POGLStaticUniform::SetDouble(POGL_DOUBLE* ptr, POGL_UINT32 count)
{
	std::lock_guard<std::mutex> lock(mMutex);

	const POGL_UINT32 clampedCount = count > 4 ? 4 : count;
	for (POGL_UINT32 i = 0; i < clampedCount; ++i)
		mDoubles[i] = ptr[i];
}

void POGLStaticUniform::SetMatrix(const POGL_MAT4& mat4)
{
	std::lock_guard<std::mutex> lock(mMutex);

	mValue._11 = mat4._11;
	mValue._12 = mat4._12;
	mValue._13 = mat4._13;
	mValue._14 = mat4._14;

	mValue._21 = mat4._21;
	mValue._22 = mat4._22;
	mValue._23 = mat4._23;
	mValue._24 = mat4._24;

	mValue._31 = mat4._31;
	mValue._32 = mat4._32;
	mValue._33 = mat4._33;
	mValue._34 = mat4._34;

	mValue._41 = mat4._41;
	mValue._42 = mat4._42;
	mValue._43 = mat4._43;
	mValue._44 = mat4._44;
}

void POGLStaticUniform::SetVector2(const POGL_VECTOR2& vec)
{
	std::lock_guard<std::mutex> lock(mMutex);

	mFloats[0] = vec.x;
	mFloats[1] = vec.y;
}

void POGLStaticUniform::SetVector3(const POGL_VECTOR3& vec)
{
	std::lock_guard<std::mutex> lock(mMutex);

	mFloats[0] = vec.x;
	mFloats[1] = vec.y;
	mFloats[2] = vec.z;
}

void POGLStaticUniform::SetVector4(const POGL_VECTOR4& vec)
{
	std::lock_guard<std::mutex> lock(mMutex);

	mFloats[0] = vec.x;
	mFloats[1] = vec.y;
	mFloats[2] = vec.z;
	mFloats[3] = vec.w;
}

void POGLStaticUniform::SetSize(const POGL_SIZE& size)
{
	std::lock_guard<std::mutex> lock(mMutex);

	mInts[0] = (POGL_UINT32)size.x;
	mInts[1] = (POGL_UINT32)size.y;
}

void POGLStaticUniform::SetRect(const POGL_RECT& rect)
{
	std::lock_guard<std::mutex> lock(mMutex);

	mInts[0] = (POGL_UINT32)rect.x;
	mInts[1] = (POGL_UINT32)rect.y;
	mInts[2] = (POGL_UINT32)rect.width;
	mInts[3] = (POGL_UINT32)rect.height;
}

IPOGLSamplerState* POGLStaticUniform::GetSamplerState()
{
	return this;
}

void POGLStaticUniform::SetTexture(IPOGLTexture* texture)
{
	std::lock_guard<std::mutex> lock(mMutex);

	if (mTexture != nullptr)
		mTexture->Release();
	mTexture = texture;
	if (mTexture != nullptr)
		mTexture->AddRef();
}

void POGLStaticUniform::SetMinFilter(POGLMinFilter::Enum minFilter)
{
	std::lock_guard<std::mutex> lock(mMutex);

	mMinFilter = minFilter;
}

void POGLStaticUniform::SetMagFilter(POGLMagFilter::Enum magFilter)
{
	std::lock_guard<std::mutex> lock(mMutex);

	mMagFilter = magFilter;
}

void POGLStaticUniform::SetTextureWrap(POGLTextureWrap::Enum s, POGLTextureWrap::Enum t)
{
	std::lock_guard<std::mutex> lock(mMutex);
	
	mWraps[0] = s;
	mWraps[1] = t;
}

void POGLStaticUniform::SetTextureWrap(POGLTextureWrap::Enum s, POGLTextureWrap::Enum t, POGLTextureWrap::Enum r)
{
	std::lock_guard<std::mutex> lock(mMutex);

	mWraps[0] = s;
	mWraps[1] = t;
	mWraps[2] = r;
}

void POGLStaticUniform::SetCompareFunc(POGLCompareFunc::Enum compareFunc)
{
	std::lock_guard<std::mutex> lock(mMutex);

	mCompareFunc = compareFunc;
}

void POGLStaticUniform::SetCompareMode(POGLCompareMode::Enum compareMode)
{
	std::lock_guard<std::mutex> lock(mMutex);

	mCompareMode = compareMode;
}
