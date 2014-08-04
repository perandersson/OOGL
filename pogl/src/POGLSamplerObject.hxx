#pragma once
#include "config.hxx"
#include <gl/pogl.hxx>

class POGLRenderState;
class POGLSamplerObject
{
public:
	POGLSamplerObject(GLuint samplerID, POGLRenderState* renderState);
	~POGLSamplerObject();

	/*!
		\brief Retrieves a unique ID for this vertex buffer
	*/
	POGL_UINT32 GetUID() const;

	/*!
		\brief Retrieves the OpenGL Buffer ID
	*/
	GLuint GetSamplerID() const;

private:
	POGL_UINT32 mUID;
	GLuint mSamplerID;
	POGLRenderState* mRenderState;
};
