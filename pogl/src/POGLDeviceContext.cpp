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
#include "POGLSyncObject.h"
#include "POGLBufferResourceStream.h"
#include <gl/poglext.h>
#include <algorithm>

POGLDeviceContext::POGLDeviceContext(IPOGLDevice* device)
: mRenderState(nullptr), mResourceStream(nullptr), mDevice(device)
{
}

POGLDeviceContext::~POGLDeviceContext()
{
}

void POGLDeviceContext::Destroy()
{
	if (mResourceStream != nullptr) {
		if (mResourceStream->IsOpen())
			mResourceStream->Close();
		delete mResourceStream;
		mResourceStream = nullptr;
	}
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

	return new POGLShaderProgram(shaderID, mDevice, type);
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

	return new POGLEffect(programID, data, uniforms, mDevice);
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

	const GLuint textureID = GenTextureID();
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

	GLsync sync = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
	glFlush();
	POGLTexture2D* texture = new POGLTexture2D(textureID, size, format, new POGLSyncObject(sync, mDevice), mDevice);
	mRenderState->SetTextureResource((POGLTextureResource*)texture->GetHandlePtr());
	return texture;
}

IPOGLTexture3D* POGLDeviceContext::CreateTexture3D()
{
	return nullptr;
}

IPOGLVertexBuffer* POGLDeviceContext::CreateVertexBuffer(const void* memory, POGL_SIZE memorySize, const POGL_VERTEX_LAYOUT* layout, POGLPrimitiveType::Enum primitiveType, POGLBufferUsage::Enum bufferUsage)
{
	assert_not_null(memory);
	assert_with_message(memorySize > 0, "You cannot create a vertex buffer of no size");
	assert_not_null(layout);

	const POGL_UINT32 numVertices = memorySize / layout->vertexSize;
	const GLuint bufferID = GenBufferID();
	const GLenum usage = POGLEnum::Convert(bufferUsage);
	const GLenum type = POGLEnum::Convert(primitiveType);

	// 
	// Fill the buffer with data
	//

	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	glBufferData(GL_ARRAY_BUFFER, memorySize, memory, usage);

	const GLenum error = glGetError();
	if (error != GL_NO_ERROR)
		THROW_EXCEPTION(POGLResourceException, "Failed to create a vertex buffer. Reason: 0x%x", error);

	GLsync sync = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
	glFlush();
	return new POGLVertexBuffer(bufferID, layout, numVertices, type, usage, new POGLSyncObject(sync, mDevice), mDevice);
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

	static const POGL_UINT32 TYPE_SIZE[POGLVertexType::COUNT] = {
		sizeof(POGL_INT8),
		sizeof(POGL_UINT8),
		sizeof(POGL_INT16),
		sizeof(POGL_UINT16),
		sizeof(POGL_INT32),
		sizeof(POGL_UINT32),
		sizeof(POGL_FLOAT),
		sizeof(POGL_DOUBLE),
	};

	const POGL_UINT32 typeSize = (POGL_UINT32)TYPE_SIZE[type];
	const POGL_UINT32 numIndices = memorySize / typeSize;
	const GLuint bufferID = GenBufferID();
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

	GLsync sync = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
	glFlush();
	return new POGLIndexBuffer(bufferID, typeSize, numIndices, indiceType, usage, new POGLSyncObject(sync, mDevice), mDevice);
}

IPOGLRenderState* POGLDeviceContext::Apply(IPOGLEffect* effect)
{
	return mRenderState->Apply(effect);
}

IPOGLResourceStream* POGLDeviceContext::OpenStream(IPOGLVertexBuffer* resource, POGLResourceStreamType::Enum e)
{
	if (mResourceStream == nullptr)
		mResourceStream = new POGLBufferResourceStream(this);

	if (mResourceStream->IsOpen())
		THROW_EXCEPTION(POGLStreamException, "You cannot have multiple streams opened at the same time on the same context");

	POGLVertexBuffer* buffer = static_cast<POGLVertexBuffer*>(resource);
	mRenderState->BindVertexBuffer(buffer);
	mResourceStream->Open(resource, buffer->GetSyncObject(), GL_ARRAY_BUFFER, buffer->GetBufferUsage(), e);
	return mResourceStream;
}

IPOGLResourceStream* POGLDeviceContext::OpenStream(IPOGLIndexBuffer* resource, POGLResourceStreamType::Enum e)
{
	if (mResourceStream == nullptr)
		mResourceStream = new POGLBufferResourceStream(this);

	if (mResourceStream->IsOpen())
		THROW_EXCEPTION(POGLStreamException, "You cannot have multiple streams opened at the same time on the same context");

	POGLIndexBuffer* buffer = static_cast<POGLIndexBuffer*>(resource);
	mRenderState->BindIndexBuffer(buffer);
	mResourceStream->Open(resource, buffer->GetSyncObject(), GL_ELEMENT_ARRAY_BUFFER, buffer->GetBufferUsage(), e);
	return mResourceStream;
}

void POGLDeviceContext::InitializeRenderState()
{
	if (mRenderState == nullptr) {
		mRenderState = new POGLRenderState(this);
	}
}

void POGLDeviceContext::LoadExtensions()
{
	SET_EXTENSION_FUNC(PFNGLGENBUFFERSPROC, glGenBuffers);
	SET_EXTENSION_FUNC(PFNGLDELETEBUFFERSPROC, glDeleteBuffers);
	SET_EXTENSION_FUNC(PFNGLBINDBUFFERPROC, glBindBuffer);
	SET_EXTENSION_FUNC(PFNGLBUFFERDATAPROC, glBufferData);
	SET_EXTENSION_FUNC(PFNGLMAPBUFFERPROC, glMapBuffer);
	SET_EXTENSION_FUNC(PFNGLMAPBUFFERRANGEPROC, glMapBufferRange);
	SET_EXTENSION_FUNC(PFNGLUNMAPBUFFERPROC, glUnmapBuffer);

	SET_EXTENSION_FUNC(PFNGLUSEPROGRAMPROC, glUseProgram);

	SET_EXTENSION_FUNC(PFNGLUNIFORM1IPROC, glUniform1i);

	SET_EXTENSION_FUNC(PFNGLUNIFORM1IVPROC, glUniform1iv);
	SET_EXTENSION_FUNC(PFNGLUNIFORM2IVPROC, glUniform2iv);
	SET_EXTENSION_FUNC(PFNGLUNIFORM3IVPROC, glUniform3iv);
	SET_EXTENSION_FUNC(PFNGLUNIFORM4IVPROC, glUniform4iv);

	SET_EXTENSION_FUNC(PFNGLUNIFORM1UIVPROC, glUniform1uiv);
	SET_EXTENSION_FUNC(PFNGLUNIFORM2UIVPROC, glUniform2uiv);
	SET_EXTENSION_FUNC(PFNGLUNIFORM3UIVPROC, glUniform3uiv);
	SET_EXTENSION_FUNC(PFNGLUNIFORM4UIVPROC, glUniform4uiv);

	SET_EXTENSION_FUNC(PFNGLUNIFORM1FVPROC, glUniform1fv);
	SET_EXTENSION_FUNC(PFNGLUNIFORM2FVPROC, glUniform2fv);
	SET_EXTENSION_FUNC(PFNGLUNIFORM3FVPROC, glUniform3fv);
	SET_EXTENSION_FUNC(PFNGLUNIFORM4FVPROC, glUniform4fv);

	SET_EXTENSION_FUNC(PFNGLUNIFORM1DVPROC, glUniform1dv);
	SET_EXTENSION_FUNC(PFNGLUNIFORM2DVPROC, glUniform2dv);
	SET_EXTENSION_FUNC(PFNGLUNIFORM3DVPROC, glUniform3dv);
	SET_EXTENSION_FUNC(PFNGLUNIFORM4DVPROC, glUniform4dv);

	SET_EXTENSION_FUNC(PFNGLUNIFORMMATRIX4FVPROC, glUniformMatrix4fv);
	SET_EXTENSION_FUNC(PFNGLUNIFORMMATRIX4DVPROC, glUniformMatrix4dv);

	SET_EXTENSION_FUNC(PFNGLCLIENTWAITSYNCPROC, glClientWaitSync);
	SET_EXTENSION_FUNC(PFNGLWAITSYNCPROC, glWaitSync);
	SET_EXTENSION_FUNC(PFNGLFENCESYNCPROC, glFenceSync);
	SET_EXTENSION_FUNC(PFNGLDELETESYNCPROC, glDeleteSync);

	SET_EXTENSION_FUNC(PFNGLGENVERTEXARRAYSPROC, glGenVertexArrays);
	SET_EXTENSION_FUNC(PFNGLBINDVERTEXARRAYPROC, glBindVertexArray);
	SET_EXTENSION_FUNC(PFNGLDELETEVERTEXARRAYSPROC, glDeleteVertexArrays);

	SET_EXTENSION_FUNC(PFNGLENABLEVERTEXATTRIBARRAYPROC, glEnableVertexAttribArray);
	SET_EXTENSION_FUNC(PFNGLDISABLEVERTEXATTRIBARRAYPROC, glDisableVertexAttribArray);

	SET_EXTENSION_FUNC(PFNGLVERTEXATTRIBIPOINTERPROC, glVertexAttribIPointer);
	SET_EXTENSION_FUNC(PFNGLVERTEXATTRIBPOINTERPROC, glVertexAttribPointer);
	SET_EXTENSION_FUNC(PFNGLVERTEXATTRIBLPOINTERPROC, glVertexAttribLPointer);

	SET_EXTENSION_FUNC(PFNGLACTIVETEXTUREPROC, glActiveTexture);

	SET_EXTENSION_FUNC(PFNGLBINDSAMPLERPROC, glBindSampler);
	SET_EXTENSION_FUNC(PFNGLGENSAMPLERSPROC, glGenSamplers);
	SET_EXTENSION_FUNC(PFNGLDELETESAMPLERSPROC, glDeleteSamplers);
	SET_EXTENSION_FUNC(PFNGLSAMPLERPARAMETERIPROC, glSamplerParameteri);

	SET_EXTENSION_FUNC(PFNGLATTACHSHADERPROC, glAttachShader);
	SET_EXTENSION_FUNC(PFNGLCOMPILESHADERPROC, glCompileShader);
	SET_EXTENSION_FUNC(PFNGLCREATEPROGRAMPROC, glCreateProgram);
	SET_EXTENSION_FUNC(PFNGLCREATESHADERPROC, glCreateShader);
	SET_EXTENSION_FUNC(PFNGLDELETEPROGRAMPROC, glDeleteProgram);
	SET_EXTENSION_FUNC(PFNGLDELETESHADERPROC, glDeleteShader);
	SET_EXTENSION_FUNC(PFNGLDETACHSHADERPROC, glDetachShader);
	SET_EXTENSION_FUNC(PFNGLSHADERSOURCEPROC, glShaderSource);
	SET_EXTENSION_FUNC(PFNGLGETSHADERIVPROC, glGetShaderiv);
	SET_EXTENSION_FUNC(PFNGLGETSHADERINFOLOGPROC, glGetShaderInfoLog);
	SET_EXTENSION_FUNC(PFNGLLINKPROGRAMPROC, glLinkProgram);
	SET_EXTENSION_FUNC(PFNGLGETPROGRAMIVPROC, glGetProgramiv);
	SET_EXTENSION_FUNC(PFNGLGETPROGRAMINFOLOGPROC, glGetProgramInfoLog);
	SET_EXTENSION_FUNC(PFNGLGETACTIVEUNIFORMPROC, glGetActiveUniform);
	SET_EXTENSION_FUNC(PFNGLGETUNIFORMLOCATIONPROC, glGetUniformLocation);
}

void POGLDeviceContext::BindBuffer(GLenum target, GLuint bufferID)
{
	(*glBindBuffer)(target, bufferID);
}

void POGLDeviceContext::BufferData(GLenum target, GLsizeiptr size, const void *data, GLenum usage)
{
	(*glBufferData)(target, size, data, usage);
}

void POGLDeviceContext::DeleteBuffer(GLuint bufferID)
{
	if (bufferID == 0)
		return;

	(*glDeleteBuffers)(1, &bufferID);
}

void* POGLDeviceContext::MapBuffer(GLenum target, GLenum access)
{
	return (*glMapBuffer)(target, access);
}

void* POGLDeviceContext::MapBufferRange(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access)
{
	return (*glMapBufferRange)(target, offset, length, access);
}

GLboolean POGLDeviceContext::UnmapBuffer(GLenum target)
{
	return (*glUnmapBuffer)(target);
}

void POGLDeviceContext::UseProgram(GLuint programID)
{
	(*glUseProgram)(programID);
}

void POGLDeviceContext::Uniform1i(GLint location, GLint v0)
{
	(*glUniform1i)(location, v0);
}

void POGLDeviceContext::Uniform1iv(GLint location, GLsizei count, const GLint *value)
{
	(*glUniform1iv)(location, count, value);
}

void POGLDeviceContext::Uniform2iv(GLint location, GLsizei count, const GLint *value)
{
	(*glUniform2iv)(location, count, value);
}

void POGLDeviceContext::Uniform3iv(GLint location, GLsizei count, const GLint *value)
{
	(*glUniform3iv)(location, count, value);
}

void POGLDeviceContext::Uniform4iv(GLint location, GLsizei count, const GLint *value)
{
	(*glUniform4iv)(location, count, value);
}

void POGLDeviceContext::Uniform1uiv(GLint location, GLsizei count, const GLuint *value)
{
	(*glUniform1uiv)(location, count, value);
}

void POGLDeviceContext::Uniform2uiv(GLint location, GLsizei count, const GLuint *value)
{
	(*glUniform2uiv)(location, count, value);
}

void POGLDeviceContext::Uniform3uiv(GLint location, GLsizei count, const GLuint *value)
{
	(*glUniform3uiv)(location, count, value);
}

void POGLDeviceContext::Uniform4uiv(GLint location, GLsizei count, const GLuint *value)
{
	(*glUniform4uiv)(location, count, value);
}

void POGLDeviceContext::Uniform1fv(GLint location, GLsizei count, const GLfloat *value)
{
	(*glUniform1fv)(location, count, value);
}

void POGLDeviceContext::Uniform2fv(GLint location, GLsizei count, const GLfloat *value)
{
	(*glUniform2fv)(location, count, value);
}

void POGLDeviceContext::Uniform3fv(GLint location, GLsizei count, const GLfloat *value)
{
	(*glUniform3fv)(location, count, value);
}

void POGLDeviceContext::Uniform4fv(GLint location, GLsizei count, const GLfloat *value)
{
	(*glUniform4fv)(location, count, value);
}

void POGLDeviceContext::Uniform1dv(GLint location, GLsizei count, const GLdouble *value)
{
	(*glUniform1dv)(location, count, value);
}

void POGLDeviceContext::Uniform2dv(GLint location, GLsizei count, const GLdouble *value)
{
	(*glUniform2dv)(location, count, value);
}

void POGLDeviceContext::Uniform3dv(GLint location, GLsizei count, const GLdouble *value)
{
	(*glUniform3dv)(location, count, value);
}

void POGLDeviceContext::Uniform4dv(GLint location, GLsizei count, const GLdouble *value)
{
	(*glUniform4dv)(location, count, value);
}

void POGLDeviceContext::UniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
{
	(*glUniformMatrix4fv)(location, count, transpose, value);
}

void POGLDeviceContext::UniformMatrix4dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble* value)
{
	(*glUniformMatrix4dv)(location, count, transpose, value);
}

void POGLDeviceContext::EnableVertexAttribArray(GLuint index)
{
	(*glEnableVertexAttribArray)(index);
}

void POGLDeviceContext::DisableVertexAttribArray(GLuint index)
{
	(*glDisableVertexAttribArray)(index);
}

void POGLDeviceContext::VertexAttribIPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const void* pointer)
{
	(*glVertexAttribIPointer)(index, size, type, stride, pointer);
}

void POGLDeviceContext::VertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer)
{
	(*glVertexAttribPointer)(index, size, type, normalized, stride, pointer);
}

void POGLDeviceContext::VertexAttribLPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const void* pointer)
{
	(*glVertexAttribLPointer)(index, size, type, stride, pointer);
}

GLuint POGLDeviceContext::GenVertexArray()
{
	GLuint id = 0;
	(*glGenVertexArrays)(1, &id);

	const GLenum error = glGetError();
	if (id == 0 || error != GL_NO_ERROR)
		THROW_EXCEPTION(POGLResourceException, "Could not generate VertexArray ID");

	return id;
}

void POGLDeviceContext::BindVertexArray(GLuint id)
{
	(*glBindVertexArray)(id);
}

void POGLDeviceContext::DeleteVertexArray(GLuint id)
{
	(*glDeleteVertexArrays)(1, &id);
}

void POGLDeviceContext::ActiveTexture(GLenum texture)
{
	(*glActiveTexture)(texture);
}

GLuint POGLDeviceContext::GenSamplerID()
{
	GLuint id = 0;
	(*glGenSamplers)(1, &id);

	const GLenum error = glGetError();
	if (id == 0 || error != GL_NO_ERROR)
		THROW_EXCEPTION(POGLResourceException, "Could not generate sampler ID");

	return id;
}

void POGLDeviceContext::BindSampler(GLuint unit, GLuint sampler)
{
	(*glBindSampler)(unit, sampler);
}

void POGLDeviceContext::DeleteSampler(GLuint samplerObject)
{
	(*glDeleteSamplers)(1, &samplerObject);
}

void POGLDeviceContext::SamplerParameteri(GLuint sampler, GLenum pname, GLint param)
{
	(*glSamplerParameteri)(sampler, pname, param);
}

void POGLDeviceContext::DeleteShader(GLuint shader)
{
	(*glDeleteShader)(shader);
}

void POGLDeviceContext::DeleteProgram(GLuint program)
{
	(*glDeleteProgram)(program);
}

GLuint POGLDeviceContext::GenBufferID()
{
	GLuint id = 0;
	(*glGenBuffers)(1, &id);

	const GLenum error = glGetError();
	if (id == 0 || error != GL_NO_ERROR)
		THROW_EXCEPTION(POGLResourceException, "Could not generate buffer ID. Reason: 0x%x", error);

	return id;
}

void POGLDeviceContext::WaitSync(GLsync sync, GLbitfield flags, GLuint64 timeout)
{
	(*glWaitSync)(sync, flags, timeout);
}

GLenum POGLDeviceContext::ClientWaitSync(GLsync sync, GLbitfield flags, GLuint64 timeout)
{
	return (*glClientWaitSync)(sync, flags, timeout);
}

GLsync POGLDeviceContext::FenceSync(GLenum condition, GLbitfield flags)
{
	return (*glFenceSync)(condition, flags);
}

void POGLDeviceContext::DeleteSync(GLsync sync)
{
	(*glDeleteSync)(sync);
}

GLuint POGLDeviceContext::GenTextureID()
{
	GLuint id = 0;
	glGenTextures(1, &id);

	const GLenum error = glGetError();
	if (id == 0 || error != GL_NO_ERROR)
		THROW_EXCEPTION(POGLResourceException, "Could not generate texture ID. Reason: 0x%x", error);

	return id;
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
