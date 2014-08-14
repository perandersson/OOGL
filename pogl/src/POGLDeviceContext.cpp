#include "MemCheck.h"
#include "POGLDeviceContext.h"
#include "POGLRenderState.h"
#include "POGLDevice.h"
#include "POGLEnum.h"
#include "POGLVertexBuffer.h"
#include "POGLIndexBuffer.h"
#include "POGLTexture2D.h"
#include "POGLShaderProgram.h"
#include "POGLEffectData.h"
#include "POGLStringUtils.h"
#include "POGLFactory.h"
#include "POGLFramebuffer.h"
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
	if (mRenderState != nullptr) {
		mRenderState->Release();
		mRenderState = nullptr;
	}
}

IPOGLDevice* POGLDeviceContext::GetDevice()
{
	mDevice->AddRef();
	return mDevice;
}

IPOGLShaderProgram* POGLDeviceContext::CreateShaderProgramFromFile(const POGL_CHAR* path, POGLShaderProgramType::Enum type)
{
	POGL_ISTREAM stream(path);
	if (!stream.is_open())
		THROW_EXCEPTION(POGLResourceException, "Effect at path: '%s' could not be found", path);

	// Read the entire file into memory
	POGL_STRING str((std::istreambuf_iterator<POGL_CHAR>(stream)), std::istreambuf_iterator<POGL_CHAR>());
	return CreateShaderProgramFromMemory(str.c_str(), str.length(), type);
}

IPOGLShaderProgram* POGLDeviceContext::CreateShaderProgramFromMemory(const POGL_CHAR* memory, POGL_UINT32 size, POGLShaderProgramType::Enum type)
{
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
		case POGLShaderProgramType::GEOMETRY_SHADER:
			THROW_EXCEPTION(POGLResourceException, "Could not compile geometry shader. Reason: '%s'", infoLogg);
		case POGLShaderProgramType::VERTEX_SHADER:
			THROW_EXCEPTION(POGLResourceException, "Could not compile vertex shader. Reason: '%s'", infoLogg);
		case POGLShaderProgramType::FRAGMENT_SHADER:
			THROW_EXCEPTION(POGLResourceException, "Could not compile fragment shader. Reason: '%s'", infoLogg);
		default:
			THROW_EXCEPTION(POGLResourceException, "Could not compile shader. Reason: '%s'", infoLogg);
		}
	}

	return new POGLShaderProgram(shaderID, type);
}

IPOGLEffect* POGLDeviceContext::CreateEffectFromPrograms(IPOGLShaderProgram** programs, POGL_UINT32 numPrograms)
{
	assert_not_null(programs);
	assert_with_message(numPrograms > 0, "You cannot create an effect with no programs");

	// Attach all the shaders to the program
	const GLuint programID = glCreateProgram();
	for (POGL_UINT32 i = 0; i < numPrograms; ++i) {
		POGLShaderProgram* program = static_cast<POGLShaderProgram*>(programs[i]);
		glAttachShader(programID, program->GetShaderID());
	}

	// Link program
	glLinkProgram(programID);

	// Detach the shaders when linking is complete: http://www.opengl.org/wiki/GLSL_Object
	for (POGL_UINT32 i = 0; i < numPrograms; ++i) {
		POGLShaderProgram* program = static_cast<POGLShaderProgram*>(programs[i]);
		glDetachShader(programID, program->GetShaderID());
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

	POGLEffectData* data = new POGLEffectData;
	data->depthFunc = POGLDepthFunc::DEFAULT;
	data->colorMask = POGLColorMask::ALL;
	data->depthMask = true;
	data->depthTest = false;
	data->stencilTest = false;
	data->blending = false;
	data->srcFactor = POGLSrcFactor::DEFAULT;
	data->dstFactor = POGLDstFactor::DEFAULT;

	// Prepare uniforms
	GLint numUniforms = 0;
	glGetProgramiv(programID, GL_ACTIVE_UNIFORMS, &numUniforms);
	GLchar nameData[256] = { 0 };
	std::hash_map<POGL_STRING, std::shared_ptr<POGLUniformProperty>> uniforms;
	for (GLint uniformIndex = 0; uniformIndex < numUniforms; ++uniformIndex) {
		GLint arraySize = 0;
		GLenum type = 0;
		GLsizei actualLength = 0;

		//
		// http://www.opengl.org/sdk/docs/man/xhtml/glGetActiveUniform.xml
		// 

		glGetActiveUniform(programID, uniformIndex, sizeof(nameData), &actualLength, &arraySize, &type, nameData);
		nameData[actualLength] = 0;

		const POGL_STRING name = POGLStringUtils::ToString(nameData);
		const GLint componentID = glGetUniformLocation(programID, nameData);

		std::shared_ptr<POGLUniformProperty> p(new POGLUniformProperty());
		p->name = name;
		p->componentID = componentID;
		p->uniformType = type;
		p->minFilter = POGLMinFilter::DEFAULT;
		p->magFilter = POGLMagFilter::DEFAULT;
		p->wrap[0] = p->wrap[1] = p->wrap[3] = POGLTextureWrap::DEFAULT;
		p->compareFunc = POGLCompareFunc::DEFAULT;
		p->compareMode = POGLCompareMode::DEFAULT;
		uniforms.insert(std::make_pair(p->name, p));
	}

	return new POGLEffect(programID, data, uniforms);
}

IPOGLTexture1D* POGLDeviceContext::CreateTexture1D()
{
	return nullptr;
}

IPOGLTexture2D* POGLDeviceContext::CreateTexture2D(const POGL_SIZEI& size, POGLTextureFormat::Enum format, const void* bytes)
{
	assert_not_null(bytes);
	assert_with_message(size.x > 0.0f, "You cannot create a texture with 0 width");
	assert_with_message(size.y > 0.0f, "You cannot create a texture with 0 height");

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

	POGLTexture2D* texture = new POGLTexture2D(textureID, size, format);
	mRenderState->SetTextureResource((POGLTextureResource*)texture->GetResourcePtr());
	return texture;
}

IPOGLTexture3D* POGLDeviceContext::CreateTexture3D()
{
	return nullptr;
}

IPOGLFramebuffer* POGLDeviceContext::CreateFramebuffer(IPOGLTexture** textures, POGL_UINT32 numTextures)
{
	return CreateFramebuffer(textures, numTextures, nullptr);
}

IPOGLFramebuffer* POGLDeviceContext::CreateFramebuffer(IPOGLTexture** textures, POGL_UINT32 numTextures, IPOGLTexture* depthTexture)
{
	THROW_EXCEPTION(POGLInitializationException, "Not implemented");
	return nullptr;
}

IPOGLVertexBuffer* POGLDeviceContext::CreateVertexBuffer(const void* memory, POGL_SIZE memorySize, const POGL_VERTEX_LAYOUT* layout, POGLPrimitiveType::Enum primitiveType, POGLBufferUsage::Enum bufferUsage)
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

IPOGLVertexBuffer* POGLDeviceContext::CreateVertexBuffer(const POGL_POSITION_VERTEX* memory, POGL_SIZE memorySize, POGLPrimitiveType::Enum primitiveType, POGLBufferUsage::Enum bufferUsage)
{
	return CreateVertexBuffer(memory, memorySize, &POGL_POSITION_VERTEX_LAYOUT, primitiveType, bufferUsage);
}

IPOGLVertexBuffer* POGLDeviceContext::CreateVertexBuffer(const POGL_POSITION_COLOR_VERTEX* memory, POGL_SIZE memorySize, POGLPrimitiveType::Enum primitiveType, POGLBufferUsage::Enum bufferUsage)
{
	return CreateVertexBuffer(memory, memorySize, &POGL_POSITION_COLOR_VERTEX_LAYOUT, primitiveType, bufferUsage);
}

IPOGLVertexBuffer* POGLDeviceContext::CreateVertexBuffer(const POGL_POSITION_TEXCOORD_VERTEX* memory, POGL_SIZE memorySize, POGLPrimitiveType::Enum primitiveType, POGLBufferUsage::Enum bufferUsage)
{
	return CreateVertexBuffer(memory, memorySize, &POGL_POSITION_TEXCOORD_VERTEX_LAYOUT, primitiveType, bufferUsage);
}

IPOGLIndexBuffer* POGLDeviceContext::CreateIndexBuffer(const void* memory, POGL_SIZE memorySize, POGLVertexType::Enum type, POGLBufferUsage::Enum bufferUsage)
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

IPOGLRenderState* POGLDeviceContext::Apply(IPOGLEffect* effect)
{
	return mRenderState->Apply(effect);
}

void* POGLDeviceContext::Map(IPOGLResource* resource, POGLResourceStreamType::Enum e)
{
	THROW_EXCEPTION(POGLInitializationException, "Not implemented");
	return nullptr;
}

void* POGLDeviceContext::Map(IPOGLResource* resource, POGL_UINT32 offset, POGL_UINT32 length, POGLResourceStreamType::Enum e)
{
	auto type = resource->GetResourceType();
	if (type == POGLResourceType::VERTEXBUFFER) {
		POGLVertexBuffer* vb = static_cast<POGLVertexBuffer*>(resource);
		const POGL_UINT32 memorySize = vb->GetCount() * vb->GetLayout()->vertexSize;
		if (offset + length > memorySize)
			THROW_EXCEPTION(POGLResourceException, "You cannot map with offset: %d and length: %d when the vertex buffer size is: %d", offset, length, memorySize);

		mRenderState->BindVertexBuffer(vb);
		void* map = glMapBufferRange(GL_ARRAY_BUFFER, offset, length, GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT);
		return map;
	}

	THROW_EXCEPTION(POGLInitializationException, "Not implemented");
	return nullptr;
}

void POGLDeviceContext::Unmap(IPOGLResource* resource)
{
	auto type = resource->GetResourceType();
	if (type == POGLResourceType::VERTEXBUFFER) {
		glUnmapBuffer(GL_ARRAY_BUFFER);
		return;
	}
	THROW_EXCEPTION(POGLInitializationException, "Not implemented");
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
	const POGL_SIZEI imageSize(*(POGL_INT32*)&(bytes[0x12]), *(POGL_INT32*)&(bytes[0x16]));

	// Calculate pixel memory size
	const POGL_UINT32 memorySize = ((imageSize.width * bitsPerPixel + 31) / 32) * 4 * imageSize.height;

	// Create a texture2D resource
	return context->CreateTexture2D(imageSize, POGLTextureFormat::BGR, &bytes[offset]);
}
