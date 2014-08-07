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
		\brief Draw this vertex buffer
	*/
	void Draw(POGL_UINT32 startIndex);
	
	/*!
		\brief Draw this vertex buffer
	*/
	void Draw(POGL_UINT32 startIndex, POGL_UINT32 count);
	
	/*!
		\brief Draw this vertex buffer
	*/
	void Draw(POGLIndexBuffer* indexBuffer, POGL_UINT32 startIndex);

	/*!
		\brief Draw this vertex buffer
	*/
	void Draw(POGLIndexBuffer* indexBuffer, POGL_UINT32 startIndex, POGL_UINT32 count);
	
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
