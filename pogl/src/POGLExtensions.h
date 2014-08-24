#pragma once

#include <gl/pogl.h>

#ifndef CHECK_GL
#ifdef _DEBUG
#define CHECK_GL(message) { const GLenum error = glGetError(); if(error != GL_NO_ERROR) THROW_EXCEPTION(POGLException, POGL_TOCHAR(message)". Reason: 0x%x", error); }
#else
#define CHECK_GL(message)
#endif
#endif

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <gl/GL.h>
#include "win32/wglext.h"
#else
#include <gl/GL.h>
#endif

#include "glext.h"

extern PFNGLGENBUFFERSPROC _poglGenBuffers;
extern PFNGLDELETEBUFFERSPROC _poglDeleteBuffers;
extern PFNGLBINDBUFFERPROC _poglBindBuffer;
extern PFNGLBUFFERDATAPROC _poglBufferData;
extern PFNGLMAPBUFFERPROC _poglMapBuffer;
extern PFNGLMAPBUFFERRANGEPROC _poglMapBufferRange;
extern PFNGLUNMAPBUFFERPROC _poglUnmapBuffer;
extern PFNGLUSEPROGRAMPROC _poglUseProgram;
extern PFNGLUNIFORM1IPROC _poglUniform1i;
extern PFNGLUNIFORM1IVPROC _poglUniform1iv;
extern PFNGLUNIFORM2IVPROC _poglUniform2iv;
extern PFNGLUNIFORM3IVPROC _poglUniform3iv;
extern PFNGLUNIFORM4IVPROC _poglUniform4iv;
extern PFNGLUNIFORM1UIVPROC _poglUniform1uiv;
extern PFNGLUNIFORM2UIVPROC _poglUniform2uiv;
extern PFNGLUNIFORM3UIVPROC _poglUniform3uiv;
extern PFNGLUNIFORM4UIVPROC _poglUniform4uiv;
extern PFNGLUNIFORM1FVPROC _poglUniform1fv;
extern PFNGLUNIFORM2FVPROC _poglUniform2fv;
extern PFNGLUNIFORM3FVPROC _poglUniform3fv;
extern PFNGLUNIFORM4FVPROC _poglUniform4fv;
extern PFNGLUNIFORM1DVPROC _poglUniform1dv;
extern PFNGLUNIFORM2DVPROC _poglUniform2dv;
extern PFNGLUNIFORM3DVPROC _poglUniform3dv;
extern PFNGLUNIFORM4DVPROC _poglUniform4dv;
extern PFNGLUNIFORMMATRIX4FVPROC _poglUniformMatrix4fv;
extern PFNGLUNIFORMMATRIX4DVPROC _poglUniformMatrix4dv;
extern PFNGLCLIENTWAITSYNCPROC _poglClientWaitSync;
extern PFNGLWAITSYNCPROC _poglWaitSync;
extern PFNGLFENCESYNCPROC _poglFenceSync;
extern PFNGLDELETESYNCPROC _poglDeleteSync;
extern PFNGLGENVERTEXARRAYSPROC _poglGenVertexArrays;
extern PFNGLBINDVERTEXARRAYPROC _poglBindVertexArray;
extern PFNGLDELETEVERTEXARRAYSPROC _poglDeleteVertexArrays;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC _poglEnableVertexAttribArray;
extern PFNGLDISABLEVERTEXATTRIBARRAYPROC _poglDisableVertexAttribArray;
extern PFNGLVERTEXATTRIBIPOINTERPROC _poglVertexAttribIPointer;
extern PFNGLVERTEXATTRIBPOINTERPROC _poglVertexAttribPointer;
extern PFNGLVERTEXATTRIBLPOINTERPROC _poglVertexAttribLPointer;
extern PFNGLACTIVETEXTUREPROC _poglActiveTexture;
extern PFNGLBINDSAMPLERPROC _poglBindSampler;
extern PFNGLGENSAMPLERSPROC _poglGenSamplers;
extern PFNGLDELETESAMPLERSPROC _poglDeleteSamplers;
extern PFNGLSAMPLERPARAMETERIPROC _poglSamplerParameteri;
extern PFNGLATTACHSHADERPROC _poglAttachShader;
extern PFNGLCOMPILESHADERPROC _poglCompileShader;
extern PFNGLCREATEPROGRAMPROC _poglCreateProgram;
extern PFNGLCREATESHADERPROC _poglCreateShader;
extern PFNGLDELETEPROGRAMPROC _poglDeleteProgram;
extern PFNGLDELETESHADERPROC _poglDeleteShader;
extern PFNGLDETACHSHADERPROC _poglDetachShader;
extern PFNGLSHADERSOURCEPROC _poglShaderSource;
extern PFNGLGETSHADERIVPROC _poglGetShaderiv;
extern PFNGLGETSHADERINFOLOGPROC _poglGetShaderInfoLog;
extern PFNGLLINKPROGRAMPROC _poglLinkProgram;
extern PFNGLGETPROGRAMIVPROC _poglGetProgramiv;
extern PFNGLGETPROGRAMINFOLOGPROC _poglGetProgramInfoLog;
extern PFNGLGETACTIVEUNIFORMPROC _poglGetActiveUniform;
extern PFNGLGETUNIFORMLOCATIONPROC _poglGetUniformLocation;
extern PFNGLBINDFRAMEBUFFERPROC _poglBindFramebuffer;
extern PFNGLBINDRENDERBUFFERPROC _poglBindRenderbuffer;
extern PFNGLGENFRAMEBUFFERSPROC _poglGenFramebuffers;
extern PFNGLDELETERENDERBUFFERSPROC _poglDeleteRenderbuffers;
extern PFNGLDELETEFRAMEBUFFERSPROC _poglDeleteFramebuffers;
extern PFNGLFRAMEBUFFERTEXTUREPROC _poglFramebufferTexture;
extern PFNGLCHECKFRAMEBUFFERSTATUSPROC _poglCheckFramebufferStatus;
extern PFNGLDRAWBUFFERSPROC _poglDrawBuffers;
extern PFNGLCOPYBUFFERSUBDATAPROC _poglCopyBufferSubData;
extern PFNGLGETSTRINGIPROC _poglGetStringi;

#define glGenBuffers _poglGenBuffers
#define glDeleteBuffers _poglDeleteBuffers
#define glBindBuffer _poglBindBuffer
#define glBufferData _poglBufferData
#define glMapBuffer _poglMapBuffer
#define glMapBufferRange _poglMapBufferRange
#define glUnmapBuffer _poglUnmapBuffer
#define glUseProgram _poglUseProgram
#define glUniform1i _poglUniform1i
#define glUniform1iv _poglUniform1iv
#define glUniform2iv _poglUniform2iv
#define glUniform3iv _poglUniform3iv
#define glUniform4iv _poglUniform4iv
#define glUniform1uiv _poglUniform1uiv
#define glUniform2uiv _poglUniform2uiv
#define glUniform3uiv _poglUniform3uiv
#define glUniform4uiv _poglUniform4uiv
#define glUniform1fv _poglUniform1fv
#define glUniform2fv _poglUniform2fv
#define glUniform3fv _poglUniform3fv
#define glUniform4fv _poglUniform4fv
#define glUniform1dv _poglUniform1dv
#define glUniform2dv _poglUniform2dv
#define glUniform3dv _poglUniform3dv
#define glUniform4dv _poglUniform4dv
#define glUniformMatrix4fv _poglUniformMatrix4fv
#define glUniformMatrix4dv _poglUniformMatrix4dv
#define glClientWaitSync _poglClientWaitSync
#define glWaitSync _poglWaitSync
#define glFenceSync _poglFenceSync
#define glDeleteSync _poglDeleteSync
#define glGenVertexArrays _poglGenVertexArrays
#define glBindVertexArray _poglBindVertexArray
#define glDeleteVertexArrays _poglDeleteVertexArrays
#define glEnableVertexAttribArray _poglEnableVertexAttribArray
#define glDisableVertexAttribArray _poglDisableVertexAttribArray
#define glVertexAttribIPointer _poglVertexAttribIPointer
#define glVertexAttribPointer _poglVertexAttribPointer
#define glVertexAttribLPointer _poglVertexAttribLPointer
#define glActiveTexture _poglActiveTexture
#define glBindSampler _poglBindSampler
#define glGenSamplers _poglGenSamplers
#define glDeleteSamplers _poglDeleteSamplers
#define glSamplerParameteri _poglSamplerParameteri
#define glAttachShader _poglAttachShader
#define glCompileShader _poglCompileShader
#define glCreateProgram _poglCreateProgram
#define glCreateShader _poglCreateShader
#define glDeleteProgram _poglDeleteProgram
#define glDeleteShader _poglDeleteShader
#define glDetachShader _poglDetachShader
#define glShaderSource _poglShaderSource
#define glGetShaderiv _poglGetShaderiv
#define glGetShaderInfoLog _poglGetShaderInfoLog
#define glLinkProgram _poglLinkProgram
#define glGetProgramiv _poglGetProgramiv
#define glGetProgramInfoLog _poglGetProgramInfoLog
#define glGetActiveUniform _poglGetActiveUniform
#define glGetUniformLocation _poglGetUniformLocation
#define glBindFramebuffer _poglBindFramebuffer
#define glBindRenderbuffer _poglBindRenderbuffer
#define glGenFramebuffers _poglGenFramebuffers
#define glDeleteRenderbuffers _poglDeleteRenderbuffers
#define glDeleteFramebuffers _poglDeleteFramebuffers
#define glFramebufferTexture _poglFramebufferTexture
#define glCheckFramebufferStatus _poglCheckFramebufferStatus
#define glDrawBuffers _poglDrawBuffers
#define glCopyBufferSubData _poglCopyBufferSubData
#define glGetStringi _poglGetStringi

#ifdef WIN32
extern PFNWGLCREATECONTEXTATTRIBSARBPROC _powglCreateContextAttribsARB;

#define wglCreateContextAttribsARB _powglCreateContextAttribsARB
#endif

/*!
	\brief Load the neccessary extensions used by the POGL framework
*/
extern bool POGLLoadExtensions();

/*
	\brief Check to see if an extension is available
*/
extern bool POGLExtensionAvailable(const POGL_CHAR* ext);