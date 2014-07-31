#include "POGLRenderState.hxx"

POGLRenderState::POGLRenderState(IPOGLDevice* device)
: mRefCount(1), mDevice(device), mCurrentEffect(nullptr)
{

}

POGLRenderState::~POGLRenderState()
{
}

void POGLRenderState::AddRef()
{
	mRefCount++;
}

void POGLRenderState::Release()
{
	if (--mRefCount == 0)
		delete this;
}

IPOGLDevice* POGLRenderState::GetDevice()
{
	mDevice->AddRef();
	return mDevice;
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

	// Clears the back buffers
	glClear(clear);
}

IPOGLUniform* POGLRenderState::FindUniformByName(const POGL_CHAR* name)
{
	return nullptr;
}

void POGLRenderState::Draw(IPOGLBuffer* vertexBuffer)
{

}

void POGLRenderState::Draw(IPOGLBuffer* vertexBuffer, IPOGLBuffer* indexBuffer)
{

}

void POGLRenderState::Draw(IPOGLBuffer* vertexBuffer, IPOGLBuffer* indexBuffer, POGL_UINT32 vertexCount)
{

}

void POGLRenderState::Draw(IPOGLBuffer* vertexBuffer, IPOGLBuffer* indexBuffer, POGL_UINT32 vertexCount, POGL_UINT32 offset)
{

}

void POGLRenderState::EndFrame()
{
	mCurrentEffect->Release();
	mCurrentEffect = nullptr;
	Release();
}

IPOGLRenderState* POGLRenderState::Apply(IPOGLEffect* effect)
{
	mCurrentEffect = effect;
	mCurrentEffect->AddRef();
	AddRef();
	return this;
}
