#include "MemCheck.h"
#include "POGLDeferredCommands.h"
#include "POGLFactory.h"
#include "POGLVertexBuffer.h"
#include "POGLRenderState.h"
#include "POGLDeferredDeviceContext.h"

void POGLCreateVertexBuffer_Command(class POGLDeferredDeviceContext* context, POGLRenderState* state, POGLDeferredCommand* command)
{
	POGLDeferredCreateVertexBufferCommand* cmd = (POGLDeferredCreateVertexBufferCommand*)command;

	//
	// Generate buffers and attach the vertex buffers layout to it
	//

	const GLuint bufferID = POGLFactory::GenBufferID();
	const GLuint vaoID = POGLFactory::GenVertexArrayObjectID(bufferID, cmd->vertexBuffer->GetLayout());
	cmd->vertexBuffer->PostConstruct(bufferID, vaoID);

	//
	// Set the vertex buffer as the "current buffer" on the render state
	//

	state->SetVertexBuffer(cmd->vertexBuffer);

	//
	// Map the data
	//

	void* map = glMapBufferRange(GL_ARRAY_BUFFER, 0, cmd->size, GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT);
	memcpy(map, context->GetMapPointer(cmd->memoryPoolOffset), cmd->size);
	glUnmapBuffer(GL_ARRAY_BUFFER);

	//
	// Release the vertex buffer
	//

	cmd->vertexBuffer->Release();
}

void POGLMapVertexBuffer_Command(class POGLDeferredDeviceContext* context, POGLRenderState* state, POGLDeferredCommand* command)
{
	POGLDeferredMapVertexBufferCommand* cmd = (POGLDeferredMapVertexBufferCommand*)command;
	state->BindVertexBuffer(cmd->vertexBuffer);
	void* map = glMapBufferRange(GL_ARRAY_BUFFER, 0, cmd->size, GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT);
	memcpy(map, context->GetMapPointer(cmd->memoryPoolOffset), cmd->size);
	glUnmapBuffer(GL_ARRAY_BUFFER);
	cmd->vertexBuffer->Release();
}

void POGLMapRangeVertexBuffer_Command(class POGLDeferredDeviceContext* context, POGLRenderState* state, POGLDeferredCommand* command)
{
	POGLDeferredMapRangeVertexBufferCommand* cmd = (POGLDeferredMapRangeVertexBufferCommand*)command;
	state->BindVertexBuffer(cmd->vertexBuffer);
	void* map = glMapBufferRange(GL_ARRAY_BUFFER, cmd->offset, cmd->length, GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT);
	memcpy(map, context->GetMapPointer(cmd->memoryPoolOffset), cmd->length);
	glUnmapBuffer(GL_ARRAY_BUFFER);
	cmd->vertexBuffer->Release();
}
