#include "MemCheck.h"
#include "POGLDeferredDeviceContext.h"

POGLDeferredDeviceContext::POGLDeferredDeviceContext(IPOGLDevice* device)
: mRefCount(1), mDevice(device)
{
}

POGLDeferredDeviceContext::~POGLDeferredDeviceContext()
{
}

void POGLDeferredDeviceContext::AddRef()
{
	mRefCount++;
}

void POGLDeferredDeviceContext::Release()
{
	if (--mRefCount == 0)
		delete this;
}

IPOGLDevice* POGLDeferredDeviceContext::GetDevice()
{
	mDevice->AddRef();
	return mDevice;
}

IPOGLShaderProgram* POGLDeferredDeviceContext::CreateShaderProgramFromFile(const POGL_CHAR* path, POGLShaderProgramType::Enum type)
{
	THROW_EXCEPTION(POGLInitializationException, "Not implemented");
	return nullptr;
}

IPOGLShaderProgram* POGLDeferredDeviceContext::CreateShaderProgramFromMemory(const POGL_CHAR* memory, POGL_UINT32 size, POGLShaderProgramType::Enum type)
{
	THROW_EXCEPTION(POGLInitializationException, "Not implemented");
	return nullptr;
}

IPOGLEffect* POGLDeferredDeviceContext::CreateEffectFromPrograms(IPOGLShaderProgram** programs, POGL_UINT32 numPrograms)
{
	THROW_EXCEPTION(POGLInitializationException, "Not implemented");
	return nullptr;
}

IPOGLTexture1D* POGLDeferredDeviceContext::CreateTexture1D()
{
	THROW_EXCEPTION(POGLInitializationException, "Not implemented");
	return nullptr;
}

IPOGLTexture2D* POGLDeferredDeviceContext::CreateTexture2D(const POGL_SIZEI& size, POGLTextureFormat::Enum format, const void* bytes)
{
	THROW_EXCEPTION(POGLInitializationException, "Not implemented");
	return nullptr;
}

IPOGLTexture3D* POGLDeferredDeviceContext::CreateTexture3D()
{
	THROW_EXCEPTION(POGLInitializationException, "Not implemented");
	return nullptr;
}

IPOGLVertexBuffer* POGLDeferredDeviceContext::CreateVertexBuffer(const void* memory, POGL_SIZE memorySize, const POGL_VERTEX_LAYOUT* layout, POGLPrimitiveType::Enum primitiveType, POGLBufferUsage::Enum bufferUsage)
{
	THROW_EXCEPTION(POGLInitializationException, "Not implemented");
	return nullptr;
}

IPOGLVertexBuffer* POGLDeferredDeviceContext::CreateVertexBuffer(const POGL_POSITION_VERTEX* memory, POGL_SIZE memorySize, POGLPrimitiveType::Enum primitiveType, POGLBufferUsage::Enum bufferUsage)
{
	return CreateVertexBuffer(memory, memorySize, &POGL_POSITION_VERTEX_LAYOUT, primitiveType, bufferUsage);
}

IPOGLVertexBuffer* POGLDeferredDeviceContext::CreateVertexBuffer(const POGL_POSITION_COLOR_VERTEX* memory, POGL_SIZE memorySize, POGLPrimitiveType::Enum primitiveType, POGLBufferUsage::Enum bufferUsage)
{
	return CreateVertexBuffer(memory, memorySize, &POGL_POSITION_COLOR_VERTEX_LAYOUT, primitiveType, bufferUsage);
}

IPOGLVertexBuffer* POGLDeferredDeviceContext::CreateVertexBuffer(const POGL_POSITION_TEXCOORD_VERTEX* memory, POGL_SIZE memorySize, POGLPrimitiveType::Enum primitiveType, POGLBufferUsage::Enum bufferUsage)
{
	return CreateVertexBuffer(memory, memorySize, &POGL_POSITION_TEXCOORD_VERTEX_LAYOUT, primitiveType, bufferUsage);
}

IPOGLIndexBuffer* POGLDeferredDeviceContext::CreateIndexBuffer(const void* memory, POGL_SIZE memorySize, POGLVertexType::Enum type, POGLBufferUsage::Enum bufferUsage)
{
	THROW_EXCEPTION(POGLInitializationException, "Not implemented");
	return nullptr;
}

IPOGLRenderState* POGLDeferredDeviceContext::Apply(IPOGLEffect* effect)
{
	THROW_EXCEPTION(POGLInitializationException, "Not implemented");
	return nullptr;
}

void* POGLDeferredDeviceContext::Map(IPOGLResource* resource, POGLResourceStreamType::Enum e)
{
	THROW_EXCEPTION(POGLInitializationException, "Not implemented");
	return nullptr;
}

void* POGLDeferredDeviceContext::Map(IPOGLResource* resource, POGL_UINT32 offset, POGL_UINT32 size, POGLResourceStreamType::Enum e)
{
	THROW_EXCEPTION(POGLInitializationException, "Not implemented");
	return nullptr;
}

void POGLDeferredDeviceContext::Unmap(IPOGLResource* resource)
{
	THROW_EXCEPTION(POGLInitializationException, "Not implemented");
}

void POGLDeferredDeviceContext::ExecuteCommands(IPOGLDeviceContext* context)
{
	ExecuteCommands(context, true);
}

void POGLDeferredDeviceContext::ExecuteCommands(IPOGLDeviceContext* context, bool clearCommands)
{

}
