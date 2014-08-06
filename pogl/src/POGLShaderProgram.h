#pragma once
#include "config.h"
#include <gl/pogl.h>

class POGLShaderProgram : public IPOGLShaderProgram
{
public:
	POGLShaderProgram(GLuint shaderID, IPOGLDevice* device, POGLShaderProgramType::Enum type, GLsync initSync);
	~POGLShaderProgram();
	
	void AddRef();
	void Release();
	IPOGLDevice* GetDevice();
	POGL_HANDLE GetHandlePtr();
	void WaitSyncDriver(POGLResourceFenceType::Enum e);
	void WaitSyncDriver(POGLResourceFenceType::Enum e, POGL_UINT64 timeout);
	void WaitSyncClient(POGLResourceFenceType::Enum e);
	bool WaitSyncClient(POGLResourceFenceType::Enum e, POGL_UINT64 timeout);
	void WaitSyncClient(POGLResourceFenceType::Enum e, POGL_UINT64 timeout, IPOGLWaitSyncJob* job);

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
	GLsync mSync;
};
