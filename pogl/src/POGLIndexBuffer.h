#pragma once
#include "config.h"

class POGLRenderState;
class POGLBufferResource;
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
	GLuint GetBufferID() const;
	
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

	void* Map(POGLResourceMapType::Enum e);
	void* Map(POGL_UINT32 offset, POGL_UINT32 length, POGLResourceMapType::Enum e);
	void Unmap();

	void DrawIndexed(GLenum primitiveType);
	void DrawIndexed(GLenum primitiveType, POGL_UINT32 count);
	void DrawIndexed(GLenum primitiveType, POGL_UINT32 count, POGL_UINT32 offset);

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
	POGL_UINT32 mNumIndices;
	POGL_UINT32 mTypeSize;
	GLenum mElementType;
	GLenum mBufferUsage;
	POGLBufferResource* mResourcePtr;
};
