#include "MemCheck.h"
#include "POGLProgram.h"

namespace {
	std::atomic<POGL_UINT32> ids;
	POGL_UINT32 GenProgramUID() {
		return ++ids;
	}
}

POGLProgram::POGLProgram(GLuint programID, POGLProgramData* data, std::hash_map<POGL_STRING, std::shared_ptr<POGLUniformProperty>> uniforms)
: mRefCount(1), mProgramID(programID), mUID(GenProgramUID()), mData(data), mUniforms(uniforms)
{
	assert_not_null(data);
}

POGLProgram::~POGLProgram()
{

}

void POGLProgram::AddRef()
{
	mRefCount++;
}

void POGLProgram::Release()
{
	if (--mRefCount == 0) {
		if (mProgramID != 0) {
			glDeleteProgram(mProgramID);
			mProgramID = 0;
		}
		if (mData != nullptr) {
			delete mData;
			mData = nullptr;
		}
		delete this;
	}
}

POGLResourceType::Enum POGLProgram::GetType() const
{
	return POGLResourceType::PROGRAM;
}

bool POGLProgram::GetDepthTest()
{
	std::lock_guard<std::recursive_mutex> lock(mMutex);
	return mData->depthTest;
}

void POGLProgram::SetDepthTest(bool b)
{
	std::lock_guard<std::recursive_mutex> lock(mMutex);
	mData->depthTest = b;
}

void POGLProgram::SetDepthFunc(POGLDepthFunc::Enum depthFunc)
{
	std::lock_guard<std::recursive_mutex> lock(mMutex);
	mData->depthFunc = depthFunc;
}

POGLDepthFunc::Enum POGLProgram::GetDepthFunc()
{
	std::lock_guard<std::recursive_mutex> lock(mMutex);
	return mData->depthFunc;
}

bool POGLProgram::GetDepthMask()
{
	std::lock_guard<std::recursive_mutex> lock(mMutex);
	return mData->depthMask;
}

void POGLProgram::SetDepthMask(bool b)
{
	std::lock_guard<std::recursive_mutex> lock(mMutex);
	mData->depthMask = b;
}

POGL_UINT8 POGLProgram::GetColorMask()
{
	std::lock_guard<std::recursive_mutex> lock(mMutex);
	return mData->colorMask;
}

void POGLProgram::SetColorMask(POGL_UINT8 colorMask)
{
	std::lock_guard<std::recursive_mutex> lock(mMutex);
	mData->colorMask = colorMask;
}

bool POGLProgram::GetStencilTest()
{
	std::lock_guard<std::recursive_mutex> lock(mMutex);
	return mData->stencilTest;
}

void POGLProgram::SetStencilTest(bool b)
{
	std::lock_guard<std::recursive_mutex> lock(mMutex);
	mData->stencilTest = b;
}

POGL_UINT32 POGLProgram::GetStencilMask()
{
	std::lock_guard<std::recursive_mutex> lock(mMutex);
	return mData->stencilMask;
}

void POGLProgram::SetStencilMask(POGL_UINT32 mask)
{
	std::lock_guard<std::recursive_mutex> lock(mMutex);
	mData->stencilMask = mask;
}

void POGLProgram::SetBlendFunc(POGLSrcFactor::Enum sfactor, POGLDstFactor::Enum dfactor)
{
	std::lock_guard<std::recursive_mutex> lock(mMutex);
	mData->srcFactor = sfactor;
	mData->dstFactor = dfactor;
}

void POGLProgram::SetBlend(bool b)
{
	std::lock_guard<std::recursive_mutex> lock(mMutex);
	mData->blending = b;
}

void POGLProgram::CopyProgramData(POGLProgramData* in)
{
	assert_not_null(in);
	std::lock_guard<std::recursive_mutex> lock(mMutex);
	// Copy!!
	*in = *mData;
}
