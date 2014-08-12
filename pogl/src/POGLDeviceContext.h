#pragma once
#include "config.h"
#include <gl/pogl.h>

class POGLRenderState;
class POGLVertexBuffer;
class POGLIndexBuffer;
//class POGLBufferResourceStream;
class POGLDeviceContext : public IPOGLDeviceContext
{
public:
	POGLDeviceContext(IPOGLDevice* device);
	~POGLDeviceContext();

	/*!
		\brief Destroys this instance and removes the internal release the associated memory with it
	*/
	void Destroy();

	virtual IPOGLDevice* GetDevice();
	virtual IPOGLShaderProgram* CreateShaderProgramFromFile(const POGL_CHAR* path, POGLShaderProgramType::Enum type);
	virtual IPOGLShaderProgram* CreateShaderProgramFromMemory(const POGL_CHAR* memory, POGL_UINT32 size, POGLShaderProgramType::Enum type);
	virtual IPOGLEffect* CreateEffectFromPrograms(IPOGLShaderProgram** programs, POGL_UINT32 numPrograms);
	virtual IPOGLTexture1D* CreateTexture1D();
	virtual IPOGLTexture2D* CreateTexture2D(const POGL_SIZEI& size, POGLTextureFormat::Enum format, const void* bytes);
	virtual IPOGLTexture3D* CreateTexture3D();
	virtual IPOGLVertexBuffer* CreateVertexBuffer(const void* memory, POGL_SIZE memorySize, const POGL_VERTEX_LAYOUT* layout, POGLPrimitiveType::Enum primitiveType, POGLBufferUsage::Enum bufferUsage);
	virtual IPOGLVertexBuffer* CreateVertexBuffer(const POGL_POSITION_VERTEX* memory, POGL_SIZE memorySize, POGLPrimitiveType::Enum primitiveType, POGLBufferUsage::Enum bufferUsage);
	virtual IPOGLVertexBuffer* CreateVertexBuffer(const POGL_POSITION_COLOR_VERTEX* memory, POGL_SIZE memorySize, POGLPrimitiveType::Enum primitiveType, POGLBufferUsage::Enum bufferUsage);
	virtual IPOGLVertexBuffer* CreateVertexBuffer(const POGL_POSITION_TEXCOORD_VERTEX* memory, POGL_SIZE memorySize, POGLPrimitiveType::Enum primitiveType, POGLBufferUsage::Enum bufferUsage);
	virtual IPOGLIndexBuffer* CreateIndexBuffer(const void* memory, POGL_SIZE memorySize, POGLVertexType::Enum type, POGLBufferUsage::Enum bufferUsage);
	virtual IPOGLRenderState* Apply(IPOGLEffect* effect);
	virtual void* Map(IPOGLResource* resource, POGLResourceStreamType::Enum e);
	virtual void* Map(IPOGLResource* resource, POGL_UINT32 offset, POGL_UINT32 size, POGLResourceStreamType::Enum e);
	virtual void Unmap(IPOGLResource* resource);

	/*!
		\brief 
	*/
	void InitializeRenderState();
	
	/*!
		\brief Load the extensions for this device context
	*/
	virtual void LoadExtensions();

	/*!
		\brief Retrieves a function based on the functionName

		\param functionName
		\return
	*/
	virtual void* GetProcAddress(const char* functionName) = 0;

	/*!
		\brief Binds the supplied bufferID to this context

		\param target
		\param bufferID
	*/
	void BindBuffer(GLenum target, GLuint bufferID);

	/*!
	
	*/
	void BufferData(GLenum target, GLsizeiptr size, const void *data, GLenum usage);

	/*!
		\brief Delete the supplied bufferID

		\param bufferID
	*/
	void DeleteBuffer(GLuint bufferID);

	/*!
		\brief Map buffer

		\param target
		\param access
		\return
	*/
	void* MapBuffer(GLenum target, GLenum access);
	
	/*!
		\brief Map buffer

		\param target
		\param access
		\return
	*/
	void* MapBufferRange(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
	
	/*!
		\brief
	*/
	GLboolean UnmapBuffer(GLenum target);

	/*!
		\brief Use the supplied program

		\param programID
	*/
	void UseProgram(GLuint programID);

	/*!
		\brief Set the uniform
	*/
	void Uniform1i(GLint location, GLint v0);

	/*!
		\brief Set the uniform
	*/
	void Uniform1iv(GLint location, GLsizei count, const GLint *value);
	
	/*!
		\brief Set the uniform
	*/
	void Uniform2iv(GLint location, GLsizei count, const GLint *value);
	
	/*!
		\brief Set the uniform
	*/
	void Uniform3iv(GLint location, GLsizei count, const GLint *value);
	
	/*!
		\brief Set the uniform
	*/
	void Uniform4iv(GLint location, GLsizei count, const GLint *value);
	
	/*!
		\brief Set the uniform
	*/
	void Uniform1uiv(GLint location, GLsizei count, const GLuint *value);
	
	/*!
		\brief Set the uniform
	*/
	void Uniform2uiv(GLint location, GLsizei count, const GLuint *value);
	
	/*!
		\brief Set the uniform
	*/
	void Uniform3uiv(GLint location, GLsizei count, const GLuint *value);
	
	/*!
		\brief Set the uniform
	*/
	void Uniform4uiv(GLint location, GLsizei count, const GLuint *value);
	
	/*!
		\brief Set the uniform
	*/
	void Uniform1fv(GLint location, GLsizei count, const GLfloat *value);
	
	/*!
		\brief Set the uniform
	*/
	void Uniform2fv(GLint location, GLsizei count, const GLfloat *value);
	
	/*!
		\brief Set the uniform
	*/
	void Uniform3fv(GLint location, GLsizei count, const GLfloat *value);
	
	/*!
		\brief Set the uniform
	*/
	void Uniform4fv(GLint location, GLsizei count, const GLfloat *value);
	
	/*!
		\brief Set the uniform
	*/
	void Uniform1dv(GLint location, GLsizei count, const GLdouble *value);
	
	/*!
		\brief Set the uniform
	*/
	void Uniform2dv(GLint location, GLsizei count, const GLdouble *value);
	
	/*!
		\brief Set the uniform
	*/
	void Uniform3dv(GLint location, GLsizei count, const GLdouble *value);
	
	/*!
		\brief Set the uniform
	*/
	void Uniform4dv(GLint location, GLsizei count, const GLdouble *value);
	
	/*!
		\brief Set the uniform
	*/
	void UniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
	
	/*!
		\brief Set the uniform
	*/
	void UniformMatrix4dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);

	/*!
	
	*/
	void EnableVertexAttribArray(GLuint index);

	/*!

	*/
	void DisableVertexAttribArray(GLuint index);

	/*!
	
	*/
	void VertexAttribIPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const void* pointer);

	/*!

	*/
	void VertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);

	/*!
	
	*/
	void VertexAttribLPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const void* pointer);

	/*!
		\brief Creates a vertex array ID
	*/
	GLuint GenVertexArray();

	/*!
		\brief Bind vertex array ID for this context
	*/
	void BindVertexArray(GLuint id);

	/*!
		\brief Delete the supplied vertex array
	*/
	void DeleteVertexArray(GLuint id);

	/*!
		\brief
	*/
	void ActiveTexture(GLenum texture);

	/*!
	
	*/
	void WaitSync(GLsync sync, GLbitfield flags, GLuint64 timeout);

	/*!
	
	*/
	GLenum ClientWaitSync(GLsync sync, GLbitfield flags, GLuint64 timeout);

	/*!
	
	*/
	GLsync FenceSync(GLenum condition, GLbitfield flags);

	/*!
		\brief

		\param sync
	*/
	void DeleteSync(GLsync sync);

	/*!
		\brief Generate a new sampler ID
	*/
	GLuint GenSamplerID();

	/*!
		\brief Delete the supplied sampler object
	*/
	void BindSampler(GLuint unit, GLuint sampler);

	/*!
		\brief Delete the supplied sampler object
	*/
	void DeleteSampler(GLuint sampler);

	/*!
		\brief Set the sampler parameter
	*/
	void SamplerParameteri(GLuint sampler, GLenum pname, GLint param);

	/*!
		\brief Delete the supplied shader
	*/
	void DeleteShader(GLuint shader);

	/*!
		\brief Delete the supplied program
	*/
	void DeleteProgram(GLuint program);

private:
	/*!
		\brief Generate a new bufferID
	*/
	GLuint GenBufferID();

	/*!
		\brief Generate a new texture ID
	*/
	GLuint GenTextureID();

protected:
	IPOGLDevice* mDevice;
	POGLRenderState* mRenderState;
	//POGLBufferResourceStream* mResourceStream;

	//
	// Extensions
	//

	PFNGLGENBUFFERSPROC glGenBuffers;
	PFNGLDELETEBUFFERSPROC glDeleteBuffers;
	PFNGLBINDBUFFERPROC glBindBuffer;
	PFNGLBUFFERDATAPROC glBufferData;
	PFNGLMAPBUFFERPROC glMapBuffer;
	PFNGLMAPBUFFERRANGEPROC glMapBufferRange;
	PFNGLUNMAPBUFFERPROC glUnmapBuffer;

	PFNGLUSEPROGRAMPROC glUseProgram;

	PFNGLUNIFORM1IPROC glUniform1i;

	PFNGLUNIFORM1IVPROC glUniform1iv;
	PFNGLUNIFORM2IVPROC glUniform2iv;
	PFNGLUNIFORM3IVPROC glUniform3iv;
	PFNGLUNIFORM4IVPROC glUniform4iv;

	PFNGLUNIFORM1UIVPROC glUniform1uiv;
	PFNGLUNIFORM2UIVPROC glUniform2uiv;
	PFNGLUNIFORM3UIVPROC glUniform3uiv;
	PFNGLUNIFORM4UIVPROC glUniform4uiv;

	PFNGLUNIFORM1FVPROC glUniform1fv;
	PFNGLUNIFORM2FVPROC glUniform2fv;
	PFNGLUNIFORM3FVPROC glUniform3fv;
	PFNGLUNIFORM4FVPROC glUniform4fv;

	PFNGLUNIFORM1DVPROC glUniform1dv;
	PFNGLUNIFORM2DVPROC glUniform2dv;
	PFNGLUNIFORM3DVPROC glUniform3dv;
	PFNGLUNIFORM4DVPROC glUniform4dv;

	PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
	PFNGLUNIFORMMATRIX4DVPROC glUniformMatrix4dv;

	PFNGLCLIENTWAITSYNCPROC glClientWaitSync;
	PFNGLWAITSYNCPROC glWaitSync;
	PFNGLFENCESYNCPROC glFenceSync;
	PFNGLDELETESYNCPROC glDeleteSync;

	PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
	PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
	PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;

	PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
	PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;

	PFNGLVERTEXATTRIBIPOINTERPROC glVertexAttribIPointer;
	PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
	PFNGLVERTEXATTRIBLPOINTERPROC glVertexAttribLPointer;

	PFNGLACTIVETEXTUREPROC glActiveTexture;
	
	PFNGLBINDSAMPLERPROC glBindSampler;
	PFNGLGENSAMPLERSPROC glGenSamplers;
	PFNGLDELETESAMPLERSPROC glDeleteSamplers;
	PFNGLSAMPLERPARAMETERIPROC glSamplerParameteri;

	PFNGLATTACHSHADERPROC glAttachShader;
	PFNGLCOMPILESHADERPROC glCompileShader;
	PFNGLCREATEPROGRAMPROC glCreateProgram;
	PFNGLCREATESHADERPROC glCreateShader;
	PFNGLDELETEPROGRAMPROC glDeleteProgram;
	PFNGLDELETESHADERPROC glDeleteShader;
	PFNGLDETACHSHADERPROC glDetachShader;
	PFNGLSHADERSOURCEPROC glShaderSource;
	PFNGLGETSHADERIVPROC glGetShaderiv;
	PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
	PFNGLLINKPROGRAMPROC glLinkProgram;
	PFNGLGETPROGRAMIVPROC glGetProgramiv;
	PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
	PFNGLGETACTIVEUNIFORMPROC glGetActiveUniform;
	PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
};

#ifndef SET_EXTENSION_FUNC
#define SET_EXTENSION_FUNC(Type, Name) Name = (Type)GetProcAddress(#Name)
#endif
