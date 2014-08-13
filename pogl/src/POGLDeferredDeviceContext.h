#pragma once
#include "config.h"
#include <gl/pogl.h>
#include <mutex>
#include <condition_variable>

typedef void(*POGLCommandFuncPtr)(class POGLDeferredDeviceContext*, class POGLRenderState*, struct POGLDeferredCommand*);

struct POGLDeferredCommand
{
	POGLDeferredCommand* tail;
	POGLCommandFuncPtr function;

	// Extra
	POGL_HANDLE extra[6];
};

struct POGLDeferredCreateVertexBufferCommand
{
	union {
		POGLDeferredCommand command;
		struct {
			POGLDeferredCommand* tail;
			POGLCommandFuncPtr function;

			class POGLVertexBuffer* vertexBuffer;
			POGL_UINT32 memoryPoolOffset;
			POGL_SIZE size;
		};
	};
};

struct POGLDeferredMapVertexBufferCommand
{
	union {
		POGLDeferredCommand command;
		struct {
			POGLDeferredCommand* tail;
			POGLCommandFuncPtr function;

			class POGLVertexBuffer* vertexBuffer;
			POGL_UINT32 memoryPoolOffset;
			POGL_SIZE size;
		};
	};
};

struct POGLDeferredMapRangeVertexBufferCommand
{
	union {
		POGLDeferredCommand command;
		struct {
			POGLDeferredCommand* tail;
			POGLCommandFuncPtr function;

			class POGLVertexBuffer* vertexBuffer;
			POGL_UINT32 memoryPoolOffset;
			POGL_SIZE offset;
			POGL_SIZE length;
		};
	};
};

class POGLDeferredDeviceContext : public IPOGLDeferredDeviceContext
{
public:
	POGLDeferredDeviceContext(IPOGLDevice* device);
	~POGLDeferredDeviceContext();
	
	/*!
	
	*/
	POGL_HANDLE GetMapPointer(POGL_UINT32 offset);

// IPOGLInterface
public:
	virtual void AddRef();
	virtual void Release();

// IPOGLDeviceContext
public:
	virtual IPOGLDevice* GetDevice();
	virtual IPOGLShaderProgram* CreateShaderProgramFromFile(const POGL_CHAR* path, POGLShaderProgramType::Enum type);
	virtual IPOGLShaderProgram* CreateShaderProgramFromMemory(const POGL_CHAR* memory, POGL_UINT32 size, POGLShaderProgramType::Enum type);
	virtual IPOGLEffect* CreateEffectFromPrograms(IPOGLShaderProgram** programs, POGL_UINT32 numPrograms);
	virtual IPOGLTexture1D* CreateTexture1D();
	virtual IPOGLTexture2D* CreateTexture2D(const POGL_SIZEI& size, POGLTextureFormat::Enum format, const void* bytes);
	virtual IPOGLTexture3D* CreateTexture3D();
	virtual IPOGLVertexBuffer* CreateVertexBuffer(const void* memory, POGL_SIZE memorySize, const POGL_VERTEX_LAYOUT* layout, POGLPrimitiveType::Enum primitiveType, POGLBufferUsage::Enum bufferUsage);
	virtual IPOGLVertexBuffer* CreateVertexBuffer(const POGL_POSITION_VERTEX* memory, POGL_SIZE memorySize, POGLPrimitiveType::Enum primitiveType, POGLBufferUsage::Enum bufferUsage);
	virtual IPOGLVertexBuffer* CreateVertexBuffer(const POGL_POSITION_COLOR_VERTEX* memory, POGL_SIZE memorySize, POGLPrimitiveType::Enum primitiveType, POGLBufferUsage::Enum bufferUsage);
	virtual IPOGLVertexBuffer* CreateVertexBuffer(const POGL_POSITION_TEXCOORD_VERTEX* memory, POGL_SIZE memorySize, POGLPrimitiveType::Enum primitiveType, POGLBufferUsage::Enum bufferUsage);
	virtual IPOGLIndexBuffer* CreateIndexBuffer(const void* memory, POGL_SIZE memorySize, POGLVertexType::Enum type, POGLBufferUsage::Enum bufferUsage);
	virtual IPOGLRenderState* Apply(IPOGLEffect* effect);
	virtual void* Map(IPOGLResource* resource, POGLResourceStreamType::Enum e);
	virtual void* Map(IPOGLResource* resource, POGL_UINT32 offset, POGL_UINT32 length, POGLResourceStreamType::Enum e);
	virtual void Unmap(IPOGLResource* resource);

// IPOGLDeferredDeviceContext
public:
	virtual void FlushAndWait(std::condition_variable& condition);
	virtual void ExecuteCommands(IPOGLDeviceContext* context);
	virtual void ExecuteCommands(IPOGLDeviceContext* context, bool clearCommands);

private:
	/*!
		\brief Retrieve a command to be executed

		If no commands are available on the memory pool then create a new one

		\param function
		\param releaseFunction
	*/
	POGLDeferredCommand* AllocCommand(POGLCommandFuncPtr function);

	/*!
		\brief Add a command to be executed after the flush

		\param command
	*/
	void AddCommand(POGLDeferredCommand* command);

	/*!
		\brief Retreive the flushed commands
	*/
	POGLDeferredCommand* GetFlushedCommands();

	/*!
		\brief Free the supplied commands

		\param commands
	*/
	void FreeCommands(POGLDeferredCommand* commands);

	/*!
		\brief Execute the release function on all the "commands to be relesed"
	*/
	void ReleaseCommands();

	/*!
		\brief 
	*/
	POGL_UINT32 GetMapOffset(POGL_UINT32 size);

protected:
	POGL_UINT32 mRefCount;
	IPOGLDevice* mDevice;

	/* Mutex used to prevent this device context from being executed during the Wait phase */
	std::mutex mWaitMutex;

	//
	// The commands generated inside the thread (before performing a flush)
	//

	POGLDeferredCommand* mCommandsToExecuteHead;
	POGLDeferredCommand* mCommandsToExecuteTail;

	//
	// Flushed commands, which will be rendered by the main thread
	//

	std::mutex mFlushedCommandsMutex;
	POGLDeferredCommand* mFlushedCommandsHead;

	//
	// Commands that's finished executing (that can be reused)
	//

	POGLDeferredCommand* mFreeCommands;

	std::mutex mReleasedCommandsMutex;
	POGLDeferredCommand* mReleasedCommands;

	/* Keep track of the vertex mapping */
	POGLDeferredCommand* mMap;

	/* Memory used when mapping */
	void* mMapMemoryPool;

	/* The size of the memory pool */
	POGL_UINT32 mMapMemoryPoolSize;

	/* The offset to the next free memory pool byte */
	POGL_UINT32 mMapMemoryPoolOffset;
};
