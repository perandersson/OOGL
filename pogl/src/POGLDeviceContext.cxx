#include "POGLDeviceContext.hxx"
#include "POGLRenderState.hxx"
#include "POGLDevice.hxx"

POGLDeviceContext::POGLDeviceContext(IPOGLDevice* device)
: mRenderState(nullptr), mRefCount(1), mDevice(device)
{
	mRenderState = new POGLRenderState(device);
}

POGLDeviceContext::~POGLDeviceContext()
{
	if (mRenderState != nullptr)
		mRenderState->Release();
	mRenderState = nullptr;
}

void POGLDeviceContext::AddRef()
{
	mRefCount++;
}

void POGLDeviceContext::Release()
{
	if (--mRefCount == 0)
		delete this;
}

IPOGLDevice* POGLDeviceContext::GetDevice()
{
	mDevice->AddRef();
	return mDevice;
}

IPOGLShaderProgram* POGLDeviceContext::CreateShaderProgramFromFile(const POGL_CHAR* path, POGLShaderProgramType::Enum type)
{
	return nullptr;
}

IPOGLShaderProgram* POGLDeviceContext::CreateShaderProgramFromMemory(const POGL_CHAR* memory, POGL_UINT32 size, POGLShaderProgramType::Enum type)
{
	return nullptr;
}

IPOGLEffect* POGLDeviceContext::CreateEffectFromFile(const POGL_CHAR* path)
{
	return nullptr;
}

IPOGLEffect* POGLDeviceContext::CreateEffectFromMemory(const POGL_CHAR* memory, POGL_UINT32 size)
{
	return nullptr;
}

IPOGLEffect* POGLDeviceContext::CreateEffectFromPrograms(IPOGLShaderProgram** programs, POGL_UINT32 numPrograms)
{
	return nullptr;
}

IPOGLTexture1D* POGLDeviceContext::CreateTexture1D()
{
	return nullptr;
}

IPOGLTexture2D* POGLDeviceContext::CreateTexture2D()
{
	return nullptr;
}

IPOGLTexture3D* POGLDeviceContext::CreateTexture3D()
{
	return nullptr;
}

IPOGLRenderState* POGLDeviceContext::Apply(IPOGLEffect* effect)
{
	return mRenderState->Apply(effect);
}

bool POGLDeviceContext::Initialize()
{
	return true;
}

void POGLDeviceContext::LoadExtensions()
{

}
