#pragma once

#include <gl/pogl.hxx>

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <gl/GL.h>
#include "wglext.h"
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
