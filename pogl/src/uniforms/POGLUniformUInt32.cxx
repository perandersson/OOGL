#include "POGLUniformUInt32.hxx"
#include "POGLDeviceContext.hxx"

POGLUniformUInt32::POGLUniformUInt32(const POGLEffect* effect, POGLRenderState* state, POGLDeviceContext* context, GLint componentID)
: POGLDefaultUniform(effect, state, context, componentID), mCount(0)
{
	mValues[0] = 0;
	mValues[1] = 0;
	mValues[2] = 0;
	mValues[3] = 0;

	mValuesSet[0] = UINT_MAX;
	mValuesSet[1] = UINT_MAX;
	mValuesSet[2] = UINT_MAX;
	mValuesSet[3] = UINT_MAX;
}

POGLUniformUInt32::~POGLUniformUInt32()
{
}

void POGLUniformUInt32::Apply()
{
	switch (mCount) {
	case 3:
		if (mValuesSet[0] == mValues[0] &&
			mValuesSet[1] == mValues[1] &&
			mValuesSet[2] == mValues[2])
			return;
		mValuesSet[0] = mValues[0];
		mValuesSet[1] = mValues[1];
		mValuesSet[2] = mValues[2];
		mDeviceContext->Uniform3uiv(mComponentID, 1, mValues);
		break;
	case 2:
		if (mValuesSet[0] == mValues[0] &&
			mValuesSet[1] == mValues[1])
			return;
		mValuesSet[0] = mValues[0];
		mValuesSet[1] = mValues[1];
		mDeviceContext->Uniform2uiv(mComponentID, 1, mValues);
		break;
	case 4:
		if (mValuesSet[0] == mValues[0] &&
			mValuesSet[1] == mValues[1] &&
			mValuesSet[2] == mValues[2] &&
			mValuesSet[3] == mValues[3])
			return;
		mValuesSet[0] = mValues[0];
		mValuesSet[1] = mValues[1];
		mValuesSet[2] = mValues[2];
		mValuesSet[3] = mValues[3];
		mDeviceContext->Uniform4uiv(mComponentID, 1, mValues);
		break;
	case 1:
		if (mValuesSet[0] == mValues[0])
			return;
		mValuesSet[0] = mValues[0];
		mDeviceContext->Uniform1uiv(mComponentID, 1, mValues);
		break;
	default:
		return;
	}
}

void POGLUniformUInt32::SetInt32(POGL_INT32 a)
{
	SetUInt32((POGL_UINT32)a);
}

void POGLUniformUInt32::SetInt32(POGL_INT32 a, POGL_INT32 b)
{
	SetUInt32((POGL_UINT32)a, (POGL_UINT32)b);
}

void POGLUniformUInt32::SetInt32(POGL_INT32 a, POGL_INT32 b, POGL_INT32 c)
{
	SetUInt32((POGL_UINT32)a, (POGL_UINT32)b, (POGL_UINT32)c);
}

void POGLUniformUInt32::SetInt32(POGL_INT32 a, POGL_INT32 b, POGL_INT32 c, POGL_INT32 d)
{
	SetUInt32((POGL_UINT32)a, (POGL_UINT32)b, (POGL_UINT32)c, (POGL_UINT32)d);
}

void POGLUniformUInt32::SetInt32(POGL_INT32* ptr, POGL_UINT32 count)
{
	SetUInt32((POGL_UINT32*)ptr, count);
}

void POGLUniformUInt32::SetUInt32(POGL_UINT32 a)
{
	mCount = 1;
	mValues[0] = a;

	if (IsEffectActive())
		POGLUniformUInt32::Apply();
}

void POGLUniformUInt32::SetUInt32(POGL_UINT32 a, POGL_UINT32 b)
{
	mCount = 2;
	mValues[0] = a;
	mValues[1] = b;

	if (IsEffectActive())
		POGLUniformUInt32::Apply();
}

void POGLUniformUInt32::SetUInt32(POGL_UINT32 a, POGL_UINT32 b, POGL_UINT32 c)
{
	mCount = 3;
	mValues[0] = a;
	mValues[1] = b;
	mValues[2] = c;

	if (IsEffectActive())
		POGLUniformUInt32::Apply();
}

void POGLUniformUInt32::SetUInt32(POGL_UINT32 a, POGL_UINT32 b, POGL_UINT32 c, POGL_UINT32 d)
{
	mCount = 4;
	mValues[0] = a;
	mValues[1] = b;
	mValues[2] = c;
	mValues[3] = c;

	if (IsEffectActive())
		POGLUniformUInt32::Apply();
}

void POGLUniformUInt32::SetUInt32(POGL_UINT32* ptr, POGL_UINT32 count)
{
	const POGL_UINT32 clampedCount = count > 4 ? 4 : count;
	memcpy(mValues, ptr, clampedCount);
}

void POGLUniformUInt32::SetVector2I(const POGL_VECTOR2I& vec)
{
	SetUInt32((POGL_UINT32)vec.x, (POGL_UINT32)vec.y);
}
