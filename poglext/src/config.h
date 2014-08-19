#pragma once

#include <gl/pogl.h>
#include <gl/poglext.h>

#include <atomic>

#ifdef __GNUC__
#include <ext/hash_map>
namespace std { using namespace __gnu_cxx; }
#else
#include <hash_map>
#endif

#include <string>
#ifdef UNICODE
typedef std::wstring POGL_STRING;
#define POGL_TOSTRING(x) L##x
#else
typedef std::string POGL_STRING;
#define POGL_TOSTRING(x) POGL_STRING(POGL_TOCHAR(x))
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
