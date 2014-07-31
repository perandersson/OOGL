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
