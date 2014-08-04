#pragma once
#include "POGLEffectState.hxx"
#include <memory>

class POGLDeviceContext;
class POGLVertexBuffer;
class POGLIndexBuffer;
struct POGLTextureHandle;
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
	void EndFrame();
	void SetDepthTest(bool b);
	void SetDepthFunc(POGLDepthFunc::Enum depthFunc);
	void SetDepthMask(bool b);
	void SetColorMask(POGL_UINT8 mask);
	bool GetStencilTest();
	void SetStencilTest(bool b);

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

		\param textureHandle
		\param idx
	*/
	void BindTextureHandle(POGLTextureHandle* textureHandle, POGL_UINT32 idx);

	/*!
		\brief Retrieves the next active texture for this render state.
	*/
	POGL_UINT32 NextActiveTexture();

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
	POGL_UINT32 mRefCount;
	POGLDeviceContext* mDeviceContext;

	IPOGLEffect* mEffect;
	POGL_UINT32 mEffectUID;
	POGLEffectState* mCurrentEffectState;
	EffectStates mEffectStates;
	bool mApplyCurrentEffectState;

	POGL_UINT32 mVertexBufferUID;
	bool mVertexAttributeLocations[MAX_VERTEX_LAYOUT_FIELD_SIZE];
	POGL_UINT32 mIndexBufferUID;
	GLuint mVertexArrayID;

	//
	// Properties
	//

	bool mDepthTest;
	POGLDepthFunc::Enum mDepthFunc;
	bool mDepthMask;
	POGL_UINT8 mColorMask;
	bool mStencilTest;

	//
	// Textures
	//

	POGL_UINT32 mMaxActiveTextures;
	POGL_UINT32 mNextActiveTexture;
	std::vector<POGL_UINT32> mTextureUID;
	std::vector<GLenum> mTextureTarget;
	std::vector<POGL_UINT32> mSamplerObjectUID;
	POGL_UINT32 mActiveTextureIndex;
};
