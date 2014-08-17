#pragma once
#include "config.h"
#include "POGLProgramData.h"
#include <mutex>
#include <memory>

struct POGLUniformProperty; 
class POGLProgram : public IPOGLProgram
{
public:
	POGLProgram(GLuint programID, POGLProgramData* data, std::hash_map<POGL_STRING, std::shared_ptr<POGLUniformProperty>> uniforms);
	virtual ~POGLProgram();
	
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
	*/
	void CopyProgramData(POGLProgramData* in);

	/*!
		\brief Retrieves the data

		\return
	*/
	inline const POGLProgramData* GetData() const {
		return mData;
	}
	
	/*!
		\brief Retrieves the uniforms for this effect
	*/
	inline std::hash_map<POGL_STRING, std::shared_ptr<POGLUniformProperty>>& GetUniforms() {
		return mUniforms;
	}

	bool GetDepthTest();
	void SetDepthTest(bool b);
	void SetDepthFunc(POGLDepthFunc::Enum depthFunc);
	POGLDepthFunc::Enum GetDepthFunc();
	bool GetDepthMask();
	void SetDepthMask(bool b);
	void SetColorMask(POGL_UINT8 colorMask);
	POGL_UINT8 GetColorMask();
	bool GetStencilTest();
	void SetStencilTest(bool b);
	void SetBlendFunc(POGLSrcFactor::Enum sfactor, POGLDstFactor::Enum dfactor);
	void SetBlend(bool b);

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
	std::hash_map<POGL_STRING, std::shared_ptr<POGLUniformProperty>> mUniforms;
};
