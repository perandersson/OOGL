#pragma once
#include "config.h"
#include <memory>

class POGLDeferredDeviceContext;
class POGLDeferredRenderState : public IPOGLRenderState
{
public:
	POGLDeferredRenderState(POGLDeferredDeviceContext* context);
	~POGLDeferredRenderState();

// IPOGLInterface
public:
	virtual void AddRef();
	virtual void Release();

// IPOGLRenderState
public:
	virtual void Clear(POGL_UINT32 clearBits);
	virtual IPOGLUniform* FindUniformByName(const POGL_CHAR* name);
	virtual void SetFramebuffer(IPOGLFramebuffer* framebuffer);
	virtual void Draw(IPOGLVertexBuffer* vertexBuffer);
	virtual void Draw(IPOGLVertexBuffer* vertexBuffer, IPOGLIndexBuffer* indexBuffer);
	virtual void Draw(IPOGLVertexBuffer* vertexBuffer, IPOGLIndexBuffer* indexBuffer, POGL_UINT32 startIndex);
	virtual void Draw(IPOGLVertexBuffer* vertexBuffer, IPOGLIndexBuffer* indexBuffer, POGL_UINT32 startIndex, POGL_UINT32 count);
	virtual void SetDepthTest(bool b);
	virtual void SetDepthFunc(POGLDepthFunc::Enum depthFunc);
	virtual void SetDepthMask(bool b);
	virtual void SetColorMask(POGL_UINT8 mask);
	virtual void SetStencilTest(bool b);
	virtual void SetStencilMask(POGL_UINT32 mask);
	virtual void SetBlendFunc(POGLSrcFactor::Enum sfactor, POGLDstFactor::Enum dfactor);
	virtual void SetBlend(bool b);
	virtual void SetFrontFace(POGLFrontFace::Enum e);
	virtual void SetCullFace(POGLCullFace::Enum e);
	virtual void SetViewport(const POGL_RECT& viewport);

private:
	REF_COUNTER mRefCount;
	POGLDeferredDeviceContext* mDeviceContext;
};
