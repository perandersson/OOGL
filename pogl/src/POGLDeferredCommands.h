#pragma once
#include "config.h"

typedef void(*POGLCommandFuncPtr)(class POGLDeferredDeviceContext*, class POGLRenderState*, struct POGLDeferredCommand*);
typedef void(*POGLCommandReleaseFuncPtr)(struct POGLDeferredCommand*);

struct POGLDeferredCommand
{
	POGLCommandFuncPtr function;
	POGLCommandReleaseFuncPtr releaseFunction;

	// Extra memory
	POGL_HANDLE extra[4];
};
extern void POGLNothing_Release(POGLDeferredCommand* command);
extern void POGLNothing_Command(class POGLDeferredDeviceContext*, class POGLRenderState*, struct POGLDeferredCommand*);

struct POGLCreateVertexBufferCommand
{
	union {
		POGLDeferredCommand _memory;
		struct {
			POGLCommandFuncPtr function;
			POGLCommandReleaseFuncPtr releaseFunction;

			class POGLVertexBuffer* vertexBuffer;
			POGL_UINT32 memoryPoolOffset;
			POGL_UINT32 size;
		};
	};
};
extern void POGLCreateVertexBuffer_Command(class POGLDeferredDeviceContext* context, POGLRenderState* state, POGLDeferredCommand* command);
extern void POGLCreateVertexBuffer_Release(POGLDeferredCommand* command);
_STATIC_ASSERT(sizeof(POGLDeferredCommand) >= sizeof(POGLCreateVertexBufferCommand));

struct POGLCreateTexture2DCommand
{
	union {
		POGLDeferredCommand _memory;
		struct {
			POGLCommandFuncPtr function;
			POGLCommandReleaseFuncPtr releaseFunction;

			class POGLTexture2D* texture;
			POGL_UINT32 memoryPoolOffset;
			POGL_UINT32 size;
		};
	};
};
extern void POGLCreateTexture2D_Command(class POGLDeferredDeviceContext* context, POGLRenderState* state, POGLDeferredCommand* command);
extern void POGLCreateTexture2D_Release(POGLDeferredCommand* command);
_STATIC_ASSERT(sizeof(POGLDeferredCommand) >= sizeof(POGLCreateVertexBufferCommand));

struct POGLMapVertexBufferCommand
{
	union {
		POGLDeferredCommand _memory;
		struct {
			POGLCommandFuncPtr function;
			POGLCommandReleaseFuncPtr releaseFunction;

			class POGLVertexBuffer* vertexBuffer;
			POGL_UINT32 memoryPoolOffset;
			POGL_UINT32 size;
		};
	};
};
extern void POGLMapVertexBuffer_Command(class POGLDeferredDeviceContext* context, POGLRenderState* state, POGLDeferredCommand* command);
extern void POGLMapVertexBuffer_Release(POGLDeferredCommand* command);
_STATIC_ASSERT(sizeof(POGLDeferredCommand) >= sizeof(POGLMapVertexBufferCommand));

struct POGLMapRangeVertexBufferCommand
{
	union {
		POGLDeferredCommand _memory;
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
extern void POGLMapRangeVertexBuffer_Command(class POGLDeferredDeviceContext* context, POGLRenderState* state, POGLDeferredCommand* command);
extern void POGLMapRangeVertexBuffer_Release(POGLDeferredCommand* command);
_STATIC_ASSERT(sizeof(POGLDeferredCommand) >= sizeof(POGLMapRangeVertexBufferCommand));

struct POGLClearCommand
{
	union {
		POGLDeferredCommand _memory;
		struct {
			POGLCommandFuncPtr function;
			POGLCommandReleaseFuncPtr releaseFunction;

			POGL_UINT32 clearBits;
		};
	};
};
extern void POGLClear_Command(class POGLDeferredDeviceContext* context, POGLRenderState* state, POGLDeferredCommand* command);
_STATIC_ASSERT(sizeof(POGLDeferredCommand) >= sizeof(POGLClearCommand));

struct POGLSetFramebufferCommand
{
	union {
		POGLDeferredCommand _memory;
		struct {
			POGLCommandFuncPtr function;
			POGLCommandReleaseFuncPtr releaseFunction;

			class POGLFramebuffer* framebuffer;
		};
	};
};
extern void POGLSetFramebuffer_Command(class POGLDeferredDeviceContext* context, POGLRenderState* state, POGLDeferredCommand* command);
extern void POGLSetFramebuffer_Release(POGLDeferredCommand* command);
_STATIC_ASSERT(sizeof(POGLDeferredCommand) >= sizeof(POGLSetFramebufferCommand));

struct POGLDeferredDrawCommand
{
	union {
		POGLDeferredCommand _memory;
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
extern void POGLDraw_Command(class POGLDeferredDeviceContext* context, POGLRenderState* state, POGLDeferredCommand* command);
extern void POGLDrawCount_Command(class POGLDeferredDeviceContext* context, POGLRenderState* state, POGLDeferredCommand* command);
extern void POGLDraw_Release(POGLDeferredCommand* command);
_STATIC_ASSERT(sizeof(POGLDeferredCommand) >= sizeof(POGLDeferredDrawCommand));

struct POGLBooleanCommand
{
	union {
		POGLDeferredCommand _memory;
		struct {
			POGLCommandFuncPtr function;
			POGLCommandReleaseFuncPtr releaseFunction;

			bool value;
		};
	};
};
extern void POGLSetDepthTest_Command(class POGLDeferredDeviceContext* context, POGLRenderState* state, POGLDeferredCommand* command);
extern void POGLSetDepthMask_Command(class POGLDeferredDeviceContext* context, POGLRenderState* state, POGLDeferredCommand* command);
extern void POGLSetStencilTest_Command(class POGLDeferredDeviceContext* context, POGLRenderState* state, POGLDeferredCommand* command);
extern void POGLSetBlend_Command(class POGLDeferredDeviceContext* context, POGLRenderState* state, POGLDeferredCommand* command);
_STATIC_ASSERT(sizeof(POGLDeferredCommand) >= sizeof(POGLBooleanCommand));


struct POGLColorMaskCommand
{
	union {
		POGLDeferredCommand _memory;
		struct {
			POGLCommandFuncPtr function;
			POGLCommandReleaseFuncPtr releaseFunction;

			POGL_UINT8 mask;
		};
	};
};
extern void POGLColorMask_Command(class POGLDeferredDeviceContext* context, POGLRenderState* state, POGLDeferredCommand* command);
_STATIC_ASSERT(sizeof(POGLDeferredCommand) >= sizeof(POGLColorMaskCommand));


struct POGLSetDepthFuncCommand
{
	union {
		POGLDeferredCommand _memory;
		struct {
			POGLCommandFuncPtr function;
			POGLCommandReleaseFuncPtr releaseFunction;

			POGLDepthFunc::Enum depthFunc;
		};
	};
};
extern void POGLSetDepthFunc_Command(class POGLDeferredDeviceContext* context, POGLRenderState* state, POGLDeferredCommand* command);
_STATIC_ASSERT(sizeof(POGLDeferredCommand) >= sizeof(POGLSetDepthFuncCommand));

struct POGLSetBlendFuncCommand
{
	union {
		POGLDeferredCommand _memory;
		struct {
			POGLCommandFuncPtr function;
			POGLCommandReleaseFuncPtr releaseFunction;

			POGLSrcFactor::Enum sfactor;
			POGLDstFactor::Enum dfactor;
		};
	};
};
extern void POGLSetBlendFunc_Command(class POGLDeferredDeviceContext* context, POGLRenderState* state, POGLDeferredCommand* command);
_STATIC_ASSERT(sizeof(POGLDeferredCommand) >= sizeof(POGLSetBlendFuncCommand));

struct POGLSetViewportCommand
{
	union {
		POGLDeferredCommand _memory;
		struct {
			POGLCommandFuncPtr function;
			POGLCommandReleaseFuncPtr releaseFunction;

			POGL_RECT viewport;
		};
	};
};
extern void POGLSetViewport_Command(class POGLDeferredDeviceContext* context, POGLRenderState* state, POGLDeferredCommand* command);
_STATIC_ASSERT(sizeof(POGLDeferredCommand) >= sizeof(POGLSetViewportCommand));

struct POGLApplyEffectCommand
{
	union {
		POGLDeferredCommand _memory;
		struct {
			POGLCommandFuncPtr function;
			POGLCommandReleaseFuncPtr releaseFunction;

			class IPOGLEffect* effect;
		};
	};
};
extern void POGLApplyEffect_Command(class POGLDeferredDeviceContext* context, POGLRenderState* state, POGLDeferredCommand* command);
extern void POGLApplyEffect_Release(POGLDeferredCommand* command);
_STATIC_ASSERT(sizeof(POGLDeferredCommand) >= sizeof(POGLApplyEffectCommand));

struct POGLCreateFrameBufferCommand
{
	union {
		POGLDeferredCommand _memory;
		struct {
			POGLCommandFuncPtr function;
			POGLCommandReleaseFuncPtr releaseFunction;

			class POGLFramebuffer* framebuffer;
		};
	};
};
extern void POGLCreateFrameBuffer_Command(class POGLDeferredDeviceContext* context, POGLRenderState* state, POGLDeferredCommand* command);
extern void POGLCreateFrameBuffer_Release(POGLDeferredCommand* command);
_STATIC_ASSERT(sizeof(POGLDeferredCommand) >= sizeof(POGLCreateFrameBufferCommand));

struct POGLResizeTexture2DCommand
{
	union {
		POGLDeferredCommand _memory;
		struct {
			POGLCommandFuncPtr function;
			POGLCommandReleaseFuncPtr releaseFunction;

			class POGLTexture2D* texture;
		};
	};
};
extern void POGLResizeTexture2D_Command(class POGLDeferredDeviceContext* context, POGLRenderState* state, POGLDeferredCommand* command);
extern void POGLResizeTexture2D_Release(POGLDeferredCommand* command);
_STATIC_ASSERT(sizeof(POGLDeferredCommand) >= sizeof(POGLCreateFrameBufferCommand));
