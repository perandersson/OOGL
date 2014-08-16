#pragma once
#include "config.h"

class POGLVertexBuffer;
class POGLIndexBuffer : public IPOGLIndexBuffer
{
public:
	POGLIndexBuffer(GLuint bufferID, POGL_UINT32 typeSize, POGL_UINT32 numIndices, GLenum type, GLenum bufferUsage);
	~POGLIndexBuffer();
	
	/*!
		\brief Retrieves a unique ID for this object
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
	
	void Draw(POGLVertexBuffer* vertexBuffer, GLenum primitiveType, POGL_UINT32 startIndex);
	void Draw(POGLVertexBuffer* vertexBuffer, GLenum primitiveType, POGL_UINT32 startIndex, POGL_UINT32 count);

// IPOGLInterface
public:
	virtual void AddRef();
	virtual void Release();

// IPOGLResource
public:
	virtual POGLResourceType::Enum GetResourceType() const;

// IPOGLIndexBuffer
public:
	virtual POGL_UINT32 GetCount() const;

private:
	REF_COUNTER mRefCount;
	POGL_UINT32 mUID;
	GLuint mBufferID;
	POGL_UINT32 mNumIndices;
	POGL_UINT32 mTypeSize;
	GLenum mType;
	GLenum mBufferUsage;
};
