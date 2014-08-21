#pragma once
#include "config.h"

class POGLDeferredRenderContext;
class POGLRenderState;
class POGLVertexBuffer;
class POGLTexture2D;
class POGLFramebuffer;

typedef void(*POGLCommandFuncPtr)(POGLDeferredRenderContext*, POGLRenderState*, POGL_HANDLE);
typedef void(*POGLCommandReleaseFuncPtr)(POGL_HANDLE);

struct POGL_DEFERRED_COMMAND
{
	POGLCommandFuncPtr function;
	POGLCommandReleaseFuncPtr releaseFunction;

	// The size of the actual command memory data
	POGL_UINT32 size;
};
extern void POGLNothing_Command(POGLDeferredRenderContext*, POGLRenderState*, POGL_HANDLE);
extern void POGLNothing_Release(POGL_HANDLE command);

#ifndef FOR_EACH_COMMAND
#define FOR_EACH_COMMAND(commands, numCommands) { \
	POGL_BYTE* ptr = commands; \
	for (POGL_UINT32 i = 0; i < numCommands;) { \
		POGL_DEFERRED_COMMAND* command = (POGL_DEFERRED_COMMAND*)ptr; \
		ptr += POGL_DEFERRED_COMMAND_SIZE;
#endif

#ifndef END_FOR_COMMANDS
#define END_FOR_COMMANDS() \
	ptr += command->size; \
	i += command->size + POGL_DEFERRED_COMMAND_SIZE; \
	} }
#endif

#ifndef OFFSET_PTR
#define OFFSET_PTR(ptr, offset) ((POGL_BYTE*)ptr + offset)
#endif

static const POGL_UINT32 POGL_DEFERRED_COMMAND_SIZE = sizeof(POGL_DEFERRED_COMMAND);
static const POGL_UINT32 POGL_DEFERRED_COMMAND_RESIZE_SIZE = 100;

struct POGL_CREATEVERTEXBUFFER_COMMAND_DATA
{
	// The vertex buffer we want to create
	POGLVertexBuffer* vertexBuffer;

	// The offset where the data begins
	POGL_UINT32 memoryOffset;

	// The size (in bytes) of the vertex buffer data
	POGL_UINT32 dataSize;
};
extern void POGLCreateVertexBuffer_Command(POGLDeferredRenderContext* context, POGLRenderState* state, POGL_HANDLE command);
extern void POGLCreateVertexBuffer_Release(POGL_HANDLE command);

struct POGL_CREATETEXTURE2D_COMMAND_DATA
{
	// The texture we want to create
	POGLTexture2D* texture;

	// The offset where the data begins
	POGL_UINT32 memoryOffset;

	// The size (in bytes) of the texture buffer data
	POGL_UINT32 dataSize;
};
extern void POGLCreateTexture2D_Command(POGLDeferredRenderContext* context, POGLRenderState* state, POGL_HANDLE command);
extern void POGLCreateTexture2D_Release(POGL_HANDLE command);

struct POGL_MAPVERTEXBUFFER_COMMAND_DATA
{
	// The vertex buffer we want to map
	POGLVertexBuffer* vertexBuffer;

	// The offset where the data begins
	POGL_UINT32 memoryOffset;

	// The size (in bytes) of the mapped data
	POGL_UINT32 dataSize;
};
extern void POGLMapVertexBuffer_Command(POGLDeferredRenderContext* context, POGLRenderState* state, POGL_HANDLE command);
extern void POGLMapVertexBuffer_Release(POGL_HANDLE command);

struct POGL_MAPRANGEVERTEXBUFFER_COMMAND_DATA
{
	// The vertex buffer we want to map
	POGLVertexBuffer* vertexBuffer;

	// The offset where the data begins
	POGL_UINT32 memoryOffset;

	// The offset, in bytes, where we should put the new data (in the vertex buffer)
	POGL_UINT32 offset;

	// The size, in bytes
	POGL_UINT32 length;
};
extern void POGLMapRangeVertexBuffer_Command(POGLDeferredRenderContext* context, POGLRenderState* state, POGL_HANDLE command);
extern void POGLMapRangeVertexBuffer_Release(POGL_HANDLE command);

struct POGL_CLEAR_COMMAND_DATA
{
	// Clear buffer bits
	POGL_UINT32 clearBits;
};
extern void POGLClear_Command(POGLDeferredRenderContext* context, POGLRenderState* state, POGL_HANDLE command);

struct POGL_SETFRAMEBUFFER_COMMAND_DATA
{
	// The framebuffer we want to set
	POGLFramebuffer* framebuffer;
};
extern void POGLSetFramebuffer_Command(POGLDeferredRenderContext* context, POGLRenderState* state, POGL_HANDLE command);
extern void POGLSetFramebuffer_Release(POGL_HANDLE command);

struct POGL_DRAW_COMMAND_DATA
{
	/* The vertex buffer we want to draw */
	IPOGLVertexBuffer* vertexBuffer;

	/* The index buffer we want to draw */
	IPOGLIndexBuffer* indexBuffer;

	/* Where we want to start render the vertex buffer */
	POGL_UINT32 startIndex;

	/* How many vertices we want to draw */
	POGL_UINT32 count;
};
extern void POGLDraw_Command(POGLDeferredRenderContext* context, POGLRenderState* state, POGL_HANDLE command);
extern void POGLDrawCount_Command(POGLDeferredRenderContext* context, POGLRenderState* state, POGL_HANDLE command);
extern void POGLDraw_Release(POGL_HANDLE command);

struct POGL_BOOLEAN_COMMAND_DATA
{
	bool value;
};
extern void POGLSetDepthTest_Command(POGLDeferredRenderContext* context, POGLRenderState* state, POGL_HANDLE command);
extern void POGLSetDepthMask_Command(POGLDeferredRenderContext* context, POGLRenderState* state, POGL_HANDLE command);
extern void POGLSetStencilTest_Command(POGLDeferredRenderContext* context, POGLRenderState* state, POGL_HANDLE command);
extern void POGLSetBlend_Command(POGLDeferredRenderContext* context, POGLRenderState* state, POGL_HANDLE command);

struct POGL_COLORMASK_COMMAND_DATA
{
	POGL_UINT8 mask;
};
extern void POGLColorMask_Command(POGLDeferredRenderContext* context, POGLRenderState* state, POGL_HANDLE command);

struct POGL_STENCILMASK_COMMAND_DATA
{
	POGL_UINT32 mask;
};
extern void POGLStencilMask_Command(POGLDeferredRenderContext* context, POGLRenderState* state, POGL_HANDLE command);

struct POGL_SETDEPTHFUNC_COMMAND_DATA
{
	POGLDepthFunc::Enum depthFunc;
};
extern void POGLSetDepthFunc_Command(POGLDeferredRenderContext* context, POGLRenderState* state, POGL_HANDLE command);

struct POGL_SETBLENDFUNC_COMMAND_DATA
{
	POGLSrcFactor::Enum sfactor;
	POGLDstFactor::Enum dfactor;
};
extern void POGLSetBlendFunc_Command(POGLDeferredRenderContext* context, POGLRenderState* state, POGL_HANDLE command);

struct POGL_SETFRONTFACE_COMMAND_DATA
{
	POGLFrontFace::Enum frontFace;
};
extern void POGLSetFrontFace_Command(POGLDeferredRenderContext* context, POGLRenderState* state, POGL_HANDLE command);

struct POGL_SETCULLFACE_COMMAND_DATA
{
	POGLCullFace::Enum cullFace;
};
extern void POGLSetCullFace_Command(POGLDeferredRenderContext* context, POGLRenderState* state, POGL_HANDLE command);

struct POGL_SETVIEWPORT_COMMAND_DATA
{
	POGL_RECT viewport;
};
extern void POGLSetViewport_Command(POGLDeferredRenderContext* context, POGLRenderState* state, POGL_HANDLE command);

struct POGL_APPLYPROGRAM_COMMAND
{
	IPOGLProgram* program;
};
extern void POGLApplyProgram_Command(POGLDeferredRenderContext* context, POGLRenderState* state, POGL_HANDLE command);
extern void POGLApplyProgram_Release(POGL_HANDLE command);

struct POGL_CREATEFRAMEBUFFER_COMMAND_DATA
{
	POGLFramebuffer* framebuffer;
};
extern void POGLCreateFrameBuffer_Command(POGLDeferredRenderContext* context, POGLRenderState* state, POGL_HANDLE command);
extern void POGLCreateFrameBuffer_Release(POGL_HANDLE command);

struct POGL_RESIZETEXTURE2D_COMMAND_DATA
{
	POGLTexture2D* texture;
	POGL_SIZE newSize;
};
extern void POGLResizeTexture2D_Command(POGLDeferredRenderContext* context, POGLRenderState* state, POGL_HANDLE command);
extern void POGLResizeTexture2D_Release(POGL_HANDLE command);

struct POGL_UNIFORM_SET_INT_COMMAND_DATA
{
	// The name of the uniform
	const POGL_STRING* name;
	
	// The uniform value
	POGL_INT32 values[4];

	// The number of values
	POGL_UINT32 count;
};
extern void POGLUniformSetInt_Command(POGLDeferredRenderContext* context, POGLRenderState* state, POGL_HANDLE command);

struct POGL_UNIFORM_SET_UINT_COMMAND_DATA
{
	// The name of the uniform
	const POGL_STRING* name;

	// The uniform value
	POGL_UINT32 values[4];

	// The number of values
	POGL_UINT32 count;
};
extern void POGLUniformSetUInt_Command(POGLDeferredRenderContext* context, POGLRenderState* state, POGL_HANDLE command);

struct POGL_UNIFORM_SET_FLOAT_COMMAND_DATA
{
	// The name of the uniform
	const POGL_STRING* name;

	// The uniform value
	POGL_FLOAT values[4];

	// The number of values
	POGL_UINT32 count;
};
extern void POGLUniformSetFloat_Command(POGLDeferredRenderContext* context, POGLRenderState* state, POGL_HANDLE command);

struct POGL_UNIFORM_SET_MAT4_COMMAND_DATA
{
	// The name of the uniform
	const POGL_STRING* name;

	// The uniform value
	POGL_MAT4 matrix;
};
extern void POGLUniformSetMat4_Command(POGLDeferredRenderContext* context, POGLRenderState* state, POGL_HANDLE command);


