#include "MemCheck.h"
#include "POGLRenderState.h"
#include "POGLDeviceContext.h"
#include "POGLEffectData.h"
#include "POGLVertexBuffer.h"
#include "POGLIndexBuffer.h"
#include "POGLEnum.h"
#include "POGLTextureHandle.h"
#include "POGLSamplerObject.h"

POGLRenderState::POGLRenderState(POGLDeviceContext* context)
: mRefCount(0), mDeviceContext(context), mEffect(nullptr), mEffectUID(0), mCurrentEffectState(nullptr), mApplyCurrentEffectState(false),
mVertexBuffer(nullptr), mVertexBufferUID(0), mIndexBuffer(nullptr), mIndexBufferUID(0), mVertexArrayID(0),
mDepthTest(false), mDepthFunc(POGLDepthFunc::DEFAULT), mDepthMask(true),
mColorMask(POGLColorMask::ALL), mStencilTest(false),
mMaxActiveTextures(0), mNextActiveTexture(0), mActiveTextureIndex(0)
{
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, (GLint*)&mMaxActiveTextures);
	mTextureUID.resize(mMaxActiveTextures, 0);
	mTextureTarget.resize(mMaxActiveTextures, 0);
	mSamplerObjectUID.resize(mMaxActiveTextures, 0);

	for (POGL_UINT32 i = 0; i < MAX_VERTEX_LAYOUT_FIELD_SIZE; ++i)
		mVertexAttributeLocations[i] = false;

	mVertexArrayID = context->GenVertexArray();
	context->BindVertexArray(mVertexArrayID);
}

POGLRenderState::~POGLRenderState()
{
	if (mVertexArrayID != 0) {
		mDeviceContext->BindVertexArray(0);
		mDeviceContext->DeleteVertexArray(mVertexArrayID);
		mVertexArrayID = 0;
	}
	mDeviceContext = nullptr;
}

void POGLRenderState::AddRef()
{
	mRefCount++;
}

void POGLRenderState::Release()
{
	if (--mRefCount == 0) {
		if (mVertexBuffer != nullptr) {
			mVertexBuffer->Release();
			mVertexBuffer = nullptr;
		}

		if (mIndexBuffer != nullptr) {
			mIndexBuffer->Release();
			mIndexBuffer = nullptr;
		}

		if (mEffect != nullptr) {
			mEffect->Release();
			mEffect = nullptr;
		}
	}
}

IPOGLDevice* POGLRenderState::GetDevice()
{
	return mDeviceContext->GetDevice();
}

IPOGLDeviceContext* POGLRenderState::GetDeviceContext()
{
	mDeviceContext->AddRef();
	return mDeviceContext;
}

void POGLRenderState::Clear(POGL_UINT32 clearBits)
{
	if (clearBits == 0)
		return;

	GLenum clear = 0;
	if (BIT_ISSET(clearBits, POGLClearType::COLOR))
		clear = GL_COLOR_BUFFER_BIT;
	if (BIT_ISSET(clearBits, POGLClearType::DEPTH))
		clear |= GL_DEPTH_BUFFER_BIT;
	if (BIT_ISSET(clearBits, POGLClearType::STENCIL))
		clear |= GL_STENCIL_BUFFER_BIT;

	glClear(clear);
}

IPOGLUniform* POGLRenderState::FindUniformByName(const POGL_CHAR* name)
{
	return mCurrentEffectState->FindUniformByName(POGL_STRING(name));
}

void POGLRenderState::Draw(IPOGLVertexBuffer* vertexBuffer)
{
	assert_not_null(vertexBuffer);
	Draw(vertexBuffer, nullptr, 0);
}

void POGLRenderState::Draw(IPOGLVertexBuffer* vertexBuffer, IPOGLIndexBuffer* indexBuffer)
{
	assert_not_null(vertexBuffer);
	Draw(vertexBuffer, indexBuffer, 0);
}

void POGLRenderState::Draw(IPOGLVertexBuffer* vertexBuffer, IPOGLIndexBuffer* indexBuffer, POGL_UINT32 startIndex)
{
	assert_not_null(vertexBuffer);

	POGLVertexBuffer* vbo = static_cast<POGLVertexBuffer*>(vertexBuffer);
	POGLIndexBuffer* ibo = static_cast<POGLIndexBuffer*>(indexBuffer);
	BindBuffers(vbo, ibo);

	if (ibo == nullptr)
		vbo->Draw(startIndex);
	else
		vbo->Draw(ibo, startIndex);

	CHECK_GL("Cannot draw vertex- and index buffer");
}

void POGLRenderState::Draw(IPOGLVertexBuffer* vertexBuffer, IPOGLIndexBuffer* indexBuffer, POGL_UINT32 startIndex, POGL_UINT32 count)
{
	assert_not_null(vertexBuffer);

	POGLVertexBuffer* vbo = static_cast<POGLVertexBuffer*>(vertexBuffer);
	POGLIndexBuffer* ibo = static_cast<POGLIndexBuffer*>(indexBuffer);
	BindBuffers(vbo, ibo);

	if (ibo == nullptr)
		vbo->Draw(startIndex, count);
	else
		vbo->Draw(ibo, startIndex, count);

	CHECK_GL("Cannot draw vertex- and index buffer");
}

void POGLRenderState::EndFrame()
{
	mApplyCurrentEffectState = true;
	Release();
}

void POGLRenderState::SetDepthTest(bool b)
{
	if (b == mDepthTest)
		return;

	if (b)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
	mDepthTest = b;

	CHECK_GL("Cannot set enable/disable depth test");
}

void POGLRenderState::SetDepthFunc(POGLDepthFunc::Enum depthFunc)
{
	if (mDepthFunc == depthFunc)
		return;

	glDepthFunc(POGLEnum::Convert(depthFunc));
	mDepthFunc = depthFunc;

	CHECK_GL("Could not set the depth function used when render faces on the screen");
}

void POGLRenderState::SetDepthMask(bool b)
{
	if (mDepthMask == b)
		return;

	glDepthMask((GLboolean)b);
	mDepthMask = b;

	CHECK_GL("Could not set the depth mask used when render faces on the screen");
}

void POGLRenderState::SetColorMask(POGL_UINT8 mask)
{
	if (mColorMask == mask) {
		return;
	}

	const GLboolean r = BIT_ISSET(mask, POGLColorMask::RED);
	const GLboolean g = BIT_ISSET(mask, POGLColorMask::GREEN);
	const GLboolean b = BIT_ISSET(mask, POGLColorMask::BLUE);
	const GLboolean a = BIT_ISSET(mask, POGLColorMask::ALPHA);

	glColorMask(r, g, b, a);
	mColorMask = mask;

	CHECK_GL("Could not set the color mask used when render faces on the screen");
}

bool POGLRenderState::GetStencilTest()
{
	return mStencilTest;
}

void POGLRenderState::SetStencilTest(bool b)
{
	if (mStencilTest == b)
		return;

	if (b)
		glEnable(GL_STENCIL_TEST);
	else
		glDisable(GL_STENCIL_TEST);
	mStencilTest = b;

	CHECK_GL("Cannot set enable/disable stencil test");
}

IPOGLRenderState* POGLRenderState::Apply(IPOGLEffect* effect)
{
	assert_not_null(effect);

	// Bind the effect if neccessary
	POGLEffect* effectImpl = static_cast<POGLEffect*>(effect);
	BindEffect(effectImpl);

	// Retrieve the effect data
	POGLEffectData data;
	effectImpl->CopyEffectData(&data);

	//
	// Update the render state with the (potentially) new properties
	//

	SetDepthTest(data.depthTest);
	SetDepthFunc(data.depthFunc);
	SetDepthMask(data.depthMask);
	SetColorMask(data.colorMask);
	SetStencilTest(data.stencilTest);

	AddRef();
	return this;
}

POGLEffectState* POGLRenderState::GetEffectState(POGLEffect* effect)
{
	assert_not_null(effect);
	const POGL_UINT32 uid = effect->GetUID();
	auto it = mEffectStates.find(uid);
	if (it == mEffectStates.end()) {
		POGLEffectState* state = new POGLEffectState(effect, this, mDeviceContext);
		mEffectStates.insert(std::make_pair(uid, std::shared_ptr<POGLEffectState>(state)));
		return state;
	}

	return it->second.get();
}

void POGLRenderState::BindSamplerObject(POGLSamplerObject* samplerObject, POGL_UINT32 idx)
{
	assert_not_null(samplerObject);
	const POGL_UINT32 uid = samplerObject != nullptr ? samplerObject->GetUID() : 0;
	if (mSamplerObjectUID[idx] == uid)
		return;

	const GLuint samplerID = samplerObject != nullptr ? samplerObject->GetSamplerID() : 0;
	mDeviceContext->BindSampler(idx, samplerID);
	mSamplerObjectUID[idx] = uid;
	CHECK_GL("Cannot bind sampler ID");
}

void POGLRenderState::BindEffect(POGLEffect* effect)
{
	const POGL_UINT32 uid = effect->GetUID();
	if (uid == mEffectUID) {
		if (mEffect == nullptr && effect != nullptr) {
			mEffect = effect;
			mEffect->AddRef();
		}
		return;
	}

	if (mEffect != nullptr)
		mEffect->Release();

	mEffect = effect;
	mEffect->AddRef();
	mEffectUID = effect->GetUID();
	mDeviceContext->UseProgram(effect->GetProgramID());
	mCurrentEffectState = GetEffectState(effect);
	mApplyCurrentEffectState = true;

	CHECK_GL("Could not bind the supplied effect");
}

void POGLRenderState::BindBuffers(POGLVertexBuffer* vertexBuffer, POGLIndexBuffer* indexBuffer)
{
	if (mApplyCurrentEffectState) {
		mCurrentEffectState->ApplyUniforms();
		mApplyCurrentEffectState = false;
	}

	// TODO: Add Framebuffer

	BindVertexBuffer(vertexBuffer);
	BindIndexBuffer(indexBuffer);
}

void POGLRenderState::BindVertexBuffer(POGLVertexBuffer* buffer)
{
	assert_not_null(buffer);
	const POGL_UINT32 uid = buffer->GetUID();
	if (mVertexBufferUID == uid && buffer != nullptr) {
		if (mVertexBuffer != nullptr) {
			mVertexBuffer = buffer;
			mVertexBuffer->AddRef();
		}
		return;
	}

	if (mVertexBuffer != nullptr) {
		mVertexBuffer->Release();
	}
	mVertexBuffer = buffer;
	mVertexBuffer->AddRef();

	mDeviceContext->BindBuffer(GL_ARRAY_BUFFER, buffer->GetBufferID());
	CHECK_GL("Could not bind the supplied vertex buffer");

	const POGL_VERTEX_LAYOUT* layout = buffer->GetLayout();
	POGL_UINT32 offset = 0;
	for (POGL_UINT32 i = 0; i < MAX_VERTEX_LAYOUT_FIELD_SIZE; ++i) {
		const POGL_VERTEX_LAYOUT_FIELD& field = layout->fields[i];
		if (field.fieldSize == 0) {
			// Disable vertex attribute location if neccessary
			if (mVertexAttributeLocations[i]) {
				mDeviceContext->DisableVertexAttribArray(i);
				mVertexAttributeLocations[i] = false;
				CHECK_GL("Could not disable vertex attrib locations for the vertex array object");
			}
			continue;
		}

		// Enable vertex attribute location if neccessary
		if (!mVertexAttributeLocations[i]) {
			mDeviceContext->EnableVertexAttribArray(i);
			mVertexAttributeLocations[i] = true;
		}

		static const POGL_UINT32 TYPE_SIZE[POGLVertexType::COUNT] = {
			sizeof(POGL_INT8),
			sizeof(POGL_UINT8),
			sizeof(POGL_INT16),
			sizeof(POGL_UINT16),
			sizeof(POGL_INT32),
			sizeof(POGL_UINT32),
			sizeof(POGL_FLOAT),
			sizeof(POGL_DOUBLE)
		};

		const GLint numElementsInField = field.fieldSize / TYPE_SIZE[(POGL_UINT32)field.type];
		const auto type = field.type;
		switch (type) {
		case POGLVertexType::BYTE:
		case POGLVertexType::UNSIGNED_BYTE:
		case POGLVertexType::SHORT:
		case POGLVertexType::UNSIGNED_SHORT:
		case POGLVertexType::INT:
		case POGLVertexType::UNSIGNED_INT:
			mDeviceContext->VertexAttribIPointer(i, numElementsInField, POGLEnum::Convert(type), layout->vertexSize, OFFSET(offset));
			break;
		case POGLVertexType::FLOAT:
			mDeviceContext->VertexAttribPointer(i, numElementsInField, POGLEnum::Convert(type), field.normalize ? GL_TRUE : GL_FALSE, layout->vertexSize, OFFSET(offset));
			break;
		case POGLVertexType::DOUBLE:
			mDeviceContext->VertexAttribLPointer(i, numElementsInField, POGLEnum::Convert(type), layout->vertexSize, OFFSET(offset));
			break;
		}

		CHECK_GL("Could not set vertex attrib location for the vertex array object");
		offset += field.fieldSize;
	}

	mVertexBufferUID = uid;
}

void POGLRenderState::BindIndexBuffer(POGLIndexBuffer* buffer)
{
	const POGL_UINT32 uid = buffer != nullptr ? buffer->GetUID() : 0;
	if (mIndexBufferUID == uid) {
		if (mIndexBuffer == nullptr && buffer != nullptr) {
			mIndexBuffer = buffer;
			mIndexBuffer->AddRef();
		}
		return;
	}

	if (mIndexBuffer != nullptr) {
		mIndexBuffer->Release();
	}
	mIndexBuffer = buffer;
	if (mIndexBuffer != nullptr)
		mIndexBuffer->AddRef();

	const GLuint bufferID = buffer != nullptr ? buffer->GetBufferID() : 0;
	mDeviceContext->BindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);

	CHECK_GL("Could not bind the supplied index buffer");
	mIndexBufferUID = uid;
}

void POGLRenderState::BindTextureHandle(POGLTextureHandle* textureHandle, POGL_UINT32 idx)
{
	if (idx >= mMaxActiveTextures) {
		THROW_EXCEPTION(POGLStateException,
			"This computer does not support %d consecutive textures. The maximum amount of texture bindable at the same time is %d", idx, mMaxActiveTextures);
	}

	if (mActiveTextureIndex != idx) {
		mDeviceContext->ActiveTexture(GL_TEXTURE0 + idx);
		mActiveTextureIndex = idx;
		CHECK_GL("Could not set active texture index");
	}

	const POGL_UINT32 uid = textureHandle != nullptr ? textureHandle->uid : 0;

	// Check if the supplied texture is already bound to this context
	if (mTextureUID[idx] == uid)
		return;

	// Bind supplied texture
	const GLuint textureID = textureHandle != nullptr ? textureHandle->textureID : 0;
	const GLenum textureTarget = textureHandle != nullptr ? textureHandle->textureTarget : mTextureTarget[idx];
	glBindTexture(textureTarget, textureID);

	mTextureUID[idx] = uid;
	mTextureTarget[idx] = textureTarget;
	CHECK_GL("Could not bind texture");
}

POGL_UINT32 POGLRenderState::NextActiveTexture()
{
	const POGL_UINT32 textureIndex = mNextActiveTexture++;
	if (mNextActiveTexture >= mMaxActiveTextures)
		mNextActiveTexture = 0;
	return textureIndex;
}
