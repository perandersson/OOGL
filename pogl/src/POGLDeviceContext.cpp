#include "MemCheck.h"
#include "POGLDeviceContext.h"
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
#include <gl/poglext.h>
#include <algorithm>

POGLDeviceContext::POGLDeviceContext(IPOGLDevice* device)
: mRenderState(nullptr), mDevice(device)
{
}

POGLDeviceContext::~POGLDeviceContext()
{
}

void POGLDeviceContext::Destroy()
{
	POGL_SAFE_RELEASE(mRenderState);
}

IPOGLDevice* POGLDeviceContext::GetDevice()
{
	mDevice->AddRef();
	return mDevice;
}

IPOGLShader* POGLDeviceContext::CreateShaderFromFile(const POGL_CHAR* path, POGLShaderType::Enum type)
{
	POGL_ISTREAM stream(path);
	if (!stream.is_open())
		THROW_EXCEPTION(POGLResourceException, "Shader at path: '%s' could not be found", path);

	// Read the entire file into memory
	POGL_STRING str((std::istreambuf_iterator<POGL_CHAR>(stream)), std::istreambuf_iterator<POGL_CHAR>());
	return CreateShaderFromMemory(str.c_str(), str.length(), type);
}

IPOGLShader* POGLDeviceContext::CreateShaderFromMemory(const POGL_CHAR* memory, POGL_UINT32 size, POGLShaderType::Enum type)
{
	if (size == 0 || memory == nullptr)
		THROW_EXCEPTION(POGLResourceException, "You cannot generate a non-existing shader");

	const GLuint shaderID = glCreateShader(POGLEnum::Convert(type));
	glShaderSource(shaderID, 1, (const GLchar**)&memory, (const GLint*)&size);
	glCompileShader(shaderID);

	GLint status = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);
	if (!status) {
		GLchar infoLogg[2048];
		glGetShaderInfoLog(shaderID, 2048, NULL, infoLogg);
		glDeleteShader(shaderID);
		switch (type) {
		case POGLShaderType::GEOMETRY_SHADER:
			THROW_EXCEPTION(POGLResourceException, "Could not compile geometry shader. Reason: '%s'", infoLogg);
		case POGLShaderType::VERTEX_SHADER:
			THROW_EXCEPTION(POGLResourceException, "Could not compile vertex shader. Reason: '%s'", infoLogg);
		case POGLShaderType::FRAGMENT_SHADER:
			THROW_EXCEPTION(POGLResourceException, "Could not compile fragment shader. Reason: '%s'", infoLogg);
		default:
			THROW_EXCEPTION(POGLResourceException, "Could not compile shader. Reason: '%s'", infoLogg);
		}
	}

	return new POGLShader(shaderID, type);
}

IPOGLProgram* POGLDeviceContext::CreateProgramFromShaders(IPOGLShader** shaders)
{
	if (shaders == nullptr)
		THROW_EXCEPTION(POGLResourceException, "You must supply at least one shader to be able to create a program");

	// Attach all the shaders to the program
	const GLuint programID = glCreateProgram();
	for (IPOGLShader** ptr = shaders; *ptr != nullptr; ++ptr) {
		POGLShader* shader = static_cast<POGLShader*>(*ptr);
		glAttachShader(programID, shader->GetShaderID());

	}

	// Link program
	glLinkProgram(programID);

	// Detach the shaders when linking is complete: http://www.opengl.org/wiki/GLSL_Object
	for (IPOGLShader** ptr = shaders; *ptr != nullptr; ++ptr) {
		POGLShader* shader = static_cast<POGLShader*>(*ptr);
		glDetachShader(programID, shader->GetShaderID());
	}

	// Verify program
	GLint status = 0;
	glGetProgramiv(programID, GL_LINK_STATUS, &status);
	GLchar infoLogg[2048] = { 0 };
	glGetProgramInfoLog(programID, sizeof(infoLogg)-1, NULL, infoLogg);
	if (!status) {
		glDeleteProgram(programID);
		THROW_EXCEPTION(POGLResourceException, "Could not link the supplied shader programs. Reason: %s", infoLogg);
	}

	POGLProgram* program = new POGLProgram();
	program->PostConstruct(programID, this);
	return program;
}

IPOGLTexture1D* POGLDeviceContext::CreateTexture1D()
{
	return nullptr;
}

IPOGLTexture2D* POGLDeviceContext::CreateTexture2D(const POGL_SIZE& size, POGLTextureFormat::Enum format, const void* bytes)
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

IPOGLTexture3D* POGLDeviceContext::CreateTexture3D()
{
	return nullptr;
}

void POGLDeviceContext::ResizeTexture2D(IPOGLTexture2D* texture, const POGL_SIZE& size)
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

IPOGLFramebuffer* POGLDeviceContext::CreateFramebuffer(IPOGLTexture** textures)
{
	return CreateFramebuffer(textures, nullptr);
}

IPOGLFramebuffer* POGLDeviceContext::CreateFramebuffer(IPOGLTexture** textures, IPOGLTexture* depthStencilTexture)
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

IPOGLVertexBuffer* POGLDeviceContext::CreateVertexBuffer(const void* memory, POGL_UINT32 memorySize, const POGL_VERTEX_LAYOUT* layout, POGLPrimitiveType::Enum primitiveType, POGLBufferUsage::Enum bufferUsage)
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

IPOGLVertexBuffer* POGLDeviceContext::CreateVertexBuffer(const POGL_POSITION_VERTEX* memory, POGL_UINT32 memorySize, POGLPrimitiveType::Enum primitiveType, POGLBufferUsage::Enum bufferUsage)
{
	return CreateVertexBuffer(memory, memorySize, &POGL_POSITION_VERTEX_LAYOUT, primitiveType, bufferUsage);
}

IPOGLVertexBuffer* POGLDeviceContext::CreateVertexBuffer(const POGL_POSITION_COLOR_VERTEX* memory, POGL_UINT32 memorySize, POGLPrimitiveType::Enum primitiveType, POGLBufferUsage::Enum bufferUsage)
{
	return CreateVertexBuffer(memory, memorySize, &POGL_POSITION_COLOR_VERTEX_LAYOUT, primitiveType, bufferUsage);
}

IPOGLVertexBuffer* POGLDeviceContext::CreateVertexBuffer(const POGL_POSITION_TEXCOORD_VERTEX* memory, POGL_UINT32 memorySize, POGLPrimitiveType::Enum primitiveType, POGLBufferUsage::Enum bufferUsage)
{
	return CreateVertexBuffer(memory, memorySize, &POGL_POSITION_TEXCOORD_VERTEX_LAYOUT, primitiveType, bufferUsage);
}

IPOGLIndexBuffer* POGLDeviceContext::CreateIndexBuffer(const void* memory, POGL_UINT32 memorySize, POGLVertexType::Enum type, POGLBufferUsage::Enum bufferUsage)
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

IPOGLResource* POGLDeviceContext::CloneResource(IPOGLResource* resource)
{
	THROW_NOT_IMPLEMENTED_EXCEPTION();
}

void POGLDeviceContext::CopyResource(IPOGLResource* source, IPOGLResource* destination)
{
	THROW_NOT_IMPLEMENTED_EXCEPTION();
}

void POGLDeviceContext::CopyResource(IPOGLResource* source, IPOGLResource* destination, POGL_UINT32 sourceOffset, POGL_UINT32 destinationOffset, POGL_UINT32 size)
{
	THROW_NOT_IMPLEMENTED_EXCEPTION();
}

IPOGLRenderState* POGLDeviceContext::Apply(IPOGLProgram* program)
{
	mRenderState->Apply(program);
	mRenderState->AddRef();
	return mRenderState;
}

void* POGLDeviceContext::Map(IPOGLResource* resource, POGLResourceMapType::Enum e)
{
	auto type = resource->GetType();
	if (type == POGLResourceType::VERTEXBUFFER) {
		POGLVertexBuffer* vb = static_cast<POGLVertexBuffer*>(resource);
		mRenderState->BindVertexBuffer(vb);
		return glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	}

	THROW_NOT_IMPLEMENTED_EXCEPTION();
}

void* POGLDeviceContext::Map(IPOGLResource* resource, POGL_UINT32 offset, POGL_UINT32 length, POGLResourceMapType::Enum e)
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

void POGLDeviceContext::Unmap(IPOGLResource* resource)
{
	auto type = resource->GetType();
	if (type == POGLResourceType::VERTEXBUFFER) {
		glUnmapBuffer(GL_ARRAY_BUFFER);
		return;
	}

	THROW_NOT_IMPLEMENTED_EXCEPTION();
}

void POGLDeviceContext::SetViewport(const POGL_RECT& viewport)
{
	mRenderState->SetViewport(viewport);
}

void POGLDeviceContext::InitializeRenderState()
{
	if (mRenderState == nullptr) {
		mRenderState = new POGLRenderState(this);
	}
}

////////////////////////
/*!
	\brief Custom delete struct used with std::shared_ptr to delete arrays

	Example:
	{@code
	std::shared_ptr<byte> bytes(new byte[10], TDeleteArray<byte>());
	}
	*/
template<typename T>
struct TDeleteArray
{
	void operator()(T const * p) {
		delete[] p;
	}
};

IPOGLTexture2D* POGLXLoadBMPImageFromFile(IPOGLDeviceContext* context, const POGL_CHAR* fileName)
{
	assert_not_null(context);
	assert_not_null(fileName);

	// Open file
	FILE* file = open_file(fileName, POGL_TOCHAR("rb"));
	if (file == nullptr)
		THROW_EXCEPTION(POGLResourceException, "File not found: %s", fileName);

	fseek(file, 0L, SEEK_END);
	POGL_UINT32 sz = ftell(file);
	fseek(file, 0L, SEEK_SET);

	std::shared_ptr<POGL_CHAR> bytes(new POGL_CHAR[sz], TDeleteArray<POGL_CHAR>());
	fread(bytes.get(), 1, sz, file);
	fclose(file);
	return POGLXLoadBMPImageFromMemory(context, bytes.get(), sz);
}

IPOGLTexture2D* POGLXLoadBMPImageFromMemory(IPOGLDeviceContext* context, const POGL_CHAR* bytes, POGL_UINT32 size)
{
	assert_not_null(context);
	assert_not_null(bytes);
	assert_with_message(size > 0, "You cannot load a non-existing image");

	if (size < 54)
		THROW_EXCEPTION(POGLResourceException, "Invalid BMP file");

	if (bytes[0] != 'B' || bytes[1] != 'M')
		THROW_EXCEPTION(POGLResourceException, "Invalid BMP file");

	// Get num bits per pixel
	const POGL_UINT16 bitsPerPixel = bytes[28];

	// Verify 24 or 32 bit image type
	if (bitsPerPixel != 24 && bitsPerPixel != 32)
	{
		THROW_EXCEPTION(POGLResourceException, "Invalid File Format for file. 24 or 32 bit Image Required.");
	}

	// Data offset
	const POGL_UINT32 offset = bytes[10] + (bytes[11] << 8);

	// Read image size from header
	const POGL_SIZE imageSize(*(POGL_INT32*)&(bytes[0x12]), *(POGL_INT32*)&(bytes[0x16]));

	// Create a texture2D resource
	return context->CreateTexture2D(imageSize, POGLTextureFormat::BGR, &bytes[offset]);
}
