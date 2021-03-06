#pragma once
#ifndef _POGL_H_
#define _POGL_H_

#include <exception>
#include <condition_variable>
#if defined(POGL_ENHANCED_INSTRUCTION_SET)
#include <xmmintrin.h>
#include <emmintrin.h>
#endif
#include <cinttypes>

#ifdef POGL_STATIC
#	define POGLAPI
#else
#	if defined(WIN32)
#		if defined(POGL_BUILD)
#			define POGLAPI __declspec(dllexport)
#		else
#			define POGLAPI __declspec(dllimport)
#		endif
#	else
#		if defined(__GNUC__) && __GNUC__>=4
#			define POGLAPI __attribute__ ((visibility("default")))
#		else
#			error Unknown compiler!
#		endif
#	endif
#endif

#ifndef OFFSET
#define OFFSET(x) ((char *)0 + x)
#endif

#ifndef BIT
#define BIT(x) 1 << x
#endif

#ifndef BIT_ISSET
#define BIT_ISSET(a, b) (( (a) & (b) ) != 0)
#endif

#ifndef BIT_UNSET
#define BIT_UNSET(a, b) (a &= ~b)
#endif

#ifndef BIT_SET
#define BIT_SET(a, b) (a |= (b))
#endif

#ifndef BIT_ALL
#define BIT_ALL 0xFFFFFFFFUL
#endif

#ifndef BIT_NONE
#define BIT_NONE 0
#endif

#ifndef POGL_DECLARE_ALIGN
#if defined(_MSC_VER)
#define POGL_DECLARE_ALIGN(x) __declspec(align(x))
#elif defined(__GNUC__)
#define POGL_DECLARE_ALIGN(x) __attribute__ ((aligned(x)))
#endif
#endif

//
// Classes
//

class POGLAPI IPOGLInterface;
class POGLAPI IPOGLResource;

class POGLAPI IPOGLDevice;
class POGLAPI IPOGLRenderContext;
class POGLAPI IPOGLRenderState;

class POGLAPI IPOGLDeferredRenderContext;

class POGLAPI IPOGLVertexBuffer;
class POGLAPI IPOGLIndexBuffer;

class POGLAPI IPOGLTexture;
class POGLAPI IPOGLTexture1D;
class POGLAPI IPOGLTexture2D;
class POGLAPI IPOGLTexture3D;
class POGLAPI IPOGLFramebuffer;

class POGLAPI IPOGLShader;
class POGLAPI IPOGLProgram;
class POGLAPI IPOGLUniform;

//
// typedefs
//

typedef int8_t POGL_INT8;
typedef uint8_t POGL_UINT8;
typedef int16_t POGL_INT16;
typedef uint16_t POGL_UINT16;
typedef int32_t POGL_INT32;
typedef uint32_t POGL_UINT32;
typedef int64_t POGL_INT64;
typedef uint64_t POGL_UINT64;
typedef void* POGL_HANDLE;
typedef unsigned char POGL_BYTE;
typedef float POGL_FLOAT;
typedef double POGL_DOUBLE;

#ifdef UNICODE
typedef wchar_t POGL_CHAR;
#define POGL_TOCHAR(x) L##x
#else
typedef char POGL_CHAR;
#define POGL_TOCHAR(x) x
#endif

//
// Enums
//

struct POGLAPI POGLDeviceInfoFlags
{
	enum Enum {
		//
		// This flag enables debug mode. Debug mode is differs depending on the graphics card you have.
		// AMD, for example, enables us to attach CodeXL (a remote debugger) and change stuff in runtime
		//
		DEBUG_MODE = BIT(0)
	};
};

struct POGLAPI POGLVendor
{
	enum Enum {
		AMD = 0,
		NVIDIA,
		INTEL,
		SOFTWARE,
		UNKNOWN
	};
};

struct POGLAPI POGLTextureFormat
{
	enum Enum {
		R = 0,
		R16F,
		R32F,
		RGB,
		RGB8,
		RGB12,
		RGB16,
		RGB32,
		RGBA,
		RGBA8,
		RGBA12,
		RGBA16,
		RGB10_A2,
		RGBA16F,
		RGBA32F,
		BGR,
		BGRA,
		DEPTH24,
		DEPTH32F,
		DEPTH24_STENCIL8,
		DEPTH32F_STENCIL8,

		/* Number of enums available */
		COUNT
	};
};

struct POGLAPI POGLPixelFormat
{
	enum Enum {
		R8G8B8A8 = 0,

		/* Number of enums available */
		COUNT
	};
};

struct POGLAPI POGLClearType
{
	enum Enum {
		COLOR = BIT(1),
		DEPTH = BIT(2),
		STENCIL = BIT(3)
	};

	/* Clear the color- and depth buffers */
	static const POGL_UINT32 COLOR_AND_DEPTH = COLOR | DEPTH;

	/* Clear the depth- and stencil buffers */
	static const POGL_UINT32 DEPTH_AND_STENCIL = DEPTH | STENCIL;

	/* Clear all buffers */
	static const POGL_UINT32 ALL = COLOR | DEPTH | STENCIL;
};

struct POGLAPI POGLShaderType
{
	enum Enum {
		GEOMETRY_SHADER = 0,
		VERTEX_SHADER,
		FRAGMENT_SHADER,

		/* Number of enums available */
		COUNT
	};
};

struct POGLAPI POGLVertexType
{
	enum Enum {
		BYTE = 0,
		UNSIGNED_BYTE,
		SHORT,
		UNSIGNED_SHORT,
		INT,
		UNSIGNED_INT,
		FLOAT,
		DOUBLE,

		/* Number of enums available */
		COUNT
	};
};

struct POGLAPI POGLPrimitiveType
{
	enum Enum {
		POINT = 0,
		TRIANGLE,
		TRIANGLE_STRIP,
		TRIANGLE_FAN,
		LINE,
		LINE_LOOP,

		/* Number of enums available */
		COUNT
	};
};

struct POGLAPI POGLBufferUsage
{
	enum Enum {
		/* The buffer object is handled as an immutable resource. */
		IMMUTABLE = 0,

		/* The buffer object is dynamic, i.e. it's changed sometimes or is small*/
		DYNAMIC,

		/* Buffers that we stream data continously from the CPU */
		STREAM,

		/* Number of enums available */
		COUNT
	};
};

struct POGLAPI POGLDepthFunc
{
	enum Enum {
		NEVER = 0,
		LESS,
		EQUAL,
		LEQUAL,
		GREATER,
		NOTEQUAL,
		GEQUAL,
		ALWAYS,

		/* Number of enums available */
		COUNT
	};

	/* Default depth function used by the rendering engine */
	static const Enum DEFAULT = LEQUAL;
};

struct POGLAPI POGLTextureWrap
{
	enum Enum {
		CLAMP_TO_EDGE = 0,
		CLAMP_TO_BORDER,
		MIRRORED_REPEAT,
		REPEAT,

		/* Number of enums available */
		COUNT
	};

	/* Default texture wrap used by the rendering engine */
	static const Enum DEFAULT = REPEAT;
};

struct POGLAPI POGLMinFilter
{
	enum Enum {
		NEAREST = 0,
		LINEAR,
		NEAREST_MIPMAP_NEAREST,
		LINEAR_MIPMAP_NEAREST,
		NEAREST_MIPMAP_LINEAR,
		LINEAR_MIPMAP_LINEAR,

		/* Number of enums available */
		COUNT
	};

	/* Default min filter used by the rendering engine */
	static const Enum DEFAULT = LINEAR;
};

struct POGLAPI POGLMagFilter
{
	enum Enum {
		NEAREST = 0,
		LINEAR,

		/* Number of enums available */
		COUNT
	};

	/* Default mag filter used by the rendering engine */
	static const Enum DEFAULT = LINEAR;
};

/*!
	\brief
*/
struct POGLAPI POGLCompareFunc
{
	enum Enum {
		NEVER = 0,
		LESS,
		EQUAL,
		LEQUAL,
		GREATER,
		NOTEQUAL,
		GEQUAL,
		ALWAYS,

		/* Number of enums available */
		COUNT
	};

	/* Default compare function used by the rendering engine */
	static const Enum DEFAULT = LEQUAL;
};

/*!
	\brief
*/
struct POGLAPI POGLCompareMode
{
	enum Enum {
		COMPARE_R_TO_TEXTURE,
		NONE,

		/* Number of enums available */
		COUNT
	};

	/* Default compare mode used by the rendering engine */
	static const Enum DEFAULT = NONE;
};

/*!

*/
struct POGLAPI POGLPolygonMode
{
	enum Enum {
		POINT = 0,
		LINE,
		FILL,

		COUNT
	};

	/* Default polygon mode used by the rendering engine */
	static const Enum DEFAULT = FILL;
};

struct POGLAPI POGLColorMask
{
	enum Enum {
		RED = BIT(1),
		GREEN = BIT(2),
		BLUE = BIT(3),
		ALPHA = BIT(4)
	};

	/* Enable all color components */
	static const POGL_UINT8 ALL = RED | GREEN | BLUE | ALPHA;

	/* Disable all color components */
	static const POGL_UINT8 NONE = 0;
};

struct POGLAPI POGLSrcFactor
{
	enum Enum {
		ZERO = 0,
		ONE,
		SRC_COLOR,
		ONE_MINUS_SRC_COLOR,
		SRC_ALPHA,
		ONE_MINUS_SRC_ALPHA,
		DST_ALPHA,
		ONE_MINUS_DST_ALPHA,
		DST_COLOR,
		ONE_MINUS_DST_COLOR,
		CONSTANT_COLOR,
		ONE_MINUS_CONSTANT_COLOR,
		CONSTANT_ALPHA,
		ONE_MINUS_CONSTANT_ALPHA,

		COUNT
	};

	/* Default source factor used by the rendering engine */
	static const Enum DEFAULT = ONE;
};

struct POGLAPI POGLDstFactor
{
	enum Enum {
		ZERO = 0,
		ONE,
		SRC_COLOR,
		ONE_MINUS_SRC_COLOR,
		SRC_ALPHA,
		ONE_MINUS_SRC_ALPHA,
		DST_ALPHA,
		ONE_MINUS_DST_ALPHA,
		DST_COLOR,
		ONE_MINUS_DST_COLOR,
		CONSTANT_COLOR,
		ONE_MINUS_CONSTANT_COLOR,
		CONSTANT_ALPHA,
		ONE_MINUS_CONSTANT_ALPHA,

		COUNT
	};

	/* Default destination factor used by the rendering engine */
	static const Enum DEFAULT = ZERO;
};

struct POGLAPI POGLFrontFace
{
	enum Enum {
		CW = 0,
		CCW,

		COUNT
	};

	/* Default front face used by the rendering engine */
	static const Enum DEFAULT = CCW;
};

/*!
	\brief
*/
struct POGLAPI POGLCullFace
{
	enum Enum {
		DISABLED = 0,
		FRONT,
		BACK,
		FRONT_AND_BACK,

		COUNT,
	};

	/* Default cull face used by the rendering engine */
	static const Enum DEFAULT = DISABLED;
};

/*!
	\brief Describes our intentions with the resource mapping
*/
struct POGLAPI POGLResourceMapType
{
	enum Enum {
		/* Our intentions is to read data to the resource */
		READ = 0,

		/* Our intentions is to write data to the resource */
		WRITE,

		COUNT
	};
};

/*!
	\brief Resource type enum
*/
struct POGLAPI POGLResourceType
{
	enum Enum {
		VERTEXBUFFER = 0,
		INDEXBUFFER,
		TEXTURE1D,
		TEXTURE2D,
		TEXTURE3D,
		SHADER,
		PROGRAM
	};
};

//
// Structs
//

#if !defined(POGL_ENHANCED_INSTRUCTION_SET)
typedef struct __POGL_VECTOR128
{
	union {
		POGL_UINT32 u[4];
		POGL_FLOAT  f[4];
	};
} __POGL_VECTOR128;

typedef struct __POGL_VECTOR64
{
	union {
		POGL_UINT32 u[2];
		POGL_FLOAT  f[2];
	};
} __POGL_VECTOR64;
#else
typedef __m128 __POGL_VECTOR128;
typedef __m64 __POGL_VECTOR64;
#endif

typedef struct POGLAPI POGL_SIZE
{
	union {
		struct {
			POGL_INT32 x;
			POGL_INT32 y;
		};
		struct {
			POGL_INT32 width;
			POGL_INT32 height;
		};
		POGL_INT32 size[2];
	};

	POGL_SIZE() {}
	POGL_SIZE(const POGL_INT32 _x, const POGL_INT32 _y) : x(_x), y(_y) {}
	POGL_SIZE(const POGL_SIZE& rhs) : x(rhs.x), y(rhs.y) {}

	POGL_SIZE& operator=(const POGL_SIZE& rhs);
	bool operator==(const POGL_SIZE& rhs) const;
	bool operator!=(const POGL_SIZE& rhs) const;
} POGL_POINTI;

struct POGLAPI POGL_VECTOR2
{
	union {
		struct {
			POGL_FLOAT x;
			POGL_FLOAT y;
		};
		struct {
			POGL_FLOAT s;
			POGL_FLOAT t;
		};
		POGL_FLOAT vec[2];
	};
	POGL_VECTOR2() {}
	POGL_VECTOR2(const POGL_FLOAT _x, const POGL_FLOAT _y) : x(_x), y(_y) {}
	POGL_VECTOR2(const POGL_VECTOR2& rhs) : x(rhs.x), y(rhs.y) {}

	POGL_VECTOR2& operator=(const POGL_VECTOR2& rhs);
	bool operator==(const POGL_VECTOR2& rhs) const;
	bool operator!=(const POGL_VECTOR2& rhs) const;
	POGL_VECTOR2 operator-() const;
	POGL_FLOAT operator[](POGL_UINT32 idx) const;
	POGL_FLOAT& operator[](POGL_UINT32 idx);
};

typedef struct POGLAPI POGL_VECTOR3
{
	union {
		struct {
			POGL_FLOAT x;
			POGL_FLOAT y;
			POGL_FLOAT z;
		};
		struct {
			POGL_FLOAT r;
			POGL_FLOAT g;
			POGL_FLOAT b;
		};
		POGL_FLOAT vec[3];
	};

	POGL_VECTOR3() {}
	POGL_VECTOR3(const POGL_FLOAT _x, const POGL_FLOAT _y, const POGL_FLOAT _z) : x(_x), y(_y), z(_z) {}
	POGL_VECTOR3(const POGL_VECTOR3& rhs) : x(rhs.x), y(rhs.y), z(rhs.z) {}

	POGL_VECTOR3& operator=(const POGL_VECTOR3& rhs);
	bool operator==(const POGL_VECTOR3& rhs) const;
	bool operator!=(const POGL_VECTOR3& rhs) const;
	POGL_VECTOR3 operator-() const;
	POGL_FLOAT operator[](POGL_UINT32 idx) const;
	POGL_FLOAT& operator[](POGL_UINT32 idx);
} POGL_COLOR3;

typedef struct POGLAPI POGL_VECTOR4
{
	union {
		struct {
			POGL_FLOAT x;
			POGL_FLOAT y;
			POGL_FLOAT z;
			POGL_FLOAT w;
		};
		struct {
			POGL_FLOAT r;
			POGL_FLOAT g;
			POGL_FLOAT b;
			POGL_FLOAT a;
		};
		POGL_FLOAT vec[4];
	};

	POGL_VECTOR4() {}
	POGL_VECTOR4(const POGL_FLOAT _x, const POGL_FLOAT _y, const POGL_FLOAT _z, const POGL_FLOAT _w) : x(_x), y(_y), z(_z), w(_w) {}
	POGL_VECTOR4(const POGL_VECTOR4& rhs) : x(rhs.x), y(rhs.y), z(rhs.z), w(rhs.w) {}

	POGL_VECTOR4& operator=(const POGL_VECTOR4& rhs);
	bool operator==(const POGL_VECTOR4& rhs) const;
	bool operator!=(const POGL_VECTOR4& rhs) const;
	POGL_VECTOR4 operator-() const;
	POGL_FLOAT operator[](POGL_UINT32 idx) const;
	POGL_FLOAT& operator[](POGL_UINT32 idx);
} POGL_COLOR4;

struct POGLAPI POGL_RECT
{
	union {
		struct {
			POGL_INT32 x;
			POGL_INT32 y;
			POGL_INT32 width;
			POGL_INT32 height;
		};
		POGL_INT32 size[4];
	};

	POGL_RECT() {}
	POGL_RECT(const POGL_INT32 _x, const POGL_INT32 _y, const POGL_INT32 _width, const POGL_INT32 _height) : x(_x), y(_y), width(_width), height(_height) {}
	POGL_RECT(const POGL_RECT& rhs) : x(rhs.x), y(rhs.y), width(rhs.width), height(rhs.height) {}

	POGL_RECT& operator=(const POGL_RECT& rhs);
	bool operator==(const POGL_RECT& rhs) const;
	bool operator!=(const POGL_RECT& rhs) const;
};

/*!
	\brief A 4x4 matrix

	Accessing the individual matrix values directly using the "m"- or "vec" field is column-major. If you
	want to access the fields as if they are row-major then you can do:

	{@code
		POGL_MAT4 mat;
		POGL_FLOAT value = mat[column * 4 + row];
	}

	You can also do:

	{@code
		POGL_MAT4 mat;
		POGL_FLOAT value = mat.m[column][row];
	}
*/
struct POGLAPI POGL_MAT4
{
	union {
		struct {
			POGL_FLOAT
			_11, _21, _31, _41,
			_12, _22, _32, _42,
			_13, _23, _33, _43,
			_14, _24, _34, _44;
		};
		struct {
			POGL_FLOAT m[4][4];
		};
		POGL_FLOAT vec[16];
	};

	POGL_MAT4() :
		_11(1.0f), _21(0.0f), _31(0.0f), _41(0.0f),
		_12(0.0f), _22(1.0f), _32(0.0f), _42(0.0f),
		_13(0.0f), _23(0.0f), _33(1.0f), _43(0.0f),
		_14(0.0f), _24(0.0f), _34(0.0f), _44(1.0f)
	{}

	POGL_MAT4(const POGL_MAT4& rhs) :
		_11(rhs._11), _21(rhs._21), _31(rhs._31), _41(rhs._41),
		_12(rhs._12), _22(rhs._22), _32(rhs._32), _42(rhs._42),
		_13(rhs._13), _23(rhs._23), _33(rhs._33), _43(rhs._43),
		_14(rhs._14), _24(rhs._24), _34(rhs._34), _44(rhs._44)
	{}

	POGL_FLOAT operator[](POGL_UINT32 idx) const;
	POGL_FLOAT& operator[](POGL_UINT32 idx);
};

/*!
	\brief
*/
struct POGLAPI POGL_DEVICE_INFO
{
	/* The window the IPOGLRenderContext should be associated with. If nullptr then the context will "draw" to the console instead */
	POGL_HANDLE windowHandle;

	/* How many bits are used for each color component */
	POGL_UINT8 colorBits;

	/* How many bits used for the depth buffer */
	POGL_UINT8 depthBits; 
	
	/* The device pixel format */
	POGLPixelFormat::Enum pixelFormat;

	/* Extra flags for the device info, for example: DEBUG mode */
	POGL_UINT8 flags;
};

/*!
	\brief
*/
struct POGLAPI POGL_VERTEX_LAYOUT_FIELD
{
	/*!
		The size of the current buffer field type. Use {@code sizeof(Type)}, for example: {@code sizeof(POGL_VEC3F)}
	*/
	POGL_UINT8 fieldSize;

	/*!
		What datatype should the element be handled as in the shader. the POGL_VEC3F type has POGL_FLOAT values, which means
		that this field should be POGLVertexLayoutFieldType::FLOAT
	*/
	POGLVertexType::Enum type;

	/*!
		Tell the graphics driver if it should normalize the value before it being supplied to the shader program
	*/
	POGL_UINT8 normalize;
};

/* */
static const POGL_UINT32 MAX_VERTEX_LAYOUT_FIELD_SIZE = 8;

/*!
	\brief
*/
struct POGLAPI POGL_VERTEX_LAYOUT
{
	/* The vertex fields */
	POGL_VERTEX_LAYOUT_FIELD fields[MAX_VERTEX_LAYOUT_FIELD_SIZE];

	/* The size of each vertex. This value will also be used as an stride between each vertex (This is most likely sizeof(Type)). */
	POGL_UINT32 vertexSize;
};


/*!
	\brief Vertex containing a position
*/
struct POGLAPI POGL_POSITION_VERTEX
{
	POGL_VECTOR3 position;

	POGL_POSITION_VERTEX() {}
	POGL_POSITION_VERTEX(const POGL_VECTOR3& p) : position(p) {}
	POGL_POSITION_VERTEX(const POGL_POSITION_VERTEX& rhs) : position(rhs.position) {}

	POGL_POSITION_VERTEX& operator=(const POGL_POSITION_VERTEX& rhs);
};

/* Vertex layout for the POGL_POSITION_VERTEX struct. */
static const POGL_VERTEX_LAYOUT POGL_POSITION_VERTEX_LAYOUT = {
	{
		{ sizeof(POGL_VECTOR3), POGLVertexType::FLOAT, false },
		0
	},
	sizeof(POGL_POSITION_VERTEX)
};

/*!
	\brief Vertex containing a position and color
*/
struct POGLAPI POGL_POSITION_COLOR_VERTEX
{
	POGL_VECTOR3 position;
	POGL_COLOR4 color;

	POGL_POSITION_COLOR_VERTEX() {}
	POGL_POSITION_COLOR_VERTEX(const POGL_VECTOR3& p, const POGL_COLOR4& c) : position(p), color(c) {}
	POGL_POSITION_COLOR_VERTEX(const POGL_POSITION_COLOR_VERTEX& rhs) : position(rhs.position), color(rhs.color) {}

	POGL_POSITION_COLOR_VERTEX& operator=(const POGL_POSITION_COLOR_VERTEX& rhs);
};

/* Vertex layout for the POGL_POSITION_COLOR_VERTEX struct. */
static const POGL_VERTEX_LAYOUT POGL_POSITION_COLOR_VERTEX_LAYOUT = {
	{
		{ sizeof(POGL_VECTOR3), POGLVertexType::FLOAT, false },
		{ sizeof(POGL_COLOR4), POGLVertexType::FLOAT, false },
		0
	},
	sizeof(POGL_POSITION_COLOR_VERTEX)
};

/*!
	\brief Vertex containing a position and a texture coordinate
*/
struct POGLAPI POGL_POSITION_TEXCOORD_VERTEX
{
	POGL_VECTOR3 position;
	POGL_VECTOR2 texCoord;

	POGL_POSITION_TEXCOORD_VERTEX() {}
	POGL_POSITION_TEXCOORD_VERTEX(const POGL_VECTOR3& p, const POGL_VECTOR2& t) : position(p), texCoord(t) {}
	POGL_POSITION_TEXCOORD_VERTEX(const POGL_POSITION_TEXCOORD_VERTEX& rhs) : position(rhs.position), texCoord(rhs.texCoord) {}

	POGL_POSITION_TEXCOORD_VERTEX& operator=(const POGL_POSITION_TEXCOORD_VERTEX& rhs);
};

/* Vertex layout for the POGL_POSITION_TEXCOORD_VERTEX struct. */
static const POGL_VERTEX_LAYOUT POGL_POSITION_TEXCOORD_VERTEX_LAYOUT = {
	{
		{ sizeof(POGL_VECTOR3), POGLVertexType::FLOAT, false },
		{ 0 },
		{ sizeof(POGL_VECTOR2), POGLVertexType::FLOAT, false },
		0
	},
	sizeof(POGL_POSITION_TEXCOORD_VERTEX)
};

//
// Class Definitions
//

class POGLAPI IPOGLInterface
{
public:
	virtual ~IPOGLInterface() {}

	/*!
		\brief Increase the reference counter with 1

		Reference counters are used internally to prevent resources from being deleted when they are in use
	*/
	virtual void AddRef() = 0;

	/*!
		\brief Decrease the reference counter with 1

		Reference counters are used internally to prevent resources from being deleted when they are in use
	*/
	virtual void Release() = 0;
};

/*!
	\brief
*/
class POGLAPI IPOGLResource : public IPOGLInterface
{
public:
	/*!
		\brief Retrieves the resource type
	*/
	virtual POGLResourceType::Enum GetType() const = 0;
};

/*!
	\brief
*/
class POGLAPI IPOGLDevice : public IPOGLInterface
{
public:
	/*!
		\brief Creates a new device context

		You use the device context to draw the geometry onto the screen

		\return A device context
	*/
	virtual IPOGLRenderContext* GetRenderContext() = 0;

	/*!
		\brief Creates a new deferred device context

		A deferred device context is esentially a command queue implementation for the IPOGLRenderContext interface. You invoke it as you normally would and
		then supply it to be rendered in the main thread when it's done

		\return A device context
	*/
	virtual IPOGLDeferredRenderContext* CreateDeferredRenderContext() = 0;

	/*!
		\brief Swap buffers
	*/
	virtual void EndFrame() = 0;

	/*!
		\brief Retrieves the device info
	*/
	virtual const POGL_DEVICE_INFO* GetDeviceInfo() const = 0;

	/*!
		\brief Retrieves the vendor for the current computer
	*/
	virtual POGLVendor::Enum GetVendor() const = 0;
};

/*!
	\brief
*/
class POGLAPI IPOGLRenderContext : public IPOGLInterface
{
public:
	/*!
		\brief Retrieves the device
	*/
	virtual IPOGLDevice* GetDevice() = 0;

	/*!
		\brief Creates a shader based on the supplied file

		\param path
				The path to the file where the shader source code is located
		\param type
				The shader type
		\throws POGLResourceException
				Exception thrown if the shader source code is not found or is invalid
		\return A shader resource
	*/
	virtual IPOGLShader* CreateShaderFromFile(const POGL_CHAR* path, POGLShaderType::Enum type) = 0;

	/*!
		\brief Creates a shader based on the memory buffer

		\param memory
				String containing the shader source code
		\param size
				The size of the memory
		\param type
				The shader type
		\throws POGLResourceException
				Exception thrown if the shader source code is invalid
		\return A shader resource
	*/
	virtual IPOGLShader* CreateShaderFromMemory(const POGL_CHAR* memory, POGL_UINT32 size, POGLShaderType::Enum type) = 0;

	/*!
		\brief Creates a GPU program based on the supplied shaders

		\param shaders
				The shaders we want to link when creating the program.
				{@code
					IPOGLShader** shaders = { shader1, shader2 };
					IPOGLProgram* program = context->CreateProgramFromShaders(shaders, 2);
				}
		\param count
				The number of shaders we want to link
		\throws POGLResourceException
				Exception is thrown if the exception failed to be loaded by some reason.
		\return A GPU program that you can use when render geometry onto the screen
	*/
	virtual IPOGLProgram* CreateProgramFromShaders(IPOGLShader** shaders, POGL_UINT32 count) = 0;

	/*!
		\brief Creates a 3D texture
	*/
	virtual IPOGLTexture1D* CreateTexture1D() = 0;

	/*!
		\brief Creates a 2D texture

		\param size
				Texture geometry size (width and height)
		\param format
				Texture format
		\param bytes
				Bytes containing the texture data.
		\return
	*/
	virtual IPOGLTexture2D* CreateTexture2D(const POGL_SIZE& size, POGLTextureFormat::Enum format, const void* bytes) = 0;

	/*!
		\brief Creates a 3D texture
	*/
	virtual IPOGLTexture3D* CreateTexture3D() = 0;

	/*!
		\brief Resize the supplied texture

		\param texture
				The texture we want to resize
		\param size
				The new texture size
	*/
	virtual void ResizeTexture2D(IPOGLTexture2D* texture, const POGL_SIZE& size) = 0;

	/*!
		\brief Creates a framebuffer that renders to the supplied textures

		\param textures
				The textures we want to use for the framebuffer.
				{@code
					IPOGLTexture** textures = { texture1, texture2 };
					context->CreateFramebuffer(textures, 2);
				}
		\param count
				The number of textures
		\return A framebuffer instance. You can use the framebuffer instance with the IPOGLRenderState to render to a texture
	*/
	virtual IPOGLFramebuffer* CreateFramebuffer(IPOGLTexture** textures, POGL_UINT32 count) = 0;

	/*!
		\brief Creates a framebuffer that renders to the supplied textures

		\param textures
				The textures we want to use for the framebuffer.
				{@code
					IPOGLTexture** textures = { texture1, texture2 };
					context->CreateFramebuffer(textures, 2, depthBuffer);
				}
		\param count
				The number of textures
		\param depthStencilTexture
				A depth- and/or stencil texture
		\return A framebuffer instance. You can use the framebuffer instance with the IPOGLRenderState to render to a texture
	*/
	virtual IPOGLFramebuffer* CreateFramebuffer(IPOGLTexture** textures, POGL_UINT32 count, IPOGLTexture* depthStencilTexture) = 0;

	/*!
		\brief Creates a vertex buffer based on the supplied parameters

		\param memory
		\param memorySize
		\param layout
		\param primitiveType
		\param bufferUsage
		\return
	*/
	virtual IPOGLVertexBuffer* CreateVertexBuffer(const void* memory, POGL_UINT32 memorySize, const POGL_VERTEX_LAYOUT* layout, POGLPrimitiveType::Enum primitiveType, POGLBufferUsage::Enum bufferUsage) = 0;

	/*!
		\brief Creates a vertex buffer based on the supplied parameters

		\param memory
		\param memorySize
		\param primitiveType
		\param bufferUsage
		\return
	*/
	virtual IPOGLVertexBuffer* CreateVertexBuffer(const POGL_POSITION_VERTEX* memory, POGL_UINT32 memorySize, POGLPrimitiveType::Enum primitiveType, POGLBufferUsage::Enum bufferUsage) = 0;

	/*!
		\brief Creates a vertex buffer based on the supplied parameters

		\param memory
		\param memorySize
		\param primitiveType
		\param bufferUsage
		\return
	*/
	virtual IPOGLVertexBuffer* CreateVertexBuffer(const POGL_POSITION_COLOR_VERTEX* memory, POGL_UINT32 memorySize, POGLPrimitiveType::Enum primitiveType, POGLBufferUsage::Enum bufferUsage) = 0;

	/*!
		\brief Creates a vertex buffer based on the supplied parameters

		\param memory
		\param memorySize
		\param primitiveType
		\param bufferUsage
		\return
	*/
	virtual IPOGLVertexBuffer* CreateVertexBuffer(const POGL_POSITION_TEXCOORD_VERTEX* memory, POGL_UINT32 memorySize, POGLPrimitiveType::Enum primitiveType, POGLBufferUsage::Enum bufferUsage) = 0;

	/*!
		\brief Creates an index buffer based on the supplied parameters

		\param memory
		\param memorySize
		\param type
		\param bufferUsage
	*/
	virtual IPOGLIndexBuffer* CreateIndexBuffer(const void* memory, POGL_UINT32 memorySize, POGLVertexType::Enum type, POGLBufferUsage::Enum bufferUsage) = 0;

	/*!
		\brief Clones the supplied resource and returns a new resource based of it

		\param resource
				Resource we want to clone
	*/
	virtual IPOGLResource* CloneResource(IPOGLResource* resource) = 0;

	/*!
		\brief Copy the source resource into the destination resource

		The destination resource will be resized to fit the source resource if they are different

		\param source
				Resource where the data will be copied from
		\param destination
				Resource where the data will be copied to
	*/
	virtual void CopyResource(IPOGLResource* source, IPOGLResource* destination) = 0;

	/*!
		\brief Copy the source resource into the destination resource

		\param source
				Resource where the data will be copied from
		\param destination
				Resource where the data will be copied to
		\param sourceOffset
				Offset, in bytes, where we start copying data from the source resource
		\param destinationOffset
				Offset, in bytes, where we start copying data to the source resource
		\param size
				How many bytes we are trying to copy
		\throw POGLResourceException
				If the source or destination buffer is invalid. If the sourceOffset + size is larger than the source resources memory. If
				the destinationOffset + size is larger than the destination resource memory.
	*/
	virtual void CopyResource(IPOGLResource* source, IPOGLResource* destination, POGL_UINT32 sourceOffset, POGL_UINT32 destinationOffset, POGL_UINT32 size) = 0;

	/*!
		\brief Apply the supplied program to the current context

		\param effect
		\return
	*/
	virtual IPOGLRenderState* Apply(IPOGLProgram* program) = 0;

	/*!
		\brief Map the data to a memory location and return a pointer to it
	*/
	virtual void* Map(IPOGLResource* resource, POGLResourceMapType::Enum e) = 0;

	/*!
		\brief Map parts of the data to a memory location and return a pointer to it

		\param offset
				The offset, in bytes, where we want to start saving the data
		\param length
				The length of the data buffer we want to map
	*/
	virtual void* Map(IPOGLResource* resource, POGL_UINT32 offset, POGL_UINT32 length, POGLResourceMapType::Enum e) = 0;

	/*!
		\brief Close the stream

		This must be done after all the operations have been completed. This unmaps potentially mapped resources and
		update your objects synchronization status so that any future rending operations will get the correct buffer data.
	*/
	virtual void Unmap(IPOGLResource* resource) = 0;

	/*!
		\brief A rectangle specifying the viewport area

		\param viewport
	*/
	virtual void SetViewport(const POGL_RECT& viewport) = 0;
};

/*!

*/
class POGLAPI IPOGLDeferredRenderContext : public IPOGLRenderContext
{
public:
	/*!
		\brief Execute the commands generated by this context and clears the command queue

		\param context
				The context we want to execute the commands in
	*/
	virtual void ExecuteCommands(IPOGLRenderContext* context) = 0;

	/*!
		\brief Execute the commands generated by this device context

		\param context
				The context we want to execute the commands in
		\param clearCommands
				Boolean value indicating if we should clear the command queue or not. You can invoke this method multiple times and get the
				same result each time if you do not clear the command queue.
	*/
	virtual void ExecuteCommands(IPOGLRenderContext* context, bool clearCommands) = 0;

	/*!
		\brief Flush this command queue

		Use this method when all the calls are complete for this context.
	*/
	virtual void Flush() = 0;
};

/*!
	\brief The sampler state
*/
class POGLAPI IPOGLSamplerState
{
public:
	virtual ~IPOGLSamplerState() {}

	/*!
		\brief Set the min filter

		\param minFilter
	*/
	virtual void SetMinFilter(POGLMinFilter::Enum minFilter) = 0;

	/*!
		\brief Set the Mag Filter

		\param minFilter
	*/
	virtual void SetMagFilter(POGLMagFilter::Enum magFilter) = 0;

	/*!
		\brief Set the texture warpping
	*/
	virtual void SetTextureWrap(POGLTextureWrap::Enum s, POGLTextureWrap::Enum t) = 0;

	/*!
		\brief Set the texture warpping
	*/
	virtual void SetTextureWrap(POGLTextureWrap::Enum s, POGLTextureWrap::Enum t, POGLTextureWrap::Enum r) = 0;

	/*!
		\brief Set the compare function
	*/
	virtual void SetCompareFunc(POGLCompareFunc::Enum compareFunc) = 0;

	/*!
		\brief Set the compare mode
	*/
	virtual void SetCompareMode(POGLCompareMode::Enum compareMode) = 0;
};

/*!
	\brief
*/
class POGLAPI IPOGLUniform
{
public:
	virtual ~IPOGLUniform() {}

	virtual void SetInt32(POGL_INT32 a) = 0;
	virtual void SetInt32(POGL_INT32 a, POGL_INT32 b) = 0;
	virtual void SetInt32(POGL_INT32 a, POGL_INT32 b, POGL_INT32 c) = 0;
	virtual void SetInt32(POGL_INT32 a, POGL_INT32 b, POGL_INT32 c, POGL_INT32 d) = 0;
	virtual void SetInt32(POGL_INT32* ptr, POGL_UINT32 count) = 0;

	virtual void SetUInt32(POGL_UINT32 a) = 0;
	virtual void SetUInt32(POGL_UINT32 a, POGL_UINT32 b) = 0;
	virtual void SetUInt32(POGL_UINT32 a, POGL_UINT32 b, POGL_UINT32 c) = 0;
	virtual void SetUInt32(POGL_UINT32 a, POGL_UINT32 b, POGL_UINT32 c, POGL_UINT32 d) = 0;
	virtual void SetUInt32(POGL_UINT32* ptr, POGL_UINT32 count) = 0;

	virtual void SetFloat(POGL_FLOAT a) = 0;
	virtual void SetFloat(POGL_FLOAT a, POGL_FLOAT b) = 0;
	virtual void SetFloat(POGL_FLOAT a, POGL_FLOAT b, POGL_FLOAT c) = 0;
	virtual void SetFloat(POGL_FLOAT a, POGL_FLOAT b, POGL_FLOAT c, POGL_FLOAT d) = 0;
	virtual void SetFloat(POGL_FLOAT* ptr, POGL_UINT32 count) = 0;

	virtual void SetDouble(POGL_DOUBLE a) = 0;
	virtual void SetDouble(POGL_DOUBLE a, POGL_DOUBLE b) = 0;
	virtual void SetDouble(POGL_DOUBLE a, POGL_DOUBLE b, POGL_DOUBLE c) = 0;
	virtual void SetDouble(POGL_DOUBLE a, POGL_DOUBLE b, POGL_DOUBLE c, POGL_DOUBLE d) = 0;
	virtual void SetDouble(POGL_DOUBLE* ptr, POGL_UINT32 count) = 0;

	virtual void SetMatrix(const POGL_MAT4& mat4) = 0;

	virtual void SetVector2(const POGL_VECTOR2& vec) = 0;
	virtual void SetVector3(const POGL_VECTOR3& vec) = 0;
	virtual void SetVector4(const POGL_VECTOR4& vec) = 0;

	virtual void SetSize(const POGL_SIZE& size) = 0;
	virtual void SetRect(const POGL_RECT& rect) = 0;

	virtual IPOGLSamplerState* GetSamplerState() = 0;

	virtual void SetTexture(IPOGLTexture* texture) = 0;
};

/*!
	\brief Interface representing a shader resource

	A shader resource is a part of a GPU program. By linking one or many shaders using the
	IPOGLRenderContext you get a GPU program
*/
class POGLAPI IPOGLShader : public IPOGLResource
{
public:
};

/*!
	\brief Interface representing a GPU program resource

	A GPU program is required if you want to draw geometry onto the screen. You link one or many shader resources
	using the IPOGLRenderContext to get a GPU program
*/
class POGLAPI IPOGLProgram : public IPOGLResource
{
public:
	/*!
		\brief Locate the uniform with the given name
	*/
	virtual IPOGLUniform* FindUniformByName(const POGL_CHAR* name) = 0;

	/*!
		\brief Retrieves if this program enables or disables depth testing
	*/
	virtual bool GetDepthTest() = 0;

	/*!
		\brief Should the effect have depth testing enabled.

		This effect needs to be re-applied for the change to come into effect. If you
		want the change to be applied directly then call IPOGLRenderState::SetDepthTest(bool) instead.

		\param b
			TRUE or FALSE
	*/
	virtual void SetDepthTest(bool b) = 0;

	/*!
		\brief

		This effect needs to be re-applied for the change to come into effect. If you
		want the change to be applied directly then call IPOGLRenderState::SetDepthTest(bool) instead.

		\param depthFunc
	*/
	virtual void SetDepthFunc(POGLDepthFunc::Enum depthFunc) = 0;

	/*!
		\brief
	*/
	virtual POGLDepthFunc::Enum GetDepthFunc() = 0;

	/*!
		\brief
	*/
	virtual bool GetDepthMask() = 0;

	/*!
		\brief

		This effect needs to be re-applied for the change to come into effect. If you
		want the change to be applied directly then call IPOGLRenderState::SetDepthMask(bool) instead.

		\param b
			TRUE or FALSE
	*/
	virtual void SetDepthMask(bool b) = 0;

	/*!
		\brief
	*/
	virtual POGL_UINT8 GetColorMask() = 0;

	/*!
		\brief
	*/
	virtual void SetColorMask(POGL_UINT8 colorMask) = 0;

	/*!
		\brief
	*/
	virtual bool GetStencilTest() = 0;

	/*!
		\brief

		This effect needs to be re-applied for the change to come into effect. If you
		want the change to be applied directly then call IPOGLRenderState::SetStencilTest(bool) instead.

		\param b
			TRUE or FALSE
	*/
	virtual void SetStencilTest(bool b) = 0;

	/*!
		\brief
	*/
	virtual POGL_UINT32 GetStencilMask() = 0;

	/*!
		\brief

		\param mask
	*/
	virtual void SetStencilMask(POGL_UINT32 mask) = 0;

	/*!
		\brief Set the blending factor

		\param sfactor
		\param dfactor
	*/
	virtual void SetBlendFunc(POGLSrcFactor::Enum sfactor, POGLDstFactor::Enum dfactor) = 0;

	/*!
		\brief
	*/
	virtual void SetBlend(bool b) = 0;

	/*!
		\brief

		\param e
	*/
	virtual void SetFrontFace(POGLFrontFace::Enum e) = 0;

	/*!
		\brief
	*/
	virtual POGLFrontFace::Enum GetFrontFace() = 0;

	/*!
		\brief

		\param e
	*/
	virtual void SetCullFace(POGLCullFace::Enum e) = 0;

	/*!
		\brief
	*/
	virtual POGLCullFace::Enum GetCullFace() = 0;
};

/*!
	\brief
*/
class POGLAPI IPOGLRenderState : public IPOGLInterface
{
public:
	/*!
		\brief Clears the current render states back buffers

		\param clearBits
	*/
	virtual void Clear(POGL_UINT32 clearBits) = 0;

	/*!
		\brief Locate the uniform with the given name
	*/
	virtual IPOGLUniform* FindUniformByName(const POGL_CHAR* name) = 0;

	/*!
		\brief Set the framebuffer used when render to this frame
	*/
	virtual void SetFramebuffer(IPOGLFramebuffer* framebuffer) = 0;

	/*!
		\brief Set the active vertex buffer

		\param vertexBuffer
	*/
	virtual void SetVertexBuffer(IPOGLVertexBuffer* vertexBuffer) = 0;

	/*!
		\brief Set the active index buffer

		\param indexBuffer
	*/
	virtual void SetIndexBuffer(IPOGLIndexBuffer* indexBuffer) = 0;
	
	/*!
		\brief Draw the active vertex buffer
	*/
	virtual void Draw() = 0;

	/*!
		\brief Draw the active vertex buffer

		\param count
				How many vertices we want to draw
	*/
	virtual void Draw(POGL_UINT32 count) = 0;

	/*!
		\brief Draw the active vertex buffer

		\param count
				How many vertices we want to draw
		\param offset
				Where the first vertex is located
	*/
	virtual void Draw(POGL_UINT32 count, POGL_UINT32 offset) = 0;

	/*!
		\brief Draw the active vertex- and index buffer
	*/
	virtual void DrawIndexed() = 0;
	
	/*!
		\brief Draw the active vertex- and index buffer

		\param count
				How many vertices we want to draw
	*/
	virtual void DrawIndexed(POGL_UINT32 count) = 0;

	/*!
		\brief Draw the active vertex- and index buffer

		\param count
				How many vertices we want to draw
		\param offset
				Where the first vertex is located
	*/
	virtual void DrawIndexed(POGL_UINT32 count, POGL_UINT32 offset) = 0;

	/*!
		\brief
	*/
	virtual void SetDepthTest(bool b) = 0;

	/*!
		\brief
	*/
	virtual void SetDepthFunc(POGLDepthFunc::Enum depthFunc) = 0;

	/*!
		\brief
	*/
	virtual void SetDepthMask(bool b) = 0;

	/*!
		\brief
	*/
	virtual void SetColorMask(POGL_UINT8 colorMask) = 0;

	/*!
		\brief

		\param b
			TRUE or FALSE
	*/
	virtual void SetStencilTest(bool b) = 0;

	/*!
		\brief

		\param mask
	*/
	virtual void SetStencilMask(POGL_UINT32 mask) = 0;

	/*!
		\brief
	*/
	virtual void SetBlendFunc(POGLSrcFactor::Enum sfactor, POGLDstFactor::Enum dfactor) = 0;

	/*!
		\brief
	*/
	virtual void SetBlend(bool b) = 0;

	/*!
		\brief

		\param e
	*/
	virtual void SetFrontFace(POGLFrontFace::Enum e) = 0;

	/*!
		\brief

		\param e
	*/
	virtual void SetCullFace(POGLCullFace::Enum e) = 0;

	/*!
		\brief A rectangle specifying the viewport area

		\param viewport
	*/
	virtual void SetViewport(const POGL_RECT& viewport) = 0;
};

/*!
	\brief The default texture interface
*/
class POGLAPI IPOGLTexture : public IPOGLResource
{
public:
	/*!
		\brief Retrieves the texture color format for this texture
	*/
	virtual POGLTextureFormat::Enum GetTextureFormat() const = 0;
};

/*!
	\brief
*/
class POGLAPI IPOGLTexture1D : public IPOGLTexture
{
public:
	/*!
		\brief Retrieves the size (width) of this texture in pixels
	*/
	virtual POGL_UINT32 GetSize() const = 0;
};

/*!
	\brief
*/
class POGLAPI IPOGLTexture2D : public IPOGLTexture
{
public:
	/*!
		\brief Retrieves the size (width and height) of this texture in pixels
	*/
	virtual const POGL_SIZE& GetSize() const = 0;
};

/*!
	\brief
*/
class POGLAPI IPOGLTexture3D : public IPOGLTexture
{
public:
	virtual POGL_UINT32 GetDepth() const = 0;

	/*!
		\brief Retrieves the size (width and height) of this texture in pixels
	*/
	virtual const POGL_SIZE& GetSize() const = 0;
};

/*!
	\brief
*/
class POGLAPI IPOGLFramebuffer : public IPOGLInterface
{
public:
	/*!
		\brief Retrieve the texture bound at the supplied index

		\param idx
		\return
	*/
	virtual IPOGLTexture* GetTexture(POGL_UINT32 idx) = 0;

	/*!
		\brief Retrieves a depth- and/or stencil texture

		\return
	*/
	virtual IPOGLTexture* GetDepthStencilTexture() = 0;
};

/*!
	\brief
*/
class POGLAPI IPOGLVertexBuffer : public IPOGLResource
{
public:
	/*!
		\brief Retrieves the layout for this buffer

		\return
	*/
	virtual const POGL_VERTEX_LAYOUT* GetLayout() const = 0;

	/*!
		\brief Retrieves the number of vertices located in this buffer
	*/
	virtual POGL_UINT32 GetCount() const = 0;

	/*!
		\brief Retrieves the memory size of this vertex buffer
	*/
	virtual POGL_UINT32 GetMemorySize() const = 0;
};

/*!
	\brief
*/
class POGLAPI IPOGLIndexBuffer : public IPOGLResource
{
public:
	/*!
		\brief Retrieves the number of elements found in this buffer
	*/
	virtual POGL_UINT32 GetCount() const = 0;
};

/*!
	\brief Creates a new device instance.

	\param deviceInfo
	\return
*/
extern POGLAPI IPOGLDevice* POGLCreateDevice(const POGL_DEVICE_INFO* info);

//
// Exceptions
//

class POGLAPI POGLException {
public:
	POGLException(const POGL_CHAR* function, const POGL_UINT64 line, const POGL_CHAR* file);
	POGLException(const POGL_CHAR* function, const POGL_UINT64 line, const POGL_CHAR* file, const POGL_CHAR* message, ...);
	virtual ~POGLException();

	const POGL_CHAR* GetMessage() const;
	const POGL_CHAR* GetFunction() const;
	const POGL_CHAR* GetFile() const;
	POGL_UINT64 GetLine() const;

protected:
	POGL_CHAR mFunction[128];
	POGL_CHAR mFile[256];
	POGL_CHAR mMessage[5096];
	POGL_UINT64 mLine;
};

/*!
	\brief Exception thrown if you are trying to use a feature that's not implemented yet
*/
class POGLAPI POGLNotImplementedException : public POGLException {
public:
	POGLNotImplementedException(const POGL_CHAR* function, const POGL_UINT64 line, const POGL_CHAR* file, const POGL_CHAR* message, ...);
	virtual ~POGLNotImplementedException();
};

/*!
	\brief Exception thrown if a resource generation has failed
*/
class POGLAPI POGLInitializationException : public POGLException {
public:
	POGLInitializationException(const POGL_CHAR* function, const POGL_UINT64 line, const POGL_CHAR* file, const POGL_CHAR* message, ...);
	virtual ~POGLInitializationException();
};

/*!
	\brief Exception thrown if a resource generation has failed
*/
class POGLAPI POGLResourceException : public POGLException {
public:
	POGLResourceException(const POGL_CHAR* function, const POGL_UINT64 line, const POGL_CHAR* file, const POGL_CHAR* message, ...);
	virtual ~POGLResourceException();
};

/*!
	\brief Exception thrown if a fatal fault happened when initializing or using a program
*/
class POGLAPI POGLProgramException : public POGLException {
public:
	POGLProgramException(const POGL_CHAR* function, const POGL_UINT64 line, const POGL_CHAR* file, const POGL_CHAR* message, ...);
	virtual ~POGLProgramException();
};

/*!
	\brief Exception thrown if a fatal fault happened when using a state in a way that's not solvable by the rendering engine
*/
class POGLAPI POGLStateException : public POGLException {
public:
	POGLStateException(const POGL_CHAR* function, const POGL_UINT64 line, const POGL_CHAR* file, const POGL_CHAR* message, ...);
	~POGLStateException();
};

#include <cstdarg>

#ifndef THROW_EXCEPTION
#define THROW_EXCEPTION(E, Message, ...) throw E(__FUNCTION__, __LINE__, __FILE__, POGL_TOCHAR(Message), __VA_ARGS__)
#endif

#ifndef THROW_NOT_IMPLEMENTED_EXCEPTION
#define THROW_NOT_IMPLEMENTED_EXCEPTION() throw POGLNotImplementedException(__FUNCTION__, __LINE__, __FILE__, POGL_TOCHAR("Feature not implemented yet!"))
#endif

#ifndef assert_with_message
#ifdef _DEBUG
#define assert_with_message(test, message) if(!(test)) THROW_EXCEPTION(POGLException, message)
#else
#define assert_with_message(test, message)
#endif
#endif

#ifndef assert_not_null
#ifdef _DEBUG
#define assert_not_null(param) if(param == nullptr) THROW_EXCEPTION(POGLException, "Parameter "## #param ## " cannot be nullptr")
#else
#define assert_not_null(param)
#endif
#endif

#ifndef assert_null
#ifdef _DEBUG
#define assert_null(param) if(param != nullptr) THROW_EXCEPTION(POGLException, "Parameter "## #param ## " is expected be nullptr")
#else
#define assert_null(param)
#endif
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
#define POGL_M_PI_F ((POGL_FLOAT)POGL_M_PI)
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

#ifndef POGL_ANG2RAD_F
#define POGL_ANG2RAD_F ((POGL_FLOAT)POGL_ANG2RAD)
#endif


#endif
