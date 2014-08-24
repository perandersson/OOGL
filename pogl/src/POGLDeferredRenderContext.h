#pragma once
#include "POGLDeferredCommands.h"
#include <mutex>
#include <condition_variable>

class POGLDevice;
class POGLDeferredRenderState;
class POGLDeferredRenderContext : public IPOGLDeferredRenderContext
{
public:
	POGLDeferredRenderContext(POGLDevice* device);
	virtual ~POGLDeferredRenderContext();
	
	/*!
		\brief Add a new command to be executed and put it onto the queue

		If no commands are available on the memory pool then create a new one

		\param function
				The function called when executing the command
		\param releaseFunction
				The function called when releasing the command
		\param size
				The memory size of the command
		\return A pointer to the command
	*/
	POGL_HANDLE AddCommand(POGLCommandFuncPtr function, POGLCommandReleaseFuncPtr releaseFunction, POGL_UINT32 size);
	
	/*!
		\brief Retrieves a pointer to a memory location based on the supplied offset

		\param offset
				Memory offset
		\return A pointer to a memory location
	*/
	POGL_HANDLE GetMapPointer(POGL_UINT32 offset);
	
	/*!
		\brief Retrieves the next offset position for the supplied vertex buffer size

		\param size
				The size that is required
		\return The offset of the memory pool we are allowed to write the amount of bytes supplied to this method
	*/
	POGL_UINT32 GetMapOffset(POGL_UINT32 size);

// IPOGLInterface
public:
	virtual void AddRef();
	virtual void Release();

// IPOGLRenderContext
public:
	virtual IPOGLDevice* GetDevice();
	virtual IPOGLShader* CreateShaderFromFile(const POGL_CHAR* path, POGLShaderType::Enum type);
	virtual IPOGLShader* CreateShaderFromMemory(const POGL_CHAR* memory, POGL_UINT32 size, POGLShaderType::Enum type);
	virtual IPOGLProgram* CreateProgramFromShaders(IPOGLShader** shaders, POGL_UINT32 count);
	virtual IPOGLTexture1D* CreateTexture1D();
	virtual IPOGLTexture2D* CreateTexture2D(const POGL_SIZE& size, POGLTextureFormat::Enum format, const void* bytes);
	virtual IPOGLTexture3D* CreateTexture3D();
	virtual void ResizeTexture2D(IPOGLTexture2D* texture, const POGL_SIZE& size);
	virtual IPOGLFramebuffer* CreateFramebuffer(IPOGLTexture** textures, POGL_UINT32 count);
	virtual IPOGLFramebuffer* CreateFramebuffer(IPOGLTexture** textures, POGL_UINT32 count, IPOGLTexture* depthTexture);
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

// IPOGLDeferredRenderContext
public:
	virtual void ExecuteCommands(IPOGLRenderContext* context);
	virtual void ExecuteCommands(IPOGLRenderContext* context, bool clearCommands);
	virtual void Flush();

protected:
	REF_COUNTER mRefCount;
	POGLDevice* mDevice;
	POGLDeferredRenderState* mRenderState;

	// 
	// Command memory pool
	//

	POGL_BYTE* mMemoryPool;
	POGL_UINT32 mMemoryPoolOffset;
	POGL_UINT32 mMemoryPoolSize;

	//
	// Flushed commands
	//

	std::mutex mFlushedCommandsMutex;
	POGL_BYTE* mFlushedCommands;
	POGL_UINT32 mFlushedCommandsSize;

	//
	// Memory pool for data
	//

	void* mMapMemoryPool;
	POGL_UINT32 mMapMemoryPoolSize;
	POGL_UINT32 mMapMemoryPoolOffset;

	//
	// Currently mapping a vertex buffer
	//

	bool mMapping;
};
