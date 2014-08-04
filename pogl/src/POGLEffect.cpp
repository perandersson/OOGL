#include "MemCheck.h"
#include "POGLEffect.h"
#include "POGLEffectData.h"
#include "POGLDeviceContext.h"
#include <atomic>

namespace {
	std::atomic<POGL_UINT32> ids;
	POGL_UINT32 GenEffectUID() {
		return ++ids;
	}
}

POGLEffect::POGLEffect(GLuint programID, POGLEffectData* data, std::hash_map<POGL_STRING, std::shared_ptr<POGLUniformProperty>> uniforms, IPOGLDevice* device)
: mRefCount(1), mProgramID(programID), mDevice(device), mUID(GenEffectUID()), mData(data), mUniforms(uniforms)
{
	assert_not_null(data);
}

POGLEffect::~POGLEffect()
{
	delete mData;
	mData = nullptr;

}

void POGLEffect::AddRef()
{
	mRefCount++;
}

void POGLEffect::Release()
{
	if (--mRefCount == 0) {
		if (mProgramID != 0) {
			IPOGLDeviceContext* context = mDevice->GetDeviceContext();
			static_cast<POGLDeviceContext*>(context)->DeleteProgram(mProgramID);
			context->Release();
			mProgramID = 0;
		}

		delete this;
	}
}

IPOGLDevice* POGLEffect::GetDevice()
{
	mDevice->AddRef();
	return mDevice;
}

POGL_HANDLE POGLEffect::GetHandlePtr()
{
	return this;
}

bool POGLEffect::GetDepthTest()
{
	std::lock_guard<std::recursive_mutex> lock(mMutex);
	return mData->depthTest;
}

void POGLEffect::SetDepthTest(bool b)
{
	std::lock_guard<std::recursive_mutex> lock(mMutex);
	mData->depthTest = b;
}

void POGLEffect::SetDepthFunc(POGLDepthFunc::Enum depthFunc)
{
	std::lock_guard<std::recursive_mutex> lock(mMutex);
	mData->depthFunc = depthFunc;
}

POGLDepthFunc::Enum POGLEffect::GetDepthFunc()
{
	std::lock_guard<std::recursive_mutex> lock(mMutex);
	return mData->depthFunc;
}

bool POGLEffect::GetDepthMask()
{
	std::lock_guard<std::recursive_mutex> lock(mMutex);
	return mData->depthMask;
}

void POGLEffect::SetDepthMask(bool b)
{
	std::lock_guard<std::recursive_mutex> lock(mMutex);
	mData->depthMask = b;
}

POGL_UINT8 POGLEffect::GetColorMask()
{
	std::lock_guard<std::recursive_mutex> lock(mMutex);
	return mData->colorMask;
}

void POGLEffect::SetColorMask(POGL_UINT8 colorMask)
{
	std::lock_guard<std::recursive_mutex> lock(mMutex);
	mData->colorMask = colorMask;
}

bool POGLEffect::GetStencilTest()
{
	std::lock_guard<std::recursive_mutex> lock(mMutex);
	return mData->stencilTest;
}

void POGLEffect::SetStencilTest(bool b)
{
	std::lock_guard<std::recursive_mutex> lock(mMutex);
	mData->stencilTest = b;
}

void POGLEffect::CopyEffectData(POGLEffectData* in)
{
	assert_not_null(in);
	std::lock_guard<std::recursive_mutex> lock(mMutex);
	// Copy!!
	*in = *mData;
}

POGL_UINT32 POGLEffect::GetUID() const
{
	return mUID;
}

GLuint POGLEffect::GetProgramID() const
{
	return mProgramID;
}

const POGLEffectData* POGLEffect::GetData() const
{
	return mData;
}
