#pragma once
#include "config.h"
#include <gl/pogl.h>

class POGLIndexBuffer : public IPOGLIndexBuffer
{
public:
	POGLIndexBuffer(GLuint bufferID, POGL_UINT32 typeSize, POGL_UINT32 numIndices, GLenum type, GLenum bufferUsage, IPOGLDevice* device);
	~POGLIndexBuffer();

	void AddRef();
	void Release();
	IPOGLDevice* GetDevice();
	POGL_HANDLE GetHandlePtr();
	POGL_UINT32 GetNumElements() const;

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

private:
	POGL_UINT32 mRefCount;
	POGL_UINT32 mUID;
	GLuint mBufferID;
	POGL_UINT32 mNumIndices;
	POGL_UINT32 mTypeSize;
	GLenum mType;
	GLenum mBufferUsage;
	IPOGLDevice* mDevice;
};
