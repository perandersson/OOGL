#pragma once
#include "POGLProgramState.h"
#include <memory>

class POGLDeviceContext;
class POGLVertexBuffer;
class POGLIndexBuffer;
class POGLTextureResource;
class POGLSamplerObject;
class POGLFramebuffer;
class POGLRenderState : public IPOGLRenderState
{
	typedef std::hash_map<POGL_UINT32, std::shared_ptr<POGLProgramState>> EffectStates;

public:
	POGLRenderState(POGLDeviceContext* context);
	virtual ~POGLRenderState();
	
	/*!
		\brief Applies the supplied program to this render state

		\param program
	*/
	void Apply(IPOGLProgram* program);

	/*!
		\brief Retrieves the state for the supplied program

		\return program
	*/
	POGLProgramState* GetProgramState(POGLProgram* program);


	/*!
		\brief Check to see if the current program is of the supplied type
	*/
	inline bool IsProgramActive(POGL_UINT32 effectUID) const {
		return mProgramUID == effectUID;
	}

	/*!
		\brief Bind the supplied sampler object
	*/
	void BindSamplerObject(POGLSamplerObject* samplerObject, POGL_UINT32 idx);
	
	/*!
		\brief Bind the supplied texture handle

		\param texture
		\param idx
	*/
	void BindTextureResource(POGLTextureResource* texture, POGL_UINT32 idx);
	
	/*!
		\brief Set the currently bound texture resource

		\param texture
	*/
	void SetTextureResource(POGLTextureResource* texture);

	/*!
		\brief Set the currently bound vertex buffer

		\param vertexBuffer
	*/
	void SetVertexBuffer(POGLVertexBuffer* vertexBuffer);
	
	/*!
		\brief Set the currently bound vertex buffer

		\param vertexBuffer
	*/
	void SetIndexBuffer(POGLIndexBuffer* indexBuffer);

	/*!
		\brief Set the currently bound framebuffer

		\param framebuffer
	*/
	void SetFramebuffer(POGLFramebuffer* framebuffer);

	/*!
		\brief Retrieves the next active texture for this render state.
	*/
	POGL_UINT32 NextActiveTexture();
	
	/*!
		\brief Bind the supplied vertex buffer

		\param buffer
	*/
	void BindVertexBuffer(POGLVertexBuffer* buffer);

	/*!
		\brief Bind the supplied index buffer

		\param buffer
	*/
	void BindIndexBuffer(POGLIndexBuffer* buffer);

// IPOGLInterface
public:
	virtual void AddRef();
	virtual void Release();

// IPOGLRenderState
public:
	virtual void Clear(POGL_UINT32 clearBits);
	virtual IPOGLUniform* FindUniformByName(const POGL_CHAR* name);
	virtual IPOGLUniform* FindUniformByName(const POGL_STRING& name);
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
	virtual void SetViewport(const POGL_RECT& viewport);

private:
	/*!
		\brief Binds the supplied program to this state

		\param effect
	*/
	void BindProgram(POGLProgram* program);

	/*!
		\brief Binds the supplied buffers
	*/
	void BindBuffers(POGLVertexBuffer* vertexBuffer, POGLIndexBuffer* indexBuffer);
	
private:
	REF_COUNTER mRefCount;
	POGLDeviceContext* mDeviceContext;

	POGLProgram* mProgram;
	POGL_UINT32 mProgramUID;
	POGLProgramState* mCurrentProgramState;
	EffectStates mProgramStates;
	bool mApplyCurrentProgramState;

	POGLVertexBuffer *mVertexBuffer;
	POGL_UINT32 mVertexBufferUID;
	POGLIndexBuffer* mIndexBuffer;
	POGL_UINT32 mIndexBufferUID;

	//
	// Properties
	//

	bool mDepthTest;
	POGLDepthFunc::Enum mDepthFunc;
	bool mDepthMask;
	POGL_UINT8 mColorMask;
	bool mStencilTest;
	POGL_UINT32 mStencilMask;
	POGLSrcFactor::Enum mSrcFactor;
	POGLDstFactor::Enum mDstFactor;
	bool mBlending;
	POGL_RECT mViewport;

	//
	// Textures
	//

	POGL_UINT32 mMaxActiveTextures;
	POGL_UINT32 mNextActiveTexture;
	std::vector<POGL_UINT32> mTextureUID;
	std::vector<POGLTextureResource*> mTextures;
	std::vector<POGL_UINT32> mSamplerObjectUID;
	POGL_UINT32 mActiveTextureIndex;

	//
	// Framebuffer
	//

	POGLFramebuffer* mFramebuffer;
	POGL_UINT32 mFramebufferUID;
};
