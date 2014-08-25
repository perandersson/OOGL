#include "MemCheck.h"
#include "POGLFactory.h"
#include "POGLEnum.h"
#include "POGLTexture2D.h"
#include "POGLTextureResource.h"
#include "POGLShader.h"

GLuint POGLFactory::GenSamplerID()
{
	GLuint id = 0;
	glGenSamplers(1, &id);

	const GLenum error = glGetError();
	if (id == 0 || error != GL_NO_ERROR)
		THROW_EXCEPTION(POGLResourceException, "Could not generate sampler ID");

	return id;
}

GLuint POGLFactory::GenTextureID()
{
	GLuint id = 0;
	glGenTextures(1, &id);

	const GLenum error = glGetError();
	if (id == 0 || error != GL_NO_ERROR)
		THROW_EXCEPTION(POGLResourceException, "Could not generate texture ID. Reason: 0x%x", error);

	return id;
}

GLuint POGLFactory::CreateShader(const POGL_CHAR* memory, POGL_UINT32 size, POGLShaderType::Enum type)
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

	return shaderID;
}


GLuint POGLFactory::CreateProgram(IPOGLShader** shaders, POGL_UINT32 count)
{
	// Attach all the shaders to the program
	const GLuint programID = glCreateProgram();
	for (POGL_UINT32 i = 0; i < count; ++i) {
		POGLShader* shader = static_cast<POGLShader*>(shaders[i]);
		glAttachShader(programID, shader->GetShaderID());

	}

	// Link program
	glLinkProgram(programID);

	// Detach the shaders when linking is complete: http://www.opengl.org/wiki/GLSL_Object
	for (POGL_UINT32 i = 0; i < count; ++i) {
		POGLShader* shader = static_cast<POGLShader*>(shaders[i]);
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

	return programID;
}