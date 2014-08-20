#pragma once
#include "config.h"

struct POGL_DEFERRED_COMMAND;

typedef void(*POGLCommandFuncPtr)(class POGLDeferredRenderContext*, class POGLRenderState*, POGL_DEFERRED_COMMAND*);
typedef void(*POGLCommandReleaseFuncPtr)(POGL_DEFERRED_COMMAND*);

struct POGL_DEFERRED_COMMAND
{
	POGLCommandFuncPtr function;
	POGLCommandReleaseFuncPtr releaseFunction;

	// Extra memory
	POGL_HANDLE extra[4];
};
extern void POGLNothing_Release(POGL_DEFERRED_COMMAND* command);
extern void POGLNothing_Command(class POGLDeferredRenderContext*, class POGLRenderState*, POGL_DEFERRED_COMMAND*);

struct POGLCreateVertexBufferCommand
{
	union {
		POGL_DEFERRED_COMMAND _memory;
		struct {
			POGLCommandFuncPtr function;
			POGLCommandReleaseFuncPtr releaseFunction;

			class POGLVertexBuffer* vertexBuffer;
			POGL_UINT32 memoryPoolOffset;
			POGL_UINT32 size;
		};
	};
};
extern void POGLCreateVertexBuffer_Command(class POGLDeferredRenderContext* context, POGLRenderState* state, POGL_DEFERRED_COMMAND* command);
extern void POGLCreateVertexBuffer_Release(POGL_DEFERRED_COMMAND* command);
_STATIC_ASSERT(sizeof(POGL_DEFERRED_COMMAND) >= sizeof(POGLCreateVertexBufferCommand));

struct POGLCreateTexture2DCommand
{
	union {
		POGL_DEFERRED_COMMAND _memory;
		struct {
			POGLCommandFuncPtr function;
			POGLCommandReleaseFuncPtr releaseFunction;

			class POGLTexture2D* texture;
			POGL_UINT32 memoryPoolOffset;
			POGL_UINT32 size;
		};
	};
};
extern void POGLCreateTexture2D_Command(class POGLDeferredRenderContext* context, POGLRenderState* state, POGL_DEFERRED_COMMAND* command);
extern void POGLCreateTexture2D_Release(POGL_DEFERRED_COMMAND* command);
_STATIC_ASSERT(sizeof(POGL_DEFERRED_COMMAND) >= sizeof(POGLCreateVertexBufferCommand));

struct POGLMapVertexBufferCommand
{
	union {
		POGL_DEFERRED_COMMAND _memory;
		struct {
			POGLCommandFuncPtr function;
			POGLCommandReleaseFuncPtr releaseFunction;

			class POGLVertexBuffer* vertexBuffer;
			POGL_UINT32 memoryPoolOffset;
			POGL_UINT32 size;
		};
	};
};
extern void POGLMapVertexBuffer_Command(class POGLDeferredRenderContext* context, POGLRenderState* state, POGL_DEFERRED_COMMAND* command);
extern void POGLMapVertexBuffer_Release(POGL_DEFERRED_COMMAND* command);
_STATIC_ASSERT(sizeof(POGL_DEFERRED_COMMAND) >= sizeof(POGLMapVertexBufferCommand));

struct POGLMapRangeVertexBufferCommand
{
	union {
		POGL_DEFERRED_COMMAND _memory;
		struct {
			POGLCommandFuncPtr function;
			POGLCommandReleaseFuncPtr releaseFunction;

			class POGLVertexBuffer* vertexBuffer;
			POGL_UINT32 memoryPoolOffset;
			POGL_UINT32 offset;
			POGL_UINT32 length;
		};
	};
};
extern void POGLMapRangeVertexBuffer_Command(class POGLDeferredRenderContext* context, POGLRenderState* state, POGL_DEFERRED_COMMAND* command);
extern void POGLMapRangeVertexBuffer_Release(POGL_DEFERRED_COMMAND* command);
_STATIC_ASSERT(sizeof(POGL_DEFERRED_COMMAND) >= sizeof(POGLMapRangeVertexBufferCommand));

struct POGLClearCommand
{
	union {
		POGL_DEFERRED_COMMAND _memory;
		struct {
			POGLCommandFuncPtr function;
			POGLCommandReleaseFuncPtr releaseFunction;

			POGL_UINT32 clearBits;
		};
	};
};
extern void POGLClear_Command(class POGLDeferredRenderContext* context, POGLRenderState* state, POGL_DEFERRED_COMMAND* command);
_STATIC_ASSERT(sizeof(POGL_DEFERRED_COMMAND) >= sizeof(POGLClearCommand));

struct POGLSetFramebufferCommand
{
	union {
		POGL_DEFERRED_COMMAND _memory;
		struct {
			POGLCommandFuncPtr function;
			POGLCommandReleaseFuncPtr releaseFunction;

			class POGLFramebuffer* framebuffer;
		};
	};
};
extern void POGLSetFramebuffer_Command(class POGLDeferredRenderContext* context, POGLRenderState* state, POGL_DEFERRED_COMMAND* command);
extern void POGLSetFramebuffer_Release(POGL_DEFERRED_COMMAND* command);
_STATIC_ASSERT(sizeof(POGL_DEFERRED_COMMAND) >= sizeof(POGLSetFramebufferCommand));

struct POGLDeferredDrawCommand
{
	union {
		POGL_DEFERRED_COMMAND _memory;
		struct {
			POGLCommandFuncPtr function;
			POGLCommandReleaseFuncPtr releaseFunction;

			IPOGLVertexBuffer* vertexBuffer;
			IPOGLIndexBuffer* indexBuffer;
			POGL_UINT32 startIndex;
			POGL_UINT32 count;
		};
	};
};
extern void POGLDraw_Command(class POGLDeferredRenderContext* context, POGLRenderState* state, POGL_DEFERRED_COMMAND* command);
extern void POGLDrawCount_Command(class POGLDeferredRenderContext* context, POGLRenderState* state, POGL_DEFERRED_COMMAND* command);
extern void POGLDraw_Release(POGL_DEFERRED_COMMAND* command);
_STATIC_ASSERT(sizeof(POGL_DEFERRED_COMMAND) >= sizeof(POGLDeferredDrawCommand));

struct POGLBooleanCommand
{
	union {
		POGL_DEFERRED_COMMAND _memory;
		struct {
			POGLCommandFuncPtr function;
			POGLCommandReleaseFuncPtr releaseFunction;

			bool value;
		};
	};
};
extern void POGLSetDepthTest_Command(class POGLDeferredRenderContext* context, POGLRenderState* state, POGL_DEFERRED_COMMAND* command);
extern void POGLSetDepthMask_Command(class POGLDeferredRenderContext* context, POGLRenderState* state, POGL_DEFERRED_COMMAND* command);
extern void POGLSetStencilTest_Command(class POGLDeferredRenderContext* context, POGLRenderState* state, POGL_DEFERRED_COMMAND* command);
extern void POGLSetBlend_Command(class POGLDeferredRenderContext* context, POGLRenderState* state, POGL_DEFERRED_COMMAND* command);
_STATIC_ASSERT(sizeof(POGL_DEFERRED_COMMAND) >= sizeof(POGLBooleanCommand));

struct POGLColorMaskCommand
{
	union {
		POGL_DEFERRED_COMMAND _memory;
		struct {
			POGLCommandFuncPtr function;
			POGLCommandReleaseFuncPtr releaseFunction;

			POGL_UINT8 mask;
		};
	};
};
extern void POGLColorMask_Command(class POGLDeferredRenderContext* context, POGLRenderState* state, POGL_DEFERRED_COMMAND* command);
_STATIC_ASSERT(sizeof(POGL_DEFERRED_COMMAND) >= sizeof(POGLColorMaskCommand));

struct POGLStencilMaskCommand
{
	union {
		POGL_DEFERRED_COMMAND _memory;
		struct {
			POGLCommandFuncPtr function;
			POGLCommandReleaseFuncPtr releaseFunction;

			POGL_UINT32 mask;
		};
	};
};
extern void POGLStencilMask_Command(class POGLDeferredRenderContext* context, POGLRenderState* state, POGL_DEFERRED_COMMAND* command);
_STATIC_ASSERT(sizeof(POGL_DEFERRED_COMMAND) >= sizeof(POGLStencilMaskCommand));

struct POGLSetDepthFuncCommand
{
	union {
		POGL_DEFERRED_COMMAND _memory;
		struct {
			POGLCommandFuncPtr function;
			POGLCommandReleaseFuncPtr releaseFunction;

			POGLDepthFunc::Enum depthFunc;
		};
	};
};
extern void POGLSetDepthFunc_Command(class POGLDeferredRenderContext* context, POGLRenderState* state, POGL_DEFERRED_COMMAND* command);
_STATIC_ASSERT(sizeof(POGL_DEFERRED_COMMAND) >= sizeof(POGLSetDepthFuncCommand));

struct POGLSetBlendFuncCommand
{
	union {
		POGL_DEFERRED_COMMAND _memory;
		struct {
			POGLCommandFuncPtr function;
			POGLCommandReleaseFuncPtr releaseFunction;

			POGLSrcFactor::Enum sfactor;
			POGLDstFactor::Enum dfactor;
		};
	};
};
extern void POGLSetBlendFunc_Command(class POGLDeferredRenderContext* context, POGLRenderState* state, POGL_DEFERRED_COMMAND* command);
_STATIC_ASSERT(sizeof(POGL_DEFERRED_COMMAND) >= sizeof(POGLSetBlendFuncCommand));

struct POGLSetFrontFaceCommand
{
	union {
		POGL_DEFERRED_COMMAND _memory;
		struct {
			POGLCommandFuncPtr function;
			POGLCommandReleaseFuncPtr releaseFunction;

			POGLFrontFace::Enum frontFace;
		};
	};
};
extern void POGLSetFrontFace_Command(class POGLDeferredRenderContext* context, POGLRenderState* state, POGL_DEFERRED_COMMAND* command);
_STATIC_ASSERT(sizeof(POGL_DEFERRED_COMMAND) >= sizeof(POGLSetFrontFaceCommand));

struct POGLSetCullFaceCommand
{
	union {
		POGL_DEFERRED_COMMAND _memory;
		struct {
			POGLCommandFuncPtr function;
			POGLCommandReleaseFuncPtr releaseFunction;

			POGLCullFace::Enum cullFace;
		};
	};
};
extern void POGLSetCullFace_Command(class POGLDeferredRenderContext* context, POGLRenderState* state, POGL_DEFERRED_COMMAND* command);
_STATIC_ASSERT(sizeof(POGL_DEFERRED_COMMAND) >= sizeof(POGLSetCullFaceCommand));

struct POGLSetViewportCommand
{
	union {
		POGL_DEFERRED_COMMAND _memory;
		struct {
			POGLCommandFuncPtr function;
			POGLCommandReleaseFuncPtr releaseFunction;

			POGL_RECT viewport;
		};
	};
};
extern void POGLSetViewport_Command(class POGLDeferredRenderContext* context, POGLRenderState* state, POGL_DEFERRED_COMMAND* command);
_STATIC_ASSERT(sizeof(POGL_DEFERRED_COMMAND) >= sizeof(POGLSetViewportCommand));

struct POGLApplyProgramCommand
{
	union {
		POGL_DEFERRED_COMMAND _memory;
		struct {
			POGLCommandFuncPtr function;
			POGLCommandReleaseFuncPtr releaseFunction;

			IPOGLProgram* program;
		};
	};
};
extern void POGLApplyProgram_Command(class POGLDeferredRenderContext* context, POGLRenderState* state, POGL_DEFERRED_COMMAND* command);
extern void POGLApplyProgram_Release(POGL_DEFERRED_COMMAND* command);
_STATIC_ASSERT(sizeof(POGL_DEFERRED_COMMAND) >= sizeof(POGLApplyProgramCommand));

struct POGLCreateFrameBufferCommand
{
	union {
		POGL_DEFERRED_COMMAND _memory;
		struct {
			POGLCommandFuncPtr function;
			POGLCommandReleaseFuncPtr releaseFunction;

			class POGLFramebuffer* framebuffer;
		};
	};
};
extern void POGLCreateFrameBuffer_Command(class POGLDeferredRenderContext* context, POGLRenderState* state, POGL_DEFERRED_COMMAND* command);
extern void POGLCreateFrameBuffer_Release(POGL_DEFERRED_COMMAND* command);
_STATIC_ASSERT(sizeof(POGL_DEFERRED_COMMAND) >= sizeof(POGLCreateFrameBufferCommand));

struct POGLResizeTexture2DCommand
{
	union {
		POGL_DEFERRED_COMMAND _memory;
		struct {
			POGLCommandFuncPtr function;
			POGLCommandReleaseFuncPtr releaseFunction;

			class POGLTexture2D* texture;
			POGL_SIZE newSize;
		};
	};
};
extern void POGLResizeTexture2D_Command(class POGLDeferredRenderContext* context, POGLRenderState* state, POGL_DEFERRED_COMMAND* command);
extern void POGLResizeTexture2D_Release(POGL_DEFERRED_COMMAND* command);
_STATIC_ASSERT(sizeof(POGL_DEFERRED_COMMAND) >= sizeof(POGLCreateFrameBufferCommand));
