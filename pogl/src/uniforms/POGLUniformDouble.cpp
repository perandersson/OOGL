#include "MemCheck.h"
#include "POGLUniformDouble.h"
#include "POGLDeviceContext.h"

POGLUniformDouble::POGLUniformDouble(POGL_UINT32 programUID, POGLRenderState* state, GLint componentID)
: POGLDefaultUniform(programUID, state, componentID), mCount(0)
{
	mValues[0] = 0;
	mValues[1] = 0;
	mValues[2] = 0;
	mValues[3] = 0;

	mValuesSet[0] = DBL_MAX;
	mValuesSet[1] = DBL_MAX;
	mValuesSet[2] = DBL_MAX;
	mValuesSet[3] = DBL_MAX;
}

POGLUniformDouble::~POGLUniformDouble()
{
}

void POGLUniformDouble::Apply()
{
	switch (mCount) {
	case 3:
		if (DBL_EQ(mValuesSet[0], mValues[0]) && DBL_EQ(mValuesSet[1], mValues[1]) && DBL_EQ(mValuesSet[2], mValues[2])) return;
		mValuesSet[0] = mValues[0];
		mValuesSet[1] = mValues[1];
		mValuesSet[2] = mValues[2];
		glUniform3dv(mComponentID, 1, mValues);
		break;
	case 2:
		if (DBL_EQ(mValuesSet[0], mValues[0]) && DBL_EQ(mValuesSet[1], mValues[1])) return;
		mValuesSet[0] = mValues[0];
		mValuesSet[1] = mValues[1];
		glUniform2dv(mComponentID, 1, mValues);
		break;
	case 4:
		if (DBL_EQ(mValuesSet[0], mValues[0]) && DBL_EQ(mValuesSet[1], mValues[1]) && DBL_EQ(mValuesSet[2], mValues[2]) && DBL_EQ(mValuesSet[3], mValues[3])) return;
		mValuesSet[0] = mValues[0];
		mValuesSet[1] = mValues[1];
		mValuesSet[2] = mValues[2];
		mValuesSet[3] = mValues[3];
		glUniform4dv(mComponentID, 1, mValues);
		break;
	case 1:
		if (DBL_EQ(mValuesSet[0], mValues[0])) return;
		mValuesSet[0] = mValues[0];
		glUniform1dv(mComponentID, 1, mValues);
		break;
	default:
		return;
	}

	CHECK_GL("Could not assign double uniform values");
}

void POGLUniformDouble::SetFloat(POGL_FLOAT a)
{
	SetDouble((POGL_DOUBLE)a);
}

void POGLUniformDouble::SetFloat(POGL_FLOAT a, POGL_FLOAT b)
{
	SetDouble((POGL_DOUBLE)a, (POGL_DOUBLE)b);
}

void POGLUniformDouble::SetFloat(POGL_FLOAT a, POGL_FLOAT b, POGL_FLOAT c)
{
	SetDouble((POGL_DOUBLE)a, (POGL_DOUBLE)b, (POGL_DOUBLE)c);
}

void POGLUniformDouble::SetFloat(POGL_FLOAT a, POGL_FLOAT b, POGL_FLOAT c, POGL_FLOAT d)
{
	SetDouble((POGL_DOUBLE)a, (POGL_DOUBLE)b, (POGL_DOUBLE)c, (POGL_DOUBLE)d);
}

void POGLUniformDouble::SetFloat(POGL_FLOAT* ptr, POGL_UINT32 count)
{
	std::lock_guard<std::mutex> lock(mMutex);

	const POGL_UINT32 clampedCount = count > 4 ? 4 : count;
	for (POGL_UINT32 i = 0; i < clampedCount; ++i)
		mValues[i] = (POGL_DOUBLE)ptr[i];

	if (IsProgramActive())
		POGLUniformDouble::Apply();
}

void POGLUniformDouble::SetDouble(POGL_DOUBLE a)
{
	std::lock_guard<std::mutex> lock(mMutex);

	mCount = 1;
	mValues[0] = a;
	mValues[1] = DBL_MAX;
	mValues[2] = DBL_MAX;
	mValues[3] = DBL_MAX;

	if (IsProgramActive())
		POGLUniformDouble::Apply();
}

void POGLUniformDouble::SetDouble(POGL_DOUBLE a, POGL_DOUBLE b)
{
	std::lock_guard<std::mutex> lock(mMutex);

	mCount = 2;
	mValues[0] = a;
	mValues[1] = b;
	mValues[2] = DBL_MAX;
	mValues[3] = DBL_MAX;

	if (IsProgramActive())
		POGLUniformDouble::Apply();
}

void POGLUniformDouble::SetDouble(POGL_DOUBLE a, POGL_DOUBLE b, POGL_DOUBLE c)
{
	std::lock_guard<std::mutex> lock(mMutex);

	mCount = 3;
	mValues[0] = a;
	mValues[1] = b;
	mValues[2] = c;
	mValues[3] = DBL_MAX;

	if (IsProgramActive())
		POGLUniformDouble::Apply();
}

void POGLUniformDouble::SetDouble(POGL_DOUBLE a, POGL_DOUBLE b, POGL_DOUBLE c, POGL_DOUBLE d)
{
	std::lock_guard<std::mutex> lock(mMutex);

	mCount = 4;
	mValues[0] = a;
	mValues[1] = b;
	mValues[2] = c;
	mValues[3] = d;

	if (IsProgramActive())
		POGLUniformDouble::Apply();
}

void POGLUniformDouble::SetDouble(POGL_DOUBLE* ptr, POGL_UINT32 count)
{
	std::lock_guard<std::mutex> lock(mMutex);

	const POGL_UINT32 clampedCount = count > 4 ? 4 : count;
	for (POGL_UINT32 i = 0; i < clampedCount; ++i)
		mValues[i] = ptr[i];

	if (IsProgramActive())
		POGLUniformDouble::Apply();
}

void POGLUniformDouble::SetVector2(const POGL_VECTOR2& vec)
{
	SetDouble((POGL_DOUBLE)vec.x, (POGL_DOUBLE)vec.y);
}

void POGLUniformDouble::SetVector3(const POGL_VECTOR3& vec)
{
	SetDouble((POGL_DOUBLE)vec.x, (POGL_DOUBLE)vec.y, (POGL_DOUBLE)vec.z);
}

void POGLUniformDouble::SetVector4(const POGL_VECTOR4& vec)
{
	SetDouble((POGL_DOUBLE)vec.x, (POGL_DOUBLE)vec.y, (POGL_DOUBLE)vec.z, (POGL_DOUBLE)vec.w);
}
