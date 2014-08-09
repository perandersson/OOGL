#pragma once
#include "config.h"
#include <gl/pogl.h>

class POGLSyncObject;
class POGLDeviceContext;
class POGLRenderState;
class POGLVertexBuffer;
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
		\brief Retrieves how this buffer is used internally by OpenGL
	*/
	inline GLenum GetBufferUsage() const {
		return mBufferUsage;
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
		\brief Retrieves the object responsible for synchronizing this object between contexts
	*/
	inline POGLSyncObject* GetSyncObject() {
		return mSyncObject;
	}
	
	void Draw(POGLDeviceContext* context, POGLVertexBuffer* vertexBuffer, GLenum primitiveType, POGL_UINT32 startIndex);
	void Draw(POGLDeviceContext* context, POGLVertexBuffer* vertexBuffer, GLenum primitiveType, POGL_UINT32 startIndex, POGL_UINT32 count);

// IPOGLInterface
public:
	void AddRef();
	void Release();

// IPOGLResource
public:
	IPOGLDevice* GetDevice();
	POGL_HANDLE GetHandlePtr();
	void WaitSyncDriver(IPOGLDeviceContext* context);
	void WaitSyncClient(IPOGLDeviceContext* context);
	bool WaitSyncClient(IPOGLDeviceContext* context, POGL_UINT64 timeout);
	bool WaitSyncClient(IPOGLDeviceContext* context, POGL_UINT64 timeout, IPOGLWaitSyncJob* job);

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
