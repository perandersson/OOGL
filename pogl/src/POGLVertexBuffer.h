#pragma once
#include "config.h"

class POGLRenderState;
class POGLIndexBuffer;
class POGLBufferResource;
class POGLVertexBuffer : public IPOGLVertexBuffer
{
public:
	POGLVertexBuffer(POGL_UINT32 count, const POGL_VERTEX_LAYOUT* layout, GLenum primitiveType, GLenum bufferUsage);
	~POGLVertexBuffer();
	
	/*!
		\brief Initializes the OpenGL specific functionality for this buffer.

		\param renderState
	*/
	void PostConstruct(POGLRenderState* renderState);

	/*!
		\brief Retrieves a unique ID for this vertex buffer
	*/
	inline POGL_UINT32 GetUID() const {
		return mUID;
	}
	
	/*!
		\brief Retrieves the OpenGL Vertex Array Object ID for this object
	*/
	inline GLuint GetVAOID() const {
		return mVAOID;
	}

	void* Map(POGLResourceMapType::Enum e);
	void* Map(POGL_UINT32 offset, POGL_UINT32 length, POGLResourceMapType::Enum e);
	void Unmap();

	void Draw();
	void Draw(POGL_UINT32 count);
	void Draw(POGL_UINT32 count, POGL_UINT32 offset);

	void DrawIndexed(POGLIndexBuffer* indexBuffer);
	void DrawIndexed(POGLIndexBuffer* indexBuffer, POGL_UINT32 count);
	void DrawIndexed(POGLIndexBuffer* indexBuffer, POGL_UINT32 count, POGL_UINT32 offset);
	
// IPOGLInterface
public:
	virtual void AddRef();
	virtual void Release();

// IPOGLResource
public:
	virtual POGLResourceType::Enum GetType() const;

// IPOGLVertexBuffer
public:
	virtual const POGL_VERTEX_LAYOUT* GetLayout() const;
	virtual POGL_UINT32 GetCount() const;

private:
	REF_COUNTER mRefCount;
	POGL_UID mUID;
	GLuint mBufferID;
	POGL_UINT32 mCount;
	GLuint mVAOID;
	const POGL_VERTEX_LAYOUT* mLayout;
	GLenum mPrimitiveType;
	GLenum mBufferUsage;
	POGLBufferResource* mResourcePtr;
};
