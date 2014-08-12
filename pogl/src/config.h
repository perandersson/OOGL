#pragma once

#include <gl/pogl.h>

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <gl/GL.h>
#include "win32/wglext.h"
#else
#include <gl/GL.h>
#endif

#include "glext.h"

#ifdef __GNUC__
#include <ext/hash_map>
namespace std { using namespace __gnu_cxx; }
#else
#include <hash_map>
#endif

#ifndef CHECK_GL
#ifdef _DEBUG
#define CHECK_GL(message) { const GLenum error = glGetError(); if(error != GL_NO_ERROR) THROW_EXCEPTION(POGLException, POGL_TOCHAR(message)". Reason: 0x%x", error); }
#else
#define CHECK_GL(message)
#endif
#endif

#ifndef FLT_EQ
#define FLT_EQ(val1, val2) (abs(val2 - val1) <= FLT_EPSILON)
#endif

#ifndef FLT_NEQ
#define FLT_NEQ(val1, val2) (abs(val2 - val1) > FLT_EPSILON)
#endif

#ifndef DBL_EQ
#define DBL_EQ(val1, val2) (abs(val2 - val1) <= DBL_EPSILON)
#endif

#ifndef DBL_NEQ
#define DBL_NEQ(val1, val2) (abs(val2 - val1) > DBL_EPSILON)
#endif

#include <fstream>
#include <sstream>
#include <cstdio>
#ifdef UNICODE
typedef std::wifstream POGL_ISTREAM;
typedef std::wstringstream POGL_STRINGSTREAM;
#define open_file _wfopen
#else
typedef std::ifstream POGL_ISTREAM;
typedef std::stringstream POGL_STRINGSTREAM;
#define open_file fopen
#endif
