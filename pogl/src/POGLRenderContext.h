#pragma once
#include "config.h"

class POGLRenderState;
class POGLVertexBuffer;
class POGLIndexBuffer;
class POGLAPI POGLRenderContext : public IPOGLRenderContext
{
public:
	POGLRenderContext(IPOGLDevice* device);
	~POGLRenderContext();

	/*!
		\brief Destroys this instance and removes the internal release the associated memory with it
	*/
	void Destroy();

	/*!
		\brief Retrieves the render state
	*/
	inline POGLRenderState* GetRenderState() {
		return mRenderState;
	}

	virtual IPOGLDevice* GetDevice();
	virtual IPOGLShader* CreateShaderFromFile(const POGL_CHAR* path, POGLShaderType::Enum type);
	virtual IPOGLShader* CreateShaderFromMemory(const POGL_CHAR* memory, POGL_UINT32 size, POGLShaderType::Enum type);
	virtual IPOGLProgram* CreateProgramFromShaders(IPOGLShader** shaders);
	virtual IPOGLTexture1D* CreateTexture1D();
	virtual IPOGLTexture2D* CreateTexture2D(const POGL_SIZE& size, POGLTextureFormat::Enum format, const void* bytes);
	virtual IPOGLTexture3D* CreateTexture3D();
	virtual void ResizeTexture2D(IPOGLTexture2D* texture, const POGL_SIZE& size);
	virtual IPOGLFramebuffer* CreateFramebuffer(IPOGLTexture** textures);
	virtual IPOGLFramebuffer* CreateFramebuffer(IPOGLTexture** textures, IPOGLTexture* depthStencilTexture);
	virtual IPOGLVertexBuffer* CreateVertexBuffer(const void* memory, POGL_UINT32 memorySize, const POGL_VERTEX_LAYOUT* layout, POGLPrimitiveType::Enum primitiveType, POGLBufferUsage::Enum bufferUsage);
	virtual IPOGLVertexBuffer* CreateVertexBuffer(const POGL_POSITION_VERTEX* memory, POGL_UINT32 memorySize, POGLPrimitiveType::Enum primitiveType, POGLBufferUsage::Enum bufferUsage);
	virtual IPOGLVertexBuffer* CreateVertexBuffer(const POGL_POSITION_COLOR_VERTEX* memory, POGL_UINT32 memorySize, POGLPrimitiveType::Enum primitiveType, POGLBufferUsage::Enum bufferUsage);
	virtual IPOGLVertexBuffer* CreateVertexBuffer(const POGL_POSITION_TEXCOORD_VERTEX* memory, POGL_UINT32 memorySize, POGLPrimitiveType::Enum primitiveType, POGLBufferUsage::Enum bufferUsage);
	virtual IPOGLIndexBuffer* CreateIndexBuffer(const void* memory, POGL_UINT32 memorySize, POGLVertexType::Enum type, POGLBufferUsage::Enum bufferUsage);
	virtual IPOGLResource* CloneResource(IPOGLResource* resource);
	virtual void CopyResource(IPOGLResource* source, IPOGLResource* destination);
	virtual void CopyResource(IPOGLResource* source, IPOGLResource* destination, POGL_UINT32 sourceOffset, POGL_UINT32 destinationOffset, POGL_UINT32 size);
	virtual IPOGLRenderState* Apply(IPOGLProgram* program);
	virtual void* Map(IPOGLResource* resource, POGLResourceMapType::Enum e);
	virtual void* Map(IPOGLResource* resource, POGL_UINT32 offset, POGL_UINT32 length, POGLResourceMapType::Enum e);
	virtual void Unmap(IPOGLResource* resource);
	virtual void SetViewport(const POGL_RECT& viewport);

	/*!
		\brief 
	*/
	void InitializeRenderState();

protected:
	IPOGLDevice* mDevice;
	POGLRenderState* mRenderState;
};
