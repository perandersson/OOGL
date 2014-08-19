#include "MemCheck.h"
#include "POGLGlobalUniform.h"
#include "POGLDefaultUniform.h"

POGLGlobalUniform::POGLGlobalUniform(POGLDefaultUniform* uniform, GLenum type)
: mAssociatedUniform(uniform), mType(type)
{
}

POGLGlobalUniform::~POGLGlobalUniform()
{
}

void POGLGlobalUniform::Apply()
{
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
		break;
	case GL_SAMPLER_2D:
		break;
	};
}

void POGLGlobalUniform::SetInt32(POGL_INT32 a)
{
}

void POGLGlobalUniform::SetInt32(POGL_INT32 a, POGL_INT32 b)
{
}

void POGLGlobalUniform::SetInt32(POGL_INT32 a, POGL_INT32 b, POGL_INT32 c)
{
}

void POGLGlobalUniform::SetInt32(POGL_INT32 a, POGL_INT32 b, POGL_INT32 c, POGL_INT32 d)
{
}

void POGLGlobalUniform::SetInt32(POGL_INT32* ptr, POGL_UINT32 count)
{
}

void POGLGlobalUniform::SetUInt32(POGL_UINT32 a)
{
}

void POGLGlobalUniform::SetUInt32(POGL_UINT32 a, POGL_UINT32 b)
{
}

void POGLGlobalUniform::SetUInt32(POGL_UINT32 a, POGL_UINT32 b, POGL_UINT32 c)
{
}

void POGLGlobalUniform::SetUInt32(POGL_UINT32 a, POGL_UINT32 b, POGL_UINT32 c, POGL_UINT32 d)
{
}

void POGLGlobalUniform::SetUInt32(POGL_UINT32* ptr, POGL_UINT32 count)
{
}

void POGLGlobalUniform::SetFloat(POGL_FLOAT a)
{
}

void POGLGlobalUniform::SetFloat(POGL_FLOAT a, POGL_FLOAT b)
{
}

void POGLGlobalUniform::SetFloat(POGL_FLOAT a, POGL_FLOAT b, POGL_FLOAT c)
{
}

void POGLGlobalUniform::SetFloat(POGL_FLOAT a, POGL_FLOAT b, POGL_FLOAT c, POGL_FLOAT d)
{
}

void POGLGlobalUniform::SetFloat(POGL_FLOAT* ptr, POGL_UINT32 count)
{
}

void POGLGlobalUniform::SetDouble(POGL_DOUBLE a)
{
}

void POGLGlobalUniform::SetDouble(POGL_DOUBLE a, POGL_DOUBLE b)
{
}

void POGLGlobalUniform::SetDouble(POGL_DOUBLE a, POGL_DOUBLE b, POGL_DOUBLE c)
{
}

void POGLGlobalUniform::SetDouble(POGL_DOUBLE a, POGL_DOUBLE b, POGL_DOUBLE c, POGL_DOUBLE d)
{
}

void POGLGlobalUniform::SetDouble(POGL_DOUBLE* ptr, POGL_UINT32 count)
{
}

void POGLGlobalUniform::SetMatrix(const POGL_MAT4& mat4)
{
}

void POGLGlobalUniform::SetVector2(const POGL_VECTOR2& vec)
{
}

void POGLGlobalUniform::SetVector3(const POGL_VECTOR3& vec)
{
}

void POGLGlobalUniform::SetVector4(const POGL_VECTOR4& vec)
{
}

IPOGLSamplerState* POGLGlobalUniform::GetSamplerState()
{
	return this;
}

void POGLGlobalUniform::SetTexture(IPOGLTexture* texture)
{
}

void POGLGlobalUniform::SetMinFilter(POGLMinFilter::Enum minFilter)
{
}

void POGLGlobalUniform::SetMagFilter(POGLMagFilter::Enum magFilter)
{
}

void POGLGlobalUniform::SetTextureWrap(POGLTextureWrap::Enum s, POGLTextureWrap::Enum t)
{
}

void POGLGlobalUniform::SetTextureWrap(POGLTextureWrap::Enum s, POGLTextureWrap::Enum t, POGLTextureWrap::Enum r)
{
}

void POGLGlobalUniform::SetCompareFunc(POGLCompareFunc::Enum compareFunc)
{
}

void POGLGlobalUniform::SetCompareMode(POGLCompareMode::Enum compareMode)
{
}
