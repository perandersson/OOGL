#include "MemCheck.h"
#include "POGLDeferredCommands.h"
#include "POGLFactory.h"
#include "POGLVertexBuffer.h"
#include "POGLTexture2D.h"
#include "POGLRenderState.h"
#include "POGLDeferredDeviceContext.h"
#include "POGLFramebuffer.h"
#include "POGLEnum.h"

void POGLNothing_Release(POGLDeferredCommand* command)
{
}

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
}

void POGLCreateVertexBuffer_Release(POGLDeferredCommand* command)
{
	POGLCreateVertexBufferCommand* cmd = (POGLCreateVertexBufferCommand*)command;
	cmd->vertexBuffer->Release();
	cmd->releaseFunction = &POGLNothing_Release;
}

void POGLCreateTexture2D_Command(class POGLDeferredDeviceContext* context, POGLRenderState* state, POGLDeferredCommand* command)
{
	POGLCreateTexture2DCommand* cmd = (POGLCreateTexture2DCommand*)command;

	const POGLTextureFormat::Enum format = cmd->texture->GetTextureFormat();
	const POGL_SIZEI& size = cmd->texture->GetSize();
	const GLenum _format = POGLEnum::ConvertToTextureFormatEnum(format);
	const GLenum _internalFormat = POGLEnum::ConvertToInternalTextureFormatEnum(format);
	const GLenum minFilter = POGLEnum::Convert(POGLMinFilter::DEFAULT);
	const GLenum magFilter = POGLEnum::Convert(POGLMagFilter::DEFAULT);
	const GLenum textureWrap = POGLEnum::Convert(POGLTextureWrap::DEFAULT);

	const GLuint textureID = POGLFactory::GenTextureID();
	cmd->texture->PostConstruct(textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	POGL_HANDLE pointer = nullptr;
	if (cmd->size > 0) {
		pointer = context->GetMapPointer(cmd->memoryPoolOffset);
	}

	glTexImage2D(GL_TEXTURE_2D, 0, _internalFormat, size.width, size.height, 0, _format, GL_UNSIGNED_BYTE, pointer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrap);

	const GLenum status = glGetError();
	if (status != GL_NO_ERROR) {
		THROW_EXCEPTION(POGLResourceException, "Could not create 2D texture. Reason: 0x%x", status);
	}

	state->SetTextureResource((POGLTextureResource*)cmd->texture->GetResourcePtr());
}

void POGLCreateTexture2D_Release(POGLDeferredCommand* command)
{
	POGLCreateTexture2DCommand* cmd = (POGLCreateTexture2DCommand*)command;
	cmd->texture->Release();
	cmd->releaseFunction = &POGLNothing_Release;
}

void POGLMapVertexBuffer_Command(class POGLDeferredDeviceContext* context, POGLRenderState* state, POGLDeferredCommand* command)
{
	POGLMapVertexBufferCommand* cmd = (POGLMapVertexBufferCommand*)command;
	state->BindVertexBuffer(cmd->vertexBuffer);
	void* map = glMapBufferRange(GL_ARRAY_BUFFER, 0, cmd->size, GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT);
	memcpy(map, context->GetMapPointer(cmd->memoryPoolOffset), cmd->size);
	glUnmapBuffer(GL_ARRAY_BUFFER);
}

void POGLMapVertexBuffer_Release(POGLDeferredCommand* command)
{
	POGLMapVertexBufferCommand* cmd = (POGLMapVertexBufferCommand*)command;
	cmd->vertexBuffer->Release();
	cmd->releaseFunction = &POGLNothing_Release;
}

void POGLMapRangeVertexBuffer_Command(class POGLDeferredDeviceContext* context, POGLRenderState* state, POGLDeferredCommand* command)
{
	POGLMapRangeVertexBufferCommand* cmd = (POGLMapRangeVertexBufferCommand*)command;
	state->BindVertexBuffer(cmd->vertexBuffer);
	void* map = glMapBufferRange(GL_ARRAY_BUFFER, cmd->offset, cmd->length, GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT);
	memcpy(map, context->GetMapPointer(cmd->memoryPoolOffset), cmd->length);
	glUnmapBuffer(GL_ARRAY_BUFFER);
}

void POGLMapRangeVertexBuffer_Release(POGLDeferredCommand* command)
{
	POGLMapRangeVertexBufferCommand* cmd = (POGLMapRangeVertexBufferCommand*)command;
	cmd->vertexBuffer->Release();
	cmd->releaseFunction = &POGLNothing_Release;
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
}

void POGLSetFramebuffer_Release(POGLDeferredCommand* command)
{
	POGLSetFramebufferCommand* cmd = (POGLSetFramebufferCommand*)command;
	if (cmd->framebuffer != nullptr)
		cmd->framebuffer->Release();
	cmd->releaseFunction = &POGLNothing_Release;
}

void POGLDraw_Command(class POGLDeferredDeviceContext* context, POGLRenderState* state, POGLDeferredCommand* command)
{
	POGLDeferredDrawCommand* cmd = (POGLDeferredDrawCommand*)command;
	state->Draw(cmd->vertexBuffer, cmd->indexBuffer, cmd->startIndex);
}

void POGLDrawCount_Command(class POGLDeferredDeviceContext* context, POGLRenderState* state, POGLDeferredCommand* command)
{
	POGLDeferredDrawCommand* cmd = (POGLDeferredDrawCommand*)command;
	state->Draw(cmd->vertexBuffer, cmd->indexBuffer, cmd->startIndex, cmd->count);
}

void POGLDraw_Release(POGLDeferredCommand* command)
{
	POGLDeferredDrawCommand* cmd = (POGLDeferredDrawCommand*)command;
	cmd->vertexBuffer->Release();
	if (cmd->indexBuffer != nullptr)
		cmd->indexBuffer->Release();
	cmd->releaseFunction = &POGLNothing_Release;
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
}

void POGLApplyEffect_Release(POGLDeferredCommand* command)
{
	POGLApplyEffectCommand* cmd = (POGLApplyEffectCommand*)command;
	cmd->effect->Release();
	cmd->releaseFunction = &POGLNothing_Release;
}