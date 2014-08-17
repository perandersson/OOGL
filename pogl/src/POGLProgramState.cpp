#include "MemCheck.h"
#include "POGLProgramState.h"
#include "uniforms/POGLUniformNotFound.h"
#include "uniforms/POGLDefaultUniform.h"
#include "uniforms/POGLUniformInt32.h"
#include "uniforms/POGLUniformUInt32.h"
#include "uniforms/POGLUniformFloat.h"
#include "uniforms/POGLUniformDouble.h"
#include "uniforms/POGLUniformMat4.h"
#include "uniforms/POGLUniformSampler2D.h"
#include "POGLRenderState.h"
#include "POGLDeviceContext.h"
#include "POGLSamplerObject.h"
#include "POGLEnum.h"
#include "POGLFactory.h"

POGLProgramState::POGLProgramState(POGLProgram* program, POGLRenderState* renderState, POGLDeviceContext* context)
: mProgram(program), mDeviceContext(context)
{
	const GLuint programID = program->GetProgramID();
	const auto data = program->GetData();
	auto it = program->GetUniforms().begin();
	auto end = program->GetUniforms().end();
	for (; it != end; ++it) {
		const auto* properties = it->second.get();
		const GLint componentID = properties->componentID;
		const GLenum uniformType = properties->uniformType;

		POGLDefaultUniform* uniform = nullptr;
		switch (uniformType) {
		case GL_FLOAT:
		case GL_FLOAT_VEC2:
		case GL_FLOAT_VEC3:
		case GL_FLOAT_VEC4:
			uniform = new POGLUniformFloat(program, renderState, context, componentID);
			break;
		case GL_DOUBLE:
		case GL_DOUBLE_VEC2:
		case GL_DOUBLE_VEC3:
		case GL_DOUBLE_VEC4:
			uniform = new POGLUniformDouble(program, renderState, context, componentID);
			break;
		case GL_INT:
		case GL_INT_VEC2:
		case GL_INT_VEC3:
		case GL_INT_VEC4:
			uniform = new POGLUniformInt32(program, renderState, context, componentID);
			break;
		case GL_UNSIGNED_INT:
		case GL_UNSIGNED_INT_VEC2:
		case GL_UNSIGNED_INT_VEC3:
		case GL_UNSIGNED_INT_VEC4:
			uniform = new POGLUniformUInt32(program, renderState, context, componentID);
			break;
		case GL_FLOAT_MAT4:
			uniform = new POGLUniformMat4(program, renderState, context, componentID);
			break;
		case GL_SAMPLER_2D:
			uniform = new POGLUniformSampler2D(program, renderState, context, componentID, renderState->NextActiveTexture(), GenSamplerObject(renderState, properties));
			break;
		case GL_SAMPLER_CUBE:
			break;
		}

		// Validate so that we actually have a uniform. If this exception is thrown then implement a new POGLUniform<x> type
		if (uniform == nullptr) {
			THROW_EXCEPTION(POGLProgramException, "Unknown uniform: %s of type %d", properties->name.c_str(), uniformType);
		}

		mUniforms.insert(std::make_pair(properties->name, uniform));
	}
	
}

POGLProgramState::~POGLProgramState()
{
	auto it = mUniforms.begin();
	auto end = mUniforms.end();
	for (; it != end; ++it) {
		delete it->second;
	}
}

IPOGLUniform* POGLProgramState::FindUniformByName(const POGL_CHAR* name)
{
	return FindUniformByName(POGL_STRING(name));
}

IPOGLUniform* POGLProgramState::FindUniformByName(const POGL_STRING& name)
{
	IPOGLUniform* uniform = nullptr;
	auto it = mUniforms.find(name);
	if (it == mUniforms.end()) {
		static POGLUniformNotFound uniformNotFound;
		return &uniformNotFound;
	}
	return it->second;
}

void POGLProgramState::ApplyUniforms()
{
	auto it = mUniforms.begin();
	auto end = mUniforms.end();
	for (; it != end; ++it) {
		it->second->Apply();
	}

	CHECK_GL("Could not apply uniforms");
}

POGLSamplerObject* POGLProgramState::GenSamplerObject(POGLRenderState* renderState, const POGLUniformProperty* uniformProperty)
{
	const GLuint samplerID = POGLFactory::GenSamplerID();
	POGLSamplerObject* samplerObject = new POGLSamplerObject(samplerID, renderState);

	glSamplerParameteri(samplerID, GL_TEXTURE_MIN_FILTER, POGLEnum::Convert(uniformProperty->minFilter));
	glSamplerParameteri(samplerID, GL_TEXTURE_MAG_FILTER, POGLEnum::Convert(uniformProperty->magFilter));
	glSamplerParameteri(samplerID, GL_TEXTURE_WRAP_S, POGLEnum::Convert(uniformProperty->wrap[0]));
	glSamplerParameteri(samplerID, GL_TEXTURE_WRAP_T, POGLEnum::Convert(uniformProperty->wrap[1]));
	glSamplerParameteri(samplerID, GL_TEXTURE_WRAP_R, POGLEnum::Convert(uniformProperty->wrap[2]));
	glSamplerParameteri(samplerID, GL_TEXTURE_COMPARE_FUNC, POGLEnum::Convert(uniformProperty->compareFunc));
	glSamplerParameteri(samplerID, GL_TEXTURE_COMPARE_MODE, POGLEnum::Convert(uniformProperty->compareMode));

	CHECK_GL("Could not set sampler parameters");
	return samplerObject;
}
