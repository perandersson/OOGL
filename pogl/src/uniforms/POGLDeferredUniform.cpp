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
	THROW_NOT_IMPLEMENTED_EXCEPTION();
}

void POGLDeferredUniform::SetUInt32(POGL_UINT32 a, POGL_UINT32 b)
{
	THROW_NOT_IMPLEMENTED_EXCEPTION();
}

void POGLDeferredUniform::SetUInt32(POGL_UINT32 a, POGL_UINT32 b, POGL_UINT32 c)
{
	THROW_NOT_IMPLEMENTED_EXCEPTION();
}

void POGLDeferredUniform::SetUInt32(POGL_UINT32 a, POGL_UINT32 b, POGL_UINT32 c, POGL_UINT32 d)
{
	THROW_NOT_IMPLEMENTED_EXCEPTION();
}

void POGLDeferredUniform::SetUInt32(POGL_UINT32* ptr, POGL_UINT32 count)
{
	THROW_NOT_IMPLEMENTED_EXCEPTION();
}

void POGLDeferredUniform::SetFloat(POGL_FLOAT a)
{
	THROW_NOT_IMPLEMENTED_EXCEPTION();
}

void POGLDeferredUniform::SetFloat(POGL_FLOAT a, POGL_FLOAT b)
{
	THROW_NOT_IMPLEMENTED_EXCEPTION();
}

void POGLDeferredUniform::SetFloat(POGL_FLOAT a, POGL_FLOAT b, POGL_FLOAT c)
{
	THROW_NOT_IMPLEMENTED_EXCEPTION();
}

void POGLDeferredUniform::SetFloat(POGL_FLOAT a, POGL_FLOAT b, POGL_FLOAT c, POGL_FLOAT d)
{
	THROW_NOT_IMPLEMENTED_EXCEPTION();
}

void POGLDeferredUniform::SetFloat(POGL_FLOAT* ptr, POGL_UINT32 count)
{
	THROW_NOT_IMPLEMENTED_EXCEPTION();
}

void POGLDeferredUniform::SetDouble(POGL_DOUBLE a)
{
	THROW_NOT_IMPLEMENTED_EXCEPTION();
}

void POGLDeferredUniform::SetDouble(POGL_DOUBLE a, POGL_DOUBLE b)
{
	THROW_NOT_IMPLEMENTED_EXCEPTION();
}

void POGLDeferredUniform::SetDouble(POGL_DOUBLE a, POGL_DOUBLE b, POGL_DOUBLE c)
{
	THROW_NOT_IMPLEMENTED_EXCEPTION();
}

void POGLDeferredUniform::SetDouble(POGL_DOUBLE a, POGL_DOUBLE b, POGL_DOUBLE c, POGL_DOUBLE d)
{
	THROW_NOT_IMPLEMENTED_EXCEPTION();
}

void POGLDeferredUniform::SetDouble(POGL_DOUBLE* ptr, POGL_UINT32 count)
{
	THROW_NOT_IMPLEMENTED_EXCEPTION();
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
	THROW_NOT_IMPLEMENTED_EXCEPTION();
}

void POGLDeferredUniform::SetVector3(const POGL_VECTOR3& vec)
{
	THROW_NOT_IMPLEMENTED_EXCEPTION();
}

void POGLDeferredUniform::SetVector4(const POGL_VECTOR4& vec)
{
	THROW_NOT_IMPLEMENTED_EXCEPTION();
}

IPOGLSamplerState* POGLDeferredUniform::GetSamplerState()
{
	return this;
}

void POGLDeferredUniform::SetTexture(IPOGLTexture* texture)
{
	THROW_NOT_IMPLEMENTED_EXCEPTION();
}

void POGLDeferredUniform::SetMinFilter(POGLMinFilter::Enum minFilter)
{
	THROW_NOT_IMPLEMENTED_EXCEPTION();
}

void POGLDeferredUniform::SetMagFilter(POGLMagFilter::Enum magFilter)
{
	THROW_NOT_IMPLEMENTED_EXCEPTION();
}

void POGLDeferredUniform::SetTextureWrap(POGLTextureWrap::Enum s, POGLTextureWrap::Enum t)
{
	THROW_NOT_IMPLEMENTED_EXCEPTION();
}

void POGLDeferredUniform::SetTextureWrap(POGLTextureWrap::Enum s, POGLTextureWrap::Enum t, POGLTextureWrap::Enum r)
{
	THROW_NOT_IMPLEMENTED_EXCEPTION();
}

void POGLDeferredUniform::SetCompareFunc(POGLCompareFunc::Enum compareFunc)
{
	THROW_NOT_IMPLEMENTED_EXCEPTION();
}

void POGLDeferredUniform::SetCompareMode(POGLCompareMode::Enum compareMode)
{
	THROW_NOT_IMPLEMENTED_EXCEPTION();
}
