#include "POGLSamplerObject.hxx"
#include "POGLDeviceContext.hxx"
#include "POGLRenderState.hxx"
#include "POGLEnum.hxx"
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
		IPOGLDeviceContext* context = mRenderState->GetDeviceContext();
		static_cast<POGLDeviceContext*>(context)->DeleteSampler(mSamplerID);
		context->Release();
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
