#include "MemCheck.h"
#include "POGLSamplerObject.h"
#include <atomic>

namespace {
	std::atomic<POGL_UINT32> uid;
	POGL_UINT32 GenSamplerStateUID() {
		return ++uid;
	}
}

POGLSamplerObject::POGLSamplerObject(GLuint samplerID, POGLRenderState* renderState)
: mUID(GenSamplerStateUID()), mSamplerID(samplerID), mRenderState(renderState)
{
}

POGLSamplerObject::~POGLSamplerObject()
{
	if (mSamplerID != 0) {
		glDeleteSamplers(1, &mSamplerID);
		mSamplerID = 0;
	}
}

POGL_UINT32 POGLSamplerObject::GetUID() const
{
	return mUID;
}

GLuint POGLSamplerObject::GetSamplerID() const
{
	return mSamplerID;
}
