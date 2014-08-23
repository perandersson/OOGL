#include "MemCheck.h"
#include "POGLDeferredUniform.h"
#include "POGLDeferredRenderContext.h"
#include "POGLDeferredCommands.h"

POGLDeferredUniform::POGLDeferredUniform(const POGL_STRING& name, POGLDeferredRenderContext* context)
: mName(name), mRenderContext(context), mAssigned(false), mTexture(nullptr)
{
	mInts[0] = mInts[1] = mInts[2] = mInts[3] = UINT_MAX;
	mFloats[0] = mFloats[1] = mFloats[2] = mFloats[3] = FLT_MAX;
	mDoubles[0] = mDoubles[1] = mDoubles[2] = mDoubles[3] = DBL_MAX;
	mCount = 0;
}

POGLDeferredUniform::~POGLDeferredUniform()
{
}

void POGLDeferredUniform::Flush()
{
	mAssigned = false;
	if (mTexture != nullptr) {
		mTexture->Release();
		mTexture = nullptr;
	}
}

void POGLDeferredUniform::SetInt32(POGL_INT32 a)
{
	if (IsIntEquals(a))
		return;

	POGL_UNIFORM_SET_INT_COMMAND_DATA* cmd = (POGL_UNIFORM_SET_INT_COMMAND_DATA*)mRenderContext->AddCommand(&POGLUniformSetInt_Command, &POGLNothing_Release,
		sizeof(POGL_UNIFORM_SET_INT_COMMAND_DATA));
	cmd->name = &mName;
	cmd->values[0] = a;
	cmd->count = 1;
}

void POGLDeferredUniform::SetInt32(POGL_INT32 a, POGL_INT32 b)
{
	if (IsIntEquals(a, b))
		return;

	POGL_UNIFORM_SET_INT_COMMAND_DATA* cmd = (POGL_UNIFORM_SET_INT_COMMAND_DATA*)mRenderContext->AddCommand(&POGLUniformSetInt_Command, &POGLNothing_Release,
		sizeof(POGL_UNIFORM_SET_INT_COMMAND_DATA));
	cmd->name = &mName;
	cmd->values[0] = a;
	cmd->values[1] = b;
	cmd->count = 2;
}

void POGLDeferredUniform::SetInt32(POGL_INT32 a, POGL_INT32 b, POGL_INT32 c)
{
	if (IsIntEquals(a, b, c))
		return;

	POGL_UNIFORM_SET_INT_COMMAND_DATA* cmd = (POGL_UNIFORM_SET_INT_COMMAND_DATA*)mRenderContext->AddCommand(&POGLUniformSetInt_Command, &POGLNothing_Release,
		sizeof(POGL_UNIFORM_SET_INT_COMMAND_DATA));
	cmd->name = &mName;
	cmd->values[0] = a;
	cmd->values[1] = b;
	cmd->values[2] = c;
	cmd->count = 3;
}

void POGLDeferredUniform::SetInt32(POGL_INT32 a, POGL_INT32 b, POGL_INT32 c, POGL_INT32 d)
{
	if (IsIntEquals(a, b, c, d))
		return;

	POGL_UNIFORM_SET_INT_COMMAND_DATA* cmd = (POGL_UNIFORM_SET_INT_COMMAND_DATA*)mRenderContext->AddCommand(&POGLUniformSetInt_Command, &POGLNothing_Release,
		sizeof(POGL_UNIFORM_SET_INT_COMMAND_DATA));
	cmd->name = &mName;
	cmd->values[0] = a;
	cmd->values[1] = b;
	cmd->values[2] = c;
	cmd->values[2] = d;
	cmd->count = 4;
}

void POGLDeferredUniform::SetInt32(POGL_INT32* ptr, POGL_UINT32 count)
{
	const POGL_UINT32 clampedCount = count > 4 ? 4 : count;

	POGL_UNIFORM_SET_INT_COMMAND_DATA* cmd = (POGL_UNIFORM_SET_INT_COMMAND_DATA*)mRenderContext->AddCommand(&POGLUniformSetInt_Command, &POGLNothing_Release,
		sizeof(POGL_UNIFORM_SET_INT_COMMAND_DATA));
	cmd->name = &mName;
	for (POGL_UINT32 i = 0; i < clampedCount; ++i)
		cmd->values[i] = (POGL_INT32)ptr[i];

	cmd->count = clampedCount;
}

void POGLDeferredUniform::SetUInt32(POGL_UINT32 a)
{
	if (IsIntEquals(a))
		return;

	POGL_UNIFORM_SET_UINT_COMMAND_DATA* cmd = (POGL_UNIFORM_SET_UINT_COMMAND_DATA*)mRenderContext->AddCommand(&POGLUniformSetUInt_Command, &POGLNothing_Release,
		sizeof(POGL_UNIFORM_SET_UINT_COMMAND_DATA));
	cmd->name = &mName;
	cmd->values[0] = a;
	cmd->count = 1;
}

void POGLDeferredUniform::SetUInt32(POGL_UINT32 a, POGL_UINT32 b)
{
	if (IsIntEquals(a, b))
		return;

	POGL_UNIFORM_SET_UINT_COMMAND_DATA* cmd = (POGL_UNIFORM_SET_UINT_COMMAND_DATA*)mRenderContext->AddCommand(&POGLUniformSetUInt_Command, &POGLNothing_Release,
		sizeof(POGL_UNIFORM_SET_UINT_COMMAND_DATA));
	cmd->name = &mName;
	cmd->values[0] = a;
	cmd->values[1] = b;
	cmd->count = 2;
}

void POGLDeferredUniform::SetUInt32(POGL_UINT32 a, POGL_UINT32 b, POGL_UINT32 c)
{
	if (IsIntEquals(a, b, c))
		return;

	POGL_UNIFORM_SET_UINT_COMMAND_DATA* cmd = (POGL_UNIFORM_SET_UINT_COMMAND_DATA*)mRenderContext->AddCommand(&POGLUniformSetUInt_Command, &POGLNothing_Release,
		sizeof(POGL_UNIFORM_SET_UINT_COMMAND_DATA));
	cmd->name = &mName;
	cmd->values[0] = a;
	cmd->values[1] = b;
	cmd->values[2] = c;
	cmd->count = 3;
}

void POGLDeferredUniform::SetUInt32(POGL_UINT32 a, POGL_UINT32 b, POGL_UINT32 c, POGL_UINT32 d)
{
	if (IsIntEquals(a, b, c, d))
		return;

	POGL_UNIFORM_SET_UINT_COMMAND_DATA* cmd = (POGL_UNIFORM_SET_UINT_COMMAND_DATA*)mRenderContext->AddCommand(&POGLUniformSetUInt_Command, &POGLNothing_Release,
		sizeof(POGL_UNIFORM_SET_UINT_COMMAND_DATA));
	cmd->name = &mName;
	cmd->values[0] = a;
	cmd->values[1] = b;
	cmd->values[2] = c;
	cmd->values[3] = d;
	cmd->count = 4;
}

void POGLDeferredUniform::SetUInt32(POGL_UINT32* ptr, POGL_UINT32 count)
{
	const POGL_UINT32 clampedCount = count > 4 ? 4 : count;

	POGL_UNIFORM_SET_UINT_COMMAND_DATA* cmd = (POGL_UNIFORM_SET_UINT_COMMAND_DATA*)mRenderContext->AddCommand(&POGLUniformSetUInt_Command, &POGLNothing_Release,
		sizeof(POGL_UNIFORM_SET_UINT_COMMAND_DATA));
	cmd->name = &mName;
	for (POGL_UINT32 i = 0; i < clampedCount; ++i)
		cmd->values[i] = (POGL_UINT32)ptr[i];

	cmd->count = clampedCount;
}

void POGLDeferredUniform::SetFloat(POGL_FLOAT a)
{
	if (IsFloatEquals(a))
		return;

	POGL_UNIFORM_SET_FLOAT_COMMAND_DATA* cmd = (POGL_UNIFORM_SET_FLOAT_COMMAND_DATA*)mRenderContext->AddCommand(&POGLUniformSetFloat_Command, &POGLNothing_Release,
		sizeof(POGL_UNIFORM_SET_FLOAT_COMMAND_DATA));
	cmd->name = &mName;
	cmd->values[0] = a;
	cmd->count = 1;
}

void POGLDeferredUniform::SetFloat(POGL_FLOAT a, POGL_FLOAT b)
{
	if (IsFloatEquals(a, b))
		return;

	POGL_UNIFORM_SET_FLOAT_COMMAND_DATA* cmd = (POGL_UNIFORM_SET_FLOAT_COMMAND_DATA*)mRenderContext->AddCommand(&POGLUniformSetFloat_Command, &POGLNothing_Release,
		sizeof(POGL_UNIFORM_SET_FLOAT_COMMAND_DATA));
	cmd->name = &mName;
	cmd->values[0] = a;
	cmd->values[1] = b;
	cmd->count = 2;
}

void POGLDeferredUniform::SetFloat(POGL_FLOAT a, POGL_FLOAT b, POGL_FLOAT c)
{
	if (IsFloatEquals(a, b, c))
		return;

	POGL_UNIFORM_SET_FLOAT_COMMAND_DATA* cmd = (POGL_UNIFORM_SET_FLOAT_COMMAND_DATA*)mRenderContext->AddCommand(&POGLUniformSetFloat_Command, &POGLNothing_Release,
		sizeof(POGL_UNIFORM_SET_FLOAT_COMMAND_DATA));
	cmd->name = &mName;
	cmd->values[0] = a;
	cmd->values[1] = b;
	cmd->values[2] = c;
	cmd->count = 3;
}

void POGLDeferredUniform::SetFloat(POGL_FLOAT a, POGL_FLOAT b, POGL_FLOAT c, POGL_FLOAT d)
{
	if (IsFloatEquals(a, b, c, d))
		return;

	POGL_UNIFORM_SET_FLOAT_COMMAND_DATA* cmd = (POGL_UNIFORM_SET_FLOAT_COMMAND_DATA*)mRenderContext->AddCommand(&POGLUniformSetFloat_Command, &POGLNothing_Release,
		sizeof(POGL_UNIFORM_SET_FLOAT_COMMAND_DATA));
	cmd->name = &mName;
	cmd->values[0] = a;
	cmd->values[1] = b;
	cmd->values[2] = c;
	cmd->values[3] = d;
	cmd->count = 4;
}

void POGLDeferredUniform::SetFloat(POGL_FLOAT* ptr, POGL_UINT32 count)
{
	const POGL_UINT32 clampedCount = count > 4 ? 4 : count;

	POGL_UNIFORM_SET_FLOAT_COMMAND_DATA* cmd = (POGL_UNIFORM_SET_FLOAT_COMMAND_DATA*)mRenderContext->AddCommand(&POGLUniformSetFloat_Command, &POGLNothing_Release,
		sizeof(POGL_UNIFORM_SET_FLOAT_COMMAND_DATA));
	cmd->name = &mName;
	for (POGL_UINT32 i = 0; i < clampedCount; ++i)
		cmd->values[i] = ptr[i];

	cmd->count = clampedCount;
}

void POGLDeferredUniform::SetDouble(POGL_DOUBLE a)
{
	if (IsDoubleEquals(a))
		return;

	POGL_UNIFORM_SET_DOUBLE_COMMAND_DATA* cmd = (POGL_UNIFORM_SET_DOUBLE_COMMAND_DATA*)mRenderContext->AddCommand(&POGLUniformSetDouble_Command, &POGLNothing_Release,
		sizeof(POGL_UNIFORM_SET_DOUBLE_COMMAND_DATA));
	cmd->name = &mName;
	cmd->values[0] = a;
	cmd->count = 1;
}

void POGLDeferredUniform::SetDouble(POGL_DOUBLE a, POGL_DOUBLE b)
{
	if (IsDoubleEquals(a, b))
		return;

	POGL_UNIFORM_SET_DOUBLE_COMMAND_DATA* cmd = (POGL_UNIFORM_SET_DOUBLE_COMMAND_DATA*)mRenderContext->AddCommand(&POGLUniformSetDouble_Command, &POGLNothing_Release,
		sizeof(POGL_UNIFORM_SET_DOUBLE_COMMAND_DATA));
	cmd->name = &mName;
	cmd->values[0] = a;
	cmd->values[1] = b;
	cmd->count = 2;
}

void POGLDeferredUniform::SetDouble(POGL_DOUBLE a, POGL_DOUBLE b, POGL_DOUBLE c)
{
	if (IsDoubleEquals(a, b, c))
		return;

	POGL_UNIFORM_SET_DOUBLE_COMMAND_DATA* cmd = (POGL_UNIFORM_SET_DOUBLE_COMMAND_DATA*)mRenderContext->AddCommand(&POGLUniformSetDouble_Command, &POGLNothing_Release,
		sizeof(POGL_UNIFORM_SET_DOUBLE_COMMAND_DATA));
	cmd->name = &mName;
	cmd->values[0] = a;
	cmd->values[1] = b;
	cmd->values[2] = c;
	cmd->count = 3;
}

void POGLDeferredUniform::SetDouble(POGL_DOUBLE a, POGL_DOUBLE b, POGL_DOUBLE c, POGL_DOUBLE d)
{
	if (IsDoubleEquals(a, b, c, d))
		return;

	POGL_UNIFORM_SET_DOUBLE_COMMAND_DATA* cmd = (POGL_UNIFORM_SET_DOUBLE_COMMAND_DATA*)mRenderContext->AddCommand(&POGLUniformSetDouble_Command, &POGLNothing_Release,
		sizeof(POGL_UNIFORM_SET_DOUBLE_COMMAND_DATA));
	cmd->name = &mName;
	cmd->values[0] = a;
	cmd->values[1] = b;
	cmd->values[2] = c;
	cmd->values[3] = d;
	cmd->count = 4;
}

void POGLDeferredUniform::SetDouble(POGL_DOUBLE* ptr, POGL_UINT32 count)
{
	const POGL_UINT32 clampedCount = count > 4 ? 4 : count;

	POGL_UNIFORM_SET_DOUBLE_COMMAND_DATA* cmd = (POGL_UNIFORM_SET_DOUBLE_COMMAND_DATA*)mRenderContext->AddCommand(&POGLUniformSetFloat_Command, &POGLNothing_Release,
		sizeof(POGL_UNIFORM_SET_DOUBLE_COMMAND_DATA));
	cmd->name = &mName;
	for (POGL_UINT32 i = 0; i < clampedCount; ++i)
		cmd->values[i] = ptr[i];

	cmd->count = clampedCount;
}

void POGLDeferredUniform::SetMatrix(const POGL_MAT4& mat4)
{
	POGL_UNIFORM_SET_MAT4_COMMAND_DATA* cmd = (POGL_UNIFORM_SET_MAT4_COMMAND_DATA*)mRenderContext->AddCommand(&POGLUniformSetMat4_Command, &POGLNothing_Release,
		sizeof(POGL_UNIFORM_SET_MAT4_COMMAND_DATA));
	cmd->name = &mName;
	cmd->matrix = mat4;
}

void POGLDeferredUniform::SetVector2(const POGL_VECTOR2& vec)
{
	POGLDeferredUniform::SetFloat(vec.x, vec.y);
}

void POGLDeferredUniform::SetVector3(const POGL_VECTOR3& vec)
{
	POGLDeferredUniform::SetFloat(vec.x, vec.y, vec.z);
}

void POGLDeferredUniform::SetVector4(const POGL_VECTOR4& vec)
{
	POGLDeferredUniform::SetFloat(vec.x, vec.y, vec.z, vec.w);
}

void POGLDeferredUniform::SetSize(const POGL_SIZE& size)
{
	POGL_UNIFORM_SET_SIZE_COMMAND_DATA* cmd = (POGL_UNIFORM_SET_SIZE_COMMAND_DATA*)mRenderContext->AddCommand(&POGLUniformSetSize_Command, &POGLNothing_Release,
		sizeof(POGL_UNIFORM_SET_SIZE_COMMAND_DATA));
	cmd->name = &mName;
	cmd->size = size;
}

void POGLDeferredUniform::SetRect(const POGL_RECT& rect)
{
	POGL_UNIFORM_SET_RECT_COMMAND_DATA* cmd = (POGL_UNIFORM_SET_RECT_COMMAND_DATA*)mRenderContext->AddCommand(&POGLUniformSetRect_Command, &POGLNothing_Release,
		sizeof(POGL_UNIFORM_SET_RECT_COMMAND_DATA));
	cmd->name = &mName;
	cmd->rect = rect;
}

IPOGLSamplerState* POGLDeferredUniform::GetSamplerState()
{
	return this;
}

void POGLDeferredUniform::SetTexture(IPOGLTexture* texture)
{
	if (IsTextureEquals(texture))
		return;

	POGL_UNIFORM_SET_TEXTURE_COMMAND_DATA* cmd = (POGL_UNIFORM_SET_TEXTURE_COMMAND_DATA*)mRenderContext->AddCommand(&POGLUniformSetTexture_Command, &POGLUniformSetTexture_Release,
		sizeof(POGL_UNIFORM_SET_TEXTURE_COMMAND_DATA));
	cmd->name = &mName;
	cmd->texture = texture;
	if (texture != nullptr)
		texture->AddRef();
}

void POGLDeferredUniform::SetMinFilter(POGLMinFilter::Enum minFilter)
{
	POGL_UNIFORM_SET_MINFILTER_COMMAND_DATA* cmd = (POGL_UNIFORM_SET_MINFILTER_COMMAND_DATA*)mRenderContext->AddCommand(&POGLUniformSetMinFilter_Command, &POGLNothing_Release,
		sizeof(POGL_UNIFORM_SET_MINFILTER_COMMAND_DATA));
	cmd->name = &mName;
	cmd->minFilter = minFilter;
}

void POGLDeferredUniform::SetMagFilter(POGLMagFilter::Enum magFilter)
{
	POGL_UNIFORM_SET_MAGFILTER_COMMAND_DATA* cmd = (POGL_UNIFORM_SET_MAGFILTER_COMMAND_DATA*)mRenderContext->AddCommand(&POGLUniformSetMagFilter_Command, &POGLNothing_Release,
		sizeof(POGL_UNIFORM_SET_MAGFILTER_COMMAND_DATA));
	cmd->name = &mName;
	cmd->magFilter = magFilter;
}

void POGLDeferredUniform::SetTextureWrap(POGLTextureWrap::Enum s, POGLTextureWrap::Enum t)
{
	POGL_UNIFORM_SET_TEXTUREWRAP_COMMAND_DATA* cmd = (POGL_UNIFORM_SET_TEXTUREWRAP_COMMAND_DATA*)mRenderContext->AddCommand(&POGLUniformSetTextureWrapST_Command, &POGLNothing_Release,
		sizeof(POGL_UNIFORM_SET_TEXTUREWRAP_COMMAND_DATA));
	cmd->name = &mName;
	cmd->textureWrap[0] = s;
	cmd->textureWrap[1] = t;
}

void POGLDeferredUniform::SetTextureWrap(POGLTextureWrap::Enum s, POGLTextureWrap::Enum t, POGLTextureWrap::Enum r)
{
	POGL_UNIFORM_SET_TEXTUREWRAP_COMMAND_DATA* cmd = (POGL_UNIFORM_SET_TEXTUREWRAP_COMMAND_DATA*)mRenderContext->AddCommand(&POGLUniformSetTextureWrapSTR_Command, &POGLNothing_Release,
		sizeof(POGL_UNIFORM_SET_TEXTUREWRAP_COMMAND_DATA));
	cmd->name = &mName;
	cmd->textureWrap[0] = s;
	cmd->textureWrap[1] = t;
	cmd->textureWrap[2] = r;
}

void POGLDeferredUniform::SetCompareFunc(POGLCompareFunc::Enum compareFunc)
{
	POGL_UNIFORM_SETCOMPAREFUNC_COMMAND_DATA* cmd = (POGL_UNIFORM_SETCOMPAREFUNC_COMMAND_DATA*)mRenderContext->AddCommand(&POGLUniformSetCompareFunc_Command, &POGLNothing_Release,
		sizeof(POGL_UNIFORM_SETCOMPAREFUNC_COMMAND_DATA));
	cmd->name = &mName;
	cmd->compareFunc = compareFunc;
}

void POGLDeferredUniform::SetCompareMode(POGLCompareMode::Enum compareMode)
{
	POGL_UNIFORM_SETCOMPAREMODE_COMMAND_DATA* cmd = (POGL_UNIFORM_SETCOMPAREMODE_COMMAND_DATA*)mRenderContext->AddCommand(&POGLUniformSetCompareMode_Command, &POGLNothing_Release,
		sizeof(POGL_UNIFORM_SETCOMPAREMODE_COMMAND_DATA));
	cmd->name = &mName;
	cmd->compareMode = compareMode;
}

bool POGLDeferredUniform::IsIntEquals(POGL_UINT32 a)
{
	if (mAssigned && mInts[0] == a)
		return true;

	mInts[0] = a;
	mAssigned = true;
	return false;
}

bool POGLDeferredUniform::IsIntEquals(POGL_UINT32 a, POGL_UINT32 b)
{
	if (mAssigned && mInts[0] == a && mInts[1] == b)
		return true;

	mInts[0] = a;
	mInts[1] = b;
	mAssigned = true;
	return false;
}

bool POGLDeferredUniform::IsIntEquals(POGL_UINT32 a, POGL_UINT32 b, POGL_UINT32 c)
{
	if (mAssigned && mInts[0] == a && mInts[1] == b && mInts[2] == c)
		return true;

	mInts[0] = a;
	mInts[1] = b;
	mInts[2] = c;
	mAssigned = true;
	return false;
}

bool POGLDeferredUniform::IsIntEquals(POGL_UINT32 a, POGL_UINT32 b, POGL_UINT32 c, POGL_UINT32 d)
{
	if (mAssigned && mInts[0] == a && mInts[1] == b && mInts[2] == c && mInts[3] == d)
		return true;

	mInts[0] = a;
	mInts[1] = b;
	mInts[2] = c;
	mInts[3] = d;
	mAssigned = true;
	return false;
}

bool POGLDeferredUniform::IsFloatEquals(POGL_FLOAT a)
{
	if (mAssigned && FLT_EQ(mFloats[0], a))
		return true;

	mFloats[0] = a;
	mAssigned = true;
	return false;
}

bool POGLDeferredUniform::IsFloatEquals(POGL_FLOAT a, POGL_FLOAT b)
{
	if (mAssigned && FLT_EQ(mFloats[0], a) && FLT_EQ(mFloats[1], b))
		return true;

	mFloats[0] = a;
	mFloats[1] = b;
	mAssigned = true;
	return false;
}

bool POGLDeferredUniform::IsFloatEquals(POGL_FLOAT a, POGL_FLOAT b, POGL_FLOAT c)
{
	if (mAssigned && FLT_EQ(mFloats[0], a) && FLT_EQ(mFloats[1], b) && FLT_EQ(mFloats[2], c))
		return true;

	mFloats[0] = a;
	mFloats[1] = b;
	mFloats[2] = c;
	mAssigned = true;
	return false;
}

bool POGLDeferredUniform::IsFloatEquals(POGL_FLOAT a, POGL_FLOAT b, POGL_FLOAT c, POGL_FLOAT d)
{
	if (mAssigned && FLT_EQ(mFloats[0], a) && FLT_EQ(mFloats[1], b) && FLT_EQ(mFloats[2], c) && FLT_EQ(mFloats[3], d))
		return true;

	mFloats[0] = a;
	mFloats[1] = b;
	mFloats[2] = c;
	mFloats[3] = d;
	mAssigned = true;
	return false;
}

bool POGLDeferredUniform::IsDoubleEquals(POGL_DOUBLE a)
{
	if (mAssigned && DBL_EQ(mDoubles[0], a))
		return true;

	mDoubles[0] = a;
	mAssigned = true;
	return false;
}

bool POGLDeferredUniform::IsDoubleEquals(POGL_DOUBLE a, POGL_DOUBLE b)
{
	if (mAssigned && DBL_EQ(mDoubles[0], a) && DBL_EQ(mDoubles[1], b))
		return true;

	mDoubles[0] = a;
	mDoubles[1] = b;
	mAssigned = true;
	return false;
}

bool POGLDeferredUniform::IsDoubleEquals(POGL_DOUBLE a, POGL_DOUBLE b, POGL_DOUBLE c)
{
	if (mAssigned && DBL_EQ(mDoubles[0], a) && DBL_EQ(mDoubles[1], b) && DBL_EQ(mDoubles[2], c))
		return true;

	mDoubles[0] = a;
	mDoubles[1] = b;
	mDoubles[2] = c;
	mAssigned = true;
	return false;
}

bool POGLDeferredUniform::IsDoubleEquals(POGL_DOUBLE a, POGL_DOUBLE b, POGL_DOUBLE c, POGL_DOUBLE d)
{
	if (mAssigned && DBL_EQ(mDoubles[0], a) && DBL_EQ(mDoubles[1], b) && DBL_EQ(mDoubles[2], c) && DBL_EQ(mDoubles[3], d))
		return true;

	mDoubles[0] = a;
	mDoubles[1] = b;
	mDoubles[2] = c;
	mDoubles[3] = d;
	mAssigned = true;
	return false;
}

bool POGLDeferredUniform::IsTextureEquals(IPOGLTexture* texture)
{
	if (mAssigned && mTexture == texture)
		return true;

	if (mTexture != nullptr)
		mTexture->Release();
	mTexture = texture;
	if (mTexture != nullptr)
		mTexture->AddRef();
	return false;
}