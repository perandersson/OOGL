#pragma once
#include "POGLEffect.hxx"

class POGLDefaultUniform;
class POGLEffectState
{
	typedef std::hash_map<POGL_STRING, POGLDefaultUniform*> Uniforms;

public:
	POGLEffectState(POGLEffect* effect, IPOGLDeviceContext* context);
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
	POGLEffect* mEffect;
	IPOGLDeviceContext* mDeviceContext;
	Uniforms mUniforms;
};
