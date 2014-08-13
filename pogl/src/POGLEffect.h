#pragma once
#include "config.h"
#include <mutex>
#include <memory>

struct POGLEffectData;
struct POGLUniformProperty; 
class POGLEffect : public IPOGLEffect
{
public:
	POGLEffect(GLuint programID, POGLEffectData* data, std::hash_map<POGL_STRING, std::shared_ptr<POGLUniformProperty>> uniforms);
	~POGLEffect();
	
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
	void CopyEffectData(POGLEffectData* in);

	/*!
		\brief Retrieves the data

		\return
	*/
	inline const POGLEffectData* GetData() const {
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
	virtual POGLResourceType::Enum GetResourceType() const;

private:
	POGL_UINT32 mRefCount;
	GLuint mProgramID;
	IPOGLDevice* mDevice;
	POGL_UINT32 mUID;
	std::recursive_mutex mMutex;
	POGLEffectData* mData;
	std::hash_map<POGL_STRING, std::shared_ptr<POGLUniformProperty>> mUniforms;
};
