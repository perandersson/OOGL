#include "MemCheck.h"
#include "POGLDeferredCommands.h"
#include "POGLFactory.h"
#include "POGLVertexBuffer.h"
#include "POGLRenderState.h"
#include "POGLDeferredDeviceContext.h"
#include "POGLFramebuffer.h"

void POGLCreateVertexBuffer_Command(class POGLDeferredDeviceContext* context, POGLRenderState* state, POGLDeferredCommand* command)
{
	POGLCreateVertexBufferCommand* cmd = (POGLCreateVertexBufferCommand*)command;

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
	POGLMapVertexBufferCommand* cmd = (POGLMapVertexBufferCommand*)command;
	state->BindVertexBuffer(cmd->vertexBuffer);
	void* map = glMapBufferRange(GL_ARRAY_BUFFER, 0, cmd->size, GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT);
	memcpy(map, context->GetMapPointer(cmd->memoryPoolOffset), cmd->size);
	glUnmapBuffer(GL_ARRAY_BUFFER);
	cmd->vertexBuffer->Release();
}

void POGLMapRangeVertexBuffer_Command(class POGLDeferredDeviceContext* context, POGLRenderState* state, POGLDeferredCommand* command)
{
	POGLMapRangeVertexBufferCommand* cmd = (POGLMapRangeVertexBufferCommand*)command;
	state->BindVertexBuffer(cmd->vertexBuffer);
	void* map = glMapBufferRange(GL_ARRAY_BUFFER, cmd->offset, cmd->length, GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT);
	memcpy(map, context->GetMapPointer(cmd->memoryPoolOffset), cmd->length);
	glUnmapBuffer(GL_ARRAY_BUFFER);
	cmd->vertexBuffer->Release();
}

void POGLClear_Command(class POGLDeferredDeviceContext* context, POGLRenderState* state, POGLDeferredCommand* command)
{
	POGLClearCommand* cmd = (POGLClearCommand*)command;
	state->Clear(cmd->clearBits);
}

void POGLSetFramebuffer_Command(class POGLDeferredDeviceContext* context, POGLRenderState* state, POGLDeferredCommand* command)
{
	POGLSetFramebufferCommand* cmd = (POGLSetFramebufferCommand*)command;
	state->SetFramebuffer(cmd->framebuffer);
	if (cmd->framebuffer != nullptr)
		cmd->framebuffer->Release();
}

void POGLDraw_Command(class POGLDeferredDeviceContext* context, POGLRenderState* state, POGLDeferredCommand* command)
{
	POGLDeferredDrawCommand* cmd = (POGLDeferredDrawCommand*)command;
	state->Draw(cmd->vertexBuffer, cmd->indexBuffer, cmd->startIndex);
	cmd->vertexBuffer->Release();
	if (cmd->indexBuffer != nullptr)
		cmd->indexBuffer->Release();
}

void POGLDrawCount_Command(class POGLDeferredDeviceContext* context, POGLRenderState* state, POGLDeferredCommand* command)
{
	POGLDeferredDrawCommand* cmd = (POGLDeferredDrawCommand*)command;
	state->Draw(cmd->vertexBuffer, cmd->indexBuffer, cmd->startIndex, cmd->count);
	cmd->vertexBuffer->Release();
	if (cmd->indexBuffer != nullptr)
		cmd->indexBuffer->Release();
}

void POGLSetDepthTest_Command(class POGLDeferredDeviceContext* context, POGLRenderState* state, POGLDeferredCommand* command)
{
	POGLBooleanCommand* cmd = (POGLBooleanCommand*)command;
	state->SetDepthTest(cmd->value);
}

void POGLSetDepthMask_Command(class POGLDeferredDeviceContext* context, POGLRenderState* state, POGLDeferredCommand* command)
{
	POGLBooleanCommand* cmd = (POGLBooleanCommand*)command;
	state->SetDepthMask(cmd->value);
}

void POGLSetStencilTest_Command(class POGLDeferredDeviceContext* context, POGLRenderState* state, POGLDeferredCommand* command)
{
	POGLBooleanCommand* cmd = (POGLBooleanCommand*)command;
	state->SetStencilTest(cmd->value);
}

void POGLSetBlend_Command(class POGLDeferredDeviceContext* context, POGLRenderState* state, POGLDeferredCommand* command)
{
	POGLBooleanCommand* cmd = (POGLBooleanCommand*)command;
	state->SetBlend(cmd->value);
}

void POGLColorMask_Command(class POGLDeferredDeviceContext* context, POGLRenderState* state, POGLDeferredCommand* command)
{
	POGLColorMaskCommand* cmd = (POGLColorMaskCommand*)command;
	state->SetColorMask(cmd->mask);
}

void POGLSetDepthFunc_Command(class POGLDeferredDeviceContext* context, POGLRenderState* state, POGLDeferredCommand* command)
{
	POGLSetDepthFuncCommand* cmd = (POGLSetDepthFuncCommand*)command;
	state->SetDepthFunc(cmd->depthFunc);
}

void POGLSetBlendFunc_Command(class POGLDeferredDeviceContext* context, POGLRenderState* state, POGLDeferredCommand* command)
{
	POGLSetBlendFuncCommand* cmd = (POGLSetBlendFuncCommand*)command;
	state->SetBlendFunc(cmd->sfactor, cmd->dfactor);
}

void POGLSetViewport_Command(class POGLDeferredDeviceContext* context, POGLRenderState* state, POGLDeferredCommand* command)
{
	POGLSetViewportCommand* cmd = (POGLSetViewportCommand*)command;
	state->SetViewport(cmd->viewport);
}

void POGLApplyEffect_Command(class POGLDeferredDeviceContext* context, POGLRenderState* state, POGLDeferredCommand* command)
{
	POGLApplyEffectCommand* cmd = (POGLApplyEffectCommand*)command;
	state->Apply(cmd->effect);
	cmd->effect->Release();
	state->Release();
}
