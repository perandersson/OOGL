#pragma once
#include "config.h"

typedef void(*POGLCommandFuncPtr)(class POGLDeferredDeviceContext*, class POGLRenderState*, struct POGLDeferredCommand*);

struct POGLDeferredCommand
{
	POGLCommandFuncPtr function;
	IPOGLResource* resource;

	// Extra memory
	POGL_HANDLE extra[3];
};

struct POGLDeferredCreateVertexBufferCommand
{
	union {
		POGLDeferredCommand command;
		struct {
			POGLCommandFuncPtr function;
			class POGLVertexBuffer* vertexBuffer;

			POGL_UINT32 memoryPoolOffset;
			POGL_SIZE size;
		};
	};
};
extern void POGLCreateVertexBuffer_Command(class POGLDeferredDeviceContext* context, POGLRenderState* state, POGLDeferredCommand* command);
_STATIC_ASSERT(sizeof(POGLDeferredCommand) >= sizeof(POGLDeferredCreateVertexBufferCommand));

struct POGLDeferredMapVertexBufferCommand
{
	union {
		POGLDeferredCommand command;
		struct {
			POGLCommandFuncPtr function;
			class POGLVertexBuffer* vertexBuffer;

			POGL_UINT32 memoryPoolOffset;
			POGL_SIZE size;
		};
	};
};
extern void POGLMapVertexBuffer_Command(class POGLDeferredDeviceContext* context, POGLRenderState* state, POGLDeferredCommand* command);
_STATIC_ASSERT(sizeof(POGLDeferredCommand) >= sizeof(POGLDeferredMapVertexBufferCommand));

struct POGLDeferredMapRangeVertexBufferCommand
{
	union {
		POGLDeferredCommand command;
		struct {
			POGLCommandFuncPtr function;
			class POGLVertexBuffer* vertexBuffer;

			POGL_UINT32 memoryPoolOffset;
			POGL_SIZE offset;
			POGL_SIZE length;
		};
	};
};
extern void POGLMapRangeVertexBuffer_Command(class POGLDeferredDeviceContext* context, POGLRenderState* state, POGLDeferredCommand* command);
_STATIC_ASSERT(sizeof(POGLDeferredCommand) >= sizeof(POGLDeferredMapRangeVertexBufferCommand));
