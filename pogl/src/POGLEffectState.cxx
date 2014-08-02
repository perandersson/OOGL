#include "POGLEffectState.hxx"
#include "uniforms/POGLUniformNotFound.hxx"
#include "uniforms/POGLDefaultUniform.hxx"
#include "uniforms/POGLUniformInt32.hxx"
#include "uniforms/POGLUniformUInt32.hxx"

POGLEffectState::POGLEffectState(POGLEffect* effect, IPOGLDeviceContext* context)
: mEffect(effect), mDeviceContext(context)
{

}

POGLEffectState::~POGLEffectState()
{
	auto it = mUniforms.begin();
	auto end = mUniforms.end();
	for (; it != end; ++it) {
		delete it->second;
	}
}

IPOGLUniform* POGLEffectState::FindUniformByName(const POGL_CHAR* name)
{
	return FindUniformByName(POGL_STRING(name));
}

IPOGLUniform* POGLEffectState::FindUniformByName(const POGL_STRING& name)
{
	IPOGLUniform* uniform = nullptr;
	auto it = mUniforms.find(name);
	if (it == mUniforms.end()) {
		static POGLUniformNotFound uniformNotFound;
		return &uniformNotFound;
	}
	return it->second;
}

void POGLEffectState::ApplyUniforms()
{
	auto it = mUniforms.begin();
	auto end = mUniforms.end();
	for (; it != end; ++it) {
		it->second->Apply();
	}

#if defined(_DEBUG)
	const GLenum err = glGetError();
	assert_with_message(err == GL_NO_ERROR, "Could not apply uniforms");
#endif
}
