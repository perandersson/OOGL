#include "POGLDeviceContext.hxx"
#include "POGLRenderState.hxx"
#include "POGLDevice.hxx"
#include "POGLEnum.hxx"
#include "POGLVertexBuffer.hxx"
#include "POGLIndexBuffer.hxx"

POGLDeviceContext::POGLDeviceContext(IPOGLDevice* device)
: mRenderState(nullptr), mRefCount(1), mDevice(device)
{
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

	const GLenum status = glGetError();
	if (status != GL_NO_ERROR)
		THROW_EXCEPTION(POGLResourceException, "Failed to create a vertex buffer");

	return new POGLVertexBuffer(bufferID, layout, numVertices, type, usage, mDevice);
}

IPOGLVertexBuffer* POGLDeviceContext::CreateVertexBuffer(const POGL_POSITION_VERTEX* memory, POGL_SIZE memorySize, POGLPrimitiveType::Enum primitiveType, POGLBufferUsage::Enum bufferUsage)
{
	return CreateVertexBuffer(memory, memorySize, &POGL_POSITION_VERTEX_LAYOUT, primitiveType, bufferUsage);
}

IPOGLVertexBuffer* POGLDeviceContext::CreateVertexBuffer(const POGL_POSITION_COLOR_VERTEX* memory, POGL_SIZE memorySize, POGLPrimitiveType::Enum primitiveType, POGLBufferUsage::Enum bufferUsage)
{
	return CreateVertexBuffer(memory, memorySize, &POGL_POSITION_COLOR_VERTEX_LAYOUT, primitiveType, bufferUsage);
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

	const GLenum status = glGetError();
	if (status != GL_NO_ERROR)
		THROW_EXCEPTION(POGLResourceException, "Failed to create a vertex buffer");

	return new POGLIndexBuffer(bufferID, typeSize, numIndices, indiceType, usage, mDevice);
}

IPOGLRenderState* POGLDeviceContext::Apply(IPOGLEffect* effect)
{
	if (mRenderState == nullptr) {
		mRenderState = new POGLRenderState(this);
	}

	return mRenderState->Apply(effect);
}

bool POGLDeviceContext::Initialize()
{
	return true;
}

void POGLDeviceContext::LoadExtensions()
{
	SET_EXTENSION_FUNC(PFNGLGENBUFFERSPROC, glGenBuffers);
	SET_EXTENSION_FUNC(PFNGLDELETEBUFFERSPROC, glDeleteBuffers);
	SET_EXTENSION_FUNC(PFNGLBINDBUFFERPROC, glBindBuffer);
	SET_EXTENSION_FUNC(PFNGLBUFFERDATAPROC, glBufferData);

	SET_EXTENSION_FUNC(PFNGLUSEPROGRAMPROC, glUseProgram);

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

	SET_EXTENSION_FUNC(PFNGLGENVERTEXARRAYSPROC, glGenVertexArrays);
	SET_EXTENSION_FUNC(PFNGLBINDVERTEXARRAYPROC, glBindVertexArray);
	SET_EXTENSION_FUNC(PFNGLDELETEVERTEXARRAYSPROC, glDeleteVertexArrays);

	SET_EXTENSION_FUNC(PFNGLENABLEVERTEXATTRIBARRAYPROC, glEnableVertexAttribArray);
	SET_EXTENSION_FUNC(PFNGLDISABLEVERTEXATTRIBARRAYPROC, glDisableVertexAttribArray);

	SET_EXTENSION_FUNC(PFNGLVERTEXATTRIBIPOINTERPROC, glVertexAttribIPointer);
	SET_EXTENSION_FUNC(PFNGLVERTEXATTRIBPOINTERPROC, glVertexAttribPointer);
	SET_EXTENSION_FUNC(PFNGLVERTEXATTRIBLPOINTERPROC, glVertexAttribLPointer);
}

void POGLDeviceContext::BindBuffer(GLenum target, GLuint bufferID)
{
	glBindBuffer(target, bufferID);
}

void POGLDeviceContext::DeleteBuffer(GLuint bufferID)
{
	if (bufferID == 0)
		return;

	glDeleteBuffers(1, &bufferID);
}

void POGLDeviceContext::UseProgram(GLuint programID)
{
	glUseProgram(programID);
}

void POGLDeviceContext::Uniform1iv(GLint location, GLsizei count, const GLint *value)
{
	glUniform1iv(location, count, value);
}

void POGLDeviceContext::Uniform2iv(GLint location, GLsizei count, const GLint *value)
{
	glUniform2iv(location, count, value);
}

void POGLDeviceContext::Uniform3iv(GLint location, GLsizei count, const GLint *value)
{
	glUniform3iv(location, count, value);
}

void POGLDeviceContext::Uniform4iv(GLint location, GLsizei count, const GLint *value)
{
	glUniform4iv(location, count, value);
}

void POGLDeviceContext::Uniform1uiv(GLint location, GLsizei count, const GLuint *value)
{
	glUniform1uiv(location, count, value);
}

void POGLDeviceContext::Uniform2uiv(GLint location, GLsizei count, const GLuint *value)
{
	glUniform2uiv(location, count, value);
}

void POGLDeviceContext::Uniform3uiv(GLint location, GLsizei count, const GLuint *value)
{
	glUniform3uiv(location, count, value);
}

void POGLDeviceContext::Uniform4uiv(GLint location, GLsizei count, const GLuint *value)
{
	glUniform4uiv(location, count, value);
}

void POGLDeviceContext::Uniform1fv(GLint location, GLsizei count, const GLfloat *value)
{
	glUniform1fv(location, count, value);
}

void POGLDeviceContext::Uniform2fv(GLint location, GLsizei count, const GLfloat *value)
{
	glUniform2fv(location, count, value);
}

void POGLDeviceContext::Uniform3fv(GLint location, GLsizei count, const GLfloat *value)
{
	glUniform3fv(location, count, value);
}

void POGLDeviceContext::Uniform4fv(GLint location, GLsizei count, const GLfloat *value)
{
	glUniform4fv(location, count, value);
}

void POGLDeviceContext::Uniform1dv(GLint location, GLsizei count, const GLdouble *value)
{
	glUniform1dv(location, count, value);
}

void POGLDeviceContext::Uniform2dv(GLint location, GLsizei count, const GLdouble *value)
{
	glUniform2dv(location, count, value);
}

void POGLDeviceContext::Uniform3dv(GLint location, GLsizei count, const GLdouble *value)
{
	glUniform3dv(location, count, value);
}

void POGLDeviceContext::Uniform4dv(GLint location, GLsizei count, const GLdouble *value)
{
	glUniform4dv(location, count, value);
}

void POGLDeviceContext::UniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
{
	glUniformMatrix4fv(location, count, transpose, value);
}

void POGLDeviceContext::UniformMatrix4dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble* value)
{
	glUniformMatrix4dv(location, count, transpose, value);
}

void POGLDeviceContext::EnableVertexAttribArray(GLuint index)
{
	glEnableVertexAttribArray(index);
}

void POGLDeviceContext::DisableVertexAttribArray(GLuint index)
{
	glDisableVertexAttribArray(index);
}

void POGLDeviceContext::VertexAttribIPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const void* pointer)
{
	glVertexAttribIPointer(index, size, type, stride, pointer);
}

void POGLDeviceContext::VertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer)
{
	glVertexAttribPointer(index, size, type, normalized, stride, pointer);
}

void POGLDeviceContext::VertexAttribLPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const void* pointer)
{
	glVertexAttribLPointer(index, size, type, stride, pointer);
}

GLuint POGLDeviceContext::GenVertexArray()
{
	GLuint id = 0;
	glGenVertexArrays(1, &id);

	const GLenum error = glGetError();
	if (id == 0 || error != GL_NO_ERROR)
		THROW_EXCEPTION(POGLResourceException, "Could not generate VertexArray ID");

	return id;
}

void POGLDeviceContext::BindVertexArray(GLuint id)
{
	glBindVertexArray(id);
}

void POGLDeviceContext::DeleteVertexArray(GLuint id)
{
	glDeleteVertexArrays(1, &id);
}

GLuint POGLDeviceContext::GenBufferID()
{
	GLuint id = 0;
	glGenBuffers(1, &id);

	const GLenum error = glGetError();
	if (id == 0 || error != GL_NO_ERROR)
		THROW_EXCEPTION(POGLResourceException, "Could not generate buffer ID");

	return id;
}
