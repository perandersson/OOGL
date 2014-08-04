#include "POGLDefaultUniform.hxx"
#include "POGLEffect.hxx"
#include "POGLRenderState.hxx"

POGLDefaultUniform::POGLDefaultUniform(const POGLEffect* effect, POGLRenderState* state, POGLDeviceContext* context, GLint componentID)
: mEffectUID(effect->GetUID()), mRenderState(state), mDeviceContext(context), mComponentID(componentID)
{
}

POGLDefaultUniform::~POGLDefaultUniform()
{
}

bool POGLDefaultUniform::IsEffectActive() const
{
	return mRenderState->IsEffectActive(mEffectUID);
}

void POGLDefaultUniform::SetInt32(POGL_INT32 a)
{
	assert(false && "Invalid uniform type");
}

void POGLDefaultUniform::SetInt32(POGL_INT32 a, POGL_INT32 b)
{
	assert(false && "Invalid uniform type");
}

void POGLDefaultUniform::SetInt32(POGL_INT32 a, POGL_INT32 b, POGL_INT32 c)
{
	assert(false && "Invalid uniform type");
}

void POGLDefaultUniform::SetInt32(POGL_INT32 a, POGL_INT32 b, POGL_INT32 c, POGL_INT32 d)
{
	assert(false && "Invalid uniform type");
}

void POGLDefaultUniform::SetInt32(POGL_INT32* ptr, POGL_UINT32 count)
{
	assert(false && "Invalid uniform type");
}

void POGLDefaultUniform::SetUInt32(POGL_UINT32 a)
{
	assert(false && "Invalid uniform type");
}

void POGLDefaultUniform::SetUInt32(POGL_UINT32 a, POGL_UINT32 b)
{
	assert(false && "Invalid uniform type");
}

void POGLDefaultUniform::SetUInt32(POGL_UINT32 a, POGL_UINT32 b, POGL_UINT32 c)
{
	assert(false && "Invalid uniform type");
}

void POGLDefaultUniform::SetUInt32(POGL_UINT32 a, POGL_UINT32 b, POGL_UINT32 c, POGL_UINT32 d)
{
	assert(false && "Invalid uniform type");
}

void POGLDefaultUniform::SetUInt32(POGL_UINT32* ptr, POGL_UINT32 count)
{
	assert(false && "Invalid uniform type");
}

void POGLDefaultUniform::SetFloat(POGL_FLOAT a)
{
	assert(false && "Invalid uniform type");
}

void POGLDefaultUniform::SetFloat(POGL_FLOAT a, POGL_FLOAT b)
{
	assert(false && "Invalid uniform type");
}

void POGLDefaultUniform::SetFloat(POGL_FLOAT a, POGL_FLOAT b, POGL_FLOAT c)
{
	assert(false && "Invalid uniform type");
}

void POGLDefaultUniform::SetFloat(POGL_FLOAT a, POGL_FLOAT b, POGL_FLOAT c, POGL_FLOAT d)
{
	assert(false && "Invalid uniform type");
}

void POGLDefaultUniform::SetFloat(POGL_FLOAT* ptr, POGL_UINT32 count)
{
	assert(false && "Invalid uniform type");
}

void POGLDefaultUniform::SetDouble(POGL_DOUBLE a)
{
	assert(false && "Invalid uniform type");
}

void POGLDefaultUniform::SetDouble(POGL_DOUBLE a, POGL_DOUBLE b)
{
	assert(false && "Invalid uniform type");
}

void POGLDefaultUniform::SetDouble(POGL_DOUBLE a, POGL_DOUBLE b, POGL_DOUBLE c)
{
	assert(false && "Invalid uniform type");
}

void POGLDefaultUniform::SetDouble(POGL_DOUBLE a, POGL_DOUBLE b, POGL_DOUBLE c, POGL_DOUBLE d)
{
	assert(false && "Invalid uniform type");
}

void POGLDefaultUniform::SetDouble(POGL_DOUBLE* ptr, POGL_UINT32 count)
{
	assert(false && "Invalid uniform type");
}

void POGLDefaultUniform::SetMatrix(const POGL_MAT4F& mat4)
{
	assert(false && "Invalid uniform type");
}

void POGLDefaultUniform::SetMatrix(const POGL_MAT4D& mat4)
{
	assert(false && "Invalid uniform type");
}

void POGLDefaultUniform::SetVector2I(const POGL_VECTOR2I& vec)
{
	assert(false && "Invalid uniform type");
}

void POGLDefaultUniform::SetVector2F(const POGL_VECTOR2F& vec)
{
	assert(false && "Invalid uniform type");
}

void POGLDefaultUniform::SetVector2D(const POGL_VECTOR2D& vec)
{
	assert(false && "Invalid uniform type");
}

void POGLDefaultUniform::SetVector3F(const POGL_VECTOR3F& vec)
{
	assert(false && "Invalid uniform type");
}

void POGLDefaultUniform::SetVector3D(const POGL_VECTOR3D& vec)
{
	assert(false && "Invalid uniform type");
}

void POGLDefaultUniform::SetVector4F(const POGL_VECTOR4F& vec)
{
	assert(false && "Invalid uniform type");
}

void POGLDefaultUniform::SetVector4D(const POGL_VECTOR4D& vec)
{
	assert(false && "Invalid uniform type");
}

IPOGLSamplerState* POGLDefaultUniform::GetSamplerState()
{
	return this;
}

void POGLDefaultUniform::SetTexture(IPOGLTexture* texture)
{
	assert(false && "Invalid uniform type");
}

void POGLDefaultUniform::SetMinFilter(POGLMinFilter::Enum minFilter)
{
	assert(false && "Invalid uniform type");
}

void POGLDefaultUniform::SetMagFilter(POGLMagFilter::Enum magFilter)
{
	assert(false && "Invalid uniform type");
}

void POGLDefaultUniform::SetTextureWrap(POGLTextureWrap::Enum s, POGLTextureWrap::Enum t)
{
	assert(false && "Invalid uniform type");
}

void POGLDefaultUniform::SetTextureWrap(POGLTextureWrap::Enum s, POGLTextureWrap::Enum t, POGLTextureWrap::Enum r)
{
	assert(false && "Invalid uniform type");
}

void POGLDefaultUniform::SetCompareFunc(POGLCompareFunc::Enum compareFunc)
{
	assert(false && "Invalid uniform type");
}

void POGLDefaultUniform::SetCompareMode(POGLCompareMode::Enum compareMode)
{
	assert(false && "Invalid uniform type");
}
