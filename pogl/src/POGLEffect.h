#pragma once
#include "config.h"
#include <gl/pogl.h>
#include <mutex>
#include <memory>

struct POGLEffectData;
struct POGLUniformProperty; 
class POGLSyncObject;
class POGLEffect : public IPOGLEffect
{
public:
	POGLEffect(GLuint programID, POGLEffectData* data, std::hash_map<POGL_STRING, std::shared_ptr<POGLUniformProperty>> uniforms, IPOGLDevice* device);
	~POGLEffect();

	virtual void AddRef();
	virtual void Release();

	virtual IPOGLDevice* GetDevice();
	virtual POGL_HANDLE GetHandlePtr();

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

	/*!
		\brief Retrieves the uniforms for this effect
	*/
	inline std::hash_map<POGL_STRING, std::shared_ptr<POGLUniformProperty>>& GetUniforms() {
		return mUniforms;
	}

	/*!
		\brief Copy the effect data to the supplied instance
	*/
	void CopyEffectData(POGLEffectData* in);

	/*!
		\brief Retrieves a unique ID for this effect

		\return The unique ID
	*/
	POGL_UINT32 GetUID() const;

	/*!
		\brief Retrieves the OpenGL program ID
	*/
	GLuint GetProgramID() const;
	
	/*!
		\brief Retrieves the data

		\return
	*/
	const POGLEffectData* GetData() const;
	
private:
	POGL_UINT32 mRefCount;
	GLuint mProgramID;
	IPOGLDevice* mDevice;
	POGL_UINT32 mUID;
	std::recursive_mutex mMutex;
	POGLEffectData* mData;
	std::hash_map<POGL_STRING, std::shared_ptr<POGLUniformProperty>> mUniforms;
};
