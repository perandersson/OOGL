#pragma once
#include "config.h"
#include <gl/pogl.h>

class POGLSyncObject;
class POGLDeviceContext;
class POGLRenderState;
class POGLIndexBuffer : public IPOGLIndexBuffer
{
public:
	POGLIndexBuffer(GLuint bufferID, POGL_UINT32 typeSize, POGL_UINT32 numIndices, GLenum type, GLenum bufferUsage, POGLSyncObject* syncObject, IPOGLDevice* device);
	~POGLIndexBuffer();
	
	/*!
		\brief Retrieves a unique ID for this object

		\return
	*/
	inline POGL_UINT32 GetUID() const {
		return mUID;
	}

	/*!
		\brief Retrieves the OpenGL Buffer ID for this object

		\return
	*/
	inline GLuint GetBufferID() const {
		return mBufferID;
	}

	/*!
		\brief Retrieves the size of one element

		\return
	*/
	inline POGL_UINT32 GetTypeSize() const {
		return mTypeSize;
	}

	/*!
		\brief Retrieves the value type 

		\return
	*/
	inline GLenum GetType() const {
		return mType;
	}
	
	/*!
		\brief 
	*/
	void* Map(POGLDeviceContext* context, POGLRenderState* state, GLenum access);
	
	/*!
		\brief 
	*/
	void* MapRange(POGLDeviceContext* context, POGLRenderState* state, POGL_UINT32 offset, POGL_UINT32 length, GLbitfield access);
	
	/*!
		\brief 
	*/
	void Unmap(POGLDeviceContext* context, POGLRenderState* state);

// IPOGLInterface
public:
	void AddRef();
	void Release();

// IPOGLResource
public:
	IPOGLDevice* GetDevice();
	POGL_HANDLE GetHandlePtr();
	void WaitSyncDriver();
	void WaitSyncClient();
	bool WaitSyncClient(POGL_UINT64 timeout);
	bool WaitSyncClient(POGL_UINT64 timeout, IPOGLWaitSyncJob* job);
	POGLResourceType::Enum GetResourceType() const;

// IPOGLIndexBuffer
public:
	POGL_UINT32 GetNumElements() const;

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
