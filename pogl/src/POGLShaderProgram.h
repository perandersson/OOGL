#pragma once
#include "config.h"

class POGLShaderProgram : public IPOGLShaderProgram
{
public:
	POGLShaderProgram(GLuint shaderID, POGLShaderProgramType::Enum type);
	~POGLShaderProgram();
	
	/*!
		\brief Retrieves a unique ID for this vertex buffer
	*/
	inline POGL_UINT32 GetUID() const {
		return mUID;
	}

	/*!
		\brief Retrieves the OpenGL Shader ID
	*/
	inline GLuint GetShaderID() const {
		return mShaderID;
	}

	/*!
		\brief Retrieves this programs type
	*/
	inline POGLShaderProgramType::Enum GetType() const {
		return mType;
	}
	
// IPOGLInterface
public:
	virtual void AddRef();
	virtual void Release();

// IPOGLResource
public:
	virtual POGLResourceType::Enum GetResourceType() const;

private:
	std::atomic<POGL_UINT32> mRefCount;
	POGL_UINT32 mUID;
	GLuint mShaderID;
	POGLShaderProgramType::Enum mType;
};
