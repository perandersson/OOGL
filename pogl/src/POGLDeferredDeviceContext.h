#pragma once
#include "POGLDeferredCommands.h"
#include <mutex>
#include <condition_variable>

class POGLDeferredRenderState;
class POGLDeferredDeviceContext : public IPOGLDeferredDeviceContext
{
public:
	POGLDeferredDeviceContext(IPOGLDevice* device);
	~POGLDeferredDeviceContext();
	
	/*!
		\brief Retrieves the memory location based on the supplied offset

		\param offset
		\return
	*/
	POGL_HANDLE GetMapPointer(POGL_UINT32 offset);
	
	/*!
		\brief Add a new command to be executed and put it onto the queue

		If no commands are available on the memory pool then create a new one

		\param function
		\param releaseFunction
	*/
	POGLDeferredCommand* AddCommand(POGLCommandFuncPtr function, POGLCommandReleaseFuncPtr releaseFunction);
	
	/*!
		\brief Retrieves the next offset position for the supplied vertex buffer size

		\param size
		\return
	*/
	POGL_UINT32 GetMapOffset(POGL_UINT32 size);

// IPOGLInterface
public:
	virtual void AddRef();
	virtual void Release();

// IPOGLDeviceContext
public:
	virtual IPOGLDevice* GetDevice();
	virtual IPOGLShaderProgram* CreateShaderProgramFromFile(const POGL_CHAR* path, POGLShaderProgramType::Enum type);
	virtual IPOGLShaderProgram* CreateShaderProgramFromMemory(const POGL_CHAR* memory, POGL_UINT32 size, POGLShaderProgramType::Enum type);
	virtual IPOGLEffect* CreateEffectFromPrograms(IPOGLShaderProgram** programs);
	virtual IPOGLTexture1D* CreateTexture1D();
	virtual IPOGLTexture2D* CreateTexture2D(const POGL_SIZE& size, POGLTextureFormat::Enum format, const void* bytes);
	virtual IPOGLTexture3D* CreateTexture3D();
	virtual void ResizeTexture2D(IPOGLTexture2D* texture, const POGL_SIZE& size);
	virtual IPOGLFramebuffer* CreateFramebuffer(IPOGLTexture** textures);
	virtual IPOGLFramebuffer* CreateFramebuffer(IPOGLTexture** textures, IPOGLTexture* depthTexture);
	virtual IPOGLVertexBuffer* CreateVertexBuffer(const void* memory, POGL_UINT32 memorySize, const POGL_VERTEX_LAYOUT* layout, POGLPrimitiveType::Enum primitiveType, POGLBufferUsage::Enum bufferUsage);
	virtual IPOGLVertexBuffer* CreateVertexBuffer(const POGL_POSITION_VERTEX* memory, POGL_UINT32 memorySize, POGLPrimitiveType::Enum primitiveType, POGLBufferUsage::Enum bufferUsage);
	virtual IPOGLVertexBuffer* CreateVertexBuffer(const POGL_POSITION_COLOR_VERTEX* memory, POGL_UINT32 memorySize, POGLPrimitiveType::Enum primitiveType, POGLBufferUsage::Enum bufferUsage);
	virtual IPOGLVertexBuffer* CreateVertexBuffer(const POGL_POSITION_TEXCOORD_VERTEX* memory, POGL_UINT32 memorySize, POGLPrimitiveType::Enum primitiveType, POGLBufferUsage::Enum bufferUsage);
	virtual IPOGLIndexBuffer* CreateIndexBuffer(const void* memory, POGL_UINT32 memorySize, POGLVertexType::Enum type, POGLBufferUsage::Enum bufferUsage);
	virtual IPOGLResource* CloneResource(IPOGLResource* resource);
	virtual void CopyResource(IPOGLResource* source, IPOGLResource* destination);
	virtual void CopyResource(IPOGLResource* source, IPOGLResource* destination, POGL_UINT32 sourceOffset, POGL_UINT32 destinationOffset, POGL_UINT32 size);
	virtual IPOGLRenderState* Apply(IPOGLEffect* effect);
	virtual void* Map(IPOGLResource* resource, POGLResourceMapType::Enum e);
	virtual void* Map(IPOGLResource* resource, POGL_UINT32 offset, POGL_UINT32 length, POGLResourceMapType::Enum e);
	virtual void Unmap(IPOGLResource* resource);
	virtual void SetViewport(const POGL_RECT& viewport);

// IPOGLDeferredDeviceContext
public:
	virtual void ExecuteCommands(IPOGLDeviceContext* context);
	virtual void ExecuteCommands(IPOGLDeviceContext* context, bool clearCommands);
	virtual void Flush();

protected:
	REF_COUNTER mRefCount;
	IPOGLDevice* mDevice;
	POGLDeferredRenderState* mRenderState;

	//
	// Commands to be flushed
	//

	POGLDeferredCommand* mCommands;
	POGL_UINT32 mCommandsSize;
	POGL_UINT32 mCommandsOffset;

	//
	// Flushed commands
	//

	std::mutex mFlushedCommandsMutex;
	POGLDeferredCommand* mFlushedCommands;
	POGL_UINT32 mFlushedCommandsSize;

	/* Keep track of the vertex mapping */
	POGLDeferredCommand* mMap;

	/* Memory used when mapping */
	void* mMapMemoryPool;
	POGL_UINT32 mMapMemoryPoolSize;
	POGL_UINT32 mMapMemoryPoolOffset;
};
