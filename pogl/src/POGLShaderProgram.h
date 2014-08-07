#pragma once
#include "config.h"
#include <gl/pogl.h>

class POGLSyncObject;
class POGLShaderProgram : public IPOGLShaderProgram
{
public:
	POGLShaderProgram(GLuint shaderID, IPOGLDevice* device, POGLShaderProgramType::Enum type);
	~POGLShaderProgram();
	
	void AddRef();
	void Release();

	/*!
		\brief Retrieves a unique ID for this vertex buffer
	*/
	POGL_UINT32 GetUID() const;

	/*!
		\brief Retrieves the OpenGL Shader ID
	*/
	GLuint GetShaderID() const;

	/*!
		\brief Retrieves this programs type
	*/
	POGLShaderProgramType::Enum GetType() const;
	
private:
	POGL_UINT32 mRefCount;
	POGL_UINT32 mUID;
	GLuint mShaderID;
	IPOGLDevice* mDevice;
	POGLShaderProgramType::Enum mType;
};
