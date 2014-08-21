#include "MemCheck.h"
#include "POGLProgram.h"
#include "uniforms/POGLDefaultUniform.h"
#include "uniforms/POGLStaticUniform.h"
#include "uniforms/POGLUniformNotFound.h"
#include "uniforms/POGLUniformInt32.h"
#include "uniforms/POGLUniformUInt32.h"
#include "uniforms/POGLUniformFloat.h"
#include "uniforms/POGLUniformDouble.h"
#include "uniforms/POGLUniformMat4.h"
#include "uniforms/POGLUniformSampler2D.h"
#include "POGLRenderContext.h"
#include "POGLRenderState.h"
#include "POGLFactory.h"
#include "POGLEnum.h"
#include "POGLSamplerObject.h"
#include "POGLStringUtils.h"

namespace {
	std::atomic<POGL_UINT32> ids;
	POGL_UINT32 GenProgramUID() {
		return ++ids;
	}
}

static POGLUniformNotFound POGL_UNIFORM_NOT_FOUND;

POGLProgram::POGLProgram()
: mRefCount(1), mProgramID(0), mUID(0), mData(new POGLProgramData)
{
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
		auto globalIt = mStaticUniforms.begin();
		auto globalEnd = mStaticUniforms.end();
		for (; globalIt != globalEnd; ++globalIt) {
			delete globalIt->second;
		}

		auto it = mUniforms.begin();
		auto end = mUniforms.end();
		for (; it != end; ++it) {
			delete it->second;
		}

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

void POGLProgram::PostConstruct(GLuint programID, POGLRenderContext* context)
{
	std::lock_guard<std::recursive_mutex> lock(mMutex);

	mProgramID = programID;
	const POGL_UINT32 programUID = GenProgramUID();

	POGLRenderState* renderState = context->GetRenderState();

	//
	// Prepare uniforms
	//

	GLint numUniforms = 0;
	glGetProgramiv(programID, GL_ACTIVE_UNIFORMS, &numUniforms);
	GLchar nameData[256] = { 0 };
	for (GLint uniformIndex = 0; uniformIndex < numUniforms; ++uniformIndex) {
		GLint arraySize = 0;
		GLenum uniformType = 0;
		GLsizei actualLength = 0;

		//
		// http://www.opengl.org/sdk/docs/man/xhtml/glGetActiveUniform.xml
		// 

		glGetActiveUniform(programID, uniformIndex, sizeof(nameData), &actualLength, &arraySize, &uniformType, nameData);
		nameData[actualLength] = 0;

		const POGL_STRING name = POGLStringUtils::ToString(nameData);
		const GLint componentID = glGetUniformLocation(programID, nameData);

		// Set default properties????

		POGLDefaultUniform* uniform = nullptr;
		switch (uniformType) {
		case GL_FLOAT:
		case GL_FLOAT_VEC2:
		case GL_FLOAT_VEC3:
		case GL_FLOAT_VEC4:
			uniform = new POGLUniformFloat(programUID, renderState, componentID, uniformType);
			break;
		case GL_DOUBLE:
		case GL_DOUBLE_VEC2:
		case GL_DOUBLE_VEC3:
		case GL_DOUBLE_VEC4:
			uniform = new POGLUniformDouble(programUID, renderState, componentID, uniformType);
			break;
		case GL_INT:
		case GL_INT_VEC2:
		case GL_INT_VEC3:
		case GL_INT_VEC4:
			uniform = new POGLUniformInt32(programUID, renderState, componentID, uniformType);
			break;
		case GL_UNSIGNED_INT:
		case GL_UNSIGNED_INT_VEC2:
		case GL_UNSIGNED_INT_VEC3:
		case GL_UNSIGNED_INT_VEC4:
			uniform = new POGLUniformUInt32(programUID, renderState, componentID, uniformType);
			break;
		case GL_FLOAT_MAT4:
			uniform = new POGLUniformMat4(programUID, renderState, componentID, uniformType);
			break;
		case GL_SAMPLER_2D:
			uniform = new POGLUniformSampler2D(programUID, renderState, componentID, renderState->NextActiveTexture(), GenSamplerObject(renderState), uniformType);
			break;
		case GL_SAMPLER_CUBE:
			break;
		}

		// Validate so that we actually have a uniform. If this exception is thrown then implement a new POGLUniform<x> type
		if (uniform == nullptr) {
			THROW_EXCEPTION(POGLProgramException, "Unknown uniform: %s of type 0x%x", name.c_str(), uniformType);
		}

		mUniforms.insert(std::make_pair(name, uniform));
	}

	mUID = programUID;
}

void POGLProgram::ApplyStateUniforms()
{
	auto it = mUniforms.begin();
	auto end = mUniforms.end();
	for (; it != end; ++it) {
		it->second->Apply();
	}

	CHECK_GL("Could not apply uniforms");
}

void POGLProgram::ApplyGlobalUniforms()
{
	std::lock_guard<std::recursive_mutex> lock(mMutex);

	auto it = mStaticUniforms.begin();
	auto end = mStaticUniforms.end();
	for (; it != end; ++it) {
		auto global = it->second;
		global->Apply();
	}

	CHECK_GL("Could not apply default uniforms");
}

POGLSamplerObject* POGLProgram::GenSamplerObject(POGLRenderState* renderState)
{
	const GLuint samplerID = POGLFactory::GenSamplerID();
	POGLSamplerObject* samplerObject = new POGLSamplerObject(samplerID, renderState);

	glSamplerParameteri(samplerID, GL_TEXTURE_MIN_FILTER, POGLEnum::Convert(POGLMinFilter::DEFAULT));
	glSamplerParameteri(samplerID, GL_TEXTURE_MAG_FILTER, POGLEnum::Convert(POGLMagFilter::DEFAULT));
	glSamplerParameteri(samplerID, GL_TEXTURE_WRAP_S, POGLEnum::Convert(POGLTextureWrap::DEFAULT));
	glSamplerParameteri(samplerID, GL_TEXTURE_WRAP_T, POGLEnum::Convert(POGLTextureWrap::DEFAULT));
	glSamplerParameteri(samplerID, GL_TEXTURE_WRAP_R, POGLEnum::Convert(POGLTextureWrap::DEFAULT));
	glSamplerParameteri(samplerID, GL_TEXTURE_COMPARE_FUNC, POGLEnum::Convert(POGLCompareFunc::DEFAULT));
	glSamplerParameteri(samplerID, GL_TEXTURE_COMPARE_MODE, POGLEnum::Convert(POGLCompareMode::DEFAULT));

	CHECK_GL("Could not set sampler parameters");
	return samplerObject;
}

IPOGLUniform* POGLProgram::FindStateUniformByName(const POGL_CHAR* name)
{
	return FindStateUniformByName(POGL_STRING(name));
}

IPOGLUniform* POGLProgram::FindStateUniformByName(const POGL_STRING& name)
{
	std::lock_guard<std::recursive_mutex> lock(mMutex);

	auto it = mUniforms.find(name);
	if (it == mUniforms.end()) {
		return &POGL_UNIFORM_NOT_FOUND;
	}
	return it->second;
}

IPOGLUniform* POGLProgram::FindUniformByName(const POGL_CHAR* name)
{
	std::lock_guard<std::recursive_mutex> lock(mMutex);

	auto it = mStaticUniforms.find(POGL_STRING(name));
	if (it == mStaticUniforms.end()) {
		auto uniform = FindStateUniformByName(name);
		if (uniform != &POGL_UNIFORM_NOT_FOUND) {
			POGLDefaultUniform* defaultUniform = static_cast<POGLDefaultUniform*>(uniform);
			POGLStaticUniform* staticUniform = new POGLStaticUniform(defaultUniform, defaultUniform->GetUniformType());
			mStaticUniforms.insert(std::make_pair(POGL_STRING(name), staticUniform));
			uniform = staticUniform;
		}
		return uniform;
	}
	return it->second;
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

void POGLProgram::SetFrontFace(POGLFrontFace::Enum e)
{
	std::lock_guard<std::recursive_mutex> lock(mMutex);
	mData->frontFace = e;
}

POGLFrontFace::Enum POGLProgram::GetFrontFace()
{
	std::lock_guard<std::recursive_mutex> lock(mMutex);
	return mData->frontFace;
}

void POGLProgram::SetCullFace(POGLCullFace::Enum e)
{
	std::lock_guard<std::recursive_mutex> lock(mMutex);
	mData->cullFace = e;
}

POGLCullFace::Enum POGLProgram::GetCullFace()
{
	std::lock_guard<std::recursive_mutex> lock(mMutex);
	return mData->cullFace;
}

void POGLProgram::CopyProgramData(POGLProgramData* _out_Data)
{
	assert_not_null(_out_Data);
	std::lock_guard<std::recursive_mutex> lock(mMutex);
	*_out_Data = *mData;
}
