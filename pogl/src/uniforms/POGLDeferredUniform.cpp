#include "MemCheck.h"
#include "POGLDeferredUniform.h"
#include "POGLDeferredRenderContext.h"
#include "POGLDeferredCommands.h"

POGLDeferredUniform::POGLDeferredUniform(const POGL_STRING& name, POGLDeferredRenderContext* context)
: mName(name), mRenderContext(context)
{
}

POGLDeferredUniform::~POGLDeferredUniform()
{
}

void POGLDeferredUniform::SetInt32(POGL_INT32 a)
{
	POGL_UNIFORM_SET_INT_COMMAND_DATA* cmd = (POGL_UNIFORM_SET_INT_COMMAND_DATA*)mRenderContext->AddCommand(&POGLUniformSetInt_Command, &POGLNothing_Release,
		sizeof(POGL_UNIFORM_SET_INT_COMMAND_DATA));
	cmd->name = &mName;
	cmd->values[0] = a;
	cmd->count = 1;
}

void POGLDeferredUniform::SetInt32(POGL_INT32 a, POGL_INT32 b)
{
	POGL_UNIFORM_SET_INT_COMMAND_DATA* cmd = (POGL_UNIFORM_SET_INT_COMMAND_DATA*)mRenderContext->AddCommand(&POGLUniformSetInt_Command, &POGLNothing_Release,
		sizeof(POGL_UNIFORM_SET_INT_COMMAND_DATA));
	cmd->name = &mName;
	cmd->values[0] = a;
	cmd->values[1] = b;
	cmd->count = 2;
}

void POGLDeferredUniform::SetInt32(POGL_INT32 a, POGL_INT32 b, POGL_INT32 c)
{
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
	POGL_UNIFORM_SET_UINT_COMMAND_DATA* cmd = (POGL_UNIFORM_SET_UINT_COMMAND_DATA*)mRenderContext->AddCommand(&POGLUniformSetUInt_Command, &POGLNothing_Release,
		sizeof(POGL_UNIFORM_SET_UINT_COMMAND_DATA));
	cmd->name = &mName;
	cmd->values[0] = a;
	cmd->count = 1;
}

void POGLDeferredUniform::SetUInt32(POGL_UINT32 a, POGL_UINT32 b)
{
	POGL_UNIFORM_SET_UINT_COMMAND_DATA* cmd = (POGL_UNIFORM_SET_UINT_COMMAND_DATA*)mRenderContext->AddCommand(&POGLUniformSetUInt_Command, &POGLNothing_Release,
		sizeof(POGL_UNIFORM_SET_UINT_COMMAND_DATA));
	cmd->name = &mName;
	cmd->values[0] = a;
	cmd->values[1] = b;
	cmd->count = 2;
}

void POGLDeferredUniform::SetUInt32(POGL_UINT32 a, POGL_UINT32 b, POGL_UINT32 c)
{
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
	POGL_UNIFORM_SET_FLOAT_COMMAND_DATA* cmd = (POGL_UNIFORM_SET_FLOAT_COMMAND_DATA*)mRenderContext->AddCommand(&POGLUniformSetFloat_Command, &POGLNothing_Release,
		sizeof(POGL_UNIFORM_SET_FLOAT_COMMAND_DATA));
	cmd->name = &mName;
	cmd->values[0] = a;
	cmd->count = 1;
}

void POGLDeferredUniform::SetFloat(POGL_FLOAT a, POGL_FLOAT b)
{
	POGL_UNIFORM_SET_FLOAT_COMMAND_DATA* cmd = (POGL_UNIFORM_SET_FLOAT_COMMAND_DATA*)mRenderContext->AddCommand(&POGLUniformSetFloat_Command, &POGLNothing_Release,
		sizeof(POGL_UNIFORM_SET_FLOAT_COMMAND_DATA));
	cmd->name = &mName;
	cmd->values[0] = a;
	cmd->values[1] = b;
	cmd->count = 2;
}

void POGLDeferredUniform::SetFloat(POGL_FLOAT a, POGL_FLOAT b, POGL_FLOAT c)
{
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
	POGL_UNIFORM_SET_DOUBLE_COMMAND_DATA* cmd = (POGL_UNIFORM_SET_DOUBLE_COMMAND_DATA*)mRenderContext->AddCommand(&POGLUniformSetDouble_Command, &POGLNothing_Release,
		sizeof(POGL_UNIFORM_SET_DOUBLE_COMMAND_DATA));
	cmd->name = &mName;
	cmd->values[0] = a;
	cmd->count = 1;
}

void POGLDeferredUniform::SetDouble(POGL_DOUBLE a, POGL_DOUBLE b)
{
	POGL_UNIFORM_SET_DOUBLE_COMMAND_DATA* cmd = (POGL_UNIFORM_SET_DOUBLE_COMMAND_DATA*)mRenderContext->AddCommand(&POGLUniformSetDouble_Command, &POGLNothing_Release,
		sizeof(POGL_UNIFORM_SET_DOUBLE_COMMAND_DATA));
	cmd->name = &mName;
	cmd->values[0] = a;
	cmd->values[1] = b;
	cmd->count = 2;
}

void POGLDeferredUniform::SetDouble(POGL_DOUBLE a, POGL_DOUBLE b, POGL_DOUBLE c)
{
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
	POGL_UNIFORM_SET_FLOAT_COMMAND_DATA* cmd = (POGL_UNIFORM_SET_FLOAT_COMMAND_DATA*)mRenderContext->AddCommand(&POGLUniformSetFloat_Command, &POGLNothing_Release,
		sizeof(POGL_UNIFORM_SET_FLOAT_COMMAND_DATA));
	cmd->name = &mName;
	cmd->values[0] = vec.x;
	cmd->values[1] = vec.y;
	cmd->count = 2;
}

void POGLDeferredUniform::SetVector3(const POGL_VECTOR3& vec)
{
	POGL_UNIFORM_SET_FLOAT_COMMAND_DATA* cmd = (POGL_UNIFORM_SET_FLOAT_COMMAND_DATA*)mRenderContext->AddCommand(&POGLUniformSetFloat_Command, &POGLNothing_Release,
		sizeof(POGL_UNIFORM_SET_FLOAT_COMMAND_DATA));
	cmd->name = &mName;
	cmd->values[0] = vec.x;
	cmd->values[1] = vec.y;
	cmd->values[2] = vec.z;
	cmd->count = 3;
}

void POGLDeferredUniform::SetVector4(const POGL_VECTOR4& vec)
{
	POGL_UNIFORM_SET_FLOAT_COMMAND_DATA* cmd = (POGL_UNIFORM_SET_FLOAT_COMMAND_DATA*)mRenderContext->AddCommand(&POGLUniformSetFloat_Command, &POGLNothing_Release,
		sizeof(POGL_UNIFORM_SET_FLOAT_COMMAND_DATA));
	cmd->name = &mName;
	cmd->values[0] = vec.x;
	cmd->values[1] = vec.y;
	cmd->values[2] = vec.z;
	cmd->values[3] = vec.w;
	cmd->count = 4;
}

IPOGLSamplerState* POGLDeferredUniform::GetSamplerState()
{
	return this;
}

void POGLDeferredUniform::SetTexture(IPOGLTexture* texture)
{
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
