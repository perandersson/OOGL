#pragma once
#include "POGLProgramData.h"
#include <mutex>
#include <memory>

struct POGLUniformProperty;
class POGLDefaultUniform;
class POGLRenderContext;
class POGLRenderState;
class POGLSamplerObject;
class POGLGlobalUniform;
class POGLProgram : public IPOGLProgram
{
	typedef std::hash_map<POGL_STRING, POGLDefaultUniform*> Uniforms;
	typedef std::hash_map<POGL_STRING, POGLGlobalUniform*> GlobalUniforms;

public:
	POGLProgram();
	virtual ~POGLProgram();

	/*!
		\brief Method called after the program is constructed
		
		This method is only allowed to be invoked from the main thread.

		\param programID
				The OpenGL program ID
		\param context
				The immediately POGLDeviceContext
	*/
	void PostConstruct(GLuint programID, POGLRenderContext* context);
	
	/*!
		\brief Retrieves a unique ID for this effect

		\return The unique ID
	*/
	inline POGL_UINT32 GetUID() const {
		return mUID;
	}

	/*!
		\brief Retrieves the OpenGL program ID
	*/
	inline GLuint GetProgramID() const {
		return mProgramID;
	}
	
	/*!
		\brief Copy the effect data to the supplied instance

		\param _out_Data
				The target instance we want to set the program data to
	*/
	void CopyProgramData(POGLProgramData* _out_Data);

	/*!
		\brief Apply all default uniform properties
	*/
	void ApplyStateUniforms();

	/*!
		\brief Apply the global uniform properties
	*/
	void ApplyGlobalUniforms();
	
	/*!
		\brief Generate a sampler object based on the supplied property

		\param renderState
		\param property
					The uniform property
	*/
	POGLSamplerObject* GenSamplerObject(POGLRenderState* renderState);

	/*!
		\brief Retrieves a uniform based on the given name
	*/
	IPOGLUniform* FindStateUniformByName(const POGL_CHAR* name);

// IPOGLProgram
public:
	virtual IPOGLUniform* FindUniformByName(const POGL_CHAR* name);
	virtual bool GetDepthTest();
	virtual void SetDepthTest(bool b);
	virtual void SetDepthFunc(POGLDepthFunc::Enum depthFunc);
	virtual POGLDepthFunc::Enum GetDepthFunc();
	virtual bool GetDepthMask();
	virtual void SetDepthMask(bool b);
	virtual void SetColorMask(POGL_UINT8 colorMask);
	virtual POGL_UINT8 GetColorMask();
	virtual bool GetStencilTest();
	virtual void SetStencilTest(bool b);
	virtual POGL_UINT32 GetStencilMask();
	virtual void SetStencilMask(POGL_UINT32 mask);
	virtual void SetBlendFunc(POGLSrcFactor::Enum sfactor, POGLDstFactor::Enum dfactor);
	virtual void SetBlend(bool b);
	virtual void SetFrontFace(POGLFrontFace::Enum e);
	virtual POGLFrontFace::Enum GetFrontFace();
	virtual void SetCullFace(POGLCullFace::Enum e);
	virtual POGLCullFace::Enum GetCullFace();

// IPOGLInterface
public:
	virtual void AddRef();
	virtual void Release();

// IPOGLResource
public:
	virtual POGLResourceType::Enum GetType() const;

private:
	REF_COUNTER mRefCount;
	GLuint mProgramID;
	POGL_UINT32 mUID;
	std::recursive_mutex mMutex;
	POGLProgramData* mData;

	Uniforms mUniforms;
	GlobalUniforms mGlobalUniforms;
};
