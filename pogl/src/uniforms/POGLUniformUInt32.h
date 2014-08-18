#pragma once
#include "POGLDefaultUniform.h"
#include <mutex>

class POGLUniformUInt32 : public POGLDefaultUniform
{
public:
	POGLUniformUInt32(POGL_UINT32 programUID, POGLRenderState* state, GLint componentID);
	~POGLUniformUInt32();

	void Apply();

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

private:
	POGL_UINT32 mCount;
	POGL_UINT32 mValues[4];
	POGL_UINT32 mValuesSet[4];
	std::mutex mMutex;
};
