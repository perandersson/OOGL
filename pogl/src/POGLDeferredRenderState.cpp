#include "MemCheck.h"
#include "POGLDeferredRenderState.h"
#include "POGLDeferredRenderContext.h"
#include "POGLFramebuffer.h"
#include "uniforms/POGLDeferredUniform.h"

POGLDeferredRenderState::POGLDeferredRenderState(POGLDeferredRenderContext* context)
: mRefCount(1), mRenderContext(context)
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
	if (--mRefCount == 0) {
		auto it = mUniforms.begin();
		auto end = mUniforms.end();
		for (; it != end; ++it) {
			delete it->second;
		}

		delete this;
	}
}

void POGLDeferredRenderState::Clear(POGL_UINT32 clearBits)
{
	POGL_CLEAR_COMMAND_DATA* cmd = (POGL_CLEAR_COMMAND_DATA*)mRenderContext->AddCommand(&POGLClear_Command, &POGLNothing_Release,
		sizeof(POGL_CLEAR_COMMAND_DATA));
	cmd->clearBits = clearBits;
}

IPOGLUniform* POGLDeferredRenderState::FindUniformByName(const POGL_CHAR* name)
{
	const POGL_STRING sname(name);
	auto it = mUniforms.find(sname);
	if (it == mUniforms.end()) {
		POGLDeferredUniform* uniform = new POGLDeferredUniform(sname, mRenderContext);
		mUniforms.insert(std::make_pair(sname, uniform));
		return uniform;
	}

	return it->second;
}

void POGLDeferredRenderState::SetFramebuffer(IPOGLFramebuffer* framebuffer)
{
	POGL_SETFRAMEBUFFER_COMMAND_DATA* cmd = (POGL_SETFRAMEBUFFER_COMMAND_DATA*)mRenderContext->AddCommand(&POGLSetFramebuffer_Command, &POGLSetFramebuffer_Release,
		sizeof(POGL_SETFRAMEBUFFER_COMMAND_DATA));
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
	POGL_DRAW_COMMAND_DATA* cmd = (POGL_DRAW_COMMAND_DATA*)mRenderContext->AddCommand(&POGLDraw_Command, &POGLDraw_Release,
		sizeof(POGL_DRAW_COMMAND_DATA));
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
	POGL_DRAW_COMMAND_DATA* cmd = (POGL_DRAW_COMMAND_DATA*)mRenderContext->AddCommand(&POGLDrawCount_Command, &POGLDraw_Release,
		sizeof(POGL_DRAW_COMMAND_DATA));
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
	POGL_BOOLEAN_COMMAND_DATA* cmd = (POGL_BOOLEAN_COMMAND_DATA*)mRenderContext->AddCommand(&POGLSetDepthTest_Command, &POGLNothing_Release,
		sizeof(POGL_BOOLEAN_COMMAND_DATA));
	cmd->value = b;
}

void POGLDeferredRenderState::SetDepthFunc(POGLDepthFunc::Enum depthFunc)
{
	POGL_SETDEPTHFUNC_COMMAND_DATA* cmd = (POGL_SETDEPTHFUNC_COMMAND_DATA*)mRenderContext->AddCommand(&POGLSetDepthFunc_Command, &POGLNothing_Release,
		sizeof(POGL_SETDEPTHFUNC_COMMAND_DATA));
	cmd->depthFunc = depthFunc;
}

void POGLDeferredRenderState::SetDepthMask(bool b)
{
	POGL_BOOLEAN_COMMAND_DATA* cmd = (POGL_BOOLEAN_COMMAND_DATA*)mRenderContext->AddCommand(&POGLSetDepthMask_Command, &POGLNothing_Release,
		sizeof(POGL_BOOLEAN_COMMAND_DATA));
	cmd->value = b;
}

void POGLDeferredRenderState::SetColorMask(POGL_UINT8 mask)
{
	POGL_COLORMASK_COMMAND_DATA* cmd = (POGL_COLORMASK_COMMAND_DATA*)mRenderContext->AddCommand(&POGLColorMask_Command, &POGLNothing_Release,
		sizeof(POGL_COLORMASK_COMMAND_DATA));
	cmd->mask = mask;
}

void POGLDeferredRenderState::SetStencilTest(bool b)
{
	POGL_BOOLEAN_COMMAND_DATA* cmd = (POGL_BOOLEAN_COMMAND_DATA*)mRenderContext->AddCommand(&POGLSetStencilTest_Command, &POGLNothing_Release,
		sizeof(POGL_BOOLEAN_COMMAND_DATA));
	cmd->value = b;
}

void POGLDeferredRenderState::SetStencilMask(POGL_UINT32 mask)
{
	POGL_STENCILMASK_COMMAND_DATA* cmd = (POGL_STENCILMASK_COMMAND_DATA*)mRenderContext->AddCommand(&POGLStencilMask_Command, &POGLNothing_Release,
		sizeof(POGL_STENCILMASK_COMMAND_DATA));
	cmd->mask = mask;
}

void POGLDeferredRenderState::SetBlendFunc(POGLSrcFactor::Enum sfactor, POGLDstFactor::Enum dfactor)
{
	POGL_SETBLENDFUNC_COMMAND_DATA* cmd = (POGL_SETBLENDFUNC_COMMAND_DATA*)mRenderContext->AddCommand(&POGLSetBlendFunc_Command, &POGLNothing_Release,
		sizeof(POGL_SETBLENDFUNC_COMMAND_DATA));
	cmd->sfactor = sfactor;
	cmd->dfactor = dfactor;
}

void POGLDeferredRenderState::SetBlend(bool b)
{
	POGL_BOOLEAN_COMMAND_DATA* cmd = (POGL_BOOLEAN_COMMAND_DATA*)mRenderContext->AddCommand(&POGLSetBlend_Command, &POGLNothing_Release,
		sizeof(POGL_BOOLEAN_COMMAND_DATA));
	cmd->value = b;
}

void POGLDeferredRenderState::SetFrontFace(POGLFrontFace::Enum e)
{
	POGL_SETFRONTFACE_COMMAND_DATA* cmd = (POGL_SETFRONTFACE_COMMAND_DATA*)mRenderContext->AddCommand(&POGLSetFrontFace_Command, &POGLNothing_Release,
		sizeof(POGL_SETFRONTFACE_COMMAND_DATA));
	cmd->frontFace = e;
}

void POGLDeferredRenderState::SetCullFace(POGLCullFace::Enum e)
{
	POGL_SETCULLFACE_COMMAND_DATA* cmd = (POGL_SETCULLFACE_COMMAND_DATA*)mRenderContext->AddCommand(&POGLSetCullFace_Command, &POGLNothing_Release,
		sizeof(POGL_SETCULLFACE_COMMAND_DATA));
	cmd->cullFace = e;
}

void POGLDeferredRenderState::SetViewport(const POGL_RECT& viewport)
{
	POGL_SETVIEWPORT_COMMAND_DATA* cmd = (POGL_SETVIEWPORT_COMMAND_DATA*)mRenderContext->AddCommand(&POGLSetViewport_Command, &POGLNothing_Release,
		sizeof(POGL_SETVIEWPORT_COMMAND_DATA));
	cmd->viewport = viewport;
}

