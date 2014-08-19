#pragma once

#include <gl/pogl.h>
#include <gl/poglmath.h>

#include <atomic>
#include <cmath>

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

#ifndef POGL_M_E
#define POGL_M_E 2.71828182845904523536
#endif

#ifndef POGL_M_LOG2E
#define POGL_M_LOG2E 1.44269504088896340736
#endif

#ifndef POGL_M_LOG10E
#define POGL_M_LOG10E 0.434294481903251827651
#endif

#ifndef POGL_M_LN2
#define POGL_M_LN2 0.693147180559945309417
#endif

#ifndef POGL_M_LN10
#define POGL_M_LN10 2.30258509299404568402
#endif

#ifndef POGL_M_PI
#define POGL_M_PI 3.14159265358979323846
#endif

#ifndef POGL_M_PI_F
#define POGL_M_PI_F ((float32)POGL_M_PI)
#endif

#ifndef POGL_M_PI_DIV180
#define POGL_M_PI_DIV180 0.01745329251994329576
#endif

#ifndef POGL_M_PI_2
#define POGL_M_PI_2 1.57079632679489661923
#endif

#ifndef POGL_M_PI_4
#define POGL_M_PI_4 0.785398163397448309616
#endif

#ifndef POGL_M_1_PI
#define POGL_M_1_PI 0.318309886183790671538
#endif

#ifndef POGL_M_2_PI
#define POGL_M_2_PI 0.636619772367581343076
#endif

#ifndef POGL_M_1_SQRTPI
#define POGL_M_1_SQRTPI 0.564189583547756286948
#endif

#ifndef POGL_M_2_SQRTPI
#define POGL_M_2_SQRTPI 1.12837916709551257390
#endif

#ifndef POGL_M_SQRT2
#define POGL_M_SQRT2 1.41421356237309504880
#endif

#ifndef POGL_M_SQRT_2
#define POGL_M_SQRT_2 0.707106781186547524401
#endif

#ifndef POGL_ANG2RAD
#define POGL_ANG2RAD 0.01745329251994329576922
#endif
