#pragma once
#include "config.h"
#include "POGLDeferredStateValue.h"
#include <memory>

class POGLDeferredRenderContext;
class POGLDeferredUniform;
class POGLProgram;
class POGLDeferredRenderState : public IPOGLRenderState
{
	typedef std::hash_map<POGL_STRING, POGLDeferredUniform*> Uniforms;

public:
	POGLDeferredRenderState(POGLDeferredRenderContext* context);
	~POGLDeferredRenderState();

	/*!
		\brief Unset this RenderState's internal state values. 
	*/
	void Flush();

	/*!
		\brief Flush program specific properties
	*/
	void FlushProgram(POGLProgram* program);

// IPOGLInterface
public:
	virtual void AddRef();
	virtual void Release();

// IPOGLRenderState
public:
	virtual void Clear(POGL_UINT32 clearBits);
	virtual IPOGLUniform* FindUniformByName(const POGL_CHAR* name);
	virtual void SetFramebuffer(IPOGLFramebuffer* framebuffer);
	virtual void SetVertexBuffer(IPOGLVertexBuffer* vertexBuffer);
	virtual void SetIndexBuffer(IPOGLIndexBuffer* indexBuffer);
	virtual void Draw();
	virtual void Draw(POGL_UINT32 count);
	virtual void Draw(POGL_UINT32 count, POGL_UINT32 offset);
	virtual void DrawIndexed();
	virtual void DrawIndexed(POGL_UINT32 count);
	virtual void DrawIndexed(POGL_UINT32 count, POGL_UINT32 offset);
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
	POGLDeferredRenderContext* mRenderContext;

	POGLDeferredStateValue<POGL_UINT32> mFramebuffer;
	POGLDeferredStateValue<POGL_UINT32> mVertexBuffer;
	POGLDeferredStateValue<POGL_UINT32> mIndexBuffer;
	POGLDeferredStateValue<bool> mDepthTest;
	POGLDeferredStateValue<POGLDepthFunc::Enum> mDepthFunc;
	POGLDeferredStateValue<bool> mDepthMask;
	POGLDeferredStateValue<POGL_UINT8> mColorMask;
	POGLDeferredStateValue<bool> mStencilTest;
	POGLDeferredStateValue<POGL_UINT32> mStencilMask;
	POGLDeferredStateValue<POGLSrcFactor::Enum> mSrcFactor;
	POGLDeferredStateValue<POGLDstFactor::Enum> mDstFactor;
	POGLDeferredStateValue<bool> mBlend;
	POGLDeferredStateValue<POGLFrontFace::Enum> mFrontFace;
	POGLDeferredStateValue<POGLCullFace::Enum> mCullFace;
	POGLDeferredStateValue<POGL_RECT> mViewport;

	Uniforms mUniforms;
};
