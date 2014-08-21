#include "MemCheck.h"
#include "POGLDeferredCommands.h"
#include "POGLFactory.h"
#include "POGLVertexBuffer.h"
#include "POGLTexture2D.h"
#include "POGLRenderState.h"
#include "POGLDeferredRenderContext.h"
#include "POGLFramebuffer.h"
#include "POGLEnum.h"

void POGLNothing_Release(POGL_HANDLE)
{
}

void POGLNothing_Command(POGLDeferredRenderContext*, POGLRenderState*, POGL_HANDLE)
{
}

void POGLCreateVertexBuffer_Command(POGLDeferredRenderContext* context, POGLRenderState* state, POGL_HANDLE command)
{
	POGL_CREATEVERTEXBUFFER_COMMAND_DATA* cmd = (POGL_CREATEVERTEXBUFFER_COMMAND_DATA*)command;

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

	void* map = glMapBufferRange(GL_ARRAY_BUFFER, 0, cmd->dataSize, GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT);
	memcpy(map, context->GetMapPointer(cmd->memoryOffset), cmd->dataSize);
	glUnmapBuffer(GL_ARRAY_BUFFER);
}

void POGLCreateVertexBuffer_Release(POGL_HANDLE command)
{
	POGL_CREATEVERTEXBUFFER_COMMAND_DATA* cmd = (POGL_CREATEVERTEXBUFFER_COMMAND_DATA*)command;
	cmd->vertexBuffer->Release();
}

void POGLCreateTexture2D_Command(POGLDeferredRenderContext* context, POGLRenderState* state, POGL_HANDLE command)
{
	POGL_CREATETEXTURE2D_COMMAND_DATA* cmd = (POGL_CREATETEXTURE2D_COMMAND_DATA*)command;

	const POGLTextureFormat::Enum format = cmd->texture->GetTextureFormat();
	const POGL_SIZE& size = cmd->texture->GetSize();
	const GLenum _format = POGLEnum::ConvertToTextureFormatEnum(format);
	const GLenum _internalFormat = POGLEnum::ConvertToInternalTextureFormatEnum(format);
	const GLenum minFilter = POGLEnum::Convert(POGLMinFilter::DEFAULT);
	const GLenum magFilter = POGLEnum::Convert(POGLMagFilter::DEFAULT);
	const GLenum textureWrap = POGLEnum::Convert(POGLTextureWrap::DEFAULT);

	const GLuint textureID = POGLFactory::GenTextureID();
	cmd->texture->PostConstruct(textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	POGL_HANDLE pointer = nullptr;
	if (cmd->dataSize > 0)
		pointer = context->GetMapPointer(cmd->memoryOffset);

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

void POGLCreateTexture2D_Release(POGL_HANDLE command)
{
	POGL_CREATETEXTURE2D_COMMAND_DATA* cmd = (POGL_CREATETEXTURE2D_COMMAND_DATA*)command;
	cmd->texture->Release();
}

void POGLMapVertexBuffer_Command(POGLDeferredRenderContext* context, POGLRenderState* state, POGL_HANDLE command)
{
	POGL_MAPVERTEXBUFFER_COMMAND_DATA* cmd = (POGL_MAPVERTEXBUFFER_COMMAND_DATA*)command;
	state->BindVertexBuffer(cmd->vertexBuffer);
	void* map = glMapBufferRange(GL_ARRAY_BUFFER, 0, cmd->dataSize, GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT);
	memcpy(map, context->GetMapPointer(cmd->memoryOffset), cmd->dataSize);
	glUnmapBuffer(GL_ARRAY_BUFFER);
}

void POGLMapVertexBuffer_Release(POGL_HANDLE command)
{
	POGL_MAPVERTEXBUFFER_COMMAND_DATA* cmd = (POGL_MAPVERTEXBUFFER_COMMAND_DATA*)command;
	cmd->vertexBuffer->Release();
}

void POGLMapRangeVertexBuffer_Command(POGLDeferredRenderContext* context, POGLRenderState* state, POGL_HANDLE command)
{
	POGL_MAPRANGEVERTEXBUFFER_COMMAND_DATA* cmd = (POGL_MAPRANGEVERTEXBUFFER_COMMAND_DATA*)command;
	state->BindVertexBuffer(cmd->vertexBuffer);
	void* map = glMapBufferRange(GL_ARRAY_BUFFER, cmd->offset, cmd->length, GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT);
	memcpy(map, context->GetMapPointer(cmd->memoryOffset), cmd->length);
	glUnmapBuffer(GL_ARRAY_BUFFER);
}

void POGLMapRangeVertexBuffer_Release(POGL_HANDLE command)
{
	POGL_MAPRANGEVERTEXBUFFER_COMMAND_DATA* cmd = (POGL_MAPRANGEVERTEXBUFFER_COMMAND_DATA*)command;
	cmd->vertexBuffer->Release();
}

void POGLClear_Command(POGLDeferredRenderContext* context, POGLRenderState* state, POGL_HANDLE command)
{
	POGL_CLEAR_COMMAND_DATA* cmd = (POGL_CLEAR_COMMAND_DATA*)command;
	state->Clear(cmd->clearBits);
}

void POGLSetFramebuffer_Command(POGLDeferredRenderContext* context, POGLRenderState* state, POGL_HANDLE command)
{
	POGL_SETFRAMEBUFFER_COMMAND_DATA* cmd = (POGL_SETFRAMEBUFFER_COMMAND_DATA*)command;
	state->SetFramebuffer(cmd->framebuffer);
}

void POGLSetFramebuffer_Release(POGL_HANDLE command)
{
	POGL_SETFRAMEBUFFER_COMMAND_DATA* cmd = (POGL_SETFRAMEBUFFER_COMMAND_DATA*)command;
	if (cmd->framebuffer != nullptr)
		cmd->framebuffer->Release();
}

void POGLDraw_Command(POGLDeferredRenderContext* context, POGLRenderState* state, POGL_HANDLE command)
{
	POGL_DRAW_COMMAND_DATA* cmd = (POGL_DRAW_COMMAND_DATA*)command;
	state->Draw(cmd->vertexBuffer, cmd->indexBuffer, cmd->startIndex);
}

void POGLDrawCount_Command(POGLDeferredRenderContext* context, POGLRenderState* state, POGL_HANDLE command)
{
	POGL_DRAW_COMMAND_DATA* cmd = (POGL_DRAW_COMMAND_DATA*)command;
	state->Draw(cmd->vertexBuffer, cmd->indexBuffer, cmd->startIndex, cmd->count);
}

void POGLDraw_Release(POGL_HANDLE command)
{
	POGL_DRAW_COMMAND_DATA* cmd = (POGL_DRAW_COMMAND_DATA*)command;
	cmd->vertexBuffer->Release();
	if (cmd->indexBuffer != nullptr)
		cmd->indexBuffer->Release();
}

void POGLSetDepthTest_Command(POGLDeferredRenderContext* context, POGLRenderState* state, POGL_HANDLE command)
{
	POGL_BOOLEAN_COMMAND_DATA* cmd = (POGL_BOOLEAN_COMMAND_DATA*)command;
	state->SetDepthTest(cmd->value);
}

void POGLSetDepthMask_Command(POGLDeferredRenderContext* context, POGLRenderState* state, POGL_HANDLE command)
{
	POGL_BOOLEAN_COMMAND_DATA* cmd = (POGL_BOOLEAN_COMMAND_DATA*)command;
	state->SetDepthMask(cmd->value);
}

void POGLSetStencilTest_Command(POGLDeferredRenderContext* context, POGLRenderState* state, POGL_HANDLE command)
{
	POGL_BOOLEAN_COMMAND_DATA* cmd = (POGL_BOOLEAN_COMMAND_DATA*)command;
	state->SetStencilTest(cmd->value);
}

void POGLSetBlend_Command(POGLDeferredRenderContext* context, POGLRenderState* state, POGL_HANDLE command)
{
	POGL_BOOLEAN_COMMAND_DATA* cmd = (POGL_BOOLEAN_COMMAND_DATA*)command;
	state->SetBlend(cmd->value);
}

void POGLColorMask_Command(POGLDeferredRenderContext* context, POGLRenderState* state, POGL_HANDLE command)
{
	POGL_COLORMASK_COMMAND_DATA* cmd = (POGL_COLORMASK_COMMAND_DATA*)command;
	state->SetColorMask(cmd->mask);
}

void POGLStencilMask_Command(POGLDeferredRenderContext* context, POGLRenderState* state, POGL_HANDLE command)
{
	POGL_STENCILMASK_COMMAND_DATA* cmd = (POGL_STENCILMASK_COMMAND_DATA*)command;
	state->SetStencilMask(cmd->mask);
}

void POGLSetDepthFunc_Command(POGLDeferredRenderContext* context, POGLRenderState* state, POGL_HANDLE command)
{
	POGL_SETDEPTHFUNC_COMMAND_DATA* cmd = (POGL_SETDEPTHFUNC_COMMAND_DATA*)command;
	state->SetDepthFunc(cmd->depthFunc);
}

void POGLSetBlendFunc_Command(POGLDeferredRenderContext* context, POGLRenderState* state, POGL_HANDLE command)
{
	POGL_SETBLENDFUNC_COMMAND_DATA* cmd = (POGL_SETBLENDFUNC_COMMAND_DATA*)command;
	state->SetBlendFunc(cmd->sfactor, cmd->dfactor);
}

void POGLSetFrontFace_Command(POGLDeferredRenderContext* context, POGLRenderState* state, POGL_HANDLE command)
{
	POGL_SETFRONTFACE_COMMAND_DATA* cmd = (POGL_SETFRONTFACE_COMMAND_DATA*)command;
	state->SetFrontFace(cmd->frontFace);
}

void POGLSetCullFace_Command(POGLDeferredRenderContext* context, POGLRenderState* state, POGL_HANDLE command)
{
	POGL_SETCULLFACE_COMMAND_DATA* cmd = (POGL_SETCULLFACE_COMMAND_DATA*)command;
	state->SetCullFace(cmd->cullFace);
}

void POGLSetViewport_Command(POGLDeferredRenderContext* context, POGLRenderState* state, POGL_HANDLE command)
{
	POGL_SETVIEWPORT_COMMAND_DATA* cmd = (POGL_SETVIEWPORT_COMMAND_DATA*)command;
	state->SetViewport(cmd->viewport);
}

void POGLApplyProgram_Command(POGLDeferredRenderContext* context, POGLRenderState* state, POGL_HANDLE command)
{
	POGL_APPLYPROGRAM_COMMAND* cmd = (POGL_APPLYPROGRAM_COMMAND*)command;
	state->Apply(cmd->program);
}

void POGLApplyProgram_Release(POGL_HANDLE command)
{
	POGL_APPLYPROGRAM_COMMAND* cmd = (POGL_APPLYPROGRAM_COMMAND*)command;
	cmd->program->Release();
}

void POGLCreateFrameBuffer_Command(POGLDeferredRenderContext* context, POGLRenderState* state, POGL_HANDLE command)
{
	POGL_CREATEFRAMEBUFFER_COMMAND_DATA* cmd = (POGL_CREATEFRAMEBUFFER_COMMAND_DATA*)command;

	IPOGLTexture* textures[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	const POGL_UINT32 size = cmd->framebuffer->GetNumDrawBuffers();
	for (POGL_UINT32 i = 0; i < size; ++i) {
		IPOGLTexture* texture = cmd->framebuffer->GetTexture(i);
		textures[i] = texture;
		texture->Release();
	}

	IPOGLTexture* depthStencilTexture = cmd->framebuffer->GetDepthStencilTexture();
	GLuint frameBufferID = POGLFactory::GenFramebufferObjectID(textures, depthStencilTexture);
	cmd->framebuffer->PostConstruct(frameBufferID);
	depthStencilTexture->Release();
	
	state->SetFramebuffer(cmd->framebuffer);
}

void POGLCreateFrameBuffer_Release(POGL_HANDLE command)
{
	POGL_CREATEFRAMEBUFFER_COMMAND_DATA* cmd = (POGL_CREATEFRAMEBUFFER_COMMAND_DATA*)command;
	cmd->framebuffer->Release();
}

void POGLResizeTexture2D_Command(POGLDeferredRenderContext* context, POGLRenderState* state, POGL_HANDLE command)
{
	POGL_RESIZETEXTURE2D_COMMAND_DATA* cmd = (POGL_RESIZETEXTURE2D_COMMAND_DATA*)command;

	POGLTextureResource* resource = cmd->texture->GetResourcePtr();
	glBindTexture(GL_TEXTURE_2D, resource->GetTextureID());
	state->SetTextureResource(resource);

	const POGLTextureFormat::Enum format = resource->GetTextureFormat();
	const GLenum _format = POGLEnum::ConvertToTextureFormatEnum(format);
	const GLenum _internalFormat = POGLEnum::ConvertToInternalTextureFormatEnum(format);
	const POGL_SIZE& size = cmd->newSize;

	glTexImage2D(GL_TEXTURE_2D, 0, _internalFormat, size.width, size.height, 0, _format, GL_UNSIGNED_BYTE, NULL);
	CHECK_GL("Could not set new texture size");

	cmd->texture->SetSize(cmd->newSize);
}

void POGLResizeTexture2D_Release(POGL_HANDLE command)
{
	POGL_RESIZETEXTURE2D_COMMAND_DATA* cmd = (POGL_RESIZETEXTURE2D_COMMAND_DATA*)command;
	cmd->texture->Release();
}

void POGLUniformSetInt_Command(POGLDeferredRenderContext* context, POGLRenderState* state, POGL_HANDLE command)
{
	POGL_UNIFORM_SET_INT_COMMAND_DATA* cmd = (POGL_UNIFORM_SET_INT_COMMAND_DATA*)command;
	const POGL_STRING& str = *cmd->name;
	auto uniform = state->FindUniformByName(str);
	switch (cmd->count) {
	case 1:
		uniform->SetInt32(cmd->values[0]);
		break;
	case 2:
		uniform->SetInt32(cmd->values[0], cmd->values[1]);
		break;
	case 3:
		uniform->SetInt32(cmd->values[0], cmd->values[1], cmd->values[2]);
		break;
	case 4:
		uniform->SetInt32(cmd->values[0], cmd->values[1], cmd->values[2], cmd->values[3]);
		break;
	default:
		uniform->SetInt32(cmd->values, cmd->count);
		break;
	}
}

void POGLUniformSetUInt_Command(POGLDeferredRenderContext* context, POGLRenderState* state, POGL_HANDLE command)
{
	POGL_UNIFORM_SET_UINT_COMMAND_DATA* cmd = (POGL_UNIFORM_SET_UINT_COMMAND_DATA*)command;
	const POGL_STRING& str = *cmd->name;
	auto uniform = state->FindUniformByName(str);
	switch (cmd->count) {
	case 1:
		uniform->SetUInt32(cmd->values[0]);
		break;
	case 2:
		uniform->SetUInt32(cmd->values[0], cmd->values[1]);
		break;
	case 3:
		uniform->SetUInt32(cmd->values[0], cmd->values[1], cmd->values[2]);
		break;
	case 4:
		uniform->SetUInt32(cmd->values[0], cmd->values[1], cmd->values[2], cmd->values[3]);
		break;
	default:
		uniform->SetUInt32(cmd->values, cmd->count);
		break;
	}
}

void POGLUniformSetFloat_Command(POGLDeferredRenderContext* context, POGLRenderState* state, POGL_HANDLE command)
{
	POGL_UNIFORM_SET_FLOAT_COMMAND_DATA* cmd = (POGL_UNIFORM_SET_FLOAT_COMMAND_DATA*)command;
	const POGL_STRING& str = *cmd->name;
	auto uniform = state->FindUniformByName(str);
	switch (cmd->count) {
	case 1:
		uniform->SetFloat(cmd->values[0]);
		break;
	case 2:
		uniform->SetFloat(cmd->values[0], cmd->values[1]);
		break;
	case 3:
		uniform->SetFloat(cmd->values[0], cmd->values[1], cmd->values[2]);
		break;
	case 4:
		uniform->SetFloat(cmd->values[0], cmd->values[1], cmd->values[2], cmd->values[3]);
		break;
	default:
		uniform->SetFloat(cmd->values, cmd->count);
		break;
	}
}
void POGLUniformSetMat4_Command(POGLDeferredRenderContext* context, POGLRenderState* state, POGL_HANDLE command)
{
	POGL_UNIFORM_SET_MAT4_COMMAND_DATA* cmd = (POGL_UNIFORM_SET_MAT4_COMMAND_DATA*)command;
	const POGL_STRING& str = *cmd->name;
	state->FindUniformByName(str)->SetMatrix(cmd->matrix);
}
