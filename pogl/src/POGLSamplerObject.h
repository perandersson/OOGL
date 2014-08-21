#pragma once
#include "config.h"
#include <gl/pogl.h>

class POGLRenderState;
class POGLAPI POGLSamplerObject
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
	POGL_UID mUID;
	GLuint mSamplerID;
	POGLRenderState* mRenderState;
};
