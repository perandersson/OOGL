#include "MemCheck.h"
#include "POGLDeferredRenderState.h"
#include "POGLDeferredDeviceContext.h"
#include "POGLFramebuffer.h"

POGLDeferredRenderState::POGLDeferredRenderState(POGLDeferredDeviceContext* context)
: mRefCount(1), mDeviceContext(context)
{
}

POGLDeferredRenderState::~POGLDeferredRenderState()
{
}

void POGLDeferredRenderState::AddRef()
{
	mRefCount++;
}

void POGLDeferredRenderState::Release()
{
	if (--mRefCount == 0)
		delete this;
}

void POGLDeferredRenderState::Clear(POGL_UINT32 clearBits)
{
	POGLClearCommand* cmd = (POGLClearCommand*)mDeviceContext->AddCommand(&POGLClear_Command, &POGLNothing_Release);
	cmd->clearBits = clearBits;
}

IPOGLUniform* POGLDeferredRenderState::FindUniformByName(const POGL_CHAR* name)
{
	THROW_NOT_IMPLEMENTED_EXCEPTION();
}

void POGLDeferredRenderState::SetFramebuffer(IPOGLFramebuffer* framebuffer)
{
	POGLSetFramebufferCommand* cmd = (POGLSetFramebufferCommand*)mDeviceContext->AddCommand(&POGLSetFramebuffer_Command, &POGLSetFramebuffer_Release);
	cmd->framebuffer = static_cast<POGLFramebuffer*>(framebuffer);
	if (framebuffer)
		framebuffer->AddRef();
}

void POGLDeferredRenderState::Draw(IPOGLVertexBuffer* vertexBuffer)
{
	assert_not_null(vertexBuffer);
	Draw(vertexBuffer, nullptr, 0);
}

void POGLDeferredRenderState::Draw(IPOGLVertexBuffer* vertexBuffer, IPOGLIndexBuffer* indexBuffer)
{
	assert_not_null(vertexBuffer);
	Draw(vertexBuffer, indexBuffer, 0);
}

void POGLDeferredRenderState::Draw(IPOGLVertexBuffer* vertexBuffer, IPOGLIndexBuffer* indexBuffer, POGL_UINT32 startIndex)
{
	assert_not_null(vertexBuffer);
	POGLDeferredDrawCommand* cmd = (POGLDeferredDrawCommand*)mDeviceContext->AddCommand(&POGLDraw_Command, &POGLDraw_Release);
	cmd->vertexBuffer = vertexBuffer;
	vertexBuffer->AddRef();
	cmd->indexBuffer = indexBuffer;
	if (indexBuffer != nullptr)
		indexBuffer->AddRef();
	cmd->startIndex = startIndex;
}

void POGLDeferredRenderState::Draw(IPOGLVertexBuffer* vertexBuffer, IPOGLIndexBuffer* indexBuffer, POGL_UINT32 startIndex, POGL_UINT32 count)
{
	assert_not_null(vertexBuffer);
	POGLDeferredDrawCommand* cmd = (POGLDeferredDrawCommand*)mDeviceContext->AddCommand(&POGLDrawCount_Command, &POGLDraw_Release);
	cmd->vertexBuffer = vertexBuffer;
	vertexBuffer->AddRef();
	cmd->indexBuffer = indexBuffer;
	if (indexBuffer != nullptr)
		indexBuffer->AddRef();
	cmd->startIndex = startIndex;
	cmd->count = count;
}

void POGLDeferredRenderState::SetDepthTest(bool b)
{
	POGLBooleanCommand* cmd = (POGLBooleanCommand*)mDeviceContext->AddCommand(&POGLSetDepthTest_Command, &POGLNothing_Release);
	cmd->value = b;
}

void POGLDeferredRenderState::SetDepthFunc(POGLDepthFunc::Enum depthFunc)
{
	POGLSetDepthFuncCommand* cmd = (POGLSetDepthFuncCommand*)mDeviceContext->AddCommand(&POGLSetDepthFunc_Command, &POGLNothing_Release);
	cmd->depthFunc = depthFunc;
}

void POGLDeferredRenderState::SetDepthMask(bool b)
{
	POGLBooleanCommand* cmd = (POGLBooleanCommand*)mDeviceContext->AddCommand(&POGLSetDepthMask_Command, &POGLNothing_Release);
	cmd->value = b;
}

void POGLDeferredRenderState::SetColorMask(POGL_UINT8 mask)
{
	POGLColorMaskCommand* cmd = (POGLColorMaskCommand*)mDeviceContext->AddCommand(&POGLColorMask_Command, &POGLNothing_Release);
	cmd->mask = mask;
}

void POGLDeferredRenderState::SetStencilTest(bool b)
{
	POGLBooleanCommand* cmd = (POGLBooleanCommand*)mDeviceContext->AddCommand(&POGLSetStencilTest_Command, &POGLNothing_Release);
	cmd->value = b;
}

void POGLDeferredRenderState::SetStencilMask(POGL_UINT32 mask)
{
	POGLStencilMaskCommand* cmd = (POGLStencilMaskCommand*)mDeviceContext->AddCommand(&POGLStencilMask_Command, &POGLNothing_Release);
	cmd->mask = mask;
}

void POGLDeferredRenderState::SetBlendFunc(POGLSrcFactor::Enum sfactor, POGLDstFactor::Enum dfactor)
{
	POGLSetBlendFuncCommand* cmd = (POGLSetBlendFuncCommand*)mDeviceContext->AddCommand(&POGLSetBlendFunc_Command, &POGLNothing_Release);
	cmd->sfactor = sfactor;
	cmd->dfactor = dfactor;
}

void POGLDeferredRenderState::SetBlend(bool b)
{
	POGLBooleanCommand* cmd = (POGLBooleanCommand*)mDeviceContext->AddCommand(&POGLSetBlend_Command, &POGLNothing_Release);
	cmd->value = b;
}

void POGLDeferredRenderState::SetFrontFace(POGLFrontFace::Enum e)
{
	POGLSetFrontFaceCommand* cmd = (POGLSetFrontFaceCommand*)mDeviceContext->AddCommand(&POGLSetFrontFace_Command, &POGLNothing_Release);
	cmd->frontFace = e;
}

void POGLDeferredRenderState::SetCullFace(POGLCullFace::Enum e)
{
	POGLSetCullFaceCommand* cmd = (POGLSetCullFaceCommand*)mDeviceContext->AddCommand(&POGLSetCullFace_Command, &POGLNothing_Release);
	cmd->cullFace = e;
}

void POGLDeferredRenderState::SetViewport(const POGL_RECT& viewport)
{
	POGLSetViewportCommand* cmd = (POGLSetViewportCommand*)mDeviceContext->AddCommand(&POGLSetViewport_Command, &POGLNothing_Release);
	cmd->viewport = viewport;
}

