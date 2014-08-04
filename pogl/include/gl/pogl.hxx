#pragma once
#ifndef _PLAYGL_HXX_
#define _PLAYGL_HXX_

#include <exception>
#include <cassert>

#ifndef assert_with_message
#define assert_with_message(test, message) assert((test) && message)
#endif

#ifndef assert_not_null
#define assert_not_null(param) \
	assert(param != nullptr && "Parameter "## #param ##" cannot be nullptr")
#endif

#ifndef assert_null
#define assert_null(param) \
	assert(param == nullptr && "Parameter "## #param ##" is expected be nullptr")
#endif

#ifndef OFFSET
#define OFFSET(x) ((char *)NULL + x)
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
#define BIT_ALL ULONG_MAX
#endif

#ifndef BIT_NONE
#define BIT_NONE 0
#endif

//
// Classes
//

class IPOGLInterface;
class IPOGLResource;

class IPOGLDevice;
class IPOGLDeviceContext;
class IPOGLRenderState;

class IPOGLVertexBuffer;
class IPOGLIndexBuffer;

class IPOGLTexture;
class IPOGLTexture1D;
class IPOGLTexture2D;
class IPOGLTexture3D;

class IPOGLShaderProgram;
class IPOGLEffect;
class IPOGLUniform;

class IPOGLFrameBuffer;

//
// typedefs
//

typedef char POGL_INT8;
typedef unsigned char POGL_UINT8;
typedef short POGL_INT16;
typedef unsigned short POGL_UINT16;
typedef int POGL_INT32;
typedef unsigned int POGL_UINT32;
typedef long POGL_INT64;
typedef unsigned long POGL_UINT64;
typedef void* POGL_HANDLE;
typedef float POGL_FLOAT;
typedef double POGL_DOUBLE;
typedef size_t POGL_SIZE;

#include <string>
#ifdef UNICODE
typedef wchar_t POGL_CHAR;
typedef std::wstring POGL_STRING;
#define POGL_TOSTRING(x) L##x
#define POGL_TOCHAR(x) L##x
#else
typedef char POGL_CHAR;
typedef std::string POGL_STRING;
#define POGL_TOSTRING(x) x
#define POGL_TOCHAR(x) x
#endif

//
// Enums
//

struct POGLDeviceInfoFlags
{
	enum Enum {
		/* Enable/Disable debug mode */
		DEBUG_MODE = BIT(0)
	};
};

struct POGLTextureFormat
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

struct POGLPixelFormat
{
	enum Enum {
		R8G8B8A8 = 0,

		/* Number of enums available */
		COUNT
	};
};

struct POGLClearType
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

struct POGLShaderProgramType
{
	enum Enum {
		GEOMETRY_SHADER = 0,
		VERTEX_SHADER,
		FRAGMENT_SHADER,

		/* Number of enums available */
		COUNT
	};
};

struct POGLVertexType
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

struct POGLPrimitiveType
{
	enum Enum {
		POINT = 0,
		TRIANGLE,
		TRIANGLE_STRIP,
		LINE,
		LINE_LOOP,

		/* Number of enums available */
		COUNT
	};
};

struct POGLBufferUsage
{
	enum Enum {
		STATIC = 0,
		DYNAMIC,

		/* Number of enums available */
		COUNT
	};
};

struct POGLDepthFunc
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

struct POGLTextureWrap
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

struct POGLMinFilter
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

struct POGLMagFilter
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
struct POGLCompareFunc
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
struct POGLCompareMode
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
struct POGLPolygonMode
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

struct POGLColorMask
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

//
// Structs
//

typedef struct POGL_SIZEI
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

	POGL_SIZEI() {}
	POGL_SIZEI(const POGL_INT32 _x, const POGL_INT32 _y) { x = _x; y = _y; }
	POGL_SIZEI(const POGL_SIZEI& rhs) { x = rhs.x; y = rhs.y; }
	~POGL_SIZEI() {}
	inline POGL_SIZEI& operator=(const POGL_SIZEI& rhs) { x = rhs.x; y = rhs.y; return *this; }
} POGL_POINTI, POGL_VECTOR2I;

struct POGL_VECTOR2F
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
	POGL_VECTOR2F() {}
	POGL_VECTOR2F(const POGL_FLOAT _x, const POGL_FLOAT _y) { x = _x; y = _y; }
	POGL_VECTOR2F(const POGL_VECTOR2F& rhs) { x = rhs.x; y = rhs.y; }
	inline POGL_VECTOR2F& operator=(const POGL_VECTOR2F& rhs) { x = rhs.x; y = rhs.y; return *this; }
};

struct POGL_VECTOR2D
{
	union {
		struct {
			POGL_DOUBLE x;
			POGL_DOUBLE y;
		};
		struct {
			POGL_DOUBLE s;
			POGL_DOUBLE t;
		};
		POGL_DOUBLE vec[2];
	};

	POGL_VECTOR2D() {}
	POGL_VECTOR2D(const POGL_DOUBLE _x, const POGL_DOUBLE _y) { x = _x; y = _y; }
	POGL_VECTOR2D(const POGL_VECTOR2D& rhs) { x = rhs.x; y = rhs.y; }
	inline POGL_VECTOR2D& operator=(const POGL_VECTOR2D& rhs) { x = rhs.x; y = rhs.y; return *this; }
};

typedef struct POGL_VECTOR3F
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
		struct {
			POGL_FLOAT s;
			POGL_FLOAT t;
			POGL_FLOAT r;
		};
		POGL_FLOAT vec[3];
	};

	POGL_VECTOR3F() {}
	POGL_VECTOR3F(const POGL_FLOAT _x, const POGL_FLOAT _y, const POGL_FLOAT _z) { x = _x; y = _y; z = _z; }
	POGL_VECTOR3F(const POGL_VECTOR3F& rhs) { x = rhs.x; y = rhs.y; z = rhs.z; }
	inline POGL_VECTOR3F& operator=(const POGL_VECTOR3F& rhs) { x = rhs.x; y = rhs.y; z = rhs.z; return *this; }
} POGL_COLOR3F;

typedef struct POGL_VECTOR3D
{
	union {
		struct {
			POGL_DOUBLE x;
			POGL_DOUBLE y;
			POGL_DOUBLE z;
		};
		struct {
			POGL_DOUBLE r;
			POGL_DOUBLE g;
			POGL_DOUBLE b;
		};
		struct {
			POGL_DOUBLE s;
			POGL_DOUBLE t;
			POGL_DOUBLE r;
		};
		POGL_DOUBLE vec[3];
	};

	POGL_VECTOR3D() {}
	POGL_VECTOR3D(const POGL_DOUBLE _x, const POGL_DOUBLE _y, const POGL_DOUBLE _z) { x = _x; y = _y; z = _z; }
	POGL_VECTOR3D(const POGL_VECTOR3D& rhs) { x = rhs.x; y = rhs.y; z = rhs.z; }
	inline POGL_VECTOR3D& operator=(const POGL_VECTOR3D& rhs) { x = rhs.x; y = rhs.y; z = rhs.z; return *this; }
} POGL_COLOR3D;

typedef struct POGL_VECTOR4F
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
		struct {
			POGL_FLOAT s;
			POGL_FLOAT t;
			POGL_FLOAT r;
			POGL_FLOAT q;
		};
		POGL_FLOAT vec[4];
	};

	POGL_VECTOR4F() {}
	POGL_VECTOR4F(const POGL_FLOAT _x, const POGL_FLOAT _y, const POGL_FLOAT _z, const POGL_FLOAT _w) { x = _x; y = _y; z = _z; w = _w; }
	POGL_VECTOR4F(const POGL_VECTOR4F& rhs) { x = rhs.x; y = rhs.y; z = rhs.z; w = rhs.w; }
	inline POGL_VECTOR4F& operator=(const POGL_VECTOR4F& rhs) { x = rhs.x; y = rhs.y; z = rhs.z; w = rhs.w; return *this; }
} POGL_COLOR4F;

typedef struct POGL_VECTOR4D
{
	union {
		struct {
			POGL_DOUBLE x;
			POGL_DOUBLE y;
			POGL_DOUBLE z;
			POGL_DOUBLE w;
		};
		struct {
			POGL_DOUBLE r;
			POGL_DOUBLE g;
			POGL_DOUBLE b;
			POGL_DOUBLE a;
		};
		struct {
			POGL_DOUBLE s;
			POGL_DOUBLE t;
			POGL_DOUBLE r;
			POGL_DOUBLE q;
		};
		POGL_DOUBLE vec[4];
	};

	POGL_VECTOR4D() {}
	POGL_VECTOR4D(const POGL_DOUBLE _x, const POGL_DOUBLE _y, const POGL_DOUBLE _z, const POGL_DOUBLE _w) { x = _x; y = _y; z = _z; w = _w; }
	POGL_VECTOR4D(const POGL_VECTOR4D& rhs) { x = rhs.x; y = rhs.y; z = rhs.z; w = rhs.w; }
	inline POGL_VECTOR4D& operator=(const POGL_VECTOR4D& rhs) { x = rhs.x; y = rhs.y; z = rhs.z; w = rhs.w; return *this; }
} POGL_COLOR4D;

struct POGL_MAT4D
{
	union {
		struct {
			POGL_DOUBLE 
			_11, _21, _31, _41,
			_12, _22, _32, _42,
			_13, _23, _33, _43,
			_14, _24, _34, _44;
		};
		struct {
			POGL_DOUBLE m[4][4];
		};
		POGL_DOUBLE vec[16];
	};
};

struct POGL_MAT4F
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
};

/*!
	\brief
*/
struct POGL_DEVICE_INFO
{
	/* The window the IPOGLDeviceContext should be associated with. If nullptr then the context will "draw" to the console instead */
	POGL_HANDLE window;

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
struct POGL_VERTEX_LAYOUT_FIELD
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
struct POGL_VERTEX_LAYOUT
{
	/* The vertex fields */
	POGL_VERTEX_LAYOUT_FIELD fields[MAX_VERTEX_LAYOUT_FIELD_SIZE];

	/* The size of each vertex. This value will also be used as an stride between each vertex (This is most likely sizeof(Type)). */
	POGL_UINT32 vertexSize;
};


/*!
	\brief Vertex containing a position
*/
struct POGL_POSITION_VERTEX
{
	POGL_VECTOR3F position;

	POGL_POSITION_VERTEX() {}
	POGL_POSITION_VERTEX(const POGL_VECTOR3F& p) { position = p; }
	POGL_POSITION_VERTEX(const POGL_POSITION_VERTEX& v) { position = v.position; }
	~POGL_POSITION_VERTEX() {}
	inline POGL_POSITION_VERTEX& operator=(const POGL_POSITION_VERTEX& rhs) { position = rhs.position; return *this; }
};

/* Vertex layout for the POGL_POSITION_VERTEX struct. */
static const POGL_VERTEX_LAYOUT POGL_POSITION_VERTEX_LAYOUT = {
	{
		{ sizeof(POGL_VECTOR3F), POGLVertexType::FLOAT, false },
		0
	},
	sizeof(POGL_POSITION_VERTEX)
};

/*!
	\brief Vertex containing a position and color
*/
struct POGL_POSITION_COLOR_VERTEX
{
	POGL_VECTOR3F position;
	POGL_COLOR4F color;

	POGL_POSITION_COLOR_VERTEX() {}
	POGL_POSITION_COLOR_VERTEX(const POGL_VECTOR3F& p, const POGL_COLOR4F& c) { position = p; color = c; }
	POGL_POSITION_COLOR_VERTEX(const POGL_POSITION_COLOR_VERTEX& v) { position = v.position; color = v.color; }
	~POGL_POSITION_COLOR_VERTEX() {}
	inline POGL_POSITION_COLOR_VERTEX& operator=(const POGL_POSITION_COLOR_VERTEX& rhs) { position = rhs.position; color = rhs.color;  return *this; }
};

/* Vertex layout for the POGL_POSITION_COLOR_VERTEX struct. */
static const POGL_VERTEX_LAYOUT POGL_POSITION_COLOR_VERTEX_LAYOUT = {
	{ 
		{ sizeof(POGL_VECTOR3F), POGLVertexType::FLOAT, false },
		{ sizeof(POGL_COLOR4F), POGLVertexType::FLOAT, false },
		0
	},
	sizeof(POGL_POSITION_COLOR_VERTEX)
};

//
// Class Definitions
//

class IPOGLInterface
{
public:
	virtual ~IPOGLInterface() {}

	virtual void AddRef() = 0;

	virtual void Release() = 0;
};

/*!
	\brief
*/
class IPOGLResource : public IPOGLInterface
{
public:
	/*!
		\brief Retrieves the device 
	*/
	virtual IPOGLDevice* GetDevice() = 0;

	/*!
		\brief Retrieves a handle for this resource
	*/
	virtual POGL_HANDLE GetHandlePtr() = 0;
};

/*!
	\brief 
*/
class IPOGLDevice : public IPOGLInterface
{
public:
	/*!
		\brief Retrieves a device context for the current thread

		If no device context is bound to the current thread then one is created and initializded for you

		\return A device context
	*/
	virtual IPOGLDeviceContext* GetDeviceContext() = 0;
	
	/*!
		\brief Swap buffers
	*/
	virtual void SwapBuffers() = 0;

	/*!
		\brief Retrieves the device info
	*/
	virtual const POGL_DEVICE_INFO* GetDeviceInfo() = 0;
};

/*!
	\brief 
*/
class IPOGLDeviceContext : public IPOGLInterface
{
public:
	/*!
		\brief Retrieves the device
	*/
	virtual IPOGLDevice* GetDevice() = 0;
	
	/*!
		\brief Creates a shader program based on the supplied file

		\param path
		\param type
		\return
	*/
	virtual IPOGLShaderProgram* CreateShaderProgramFromFile(const POGL_CHAR* path, POGLShaderProgramType::Enum type) = 0;

	/*!
		\brief Creates a shader program based on the memory buffer

		\param memory
		\param size
		\param type
		\return
	*/
	virtual IPOGLShaderProgram* CreateShaderProgramFromMemory(const POGL_CHAR* memory, POGL_UINT32 size, POGLShaderProgramType::Enum type) = 0;

	/*!
		\brief Creates an effect based on the supplied programs

		\param programs
		\param numPrograms
		\return
	*/
	virtual IPOGLEffect* CreateEffectFromPrograms(IPOGLShaderProgram** programs, POGL_UINT32 numPrograms) = 0;

	/*!
		\brief Creates a 3D texture
	*/
	virtual IPOGLTexture1D* CreateTexture1D() = 0;

	/*!
		\brief Creates a 2D texture
	*/
	virtual IPOGLTexture2D* CreateTexture2D() = 0;

	/*!
		\brief Creates a 3D texture
	*/
	virtual IPOGLTexture3D* CreateTexture3D() = 0;
	
	/*!
		\brief Creates a vertex buffer based on the supplied parameters

		\param memory
		\param memorySize
		\param layout
		\param primitiveType
		\param bufferUsage
		\return
	*/
	virtual IPOGLVertexBuffer* CreateVertexBuffer(const void* memory, POGL_SIZE memorySize, const POGL_VERTEX_LAYOUT* layout, POGLPrimitiveType::Enum primitiveType, POGLBufferUsage::Enum bufferUsage) = 0;
	
	/*!
		\brief Creates a vertex buffer based on the supplied parameters

		\param memory
		\param memorySize
		\param primitiveType
		\param bufferUsage
		\return
	*/
	virtual IPOGLVertexBuffer* CreateVertexBuffer(const POGL_POSITION_VERTEX* memory, POGL_SIZE memorySize, POGLPrimitiveType::Enum primitiveType, POGLBufferUsage::Enum bufferUsage) = 0;
	
	/*!
		\brief Creates a vertex buffer based on the supplied parameters

		\param memory
		\param memorySize
		\param primitiveType
		\param bufferUsage
		\return
	*/
	virtual IPOGLVertexBuffer* CreateVertexBuffer(const POGL_POSITION_COLOR_VERTEX* memory, POGL_SIZE memorySize, POGLPrimitiveType::Enum primitiveType, POGLBufferUsage::Enum bufferUsage) = 0;

	/*!
		\brief Creates an index buffer based on the supplied parameters

		\param memory
		\param memorySize
		\param type
		\param bufferUsage
	*/
	virtual IPOGLIndexBuffer* CreateIndexBuffer(const void* memory, POGL_SIZE memorySize, POGLVertexType::Enum type, POGLBufferUsage::Enum bufferUsage) = 0;

	/*!
		\brief Apply the supplied effect to the current context

		\param effect
		\return
	*/
	virtual IPOGLRenderState* Apply(IPOGLEffect* effect) = 0;
};

/*!
	\brief The sampler state
*/
class IPOGLSamplerState
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
class IPOGLUniform
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

	virtual void SetMatrix(const POGL_MAT4F& mat4) = 0;
	virtual void SetMatrix(const POGL_MAT4D& mat4) = 0;

	virtual void SetVector2I(const POGL_VECTOR2I& vec) = 0;
	virtual void SetVector2F(const POGL_VECTOR2F& vec) = 0;
	virtual void SetVector2D(const POGL_VECTOR2D& vec) = 0;

	virtual void SetVector3F(const POGL_VECTOR3F& vec) = 0;
	virtual void SetVector3D(const POGL_VECTOR3D& vec) = 0;

	virtual void SetVector4F(const POGL_VECTOR4F& vec) = 0;
	virtual void SetVector4D(const POGL_VECTOR4D& vec) = 0;

	virtual IPOGLSamplerState* GetSamplerState() = 0;

	virtual void SetTexture(IPOGLTexture* texture) = 0;
};

/*!
	\brief
*/
class IPOGLShaderProgram : public IPOGLResource
{
public:
};

/*!
	\brief
*/
class IPOGLEffect : public IPOGLResource
{
public:
	/*!
		\brief
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
};

/*!
	\brief
*/
class IPOGLRenderState : public IPOGLInterface
{
public:
	/*!
		\brief Retrieves the device 
	*/
	virtual IPOGLDevice* GetDevice() = 0;
	
	/*!
		\brief Retrieves the device context
	*/
	virtual IPOGLDeviceContext* GetDeviceContext() = 0;

	/*!
		\brief Clears the current render states back buffers

		\param clearBits
	*/
	virtual void Clear(POGL_UINT32 clearBits) = 0;

	/*!
		\brief 
	*/
	virtual IPOGLUniform* FindUniformByName(const POGL_CHAR* name) = 0;

	/*!
		\brief Draw the entire vertex buffer

		\param vertexBuffer
	*/
	virtual void Draw(IPOGLVertexBuffer* vertexBuffer) = 0;
	
	/*!
		\brief Draw the entire vertex buffer using the supplied index buffer

		\param vertexBuffer
		\param indexBuffer
	*/
	virtual void Draw(IPOGLVertexBuffer* vertexBuffer, IPOGLIndexBuffer* indexBuffer) = 0;
	
	/*!
		\brief Draw the entire vertex buffer using the supplied index buffer

		\param vertexBuffer
		\param indexBuffer
		\param startIndex
	*/
	virtual void Draw(IPOGLVertexBuffer* vertexBuffer, IPOGLIndexBuffer* indexBuffer, POGL_UINT32 startIndex) = 0;
	
	/*!
		\brief Draw the entire vertex buffer using the supplied index buffer

		\param vertexBuffer
		\param indexBuffer
		\param startIndex
		\param count
	*/
	virtual void Draw(IPOGLVertexBuffer* vertexBuffer, IPOGLIndexBuffer* indexBuffer, POGL_UINT32 startIndex, POGL_UINT32 count) = 0;

	/*!
		\brief Ends the current frame
	*/
	virtual void EndFrame() = 0;
	
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
};

/*!
	\brief The default texture interface
*/
class IPOGLTexture : public IPOGLResource
{
public:
	/*!
		\brief Retrieves the texture color format for this texture
	*/
	virtual POGLTextureFormat GetTextureFormat() const = 0;
};

/*!
	\brief
*/
class IPOGLTexture1D : public IPOGLTexture
{
public:
	virtual POGL_UINT32 GetSize() const = 0;
};

/*!
	\brief
*/
class IPOGLTexture2D : public IPOGLTexture
{
public:
	virtual POGL_UINT32 GetWidth() const = 0;
	virtual POGL_UINT32 GetHeight() const = 0;
	virtual const POGL_SIZEI& GetSize() const = 0;
};

/*!
	\brief
*/
class IPOGLTexture3D : public IPOGLTexture
{
public:
	virtual POGL_UINT32 GetWidth() const = 0;
	virtual POGL_UINT32 GetHeight() const = 0;
	virtual POGL_UINT32 GetDepth() const = 0;
	virtual const POGL_SIZEI& GetSize() const = 0;
};

/*!
	\brief
*/
class IPOGLVertexBuffer : public IPOGLResource
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
	virtual POGL_UINT32 GetNumVertices() const = 0;
};

/*!
	\brief
*/
class IPOGLIndexBuffer : public IPOGLResource
{
public:
	/*!
		\brief Retrieves the number of elements found in this buffer
	*/
	virtual POGL_UINT32 GetNumElements() const = 0;
};

/*!
	\brief Creates a new device instance.

	\param deviceInfo
	\return
*/
extern IPOGLDevice* POGLCreateDevice(const POGL_DEVICE_INFO* info);

//
// Exceptions
//

class POGLException {
public:
	POGLException(const POGL_CHAR* function, const POGL_UINT64 line, const POGL_CHAR* file);
	POGLException(const POGL_CHAR* function, const POGL_UINT64 line, const POGL_CHAR* file, const POGL_CHAR* message, ...);
	virtual ~POGLException();

	inline const POGL_CHAR* GetMessage() const { return mMessage; }
	inline const POGL_CHAR* GetFunction() const { return mFunction; }
	inline const POGL_CHAR* GetFile() const { return mFile; }
	inline POGL_UINT64 GetLine() const { return mLine; }

protected:
	POGL_CHAR mFunction[128];
	POGL_CHAR mFile[128];
	POGL_CHAR mMessage[256];
	POGL_UINT64 mLine;
};

/*!
	\brief Exception thrown if a resource generation has failed
*/
class POGLResourceException : public POGLException {
public:
	POGLResourceException(const POGL_CHAR* function, const POGL_UINT64 line, const POGL_CHAR* file, const POGL_CHAR* message, ...);
	~POGLResourceException();
};

/*!
	\brief Exception thrown if a fatal fault happened when initializing or using an effect
*/
class POGLEffectException : public POGLException {
public:
	POGLEffectException(const POGL_CHAR* function, const POGL_UINT64 line, const POGL_CHAR* file, const POGL_CHAR* message, ...);
	~POGLEffectException();
};

/*!
	\brief Exception thrown if a fatal fault happened when using a state in a way that's not solvable by the rendering engine
*/
class POGLStateException : public POGLException {
public:
	POGLStateException(const POGL_CHAR* function, const POGL_UINT64 line, const POGL_CHAR* file, const POGL_CHAR* message, ...);
	~POGLStateException();
};

#include <cstdarg>

#ifndef THROW_EXCEPTION
#define THROW_EXCEPTION(E, Message, ...) throw E(__FUNCTION__, __LINE__, __FILE__, Message, __VA_ARGS__)
#endif

#endif
