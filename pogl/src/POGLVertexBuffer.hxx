#pragma once
#include "config.hxx"
#include <gl/pogl.hxx>

class POGLIndexBuffer;
class POGLVertexBuffer : public IPOGLVertexBuffer
{
public:
	POGLVertexBuffer(GLuint bufferID, const POGL_VERTEX_LAYOUT* layout, POGL_UINT32 numVertices, GLenum primitiveType, GLenum bufferUsage, IPOGLDevice* device);
	~POGLVertexBuffer();

	void AddRef();
	void Release();
	IPOGLDevice* GetDevice();
	POGL_HANDLE GetHandlePtr();
	const POGL_VERTEX_LAYOUT* GetLayout() const;
	POGL_UINT32 GetNumVertices() const;

	/*!
		\brief Retrieves a unique ID for this vertex buffer
	*/
	POGL_UINT32 GetUID() const;

	/*!
		\brief Retrieves the OpenGL Buffer ID
	*/
	GLuint GetBufferID() const;

	/*!
		\brief 
	*/
	void Draw(POGL_UINT32 startIndex);
	
	/*!
		\brief 
	*/
	void Draw(POGL_UINT32 startIndex, POGL_UINT32 count);
	
	/*!
		\brief 
	*/
	void Draw(POGLIndexBuffer* indexBuffer, POGL_UINT32 startIndex);

	/*!
		\brief 
	*/
	void Draw(POGLIndexBuffer* indexBuffer, POGL_UINT32 startIndex, POGL_UINT32 count);

private:
	POGL_UINT32 mRefCount;
	POGL_UINT32 mUID;
	GLuint mBufferID;
	const POGL_VERTEX_LAYOUT* mLayout;
	POGL_UINT32 mNumVertices;
	GLenum mPrimitiveType;
	GLenum mBufferUsage;
	IPOGLDevice* mDevice;
};