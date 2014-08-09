#pragma once
#include "config.h"
#include <gl/pogl.h>

class POGLIndexBuffer;
class POGLSyncObject; 
class POGLDeviceContext;
class POGLRenderState;
class POGLVertexBuffer : public IPOGLVertexBuffer
{
public:
	POGLVertexBuffer(GLuint bufferID, const POGL_VERTEX_LAYOUT* layout, POGL_UINT32 numVertices, GLenum primitiveType, GLenum bufferUsage, POGLSyncObject* syncObject, IPOGLDevice* device);
	~POGLVertexBuffer();
	
	/*!
		\brief Retrieves a unique ID for this vertex buffer
	*/
	inline POGL_UINT32 GetUID() const {
		return mUID;
	}

	/*!
		\brief Retrieves the OpenGL Buffer ID
	*/
	inline GLuint GetBufferID() const {
		return mBufferID;
	}

	/*!
		\brief How this buffer is used internally
	*/
	inline GLenum GetBufferUsage() const {
		return mBufferUsage;
	}

	/*!
		\brief Retrieves the object responsible for synchronizing this object between contexts
	*/
	inline POGLSyncObject* GetSyncObject() {
		return mSyncObject;
	}
	
	/*!
		\brief Draw this vertex buffer
	*/
	void Draw(POGLDeviceContext* context, POGL_UINT32 startIndex);
	
	/*!
		\brief Draw this vertex buffer
	*/
	void Draw(POGLDeviceContext* context, POGL_UINT32 startIndex, POGL_UINT32 count);
	
	/*!
		\brief Draw this vertex buffer
	*/
	void Draw(POGLDeviceContext* context, POGLIndexBuffer* indexBuffer, POGL_UINT32 startIndex);

	/*!
		\brief Draw this vertex buffer
	*/
	void Draw(POGLDeviceContext* context, POGLIndexBuffer* indexBuffer, POGL_UINT32 startIndex, POGL_UINT32 count);

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
	//POGLResourceType::Enum GetResourceType() const;

// IPOGLVertexBuffer
public:
	inline const POGL_VERTEX_LAYOUT* GetLayout() const {
		return mLayout;
	}

	POGL_UINT32 GetNumVertices() const;

private:
	POGL_UINT32 mRefCount;
	POGL_UINT32 mUID;
	GLuint mBufferID;
	const POGL_VERTEX_LAYOUT* mLayout;
	POGL_UINT32 mNumVertices;
	GLenum mPrimitiveType;
	GLenum mBufferUsage;
	POGLSyncObject* mSyncObject;
	IPOGLDevice* mDevice;
};
