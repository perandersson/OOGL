#include "MemCheck.h"
#include "POGLDeferredDeviceContext.h"
#include "POGLVertexBuffer.h"
#include "POGLEnum.h"
#include "POGLFactory.h"
#include "POGLRenderState.h"
#include "POGLDeviceContext.h"
#include "POGLTexture2D.h"
#include "POGLDeferredRenderState.h"

POGLDeferredDeviceContext::POGLDeferredDeviceContext(IPOGLDevice* device)
: mRefCount(1), mDevice(device), mRenderState(nullptr),
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
		
		//
		// Release the flushed commands. This is needed because some resources
		// might be in the flushed command queue but not executed
		//

		if (mFlushedCommands != nullptr) {
			for (POGL_UINT32 i = 0; i < mFlushedCommandsSize; ++i) {
				POGLDeferredCommand* command = &mFlushedCommands[i];
				(*command->releaseFunction)(command);
			}
			mFlushedCommands = nullptr;
			mFlushedCommandsSize = 0;
		}

		//
		// Free the memory pools memory
		//

		if (mCommands != nullptr) {
			for (POGL_UINT32 i = 0; i < mCommandsOffset; ++i) {
				POGLDeferredCommand* command = &mCommands[i];
				(*command->releaseFunction)(command);
			}
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

		if (mRenderState != nullptr) {
			mRenderState->Release();
			mRenderState = nullptr;
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
	THROW_NOT_IMPLEMENTED_EXCEPTION();
}

IPOGLShaderProgram* POGLDeferredDeviceContext::CreateShaderProgramFromMemory(const POGL_CHAR* memory, POGL_UINT32 size, POGLShaderProgramType::Enum type)
{
	THROW_NOT_IMPLEMENTED_EXCEPTION();
}

IPOGLEffect* POGLDeferredDeviceContext::CreateEffectFromPrograms(IPOGLShaderProgram** programs)
{
	THROW_NOT_IMPLEMENTED_EXCEPTION();
}

IPOGLTexture1D* POGLDeferredDeviceContext::CreateTexture1D()
{
	THROW_NOT_IMPLEMENTED_EXCEPTION();
}

IPOGLTexture2D* POGLDeferredDeviceContext::CreateTexture2D(const POGL_SIZEI& size, POGLTextureFormat::Enum format, const void* bytes)
{
	if (size.width <= 0)
		THROW_EXCEPTION(POGLResourceException, "You cannot create a texture with width: %d", size.width);

	if (size.height <= 0)
		THROW_EXCEPTION(POGLResourceException, "You cannot create a texture with height: %d", size.height);

	//
	// When the bytes container is null then it usually means that we want to create a texture used togehter
	// with a framebuffer
	//

	POGLTexture2D* texture = new POGLTexture2D(size, format);
	POGLCreateTexture2DCommand* cmd = (POGLCreateTexture2DCommand*)AddCommand(&POGLCreateTexture2D_Command, &POGLCreateTexture2D_Release);
	cmd->texture = texture;
	cmd->texture->AddRef();

	if (bytes == nullptr) {
		cmd->memoryPoolOffset = 0;
		cmd->size = 0;
	}
	else {
		const POGL_UINT32 memorySize = POGLEnum::TextureFormatToSize(format, size);
		cmd->memoryPoolOffset = GetMapOffset(memorySize);
		memcpy(GetMapPointer(cmd->memoryPoolOffset), bytes, memorySize);
		cmd->size = memorySize;
	}

	return texture;
}

IPOGLTexture3D* POGLDeferredDeviceContext::CreateTexture3D()
{
	THROW_NOT_IMPLEMENTED_EXCEPTION();
}

IPOGLFramebuffer* POGLDeferredDeviceContext::CreateFramebuffer(IPOGLTexture** textures)
{
	return CreateFramebuffer(textures, nullptr);
}

IPOGLFramebuffer* POGLDeferredDeviceContext::CreateFramebuffer(IPOGLTexture** textures, IPOGLTexture* depthTexture)
{
	THROW_NOT_IMPLEMENTED_EXCEPTION();
}

IPOGLVertexBuffer* POGLDeferredDeviceContext::CreateVertexBuffer(const void* memory, POGL_UINT32 memorySize, const POGL_VERTEX_LAYOUT* layout, POGLPrimitiveType::Enum primitiveType, POGLBufferUsage::Enum bufferUsage)
{
	const POGL_UINT32 numVertices = memorySize / layout->vertexSize;
	const GLenum usage = POGLEnum::Convert(bufferUsage);
	const GLenum type = POGLEnum::Convert(primitiveType);

	POGLVertexBuffer* vb = new POGLVertexBuffer(numVertices, layout, type, bufferUsage);
	POGLCreateVertexBufferCommand* command = (POGLCreateVertexBufferCommand*)AddCommand(&POGLCreateVertexBuffer_Command, &POGLCreateVertexBuffer_Release);
	command->vertexBuffer = vb;
	command->vertexBuffer->AddRef();
	command->memoryPoolOffset = GetMapOffset(memorySize);
	memcpy(GetMapPointer(command->memoryPoolOffset), memory, memorySize);
	command->size = memorySize;
	return vb;
}

IPOGLVertexBuffer* POGLDeferredDeviceContext::CreateVertexBuffer(const POGL_POSITION_VERTEX* memory, POGL_UINT32 memorySize, POGLPrimitiveType::Enum primitiveType, POGLBufferUsage::Enum bufferUsage)
{
	return CreateVertexBuffer(memory, memorySize, &POGL_POSITION_VERTEX_LAYOUT, primitiveType, bufferUsage);
}

IPOGLVertexBuffer* POGLDeferredDeviceContext::CreateVertexBuffer(const POGL_POSITION_COLOR_VERTEX* memory, POGL_UINT32 memorySize, POGLPrimitiveType::Enum primitiveType, POGLBufferUsage::Enum bufferUsage)
{
	return CreateVertexBuffer(memory, memorySize, &POGL_POSITION_COLOR_VERTEX_LAYOUT, primitiveType, bufferUsage);
}

IPOGLVertexBuffer* POGLDeferredDeviceContext::CreateVertexBuffer(const POGL_POSITION_TEXCOORD_VERTEX* memory, POGL_UINT32 memorySize, POGLPrimitiveType::Enum primitiveType, POGLBufferUsage::Enum bufferUsage)
{
	return CreateVertexBuffer(memory, memorySize, &POGL_POSITION_TEXCOORD_VERTEX_LAYOUT, primitiveType, bufferUsage);
}

IPOGLIndexBuffer* POGLDeferredDeviceContext::CreateIndexBuffer(const void* memory, POGL_UINT32 memorySize, POGLVertexType::Enum type, POGLBufferUsage::Enum bufferUsage)
{
	THROW_NOT_IMPLEMENTED_EXCEPTION();
}

IPOGLRenderState* POGLDeferredDeviceContext::Apply(IPOGLEffect* effect)
{
	if (effect == nullptr)
		THROW_EXCEPTION(POGLStateException, "You are not allowed to apply a non-existing effect");

	if (mRenderState == nullptr) {
		mRenderState = new POGLDeferredRenderState(this);
	}

	POGLApplyEffectCommand* cmd = (POGLApplyEffectCommand*)AddCommand(&POGLApplyEffect_Command, &POGLApplyEffect_Release);
	cmd->effect = effect;
	cmd->effect->AddRef();
	mRenderState->AddRef();
	return mRenderState;
}

void* POGLDeferredDeviceContext::Map(IPOGLResource* resource, POGLResourceMapType::Enum e)
{
	if (mMap != nullptr)
		THROW_EXCEPTION(POGLStateException, "You are not allowed to map more than one resource at the same time");

	auto type = resource->GetResourceType();
	if (type == POGLResourceType::VERTEXBUFFER) {
		POGLVertexBuffer* vb = static_cast<POGLVertexBuffer*>(resource);
		POGLMapVertexBufferCommand* map = (POGLMapVertexBufferCommand*)AddCommand(&POGLMapVertexBuffer_Command, &POGLMapVertexBuffer_Release);
		map->size = vb->GetCount() * vb->GetLayout()->vertexSize;
		map->memoryPoolOffset = GetMapOffset(map->size);
		map->vertexBuffer = vb;
		map->vertexBuffer->AddRef();
		mMap = (POGLDeferredCommand*)map;
		return GetMapPointer(map->memoryPoolOffset);
	}

	THROW_NOT_IMPLEMENTED_EXCEPTION();
}

void* POGLDeferredDeviceContext::Map(IPOGLResource* resource, POGL_UINT32 offset, POGL_UINT32 length, POGLResourceMapType::Enum e)
{
	if (mMap != nullptr)
		THROW_EXCEPTION(POGLStateException, "You are not allowed to map more than one resource at the same time");

	auto type = resource->GetResourceType();
	if (type == POGLResourceType::VERTEXBUFFER) {
		POGLVertexBuffer* vb = static_cast<POGLVertexBuffer*>(resource);
		const POGL_UINT32 memorySize = vb->GetCount() * vb->GetLayout()->vertexSize;
		if (offset + length > memorySize)
			THROW_EXCEPTION(POGLStateException, "You cannot map with offset: %d and length: %d when the vertex buffer size is: %d", offset, length, memorySize);
		
		POGLMapRangeVertexBufferCommand* map = (POGLMapRangeVertexBufferCommand*)AddCommand(&POGLMapRangeVertexBuffer_Command, &POGLMapRangeVertexBuffer_Release);
		map->offset = offset;
		map->length = length;
		map->memoryPoolOffset = GetMapOffset(memorySize);
		map->vertexBuffer = vb;
		map->vertexBuffer->AddRef();
		mMap = (POGLDeferredCommand*)map;
		return GetMapPointer(map->memoryPoolOffset);
	}

	THROW_NOT_IMPLEMENTED_EXCEPTION();
}

void POGLDeferredDeviceContext::Unmap(IPOGLResource* resource)
{
	if (mMap == nullptr)
		THROW_EXCEPTION(POGLStateException, "You are not allowed to unmap a non-mapped resource");

	auto type = resource->GetResourceType();
	if (type == POGLResourceType::VERTEXBUFFER) {
		mMap = nullptr;
		return;
	}

	THROW_NOT_IMPLEMENTED_EXCEPTION();
}

void POGLDeferredDeviceContext::SetViewport(const POGL_RECT& viewport)
{
	mRenderState->SetViewport(viewport);
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
		(*command->releaseFunction)(command);
	}
	
	//
	// Free the commands
	//

	if (clearCommands) {
		mFlushedCommandsSize = 0;
		mFlushedCommands = nullptr;
	}
}

void POGLDeferredDeviceContext::Flush()
{
	mFlushedCommandsMutex.lock();
	mFlushedCommands = mCommands;
	mFlushedCommandsSize = mCommandsOffset;
	mCommandsOffset = 0;
	mMapMemoryPoolOffset = 0;
	mFlushedCommandsMutex.unlock();
}

POGLDeferredCommand* POGLDeferredDeviceContext::AddCommand(POGLCommandFuncPtr function, POGLCommandReleaseFuncPtr releaseFunction)
{
	if (mCommandsOffset >= mCommandsSize) {
		static const POGL_UINT32 INCREASE_SIZE = 100;
		mCommandsSize += INCREASE_SIZE;
		mCommands = (POGLDeferredCommand*)realloc(mCommands, mCommandsSize * sizeof(POGLDeferredCommand));
	}

	POGLDeferredCommand* command = mCommands + mCommandsOffset;
	mCommandsOffset++;
	command->function = function;
	command->releaseFunction = releaseFunction;
	return command;
}
