#include "MemCheck.h"
#include "POGLDeferredDeviceContext.h"
#include "POGLVertexBuffer.h"
#include "POGLEnum.h"
#include "POGLRenderState.h"
#include "POGLDeviceContext.h"

POGLDeferredDeviceContext::POGLDeferredDeviceContext(IPOGLDevice* device)
: mRefCount(1), mDevice(device), mCommandsToExecuteHead(nullptr), mCommandsToExecuteTail(nullptr), mFreeCommands(nullptr), mReleasedCommands(nullptr), mMap(nullptr)
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
		auto command = mReleasedCommands;
		while (command != nullptr) {
			auto next = command->tail;
			(*command->releaseFunction)(this, command);
			free(command);
			command = next;
		}

		command = mFreeCommands;
		while (command != nullptr) {
			auto next = command->tail;
			free(command);
			command = next;
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

void POGLCreateVertexBuffer_Command(POGLRenderState* state, POGLDeferredCommand* command)
{
	POGLDeferredCreateVertexBufferCommand* cmd = (POGLDeferredCreateVertexBufferCommand*)command;
	state->BindVertexBuffer(cmd->vertexBuffer);
	void* map = glMapBufferRange(GL_ARRAY_BUFFER, 0, cmd->size, GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT);
	memcpy(map, cmd->memory, cmd->size);
	glUnmapBuffer(GL_ARRAY_BUFFER);
}

void POGLCreateVertexBuffer_Release(class POGLDeferredDeviceContext*, struct POGLDeferredCommand* command)
{
	POGLDeferredCreateVertexBufferCommand* cmd = (POGLDeferredCreateVertexBufferCommand*)command;
	// TODO: Add memory pool
	free(cmd->memory);
}

void POGLMapVertexBuffer_Command(POGLRenderState* state, POGLDeferredCommand* command)
{
	POGLDeferredMapVertexBufferCommand* cmd = (POGLDeferredMapVertexBufferCommand*)command;
	state->BindVertexBuffer(cmd->vertexBuffer);
	void* map = glMapBufferRange(GL_ARRAY_BUFFER, 0, cmd->size, GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT);
	memcpy(map, cmd->memory, cmd->size);
	glUnmapBuffer(GL_ARRAY_BUFFER);
}

void POGLMapVertexBuffer_Release(class POGLDeferredDeviceContext*, struct POGLDeferredCommand* command)
{
	POGLDeferredMapVertexBufferCommand* cmd = (POGLDeferredMapVertexBufferCommand*)command;
	// TODO: Add memory pool
	free(cmd->memory);
}

void POGLMapRangeVertexBuffer_Command(POGLRenderState* state, POGLDeferredCommand* command)
{
	POGLDeferredMapRangeVertexBufferCommand* cmd = (POGLDeferredMapRangeVertexBufferCommand*)command;
	state->BindVertexBuffer(cmd->vertexBuffer);
	void* map = glMapBufferRange(GL_ARRAY_BUFFER, cmd->offset, cmd->length, GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT);
	memcpy(map, cmd->memory, cmd->length);
	glUnmapBuffer(GL_ARRAY_BUFFER);
}

void POGLMapRangeVertexBuffer_Release(class POGLDeferredDeviceContext*, struct POGLDeferredCommand* command)
{
	POGLDeferredMapRangeVertexBufferCommand* cmd = (POGLDeferredMapRangeVertexBufferCommand*)command;
	// TODO: Add memory pool
	free(cmd->memory);
}

IPOGLVertexBuffer* POGLDeferredDeviceContext::CreateVertexBuffer(const void* memory, POGL_SIZE memorySize, const POGL_VERTEX_LAYOUT* layout, POGLPrimitiveType::Enum primitiveType, POGLBufferUsage::Enum bufferUsage)
{
	const POGL_UINT32 numVertices = memorySize / layout->vertexSize;
	const GLenum usage = POGLEnum::Convert(bufferUsage);
	const GLenum type = POGLEnum::Convert(primitiveType);

	POGLVertexBuffer* vb = new POGLVertexBuffer(0, numVertices, 0, layout, type, bufferUsage);
	POGLDeferredCreateVertexBufferCommand* command = (POGLDeferredCreateVertexBufferCommand*)AllocCommand(&POGLCreateVertexBuffer_Command, &POGLCreateVertexBuffer_Release);
	command->vertexBuffer = vb;
	command->memory = malloc(memorySize); memcpy(command->memory, memory, memorySize);
	command->size = memorySize;
	AddCommand((POGLDeferredCommand*)command);
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

		POGLDeferredMapVertexBufferCommand* map = (POGLDeferredMapVertexBufferCommand*)AllocCommand(&POGLMapVertexBuffer_Command, &POGLMapVertexBuffer_Release);
		map->size = vb->GetCount() * vb->GetLayout()->vertexSize;
		map->memory = malloc(map->size);
		map->vertexBuffer = vb;
		mMap = (POGLDeferredCommand*)map;
		return map->memory;
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

		POGLDeferredMapRangeVertexBufferCommand* map = (POGLDeferredMapRangeVertexBufferCommand*)AllocCommand(&POGLMapRangeVertexBuffer_Command, &POGLMapRangeVertexBuffer_Release);
		map->offset = offset;
		map->length = length;
		map->memory = malloc(length);
		map->vertexBuffer = vb;
		mMap = (POGLDeferredCommand*)map;
		return map->memory;
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
		AddCommand((POGLDeferredCommand*)mMap);
		mMap = nullptr;
		return;
	}

	THROW_EXCEPTION(POGLInitializationException, "Not implemented");
}

POGLDeferredCommand* POGLDeferredDeviceContext::AllocCommand(POGLCommandFuncPtr function, POGLReleaseCommandFuncPtr releaseFunction)
{
	POGLDeferredCommand* command = nullptr;
	mFreeMutex.lock();
	command = mFreeCommands;
	if (command == nullptr) {
		command = (POGLDeferredCommand*)malloc(sizeof(POGLDeferredCommand));
		memset(command, 0, sizeof(POGLDeferredCommand));
		command->function = function;
		command->releaseFunction = releaseFunction;
	}
	mFreeCommands = command->tail;
	command->tail = nullptr; // Detach the command from the linked list
	mFreeMutex.unlock();
	return command;
}

void POGLDeferredDeviceContext::AddCommand(POGLDeferredCommand* command)
{
	//
	// Release any commands if any exists
	//

	ReleaseCommands();

	//
	// Add the commands to the "to be executed" queue
	//

	mCommandsMutex.lock();
	if (mCommandsToExecuteHead == nullptr) {
		mCommandsToExecuteHead = command;
	}

	if (mCommandsToExecuteTail == nullptr) {
		mCommandsToExecuteTail = command;
	}
	else {
		mCommandsToExecuteTail->tail = command;
		mCommandsToExecuteTail = command;
	}
	mCommandsMutex.unlock();
}

void POGLDeferredDeviceContext::ExecuteCommands(IPOGLDeviceContext* context)
{
	ExecuteCommands(context, true);
}

POGLDeferredCommand* POGLDeferredDeviceContext::GetCommands()
{
	POGLDeferredCommand* commandList = nullptr;
	mCommandsMutex.lock();
	commandList = mCommandsToExecuteHead;
	mCommandsToExecuteHead = mCommandsToExecuteTail = nullptr;
	mCommandsMutex.unlock();
	return commandList;
}

void POGLDeferredDeviceContext::FreeCommands(POGLDeferredCommand* commands)
{
	mFreeMutex.lock();
	if (mReleasedCommands != nullptr)
		mReleasedCommands->tail = commands;
	else
		mReleasedCommands = commands;
	mFreeMutex.unlock();
}

void POGLDeferredDeviceContext::ReleaseCommands()
{
	if (mReleasedCommands == nullptr)
		return;

	//
	// Get the commands to be released
	//

	mFreeMutex.lock();
	POGLDeferredCommand* commands = mReleasedCommands;
	mReleasedCommands = nullptr;
	mFreeMutex.unlock();

	//
	// Release all commands
	//

	POGLDeferredCommand* command = commands;
	while (command != nullptr) {
		(*command->releaseFunction)(this, command);
		command = command->tail;
	}

	//
	// Put the released commands back to the queue
	//

	mFreeMutex.lock();
	if (mFreeCommands != nullptr)
		mFreeCommands->tail = commands;
	else
		mFreeCommands = commands;
	mFreeMutex.unlock();
}

void POGLDeferredDeviceContext::ExecuteCommands(IPOGLDeviceContext* context, bool clearCommands)
{
	// Execute the deferred render commands and then return the allocated command-pointers to the memory pool
	auto renderState = static_cast<POGLDeviceContext*>(context)->GetRenderState();

	//
	// Execute the commands
	//

	POGLDeferredCommand* commands = GetCommands();
	POGLDeferredCommand* command = commands;
	while (command != nullptr) {
		(*command->function)(renderState, command);
		command = command->tail;
	}

	//
	// Free the commands
	//

	FreeCommands(commands);
	mBeginEndCondition.notify_all();
}
