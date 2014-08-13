#pragma once
#include "POGLEffectState.h"
#include <memory>

class POGLDeviceContext;
class POGLVertexBuffer;
class POGLIndexBuffer;
class POGLTextureResource;
class POGLSamplerObject;
class POGLRenderState : public IPOGLRenderState
{
	typedef std::hash_map<POGL_UINT32, std::shared_ptr<POGLEffectState>> EffectStates;

public:
	POGLRenderState(POGLDeviceContext* context);
	~POGLRenderState();

	void AddRef();
	void Release();	
	IPOGLDevice* GetDevice();
	IPOGLDeviceContext* GetDeviceContext();

	void Clear(POGL_UINT32 clearBits);
	IPOGLUniform* FindUniformByName(const POGL_CHAR* name);
	void Draw(IPOGLVertexBuffer* vertexBuffer);
	void Draw(IPOGLVertexBuffer* vertexBuffer, IPOGLIndexBuffer* indexBuffer);
	void Draw(IPOGLVertexBuffer* vertexBuffer, IPOGLIndexBuffer* indexBuffer, POGL_UINT32 startIndex);
	void Draw(IPOGLVertexBuffer* vertexBuffer, IPOGLIndexBuffer* indexBuffer, POGL_UINT32 startIndex, POGL_UINT32 count);
	void SetDepthTest(bool b);
	void SetDepthFunc(POGLDepthFunc::Enum depthFunc);
	void SetDepthMask(bool b);
	void SetColorMask(POGL_UINT8 mask);
	bool GetStencilTest();
	void SetStencilTest(bool b);
	void SetBlendFunc(POGLSrcFactor::Enum sfactor, POGLDstFactor::Enum dfactor);
	void SetBlend(bool b);

	/*!
		\brief Applies the supplied effect to this render state
	*/
	IPOGLRenderState* Apply(IPOGLEffect* effect);

	/*!
		\brief Retrieves the effect state for the supplied effect

		\return effect
	*/
	POGLEffectState* GetEffectState(POGLEffect* effect);


	/*!
		\brief Check to see if the current effect is of the supplied type
	*/
	inline bool IsEffectActive(POGL_UINT32 effectUID) const {
		return mEffectUID == effectUID;
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

private:
	/*!
		\brief Binds the supplied effect to this state

		\param effect
	*/
	void BindEffect(POGLEffect* effect);

	/*!
		\brief Binds the supplied buffers
	*/
	void BindBuffers(POGLVertexBuffer* vertexBuffer, POGLIndexBuffer* indexBuffer);
	
private:
	POGL_UINT32 mRefCount;
	POGLDeviceContext* mDeviceContext;

	IPOGLEffect* mEffect;
	POGL_UINT32 mEffectUID;
	POGLEffectState* mCurrentEffectState;
	EffectStates mEffectStates;
	bool mApplyCurrentEffectState;

	IPOGLVertexBuffer *mVertexBuffer;
	POGL_UINT32 mVertexBufferUID;
	IPOGLIndexBuffer* mIndexBuffer;
	POGL_UINT32 mIndexBufferUID;

	//
	// Properties
	//

	bool mDepthTest;
	POGLDepthFunc::Enum mDepthFunc;
	bool mDepthMask;
	POGL_UINT8 mColorMask;
	bool mStencilTest;
	POGLSrcFactor::Enum mSrcFactor;
	POGLDstFactor::Enum mDstFactor;
	bool mBlending;

	//
	// Textures
	//

	POGL_UINT32 mMaxActiveTextures;
	POGL_UINT32 mNextActiveTexture;
	std::vector<POGL_UINT32> mTextureUID;
	std::vector<POGLTextureResource*> mTextures;
	std::vector<POGL_UINT32> mSamplerObjectUID;
	POGL_UINT32 mActiveTextureIndex;
};
