#pragma once
#include "config.hxx"
#include <gl/pogl.hxx>
#include <mutex>

struct POGLEffectData;
class POGLEffect : public IPOGLEffect
{
public:
	POGLEffect(GLuint programID, POGLEffectData* data, IPOGLDevice* device);
	~POGLEffect();

	void AddRef();
	void Release();
	IPOGLDevice* GetDevice();
	POGL_HANDLE GetHandlePtr();
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
};
