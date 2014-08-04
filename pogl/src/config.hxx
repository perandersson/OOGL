#pragma once

#include <gl/pogl.hxx>

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <gl/GL.h>
#include "win32/wglext.h"
#else
#include <gl/GL.h>
#endif

#include "glext.h"

#ifdef WIN32
#define THREAD_LOCAL __declspec(thread)
#else
#define THREAD_LOCAL thread_local
#endif

#ifdef __GNUC__
#include <ext/hash_map>
namespace std { using namespace __gnu_cxx; }
#else
#include <hash_map>
#endif

#ifndef CHECK_GL
#ifdef _DEBUG
#define CHECK_GL(message) assert_with_message(glGetError() == GL_NO_ERROR, message);
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
#ifdef UNICODE
typedef std::wifstream POGL_ISTREAM;
typedef std::wstringstream POGL_STRINGSTREAM;
#else
typedef std::ifstream POGL_ISTREAM;
typedef std::stringstream POGL_STRINGSTREAM;
#endif
