#pragma once
#ifndef _PLAYGL_HXX_
#define _PLAYGL_HXX_

#include <exception>
#include <cassert>

#ifndef assert_with_message
#define assert_with_message(test, message) assert(test && message)
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

class IPOGLBuffer;

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

typedef short POGL_INT16;
typedef unsigned short POGL_UINT16;
typedef int POGL_INT32;
typedef unsigned int POGL_UINT32;
typedef long POGL_INT64;
typedef unsigned long POGL_UINT64;
typedef void* POGL_HANDLE;
typedef float POGL_SINGLE;
typedef double POGL_DOUBLE;

#ifdef UNICODE
typedef wchar_t POGL_CHAR;
#else
typedef char POGL_CHAR;
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
		SIZE
	};
};

struct POGLPixelFormat
{
	enum Enum {
		R8G8B8A8 = 0,

		/* Number of enums available */
		SIZE
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
		SIZE
	};
};

//
// Structs
//

struct POGL_SIZEI
{
	union {
		struct {
			POGL_INT32 width;
			POGL_INT32 height;
		};
		POGL_INT32 size[2];
	};
};

struct POGL_VECTOR2F
{
	union {
		struct {
			POGL_SINGLE x;
			POGL_SINGLE y;
		};
		POGL_SINGLE vec[2];
	};
};

struct POGL_VECTOR2D
{
	union {
		struct {
			POGL_DOUBLE x;
			POGL_DOUBLE y;
		};
		POGL_DOUBLE vec[2];
	};
};

struct POGL_VECTOR3F
{
	union {
		struct {
			POGL_SINGLE x;
			POGL_SINGLE y;
			POGL_SINGLE z;
		};
		POGL_SINGLE vec[3];
	};
};

struct POGL_VECTOR3D
{
	union {
		struct {
			POGL_DOUBLE x;
			POGL_DOUBLE y;
			POGL_DOUBLE z;
		};
		POGL_DOUBLE vec[3];
	};
};

struct POGL_VECTOR4F
{
	union {
		struct {
			POGL_SINGLE x;
			POGL_SINGLE y;
			POGL_SINGLE z;
			POGL_SINGLE w;
		};
		POGL_SINGLE vec[4];
	};
};

struct POGL_VECTOR4D
{
	union {
		struct {
			POGL_DOUBLE x;
			POGL_DOUBLE y;
			POGL_DOUBLE z;
			POGL_DOUBLE w;
		};
		POGL_DOUBLE vec[4];
	};
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
			POGL_SINGLE
			_11, _21, _31, _41,
			_12, _22, _32, _42,
			_13, _23, _33, _43,
			_14, _24, _34, _44;
		};
		struct {
			POGL_SINGLE m[4][4];
		};
		POGL_SINGLE vec[16];
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
	POGL_UINT16 colorBits;

	/* How many bits used for the depth buffer */
	POGL_UINT16 depthBits;

	/* The device pixel format */
	POGLPixelFormat::Enum pixelFormat;

	/* Extra flags for the device info, for example: DEBUG mode */
	POGL_UINT32 flags;
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
	virtual IPOGLDeviceContext* GetContext() = 0;
	
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
		\brief Creates an effect based on the supplied file

		\param path
		\param type
		\return
	*/
	virtual IPOGLEffect* CreateEffectFromFile(const POGL_CHAR* path) = 0;
	
	/*!
		\brief Creates an effect based on the memory buffer

		\param memory
		\param size
		\param type
		\return
	*/
	virtual IPOGLEffect* CreateEffectFromMemory(const POGL_CHAR* memory, POGL_UINT32 size) = 0;

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
		\brief Apply the supplied effect to the current context

		\param effect
		\return
	*/
	virtual IPOGLRenderState* Apply(IPOGLEffect* effect) = 0;
};

/*!
	\brief
*/
class IPOGLUniform : public IPOGLInterface
{
public:
	virtual void SetMatrix(const POGL_MAT4F* mat4) = 0;
	virtual void SetMatrix(const POGL_MAT4D* mat4) = 0;
};

/*!
	\brief
*/
class IPOGLEffect : public IPOGLInterface
{
public:
	virtual IPOGLDevice* GetDevice() = 0;
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
	virtual void Draw(IPOGLBuffer* vertexBuffer) = 0;
	
	/*!
		\brief Draw the entire vertex buffer using the supplied index buffer

		\param vertexBuffer
		\param indexBuffer
	*/
	virtual void Draw(IPOGLBuffer* vertexBuffer, IPOGLBuffer* indexBuffer) = 0;
	
	/*!
		\brief Draw the entire vertex buffer using the supplied index buffer

		\param vertexBuffer
		\param indexBuffer
		\param vertexCount
	*/
	virtual void Draw(IPOGLBuffer* vertexBuffer, IPOGLBuffer* indexBuffer, POGL_UINT32 vertexCount) = 0;
	
	/*!
		\brief Draw the entire vertex buffer using the supplied index buffer

		\param vertexBuffer
		\param indexBuffer
		\param vertexCount
		\param offset
	*/
	virtual void Draw(IPOGLBuffer* vertexBuffer, IPOGLBuffer* indexBuffer, POGL_UINT32 vertexCount, POGL_UINT32 offset) = 0;

	/*!
		\brief Ends the current frame
	*/
	virtual void EndFrame() = 0;
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
	\brief Creates a new device instance.

	\param deviceInfo
	\return
*/
extern IPOGLDevice* POGLCreateDevice(POGL_DEVICE_INFO* info);

#endif
