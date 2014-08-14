#include "MemCheck.h"
#include "POGLDeferredDeviceContext.h"
#include "POGLVertexBuffer.h"
#include "POGLEnum.h"
#include "POGLFactory.h"
#include "POGLRenderState.h"
#include "POGLDeviceContext.h"

POGLDeferredDeviceContext::POGLDeferredDeviceContext(IPOGLDevice* device)
: mRefCount(1), mDevice(device), 
mCommands(nullptr), mCommandsSize(0), mCommandsOffset(0),
mFlushedCommands(nullptr), mFlushedCommandsSize(0), 
mMap(nullptr),
mMapMemoryPool(nullptr), mMapMemoryPoolSize(0), mMapMemoryPoolOffset(0)
{
}

POGLDeferredDeviceContext::~POGLDeferredDeviceContext()
{
}

void POGLDeferredDeviceContext::AddRef()
{
	mRefCount++;
}

void POGLDeferredDeviceContext::Release()
{
	if (--mRefCount == 0) {
		if (mFlushedCommands != nullptr) {
			for (POGL_UINT32 i = 0; i < mFlushedCommandsSize; ++i) {
				mFlushedCommands[i].resource->Release();
			}
			mFlushedCommands = nullptr;
			mFlushedCommandsSize = 0;
		}

		if (mCommands != nullptr) {
			free(mCommands);
			mCommands = nullptr;
			mCommandsSize = mCommandsOffset = 0;
		}

		if (mMapMemoryPool != nullptr) {
			free(mMapMemoryPool);
			mMapMemoryPool = nullptr;
			mMapMemoryPoolOffset = 0;
			mMapMemoryPoolSize = 0;
		}

		delete this;
	}
}

IPOGLDevice* POGLDeferredDeviceContext::GetDevice()
{
	mDevice->AddRef();
	return mDevice;
}

IPOGLShaderProgram* POGLDeferredDeviceContext::CreateShaderProgramFromFile(const POGL_CHAR* path, POGLShaderProgramType::Enum type)
{
	THROW_EXCEPTION(POGLInitializationException, "Not implemented");
	return nullptr;
}

IPOGLShaderProgram* POGLDeferredDeviceContext::CreateShaderProgramFromMemory(const POGL_CHAR* memory, POGL_UINT32 size, POGLShaderProgramType::Enum type)
{
	THROW_EXCEPTION(POGLInitializationException, "Not implemented");
	return nullptr;
}

IPOGLEffect* POGLDeferredDeviceContext::CreateEffectFromPrograms(IPOGLShaderProgram** programs, POGL_UINT32 numPrograms)
{
	THROW_EXCEPTION(POGLInitializationException, "Not implemented");
	return nullptr;
}

IPOGLTexture1D* POGLDeferredDeviceContext::CreateTexture1D()
{
	THROW_EXCEPTION(POGLInitializationException, "Not implemented");
	return nullptr;
}

IPOGLTexture2D* POGLDeferredDeviceContext::CreateTexture2D(const POGL_SIZEI& size, POGLTextureFormat::Enum format, const void* bytes)
{
	THROW_EXCEPTION(POGLInitializationException, "Not implemented");
	return nullptr;
}

IPOGLTexture3D* POGLDeferredDeviceContext::CreateTexture3D()
{
	THROW_EXCEPTION(POGLInitializationException, "Not implemented");
	return nullptr;
}

IPOGLVertexBuffer* POGLDeferredDeviceContext::CreateVertexBuffer(const void* memory, POGL_SIZE memorySize, const POGL_VERTEX_LAYOUT* layout, POGLPrimitiveType::Enum primitiveType, POGLBufferUsage::Enum bufferUsage)
{
	const POGL_UINT32 numVertices = memorySize / layout->vertexSize;
	const GLenum usage = POGLEnum::Convert(bufferUsage);
	const GLenum type = POGLEnum::Convert(primitiveType);

	POGLVertexBuffer* vb = new POGLVertexBuffer(numVertices, layout, type, bufferUsage);
	POGLDeferredCreateVertexBufferCommand* command = (POGLDeferredCreateVertexBufferCommand*)AddCommand(&POGLCreateVertexBuffer_Command);
	command->vertexBuffer = vb;
	command->vertexBuffer->AddRef();
	command->memoryPoolOffset = GetMapOffset(memorySize);
	memcpy(GetMapPointer(command->memoryPoolOffset), memory, memorySize);
	command->size = memorySize;
	return vb;
}

IPOGLVertexBuffer* POGLDeferredDeviceContext::CreateVertexBuffer(const POGL_POSITION_VERTEX* memory, POGL_SIZE memorySize, POGLPrimitiveType::Enum primitiveType, POGLBufferUsage::Enum bufferUsage)
{
	return CreateVertexBuffer(memory, memorySize, &POGL_POSITION_VERTEX_LAYOUT, primitiveType, bufferUsage);
}

IPOGLVertexBuffer* POGLDeferredDeviceContext::CreateVertexBuffer(const POGL_POSITION_COLOR_VERTEX* memory, POGL_SIZE memorySize, POGLPrimitiveType::Enum primitiveType, POGLBufferUsage::Enum bufferUsage)
{
	return CreateVertexBuffer(memory, memorySize, &POGL_POSITION_COLOR_VERTEX_LAYOUT, primitiveType, bufferUsage);
}

IPOGLVertexBuffer* POGLDeferredDeviceContext::CreateVertexBuffer(const POGL_POSITION_TEXCOORD_VERTEX* memory, POGL_SIZE memorySize, POGLPrimitiveType::Enum primitiveType, POGLBufferUsage::Enum bufferUsage)
{
	return CreateVertexBuffer(memory, memorySize, &POGL_POSITION_TEXCOORD_VERTEX_LAYOUT, primitiveType, bufferUsage);
}

IPOGLIndexBuffer* POGLDeferredDeviceContext::CreateIndexBuffer(const void* memory, POGL_SIZE memorySize, POGLVertexType::Enum type, POGLBufferUsage::Enum bufferUsage)
{
	THROW_EXCEPTION(POGLInitializationException, "Not implemented");
	return nullptr;
}

IPOGLRenderState* POGLDeferredDeviceContext::Apply(IPOGLEffect* effect)
{
	THROW_EXCEPTION(POGLInitializationException, "Not implemented");
	return nullptr;
}

void* POGLDeferredDeviceContext::Map(IPOGLResource* resource, POGLResourceStreamType::Enum e)
{
	if (mMap != nullptr)
		THROW_EXCEPTION(POGLResourceException, "You are not allowed to map more than one resource at the same time");

	auto type = resource->GetResourceType();
	if (type == POGLResourceType::VERTEXBUFFER) {
		POGLVertexBuffer* vb = static_cast<POGLVertexBuffer*>(resource);
		POGLDeferredMapVertexBufferCommand* map = (POGLDeferredMapVertexBufferCommand*)AddCommand(&POGLMapVertexBuffer_Command);
		map->size = vb->GetCount() * vb->GetLayout()->vertexSize;
		map->memoryPoolOffset = GetMapOffset(map->size);
		map->vertexBuffer = vb;
		map->vertexBuffer->AddRef();
		mMap = (POGLDeferredCommand*)map;
		return ((char*)mMapMemoryPool + map->memoryPoolOffset);
	}

	THROW_EXCEPTION(POGLInitializationException, "Not implemented");
	return nullptr;
}

void* POGLDeferredDeviceContext::Map(IPOGLResource* resource, POGL_UINT32 offset, POGL_UINT32 length, POGLResourceStreamType::Enum e)
{
	if (mMap != nullptr)
		THROW_EXCEPTION(POGLResourceException, "You are not allowed to map more than one resource at the same time");

	auto type = resource->GetResourceType();
	if (type == POGLResourceType::VERTEXBUFFER) {
		POGLVertexBuffer* vb = static_cast<POGLVertexBuffer*>(resource);
		const POGL_UINT32 memorySize = vb->GetCount() * vb->GetLayout()->vertexSize;
		if (offset + length > memorySize)
			THROW_EXCEPTION(POGLResourceException, "You cannot map with offset: %d and length: %d when the vertex buffer size is: %d", offset, length, memorySize);
		
		POGLDeferredMapRangeVertexBufferCommand* map = (POGLDeferredMapRangeVertexBufferCommand*)AddCommand(&POGLMapRangeVertexBuffer_Command);
		map->offset = offset;
		map->length = length;
		map->memoryPoolOffset = GetMapOffset(memorySize);
		map->vertexBuffer = vb;
		map->vertexBuffer->AddRef();
		mMap = (POGLDeferredCommand*)map;
		return ((char*)mMapMemoryPool + map->memoryPoolOffset);
	}


	THROW_EXCEPTION(POGLInitializationException, "Not implemented");
	return nullptr;
}

void POGLDeferredDeviceContext::Unmap(IPOGLResource* resource)
{
	if (mMap == nullptr)
		THROW_EXCEPTION(POGLResourceException, "You are not allowed to unmap a non-mapped resource");

	auto type = resource->GetResourceType();
	if (type == POGLResourceType::VERTEXBUFFER) {
		mMap = nullptr;
		return;
	}

	THROW_EXCEPTION(POGLInitializationException, "Not implemented");
}

void POGLDeferredDeviceContext::ExecuteCommands(IPOGLDeviceContext* context)
{
	ExecuteCommands(context, true);
}

POGL_UINT32 POGLDeferredDeviceContext::GetMapOffset(POGL_UINT32 size)
{
	POGL_UINT32 memoryLeft = mMapMemoryPoolSize - mMapMemoryPoolOffset;
	if (memoryLeft < size) {
		mMapMemoryPoolSize += size;
		mMapMemoryPool = realloc(mMapMemoryPool, mMapMemoryPoolSize);
	}

	POGL_UINT32 offset = mMapMemoryPoolOffset;
	mMapMemoryPoolOffset += size;
	return offset;

}

POGL_HANDLE POGLDeferredDeviceContext::GetMapPointer(POGL_UINT32 offset)
{
	return (char*)mMapMemoryPool + offset;
}

void POGLDeferredDeviceContext::ExecuteCommands(IPOGLDeviceContext* context, bool clearCommands)
{
	// Execute the deferred render commands and then return the allocated command-pointers to the memory pool
	auto renderState = static_cast<POGLDeviceContext*>(context)->GetRenderState();

	//
	// Retrieve the flushed commands if they exists and execute them
	//

	std::lock_guard<std::mutex> lock(mFlushedCommandsMutex);
	const POGL_UINT32 size = mFlushedCommandsSize;
	for (POGL_UINT32 i = 0; i < size; ++i) {
		POGLDeferredCommand* command = &mFlushedCommands[i];
		(*command->function)(this, renderState, command);
	}
	
	//
	// Free the commands
	//

	if (clearCommands) {
		mFlushedCommandsSize = 0;
		mFlushedCommands = nullptr;
		mCommandsOffset = 0;
	}
}

void POGLDeferredDeviceContext::Flush()
{
	mFlushedCommandsMutex.lock();
	mFlushedCommands = mCommands;
	mFlushedCommandsSize = mCommandsOffset;
	mFlushedCommandsMutex.unlock();

	mMapMemoryPoolOffset = 0;
}

void POGLDeferredDeviceContext::FlushAndWait(std::condition_variable& condition)
{
	mFlushedCommandsMutex.lock();
	mFlushedCommands = mCommands;
	mFlushedCommandsSize = mCommandsOffset;
	mFlushedCommandsMutex.unlock();

	//
	// Wait for the next frame
	//

	std::unique_lock<std::mutex> lock(mFlushAndWaitMutex);
	condition.wait(lock);

	//
	// Reset any used buffers
	//

	mMapMemoryPoolOffset = 0;
}

POGLDeferredCommand* POGLDeferredDeviceContext::AddCommand(POGLCommandFuncPtr function)
{
	if (mCommandsOffset >= mCommandsSize) {
		mCommandsSize += 100;
		mCommands = (POGLDeferredCommand*)realloc(mCommands, mCommandsSize * sizeof(POGLDeferredCommand));
	}

	POGLDeferredCommand* command = mCommands + mCommandsOffset;
	mCommandsOffset++;
	command->function = function;
	return command;
}
