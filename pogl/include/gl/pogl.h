#pragma once
#ifndef _POGL_H_
#define _POGL_H_

#include <exception>
#include <condition_variable>

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

class IPOGLDeferredDeviceContext;

class IPOGLVertexBuffer;
class IPOGLIndexBuffer;

class IPOGLTexture;
class IPOGLTexture1D;
class IPOGLTexture2D;
class IPOGLTexture3D;
class IPOGLFramebuffer;

class IPOGLShaderProgram;
class IPOGLEffect;
class IPOGLUniform;

//
// typedefs
//
#include <cinttypes>

typedef char POGL_INT8;
typedef unsigned char POGL_UINT8;
typedef short POGL_INT16;
typedef unsigned short POGL_UINT16;
typedef int POGL_INT32;
typedef unsigned int POGL_UINT32;
typedef int64_t POGL_INT64;
typedef uint64_t POGL_UINT64;
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
#define POGL_TOSTRING(x) POGL_STRING(POGL_TOCHAR(x))
#define POGL_TOCHAR(x) x
#endif

//
// Enums
//

struct POGLDeviceInfoFlags
{
	enum Enum {
		//
		// Enable debug mode if this flag is set. Debug mode is different depending on which graphics card you have. 
		// AMD, for example, enables us to attach CodeXL (a remote debugger) and change stuff in runtime
		//
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
		TRIANGLE_FAN,
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

struct POGLSrcFactor
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

struct POGLDstFactor
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

struct POGLResourceStreamType
{
	enum Enum {
		/* Open a reading stream */
		//READ = 0,

		/* Open a writing stream */
		WRITE = 0,

		/* Open the stream for reading and writing */
		//READ_AND_WRITE,
		COUNT
	};
};

/*!
	\brief Resource type enum
*/
struct POGLResourceType
{
	enum Enum {
		VERTEXBUFFER = 0,
		INDEXBUFFER,
		TEXTURE1D,
		TEXTURE2D,
		TEXTURE3D,
		SHADER,
		EFFECT,
		FRAMEBUFFER
	};
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

struct POGL_RECTI
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

	POGL_RECTI() {}
	POGL_RECTI(const POGL_INT32 _x, const POGL_INT32 _y, const POGL_INT32 _width, const POGL_INT32 _height) { x = _x; y = _y; width = _width; height = _height; }
	POGL_RECTI(const POGL_RECTI& rhs) { x = rhs.x; y = rhs.y; width = rhs.width; height = rhs.height; }
	~POGL_RECTI() {}
	inline POGL_RECTI& operator=(const POGL_RECTI& rhs) { x = rhs.x; y = rhs.y; width = rhs.width; height = rhs.height; return *this; }
};

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

/*!
	\brief Vertex containing a position and a texture coordinate
*/
struct POGL_POSITION_TEXCOORD_VERTEX
{
	POGL_VECTOR3F position;
	POGL_VECTOR2F texCoord;

	POGL_POSITION_TEXCOORD_VERTEX() {}
	POGL_POSITION_TEXCOORD_VERTEX(const POGL_VECTOR3F& p, const POGL_VECTOR2F& t) { position = p; texCoord = t; }
	POGL_POSITION_TEXCOORD_VERTEX(const POGL_POSITION_TEXCOORD_VERTEX& v) { position = v.position; texCoord = v.texCoord; }
	~POGL_POSITION_TEXCOORD_VERTEX() {}
	inline POGL_POSITION_TEXCOORD_VERTEX& operator=(const POGL_POSITION_TEXCOORD_VERTEX& rhs) { position = rhs.position; texCoord = rhs.texCoord;  return *this; }
};

/* Vertex layout for the POGL_POSITION_TEXCOORD_VERTEX struct. */
static const POGL_VERTEX_LAYOUT POGL_POSITION_TEXCOORD_VERTEX_LAYOUT = {
	{ 
		{ sizeof(POGL_VECTOR3F), POGLVertexType::FLOAT, false },
		{ 0 }, 
		{ sizeof(POGL_VECTOR2F), POGLVertexType::FLOAT, false },
		0
	},
	sizeof(POGL_POSITION_TEXCOORD_VERTEX)
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
		\brief Retrieves the resource type
	*/
	virtual POGLResourceType::Enum GetResourceType() const = 0;
};

/*!
	\brief 
*/
class IPOGLDevice : public IPOGLInterface
{
public:
	/*!
		\brief Creates a new device context

		You use the device context to draw the geometry onto the screen

		\return A device context
	*/
	virtual IPOGLDeviceContext* GetDeviceContext() = 0;
	
	/*!
		\brief Creates a new deferred device context

		A deferred device context is esentially a command queue implementation for the IPOGLDeviceContext interface. You invoke it as you normally would and
		then supply it to be rendered in the main thread when it's done

		\return A device context
	*/
	virtual IPOGLDeferredDeviceContext* CreateDeferredDeviceContext() = 0;

	/*!
		\brief Swap buffers
	*/
	virtual void EndFrame() = 0;

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
				The programs to be bound to the effect
		\param numPrograms
				The number of programs to be bound to the effect
		\throwd POGLResourceException 
				Exception is thrown if the exception failed to be loaded by some reason.
		\return An effect instance
	*/
	virtual IPOGLEffect* CreateEffectFromPrograms(IPOGLShaderProgram** programs, POGL_UINT32 numPrograms) = 0;

	/*!
		\brief Creates a 3D texture
	*/
	virtual IPOGLTexture1D* CreateTexture1D() = 0;

	/*!
		\brief Creates a 2D texture

		\param size
		\param format
		\param bytes
	*/
	virtual IPOGLTexture2D* CreateTexture2D(const POGL_SIZEI& size, POGLTextureFormat::Enum format, const void* bytes) = 0;

	/*!
		\brief Creates a 3D texture
	*/
	virtual IPOGLTexture3D* CreateTexture3D() = 0;

	/*!
		\brief Creates a framebuffer that renders to the supplied textures

		\param textures
		\param numTextures
	*/
	virtual IPOGLFramebuffer* CreateFramebuffer(IPOGLTexture** textures, POGL_UINT32 numTextures) = 0;
	
	/*!
		\brief Creates a framebuffer that renders to the supplied textures

		\param textures
		\param numTextures
		\param depthStencilTexture
	*/
	virtual IPOGLFramebuffer* CreateFramebuffer(IPOGLTexture** textures, POGL_UINT32 numTextures, IPOGLTexture* depthStencilTexture) = 0;

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
		\brief Creates a vertex buffer based on the supplied parameters

		\param memory
		\param memorySize
		\param primitiveType
		\param bufferUsage
		\return
	*/
	virtual IPOGLVertexBuffer* CreateVertexBuffer(const POGL_POSITION_TEXCOORD_VERTEX* memory, POGL_SIZE memorySize, POGLPrimitiveType::Enum primitiveType, POGLBufferUsage::Enum bufferUsage) = 0;

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

	/*!
		\brief Map the data to a memory location and return a pointer to it
	*/
	virtual void* Map(IPOGLResource* resource, POGLResourceStreamType::Enum e) = 0;
	
	/*!
		\brief Map parts of the data to a memory location and return a pointer to it
		
		\param offset
				The offset, in bytes, where we want to start saving the data
		\param length
				The length of the data buffer we want to map
	*/
	virtual void* Map(IPOGLResource* resource, POGL_UINT32 offset, POGL_UINT32 length, POGLResourceStreamType::Enum e) = 0;

	/*!
		\brief Close the stream

		This must be done after all the operations have been completed. This unmaps potentially mapped resources and
		update your objects synchronization status so that any future rending operations will get the correct buffer data.
	*/
	virtual void Unmap(IPOGLResource* resource) = 0;
};

/*!

*/
class IPOGLDeferredDeviceContext : public IPOGLDeviceContext
{
public:
	/*!
		\brief Execute the commands generated by this context and clears the command queue

		\param context
	*/
	virtual void ExecuteCommands(IPOGLDeviceContext* context) = 0;

	/*!
		\brief Execute the commands generated by this device context

		\param context
		\param clearCommands
	*/
	virtual void ExecuteCommands(IPOGLDeviceContext* context, bool clearCommands) = 0;

	/*!
		\brief Flush this command queue 

		Use this method when all the calls are complete to this context. This method should only be called if you are rendering to this context inside the
		main thread. Use IPOGLDeferredDeviceContext::FlushAndWait otherwise
	*/
	virtual void Flush() = 0;

	/*!
		\brief Flush the commands queue and put the current thread to sleep and wait until the supplied condition have been met

		This method will flush the commands from the internal buffer so that they can be executed by IPOGLDeferredDeviceContext::ExecuteCommands method.

		\param condition
				Condition used by the flushing mechanism to know when we can start render again
	*/
	virtual void FlushAndWait(std::condition_variable& condition) = 0;
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
		\brief Locate the uniform with the given name
	*/
	virtual IPOGLUniform* FindUniformByName(const POGL_CHAR* name) = 0;

	/*!
		\brief Set the framebuffer used when render to this frame
	*/
	virtual void SetFramebuffer(IPOGLFramebuffer* framebuffer) = 0;

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
	*/
	virtual void SetBlendFunc(POGLSrcFactor::Enum sfactor, POGLDstFactor::Enum dfactor) = 0;

	/*!
		\brief
	*/
	virtual void SetBlend(bool b) = 0;

	/*!
		\brief
	*/
	virtual void SetViewport(const POGL_RECTI& viewport) = 0;
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
	virtual POGLTextureFormat::Enum GetTextureFormat() const = 0;
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
	virtual const POGL_SIZEI& GetSize() const = 0;
};

/*!
	\brief
*/
class IPOGLTexture3D : public IPOGLTexture
{
public:
	virtual POGL_UINT32 GetDepth() const = 0;
	virtual const POGL_SIZEI& GetSize() const = 0;
};

/*!
	\brief
*/
class IPOGLFramebuffer : public IPOGLResource
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
	virtual POGL_UINT32 GetCount() const = 0;
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
	virtual POGL_UINT32 GetCount() const = 0;
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
	POGL_CHAR mFile[256];
	POGL_CHAR mMessage[5096];
	POGL_UINT64 mLine;
};

/*!
	\brief Exception thrown if a resource generation has failed
*/
class POGLInitializationException : public POGLException {
public:
	POGLInitializationException(const POGL_CHAR* function, const POGL_UINT64 line, const POGL_CHAR* file, const POGL_CHAR* message, ...);
	~POGLInitializationException();
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

/*!
	\brief Excepton thrown if synchronization between threads failed.
*/
class POGLSyncException : public POGLException {
public:
	POGLSyncException(const POGL_CHAR* function, const POGL_UINT64 line, const POGL_CHAR* file, const POGL_CHAR* message, ...);
	~POGLSyncException();
};

#include <cstdarg>

#ifndef THROW_EXCEPTION
#define THROW_EXCEPTION(E, Message, ...) throw E(__FUNCTION__, __LINE__, __FILE__, Message, __VA_ARGS__)
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

#endif
