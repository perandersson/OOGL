#pragma once
#include "POGLEffect.h"

class POGLDefaultUniform;
class POGLRenderState;
class POGLDeviceContext;
class POGLSamplerObject;
struct POGLUniformProperty;

class POGLEffectState
{
	typedef std::hash_map<POGL_STRING, POGLDefaultUniform*> Uniforms;

public:
	POGLEffectState(POGLEffect* effect, POGLRenderState* renderState, POGLDeviceContext* context);
	~POGLEffectState();
	
	/*!
		\brief Retrieves a uniform by the given name

		\param name
		\return
	*/
	IPOGLUniform* FindUniformByName(const POGL_STRING& name);

	/*!
		\brief Retrieves a uniform by the given name

		\param name
		\return
	*/
	IPOGLUniform* FindUniformByName(const POGL_CHAR* name);

	/*!
		\brief Apply the effect state's uniforms
	*/
	void ApplyUniforms();

private:
	/*!
		\brief Generate a sampler object based on the supplied property

		\param renderState
		\param property
					The uniform property
	*/
	POGLSamplerObject* GenSamplerObject(POGLRenderState* renderState, const POGLUniformProperty* uniformProperty);


private:
	POGLEffect* mEffect;
	POGLDeviceContext* mDeviceContext;
	Uniforms mUniforms;
};
