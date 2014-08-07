#pragma once
#include "config.h"
#include <gl/pogl.h>

class POGLSyncObject;
class POGLIndexBuffer : public IPOGLIndexBuffer
{
public:
	POGLIndexBuffer(GLuint bufferID, POGL_UINT32 typeSize, POGL_UINT32 numIndices, GLenum type, GLenum bufferUsage, POGLSyncObject* syncObject, IPOGLDevice* device);
	~POGLIndexBuffer();

	void AddRef();
	void Release();
	IPOGLDevice* GetDevice();
	POGL_HANDLE GetHandlePtr();
	void WaitSyncDriver();
	void WaitSyncClient();
	bool WaitSyncClient(POGL_UINT64 timeout);
	bool WaitSyncClient(POGL_UINT64 timeout, IPOGLWaitSyncJob* job);

	POGL_UINT32 GetNumElements() const;
	IPOGLStream* OpenStream(POGLStreamType::Enum e);

	/*!
		\brief
	*/
	POGL_UINT32 GetUID() const;

	/*!
		\brief
	*/
	GLuint GetBufferID() const;

	/*!
		\brief Retrieves the size of one element

		\return
	*/
	inline POGL_UINT32 GetTypeSize() const {
		return mTypeSize;
	}

	/*!
		\brief Retrieves the value type 
	*/
	GLenum GetType() const;

	/*!
		\brief Retrieves the object responsible for making sure that all resources are synchronized between all the OpenGL threads
	*/
	POGLSyncObject* GetSyncObject();

private:
	POGL_UINT32 mRefCount;
	POGL_UINT32 mUID;
	GLuint mBufferID;
	POGL_UINT32 mNumIndices;
	POGL_UINT32 mTypeSize;
	GLenum mType;
	GLenum mBufferUsage;
	POGLSyncObject* mSyncObject;
	IPOGLDevice* mDevice;
};
