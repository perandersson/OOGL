#pragma once
#include "POGLRenderState.hxx"

class POGLDeviceContext : public IPOGLDeviceContext
{
public:
	POGLDeviceContext(IPOGLDevice* device);
	~POGLDeviceContext();

	void AddRef();
	void Release();

	IPOGLDevice* GetDevice();
	IPOGLShaderProgram* CreateShaderProgramFromFile(const POGL_CHAR* path, POGLShaderProgramType::Enum type);
	IPOGLShaderProgram* CreateShaderProgramFromMemory(const POGL_CHAR* memory, POGL_UINT32 size, POGLShaderProgramType::Enum type);
	IPOGLEffect* CreateEffectFromFile(const POGL_CHAR* path);
	IPOGLEffect* CreateEffectFromMemory(const POGL_CHAR* memory, POGL_UINT32 size);
	IPOGLEffect* CreateEffectFromPrograms(IPOGLShaderProgram** programs, POGL_UINT32 numPrograms);
	IPOGLTexture1D* CreateTexture1D();
	IPOGLTexture2D* CreateTexture2D();
	IPOGLTexture3D* CreateTexture3D();
	IPOGLRenderState* Apply(IPOGLEffect* effect);
	
	/*!
		\brief

		\param
		\return
	*/
	bool Initialize();
	
	/*!
		\brief Load the extensions for this device context
	*/
	virtual void LoadExtensions();

protected:
	POGL_UINT32 mRefCount;
	IPOGLDevice* mDevice;
	POGLRenderState* mRenderState;
};
