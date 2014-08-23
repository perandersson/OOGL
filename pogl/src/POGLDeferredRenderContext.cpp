#include "MemCheck.h"
#include "POGLDeferredRenderContext.h"
#include "POGLVertexBuffer.h"
#include "POGLEnum.h"
#include "POGLFactory.h"
#include "POGLRenderState.h"
#include "POGLRenderContext.h"
#include "POGLTexture2D.h"
#include "POGLDeferredRenderState.h"
#include "POGLFramebuffer.h"
#include "POGLShader.h"
#include "POGLProgram.h"
#include "POGLIndexBuffer.h"

POGLDeferredRenderContext::POGLDeferredRenderContext(IPOGLDevice* device)
: mRefCount(1), mDevice(device), mRenderState(nullptr),
mMemoryPool(nullptr), mMemoryPoolOffset(0), mMemoryPoolSize(0),
mFlushedCommands(nullptr), mFlushedCommandsSize(0),
mMapMemoryPool(nullptr), mMapMemoryPoolSize(0), mMapMemoryPoolOffset(0),
mMapping(false)
{
	mRenderState = new POGLDeferredRenderState(this);
}

POGLDeferredRenderContext::~POGLDeferredRenderContext()
{
}

void POGLDeferredRenderContext::AddRef()
{
	mRefCount++;
}

void POGLDeferredRenderContext::Release()
{
	if (--mRefCount == 0) {
		
		//
		// Release the flushed commands. This is needed because some resources
		// might be in the flushed command queue but not executed
		//

		if (mFlushedCommands != nullptr) {
			FOR_EACH_COMMAND(mFlushedCommands, mFlushedCommandsSize)
				(*command->releaseFunction)(ptr);
				command->releaseFunction = &POGLNothing_Release;
			END_FOR_COMMANDS()
			mFlushedCommands = nullptr;
			mFlushedCommandsSize = 0;
		}

		//
		// Free the memory pools memory
		//

		if (mMemoryPool != nullptr) {
			FOR_EACH_COMMAND(mMemoryPool, mMemoryPoolOffset)
				(*command->releaseFunction)(ptr);
				command->releaseFunction = &POGLNothing_Release;
			END_FOR_COMMANDS()
			free(mMemoryPool);
			mMemoryPool = nullptr;
			mMemoryPoolOffset = mMemoryPoolSize = 0;
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

IPOGLDevice* POGLDeferredRenderContext::GetDevice()
{
	mDevice->AddRef();
	return mDevice;
}

IPOGLShader* POGLDeferredRenderContext::CreateShaderFromFile(const POGL_CHAR* path, POGLShaderType::Enum type)
{
	POGL_ISTREAM stream(path);
	if (!stream.is_open())
		THROW_EXCEPTION(POGLResourceException, "Shader at path: '%s' could not be found", path);

	// Read the entire file into memory
	POGL_STRING str((std::istreambuf_iterator<POGL_CHAR>(stream)), std::istreambuf_iterator<POGL_CHAR>());
	return CreateShaderFromMemory(str.c_str(), str.length(), type);
}

IPOGLShader* POGLDeferredRenderContext::CreateShaderFromMemory(const POGL_CHAR* memory, POGL_UINT32 size, POGLShaderType::Enum type)
{
	if (size == 0 || memory == nullptr)
		THROW_EXCEPTION(POGLResourceException, "You cannot generate a non-existing shader");

	POGL_CREATESHADER_COMMAND_DATA* cmd = (POGL_CREATESHADER_COMMAND_DATA*)AddCommand(&POGLCreateShader_Command, &POGLCreateShader_Release,
		sizeof(POGL_CREATESHADER_COMMAND_DATA));
	cmd->dataSize = size;
	cmd->memoryOffset = GetMapOffset(size);
	memcpy(GetMapPointer(cmd->memoryOffset), memory, size);
	
	POGLShader* shader = new POGLShader(type);
	cmd->shader = shader;
	cmd->shader->AddRef();
	return shader;
}

IPOGLProgram* POGLDeferredRenderContext::CreateProgramFromShaders(IPOGLShader** shaders)
{
	if (shaders == nullptr)
		THROW_EXCEPTION(POGLResourceException, "You must supply at least one shader to be able to create a program");

	POGL_CREATEPROGRAM_COMMAND_DATA* cmd = (POGL_CREATEPROGRAM_COMMAND_DATA*)AddCommand(&POGLCreateProgram_Command, &POGLCreateProgram_Release,
		sizeof(POGL_CREATEPROGRAM_COMMAND_DATA));
	POGL_UINT32 count = 0;
	memset(cmd->shaders, 0, sizeof(cmd->shaders));
	for (IPOGLShader** ptr = shaders; *ptr != nullptr; ++ptr) {
		POGLShader* shader = static_cast<POGLShader*>(*ptr);
		cmd->shaders[count++] = shader;
		shader->AddRef();
	}
	cmd->shaderCount = count;
	POGLProgram* program = new POGLProgram();
	cmd->program = program;
	cmd->program->AddRef();
	return program;
}

IPOGLTexture1D* POGLDeferredRenderContext::CreateTexture1D()
{
	THROW_NOT_IMPLEMENTED_EXCEPTION();
}

IPOGLTexture2D* POGLDeferredRenderContext::CreateTexture2D(const POGL_SIZE& size, POGLTextureFormat::Enum format, const void* bytes)
{
	if (size.width <= 0)
		THROW_EXCEPTION(POGLResourceException, "You cannot create a texture with width: %d", size.width);

	if (size.height <= 0)
		THROW_EXCEPTION(POGLResourceException, "You cannot create a texture with height: %d", size.height);
	
	POGL_CREATETEXTURE2D_COMMAND_DATA* cmd = (POGL_CREATETEXTURE2D_COMMAND_DATA*)AddCommand(&POGLCreateTexture2D_Command, &POGLCreateTexture2D_Release, 
		sizeof(POGL_CREATETEXTURE2D_COMMAND_DATA));
	cmd->dataSize = 0;
	cmd->memoryOffset = 0;
	if (bytes != nullptr) {
		const POGL_UINT32 dataSize = POGLEnum::TextureFormatToSize(format, size);
		cmd->dataSize = dataSize;
		cmd->memoryOffset = GetMapOffset(dataSize);
		memcpy(GetMapPointer(cmd->memoryOffset), bytes, dataSize);
	}

	POGLTexture2D* texture = new POGLTexture2D(size, format);
	cmd->texture = texture;
	cmd->texture->AddRef();
	
	return texture;
}

IPOGLTexture3D* POGLDeferredRenderContext::CreateTexture3D()
{
	THROW_NOT_IMPLEMENTED_EXCEPTION();
}

void POGLDeferredRenderContext::ResizeTexture2D(IPOGLTexture2D* texture, const POGL_SIZE& size)
{
	if (texture == nullptr)
		THROW_EXCEPTION(POGLStateException, "You cannot resize a non-existing texture");

	if (size.width <= 0)
		THROW_EXCEPTION(POGLStateException, "You cannot resize a texture to 0 width");

	if (size.height <= 0)
		THROW_EXCEPTION(POGLStateException, "You cannot resize a texture to 0 height");

	POGL_RESIZETEXTURE2D_COMMAND_DATA* cmd = (POGL_RESIZETEXTURE2D_COMMAND_DATA*)AddCommand(&POGLResizeTexture2D_Command, &POGLResizeTexture2D_Release, 
		sizeof(POGL_RESIZETEXTURE2D_COMMAND_DATA));
	cmd->texture = static_cast<POGLTexture2D*>(texture);
	cmd->texture->AddRef();
	cmd->newSize = size;
}

IPOGLFramebuffer* POGLDeferredRenderContext::CreateFramebuffer(IPOGLTexture** textures)
{
	return CreateFramebuffer(textures, nullptr);
}

IPOGLFramebuffer* POGLDeferredRenderContext::CreateFramebuffer(IPOGLTexture** textures, IPOGLTexture* depthTexture)
{
	std::vector<IPOGLTexture*> texturesVector;
	if (textures != nullptr) {
		for (IPOGLTexture** ptr = textures; *ptr != nullptr; ++ptr) {
			IPOGLTexture* texture = *ptr;
			texturesVector.push_back(texture);
		}
	}

	POGLFramebuffer* framebuffer = new POGLFramebuffer(texturesVector, depthTexture);

	POGL_CREATEFRAMEBUFFER_COMMAND_DATA* cmd = (POGL_CREATEFRAMEBUFFER_COMMAND_DATA*)AddCommand(&POGLCreateFrameBuffer_Command, &POGLCreateFrameBuffer_Release, 
		sizeof(POGL_CREATEFRAMEBUFFER_COMMAND_DATA));
	cmd->framebuffer = framebuffer;
	cmd->framebuffer->AddRef();
	return framebuffer;
}

IPOGLVertexBuffer* POGLDeferredRenderContext::CreateVertexBuffer(const void* memory, POGL_UINT32 memorySize, const POGL_VERTEX_LAYOUT* layout, POGLPrimitiveType::Enum primitiveType, POGLBufferUsage::Enum bufferUsage)
{
	if (memorySize == 0)
		THROW_EXCEPTION(POGLStateException, "You cannot create a non-existing vertex buffer");

	if (layout == nullptr)
		THROW_EXCEPTION(POGLStateException, "You cannot create a vertex buffer without a layout");

	const POGL_UINT32 numVertices = memorySize / layout->vertexSize;
	const GLenum usage = POGLEnum::Convert(bufferUsage);
	const GLenum type = POGLEnum::Convert(primitiveType);

	POGLVertexBuffer* vb = new POGLVertexBuffer(numVertices, layout, type, usage);
	POGL_CREATEVERTEXBUFFER_COMMAND_DATA* cmd = (POGL_CREATEVERTEXBUFFER_COMMAND_DATA*)AddCommand(&POGLCreateVertexBuffer_Command, &POGLCreateVertexBuffer_Release,
		sizeof(POGL_CREATEVERTEXBUFFER_COMMAND_DATA));
	cmd->vertexBuffer = vb;
	cmd->vertexBuffer->AddRef();
	if (memory != nullptr) {
		cmd->memoryOffset = GetMapOffset(memorySize);
		memcpy(GetMapPointer(cmd->memoryOffset), memory, memorySize);
	}
	else {
		cmd->memoryOffset = -1;
	}
	cmd->dataSize = memorySize;
	return vb;
}

IPOGLVertexBuffer* POGLDeferredRenderContext::CreateVertexBuffer(const POGL_POSITION_VERTEX* memory, POGL_UINT32 memorySize, POGLPrimitiveType::Enum primitiveType, POGLBufferUsage::Enum bufferUsage)
{
	return CreateVertexBuffer(memory, memorySize, &POGL_POSITION_VERTEX_LAYOUT, primitiveType, bufferUsage);
}

IPOGLVertexBuffer* POGLDeferredRenderContext::CreateVertexBuffer(const POGL_POSITION_COLOR_VERTEX* memory, POGL_UINT32 memorySize, POGLPrimitiveType::Enum primitiveType, POGLBufferUsage::Enum bufferUsage)
{
	return CreateVertexBuffer(memory, memorySize, &POGL_POSITION_COLOR_VERTEX_LAYOUT, primitiveType, bufferUsage);
}

IPOGLVertexBuffer* POGLDeferredRenderContext::CreateVertexBuffer(const POGL_POSITION_TEXCOORD_VERTEX* memory, POGL_UINT32 memorySize, POGLPrimitiveType::Enum primitiveType, POGLBufferUsage::Enum bufferUsage)
{
	return CreateVertexBuffer(memory, memorySize, &POGL_POSITION_TEXCOORD_VERTEX_LAYOUT, primitiveType, bufferUsage);
}

IPOGLIndexBuffer* POGLDeferredRenderContext::CreateIndexBuffer(const void* memory, POGL_UINT32 memorySize, POGLVertexType::Enum type, POGLBufferUsage::Enum bufferUsage)
{
	if (memorySize == 0)
		THROW_EXCEPTION(POGLStateException, "You cannot create a non-existing index buffer");

	if (type == POGLVertexType::FLOAT || type == POGLVertexType::DOUBLE)
		THROW_EXCEPTION(POGLStateException, "You are not allowed to create an index buffer of a decimal type");

	const POGL_UINT32 typeSize = POGLEnum::VertexTypeSize(type);
	const POGL_UINT32 numIndices = memorySize / typeSize;
	const GLenum usage = POGLEnum::Convert(bufferUsage);
	const GLenum indiceType = POGLEnum::Convert(type);

	POGLIndexBuffer* ib = new POGLIndexBuffer(typeSize, numIndices, indiceType, usage);
	POGL_CREATEINDEXBUFFER_COMMAND_DATA* cmd = (POGL_CREATEINDEXBUFFER_COMMAND_DATA*)AddCommand(&POGLCreateIndexBuffer_Command, &POGLCreateIndexBuffer_Release,
		sizeof(POGL_CREATEINDEXBUFFER_COMMAND_DATA));
	cmd->indexBuffer = ib;
	cmd->indexBuffer->AddRef(); 
	if (memory != nullptr) {
		cmd->memoryOffset = GetMapOffset(memorySize);
		memcpy(GetMapPointer(cmd->memoryOffset), memory, memorySize);
	}
	else {
		cmd->memoryOffset = -1;
	}
	cmd->dataSize = memorySize;
	return ib;
}

IPOGLResource* POGLDeferredRenderContext::CloneResource(IPOGLResource* resource)
{
	THROW_NOT_IMPLEMENTED_EXCEPTION();
}

void POGLDeferredRenderContext::CopyResource(IPOGLResource* source, IPOGLResource* destination)
{
	THROW_NOT_IMPLEMENTED_EXCEPTION();
}

void POGLDeferredRenderContext::CopyResource(IPOGLResource* source, IPOGLResource* destination, POGL_UINT32 sourceOffset, POGL_UINT32 destinationOffset, POGL_UINT32 size)
{
	THROW_NOT_IMPLEMENTED_EXCEPTION();
}

IPOGLRenderState* POGLDeferredRenderContext::Apply(IPOGLProgram* program)
{
	if (program == nullptr)
		THROW_EXCEPTION(POGLStateException, "You are not allowed to apply a non-existing program");

	POGL_APPLYPROGRAM_COMMAND* cmd = (POGL_APPLYPROGRAM_COMMAND*)AddCommand(&POGLApplyProgram_Command, &POGLApplyProgram_Release,
		sizeof(POGL_APPLYPROGRAM_COMMAND));
	cmd->program = static_cast<POGLProgram*>(program);
	cmd->program->AddRef();

	mRenderState->AddRef();
	mRenderState->FlushProgram(cmd->program);
	return mRenderState;
}

void* POGLDeferredRenderContext::Map(IPOGLResource* resource, POGLResourceMapType::Enum e)
{
	if (mMapping)
		THROW_EXCEPTION(POGLStateException, "You are not allowed to map more than one resource at the same time");

	auto type = resource->GetType();
	if (type == POGLResourceType::VERTEXBUFFER) {
		POGLVertexBuffer* vb = static_cast<POGLVertexBuffer*>(resource);
		POGL_MAPVERTEXBUFFER_COMMAND_DATA* cmd = (POGL_MAPVERTEXBUFFER_COMMAND_DATA*)AddCommand(&POGLMapVertexBuffer_Command, &POGLMapVertexBuffer_Release,
			sizeof(POGL_MAPVERTEXBUFFER_COMMAND_DATA));
		cmd->dataSize = vb->GetCount() * vb->GetLayout()->vertexSize;
		cmd->memoryOffset = GetMapOffset(cmd->dataSize);
		cmd->vertexBuffer = vb;
		cmd->vertexBuffer->AddRef();
		mMapping = true;
		return GetMapPointer(cmd->memoryOffset);
	}

	THROW_NOT_IMPLEMENTED_EXCEPTION();
}

void* POGLDeferredRenderContext::Map(IPOGLResource* resource, POGL_UINT32 offset, POGL_UINT32 length, POGLResourceMapType::Enum e)
{
	if (mMapping)
		THROW_EXCEPTION(POGLStateException, "You are not allowed to map more than one resource at the same time");

	auto type = resource->GetType();
	if (type == POGLResourceType::VERTEXBUFFER) {
		POGLVertexBuffer* vb = static_cast<POGLVertexBuffer*>(resource);
		const POGL_UINT32 memorySize = vb->GetCount() * vb->GetLayout()->vertexSize;
		if (offset + length > memorySize)
			THROW_EXCEPTION(POGLStateException, "You cannot map with offset: %d and length: %d when the vertex buffer size is: %d", offset, length, memorySize);
		
		POGL_MAPRANGEVERTEXBUFFER_COMMAND_DATA* cmd = (POGL_MAPRANGEVERTEXBUFFER_COMMAND_DATA*)AddCommand(&POGLMapRangeVertexBuffer_Command, &POGLMapRangeVertexBuffer_Release,
			sizeof(POGL_MAPRANGEVERTEXBUFFER_COMMAND_DATA));
		cmd->offset = offset;
		cmd->length = length;
		cmd->memoryOffset = GetMapOffset(memorySize);
		cmd->vertexBuffer = vb;
		cmd->vertexBuffer->AddRef();
		mMapping = true;
		return GetMapPointer(cmd->memoryOffset);
	}

	THROW_NOT_IMPLEMENTED_EXCEPTION();
}

void POGLDeferredRenderContext::Unmap(IPOGLResource* resource)
{
	if (!mMapping)
		THROW_EXCEPTION(POGLStateException, "You are not allowed to unmap a non-mapped resource");

	auto type = resource->GetType();
	if (type == POGLResourceType::VERTEXBUFFER) {
		mMapping = false;
		return;
	}

	THROW_NOT_IMPLEMENTED_EXCEPTION();
}

void POGLDeferredRenderContext::SetViewport(const POGL_RECT& viewport)
{
	mRenderState->SetViewport(viewport);
}

void POGLDeferredRenderContext::ExecuteCommands(IPOGLRenderContext* context)
{
	ExecuteCommands(context, true);
}

POGL_UINT32 POGLDeferredRenderContext::GetMapOffset(POGL_UINT32 size)
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

POGL_HANDLE POGLDeferredRenderContext::GetMapPointer(POGL_UINT32 offset)
{
	return (char*)mMapMemoryPool + offset;
}

void POGLDeferredRenderContext::ExecuteCommands(IPOGLRenderContext* context, bool clearCommands)
{
	// Execute the deferred render commands and then return the allocated command-pointers to the memory pool
	auto renderState = static_cast<POGLRenderContext*>(context)->GetRenderState();

	//
	// Retrieve the flushed commands if they exists and execute them
	//

	std::lock_guard<std::mutex> lock(mFlushedCommandsMutex);
	FOR_EACH_COMMAND(mFlushedCommands, mFlushedCommandsSize)
		(*command->function)(this, renderState, ptr);
		(*command->releaseFunction)(ptr);
		command->function = &POGLNothing_Command;
		command->releaseFunction = &POGLNothing_Release;
	END_FOR_COMMANDS()

	//
	// Free the commands
	//

	if (clearCommands) {
		mFlushedCommandsSize = 0;
		mFlushedCommands = nullptr;
	}
}

void POGLDeferredRenderContext::Flush()
{
	// Ensure that the currently assigned states are unset
	mRenderState->Flush();

	mFlushedCommandsMutex.lock();
	mFlushedCommands = mMemoryPool;
	mFlushedCommandsSize = mMemoryPoolOffset;
	mMemoryPoolOffset = 0;
	mMapMemoryPoolOffset = 0;
	mFlushedCommandsMutex.unlock();
}

POGL_HANDLE POGLDeferredRenderContext::AddCommand(POGLCommandFuncPtr function, POGLCommandReleaseFuncPtr releaseFunction, POGL_UINT32 size)
{
	// Calculate the total memory (bytes) required by the command
	const POGL_UINT32 memoryRequired = mMemoryPoolOffset + POGL_DEFERRED_COMMAND_SIZE + size;

	if (memoryRequired > mMemoryPoolSize) {
		mMemoryPoolSize += POGL_DEFERRED_COMMAND_RESIZE_SIZE;
		mMemoryPool = (POGL_BYTE*)realloc(mMemoryPool, mMemoryPoolSize);
	}

	// Retrieve the command item
	POGL_DEFERRED_COMMAND* item = (POGL_DEFERRED_COMMAND*)OFFSET_PTR(mMemoryPool, mMemoryPoolOffset);
	item->function = function;
	item->releaseFunction = releaseFunction;
	item->size = size;

	// Pointer where we can begin put the command data
	POGL_HANDLE dataMemoryBlock = OFFSET_PTR(mMemoryPool, mMemoryPoolOffset + POGL_DEFERRED_COMMAND_SIZE);

	// Move the offset pointer to the next free area in the memory block
	mMemoryPoolOffset = memoryRequired;

	// Return the memory block for the data
	return dataMemoryBlock;
}
