#pragma once
#include "config.h"

class POGLRenderState;
class POGLIndexBuffer : public IPOGLIndexBuffer
{
public:
	POGLIndexBuffer(POGL_UINT32 typeSize, POGL_UINT32 numIndices, GLenum type, GLenum bufferUsage);
	~POGLIndexBuffer();

	/*!
		\brief Method called after this buffer's construction is completed
	*/
	void PostConstruct(POGLRenderState* renderState);
	
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
	inline GLenum GetElementType() const {
		return mElementType;
	}

	void Draw(GLenum primitiveType);
	void Draw(GLenum primitiveType, POGL_UINT32 count);
	void Draw(GLenum primitiveType, POGL_UINT32 count, POGL_UINT32 offset);

// IPOGLInterface
public:
	virtual void AddRef();
	virtual void Release();

// IPOGLResource
public:
	virtual POGLResourceType::Enum GetType() const;

// IPOGLIndexBuffer
public:
	virtual POGL_UINT32 GetCount() const;

private:
	REF_COUNTER mRefCount;
	POGL_UID mUID;
	GLuint mBufferID;
	POGL_UINT32 mNumIndices;
	POGL_UINT32 mTypeSize;
	GLenum mElementType;
	GLenum mBufferUsage;
};
