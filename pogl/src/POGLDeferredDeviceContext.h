#pragma once
#include "config.h"
#include <gl/pogl.h>
#include <mutex>
#include <condition_variable>

typedef void(*POGLCommandFuncPtr)(class POGLRenderState*, struct POGLDeferredCommand*);
typedef void(*POGLReleaseCommandFuncPtr)(class POGLDeferredDeviceContext*, struct POGLDeferredCommand*);

struct POGLDeferredCommand
{
	POGLDeferredCommand* tail;
	POGLCommandFuncPtr function;
	POGLReleaseCommandFuncPtr releaseFunction;

	// Extra
	POGL_HANDLE extra[10];
};

struct POGLDeferredCreateVertexBufferCommand
{
	union {
		POGLDeferredCommand command;
		struct {
			POGLDeferredCommand* tail;
			POGLCommandFuncPtr function;
			POGLReleaseCommandFuncPtr releaseFunction;

			class POGLVertexBuffer* vertexBuffer;
			void* memory;
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
			POGLReleaseCommandFuncPtr releaseFunction;

			class POGLVertexBuffer* vertexBuffer;
			void* memory;
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
			POGLReleaseCommandFuncPtr releaseFunction;

			class POGLVertexBuffer* vertexBuffer;
			void* memory;
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
	virtual void ExecuteCommands(IPOGLDeviceContext* context);
	virtual void ExecuteCommands(IPOGLDeviceContext* context, bool clearCommands);

private:
	POGLDeferredCommand* AllocCommand(POGLCommandFuncPtr function, POGLReleaseCommandFuncPtr releaseFunction);
	void AddCommand(POGLDeferredCommand* command);
	POGLDeferredCommand* GetCommands();
	void FreeCommands(POGLDeferredCommand* commands);
	void ReleaseCommands();

protected:
	POGL_UINT32 mRefCount;
	IPOGLDevice* mDevice;

	std::mutex mBeginEndMutex;
	std::condition_variable mBeginEndCondition;

	//
	// The commands generated inside the thread
	//

	std::mutex mCommandsMutex;
	POGLDeferredCommand* mCommandsToExecuteHead;
	POGLDeferredCommand* mCommandsToExecuteTail;

	std::mutex mFreeMutex;
	POGLDeferredCommand* mFreeCommands;
	POGLDeferredCommand* mReleasedCommands;

	/* Keep track of the vertex mapping */
	POGLDeferredCommand* mMap;

	// Map memory
	void* mMapMemory;
	POGL_UINT32 mMapMemoryOffset;
};
