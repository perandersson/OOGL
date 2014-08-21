#include "MemCheck.h"
#include "POGLRenderContext.h"
#include "POGLRenderState.h"
#include "POGLDevice.h"
#include "POGLEnum.h"
#include "POGLVertexBuffer.h"
#include "POGLIndexBuffer.h"
#include "POGLTexture2D.h"
#include "POGLShader.h"
#include "POGLProgramData.h"
#include "POGLFactory.h"
#include "POGLFramebuffer.h"
#include "POGLProgram.h"
#include <algorithm>

POGLRenderContext::POGLRenderContext(IPOGLDevice* device)
: mRenderState(nullptr), mDevice(device)
{
}

POGLRenderContext::~POGLRenderContext()
{
}

void POGLRenderContext::Destroy()
{
	POGL_SAFE_RELEASE(mRenderState);
}

IPOGLDevice* POGLRenderContext::GetDevice()
{
	mDevice->AddRef();
	return mDevice;
}

IPOGLShader* POGLRenderContext::CreateShaderFromFile(const POGL_CHAR* path, POGLShaderType::Enum type)
{
	POGL_ISTREAM stream(path);
	if (!stream.is_open())
		THROW_EXCEPTION(POGLResourceException, "Shader at path: '%s' could not be found", path);

	// Read the entire file into memory
	POGL_STRING str((std::istreambuf_iterator<POGL_CHAR>(stream)), std::istreambuf_iterator<POGL_CHAR>());
	return CreateShaderFromMemory(str.c_str(), str.length(), type);
}

IPOGLShader* POGLRenderContext::CreateShaderFromMemory(const POGL_CHAR* memory, POGL_UINT32 size, POGLShaderType::Enum type)
{
	// Generate a shader ID based on the supplied memory, size and type
	const GLuint shaderID = POGLFactory::CreateShader(memory, size, type);

	POGLShader* shader = new POGLShader(type);
	shader->PostConstruct(shaderID);
	return shader;
}

IPOGLProgram* POGLRenderContext::CreateProgramFromShaders(IPOGLShader** shaders)
{
	if (shaders == nullptr)
		THROW_EXCEPTION(POGLResourceException, "You must supply at least one shader to be able to create a program");

	// Attach all the shaders to the program
	const GLuint programID = POGLFactory::CreateProgram(shaders);
	POGLProgram* program = new POGLProgram();
	program->PostConstruct(programID, GetRenderState());
	return program;
}

IPOGLTexture1D* POGLRenderContext::CreateTexture1D()
{
	return nullptr;
}

IPOGLTexture2D* POGLRenderContext::CreateTexture2D(const POGL_SIZE& size, POGLTextureFormat::Enum format, const void* bytes)
{
	if (size.width <= 0)
		THROW_EXCEPTION(POGLResourceException, "You cannot create a texture with width: %d", size.width);

	if (size.height <= 0)
		THROW_EXCEPTION(POGLResourceException, "You cannot create a texture with height: %d", size.height);

	const GLenum _format = POGLEnum::ConvertToTextureFormatEnum(format);
	const GLenum _internalFormat = POGLEnum::ConvertToInternalTextureFormatEnum(format);
	const GLenum minFilter = POGLEnum::Convert(POGLMinFilter::DEFAULT);
	const GLenum magFilter = POGLEnum::Convert(POGLMagFilter::DEFAULT);
	const GLenum textureWrap = POGLEnum::Convert(POGLTextureWrap::DEFAULT);

	const GLuint textureID = POGLFactory::GenTextureID();
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, _internalFormat, size.width, size.height, 0, _format, GL_UNSIGNED_BYTE, bytes);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrap);

	const GLenum status = glGetError();
	if (status != GL_NO_ERROR) {
		THROW_EXCEPTION(POGLResourceException, "Could not create 2D texture. Reason: 0x%x", status);
	}

	POGLTexture2D* texture = new POGLTexture2D(size, format);
	texture->PostConstruct(textureID);
	mRenderState->SetTextureResource((POGLTextureResource*)texture->GetResourcePtr());
	return texture;
}

IPOGLTexture3D* POGLRenderContext::CreateTexture3D()
{
	return nullptr;
}

void POGLRenderContext::ResizeTexture2D(IPOGLTexture2D* texture, const POGL_SIZE& size)
{
	if (texture == nullptr)
		THROW_EXCEPTION(POGLStateException, "You cannot resize a non-existing texture");

	if (size.width <= 0)
		THROW_EXCEPTION(POGLStateException, "You cannot resize a texture to 0 width");

	if (size.height <= 0)
		THROW_EXCEPTION(POGLStateException, "You cannot resize a texture to 0 height");

	POGLTexture2D* impl = static_cast<POGLTexture2D*>(texture);
	POGLTextureResource* resource = impl->GetResourcePtr();
	mRenderState->BindTextureResource(resource, 0);

	const POGLTextureFormat::Enum format = resource->GetTextureFormat();
	const GLenum _format = POGLEnum::ConvertToTextureFormatEnum(format);
	const GLenum _internalFormat = POGLEnum::ConvertToInternalTextureFormatEnum(format);
	
	glTexImage2D(GL_TEXTURE_2D, 0, _internalFormat, size.width, size.height, 0, _format, GL_UNSIGNED_BYTE, NULL);
	impl->SetSize(size);
	CHECK_GL("Could not set new texture size");
}

IPOGLFramebuffer* POGLRenderContext::CreateFramebuffer(IPOGLTexture** textures)
{
	return CreateFramebuffer(textures, nullptr);
}

IPOGLFramebuffer* POGLRenderContext::CreateFramebuffer(IPOGLTexture** textures, IPOGLTexture* depthStencilTexture)
{
	//
	// Generate a framebuffer ID
	//

	const GLuint framebufferID = POGLFactory::GenFramebufferObjectID(textures, depthStencilTexture);

	//
	// Convert the textures array into a std::vector
	//

	std::vector<IPOGLTexture*> texturesVector;
	if (textures != nullptr) {
		for (IPOGLTexture** ptr = textures; *ptr != nullptr; ++ptr) {
			IPOGLTexture* texture = *ptr;
			texturesVector.push_back(texture);
		}
	}
	
	//
	// Create the actual object
	//

	POGLFramebuffer* framebuffer = new POGLFramebuffer(texturesVector, depthStencilTexture);
	framebuffer->PostConstruct(framebufferID);
	mRenderState->SetFramebuffer(framebuffer);
	return framebuffer;
}

IPOGLVertexBuffer* POGLRenderContext::CreateVertexBuffer(const void* memory, POGL_UINT32 memorySize, const POGL_VERTEX_LAYOUT* layout, POGLPrimitiveType::Enum primitiveType, POGLBufferUsage::Enum bufferUsage)
{
	assert_not_null(memory);
	assert_with_message(memorySize > 0, "You cannot create a vertex buffer of no size");
	assert_not_null(layout);

	const POGL_UINT32 numVertices = memorySize / layout->vertexSize;
	const GLenum usage = POGLEnum::Convert(bufferUsage);
	const GLenum type = POGLEnum::Convert(primitiveType);
	const GLuint bufferID = POGLFactory::GenBufferID();
	const GLuint vaoID = POGLFactory::GenVertexArrayObjectID(bufferID, layout);

	//
	// Create the object
	//

	POGLVertexBuffer* vb = new POGLVertexBuffer(numVertices, layout, type, usage);
	vb->PostConstruct(bufferID, vaoID);
	
	// 
	// Fill the buffer with data
	//

	glBufferData(GL_ARRAY_BUFFER, memorySize, memory, usage);

	//
	// Make sure to mark this buffer as the current vertex buffer
	//

	mRenderState->SetVertexBuffer(vb);

	// Finished
	return vb;
}

IPOGLVertexBuffer* POGLRenderContext::CreateVertexBuffer(const POGL_POSITION_VERTEX* memory, POGL_UINT32 memorySize, POGLPrimitiveType::Enum primitiveType, POGLBufferUsage::Enum bufferUsage)
{
	return CreateVertexBuffer(memory, memorySize, &POGL_POSITION_VERTEX_LAYOUT, primitiveType, bufferUsage);
}

IPOGLVertexBuffer* POGLRenderContext::CreateVertexBuffer(const POGL_POSITION_COLOR_VERTEX* memory, POGL_UINT32 memorySize, POGLPrimitiveType::Enum primitiveType, POGLBufferUsage::Enum bufferUsage)
{
	return CreateVertexBuffer(memory, memorySize, &POGL_POSITION_COLOR_VERTEX_LAYOUT, primitiveType, bufferUsage);
}

IPOGLVertexBuffer* POGLRenderContext::CreateVertexBuffer(const POGL_POSITION_TEXCOORD_VERTEX* memory, POGL_UINT32 memorySize, POGLPrimitiveType::Enum primitiveType, POGLBufferUsage::Enum bufferUsage)
{
	return CreateVertexBuffer(memory, memorySize, &POGL_POSITION_TEXCOORD_VERTEX_LAYOUT, primitiveType, bufferUsage);
}

IPOGLIndexBuffer* POGLRenderContext::CreateIndexBuffer(const void* memory, POGL_UINT32 memorySize, POGLVertexType::Enum type, POGLBufferUsage::Enum bufferUsage)
{
	assert_not_null(memory);
	assert_with_message(memorySize > 0, "You cannot create a index buffer of no size");
	assert_with_message(type != POGLVertexType::FLOAT && type != POGLVertexType::DOUBLE, "You are not allowed to create an index buffer of a decimal type");

	const POGL_UINT32 typeSize = POGLEnum::VertexTypeSize(type);
	const POGL_UINT32 numIndices = memorySize / typeSize;
	const GLuint bufferID = POGLFactory::GenBufferID();
	const GLenum usage = POGLEnum::Convert(bufferUsage);
	const GLenum indiceType = POGLEnum::Convert(type);

	// 
	// Fill the buffer with data
	//

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, memorySize, memory, usage);

	const GLenum error = glGetError();
	if (error != GL_NO_ERROR)
		THROW_EXCEPTION(POGLResourceException, "Failed to create a vertex buffer. Reason: 0x%x", error);

	POGLIndexBuffer* ib = new POGLIndexBuffer(bufferID, typeSize, numIndices, indiceType, usage);
	mRenderState->SetIndexBuffer(ib);
	return ib;
}

IPOGLResource* POGLRenderContext::CloneResource(IPOGLResource* resource)
{
	THROW_NOT_IMPLEMENTED_EXCEPTION();
}

void POGLRenderContext::CopyResource(IPOGLResource* source, IPOGLResource* destination)
{
	THROW_NOT_IMPLEMENTED_EXCEPTION();
}

void POGLRenderContext::CopyResource(IPOGLResource* source, IPOGLResource* destination, POGL_UINT32 sourceOffset, POGL_UINT32 destinationOffset, POGL_UINT32 size)
{
	THROW_NOT_IMPLEMENTED_EXCEPTION();
}

IPOGLRenderState* POGLRenderContext::Apply(IPOGLProgram* program)
{
	if (program == nullptr)
		THROW_EXCEPTION(POGLResourceException, "You are not allowed to apply a non-existing program");

	mRenderState->Apply(static_cast<POGLProgram*>(program));
	mRenderState->AddRef();
	return mRenderState;
}

void* POGLRenderContext::Map(IPOGLResource* resource, POGLResourceMapType::Enum e)
{
	auto type = resource->GetType();
	if (type == POGLResourceType::VERTEXBUFFER) {
		POGLVertexBuffer* vb = static_cast<POGLVertexBuffer*>(resource);
		mRenderState->BindVertexBuffer(vb);
		return glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	}

	THROW_NOT_IMPLEMENTED_EXCEPTION();
}

void* POGLRenderContext::Map(IPOGLResource* resource, POGL_UINT32 offset, POGL_UINT32 length, POGLResourceMapType::Enum e)
{
	auto type = resource->GetType();
	if (type == POGLResourceType::VERTEXBUFFER) {
		POGLVertexBuffer* vb = static_cast<POGLVertexBuffer*>(resource);
		const POGL_UINT32 memorySize = vb->GetCount() * vb->GetLayout()->vertexSize;
		if (offset + length > memorySize)
			THROW_EXCEPTION(POGLStateException, "You cannot map with offset: %d and length: %d when the vertex buffer size is: %d", offset, length, memorySize);

		mRenderState->BindVertexBuffer(vb);
		return glMapBufferRange(GL_ARRAY_BUFFER, offset, length, GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT);
	}

	THROW_NOT_IMPLEMENTED_EXCEPTION();
}

void POGLRenderContext::Unmap(IPOGLResource* resource)
{
	auto type = resource->GetType();
	if (type == POGLResourceType::VERTEXBUFFER) {
		glUnmapBuffer(GL_ARRAY_BUFFER);
		return;
	}

	THROW_NOT_IMPLEMENTED_EXCEPTION();
}

void POGLRenderContext::SetViewport(const POGL_RECT& viewport)
{
	mRenderState->SetViewport(viewport);
}

void POGLRenderContext::InitializeRenderState()
{
	if (mRenderState == nullptr) {
		mRenderState = new POGLRenderState(this);
	}
}
