#pragma once
#include "config.h"

class POGLIndexBuffer;
class POGLVertexBuffer : public IPOGLVertexBuffer
{
public:
	POGLVertexBuffer(POGL_UINT32 count, const POGL_VERTEX_LAYOUT* layout, GLenum primitiveType, GLenum bufferUsage);
	~POGLVertexBuffer();
	
	/*!
		\brief Retrieves a unique ID for this vertex buffer
	*/
	inline POGL_UINT32 GetUID() const {
		return mUID;
	}
	
	/*!
		\brief Retrieves the OpenGL Buffer ID for this object
	*/
	inline GLuint GetBufferID() const {
		return mBufferID;
	}
	
	/*!
		\brief Retrieves the OpenGL Vertex Array Object ID for this object
	*/
	inline GLuint GetVAOID() const {
		return mVAOID;
	}

	/*!
		\brief How this buffer is used internally
	*/
	inline GLenum GetBufferUsage() const {
		return mBufferUsage;
	}

	/*!
		\brief Retrieves the primitive type 
	*/
	inline GLenum GetPrimitiveType() const {
		return mPrimitiveType;
	}

	void Draw();
	void Draw(POGL_UINT32 count);
	void Draw(POGL_UINT32 count, POGL_UINT32 offset);
	
	/*!
		\brief Set buffer-, count and vertex array object after the construction is complete.

		This method is called if this object is created in another thread and we want, after the command queue has been executed, to put the
		values into this buffer.
	*/
	void PostConstruct(GLuint bufferID, GLuint vaoID);

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
};
