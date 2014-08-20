#pragma once
#ifndef _POGLEXT_H_
#define _POGLEXT_H_

#ifndef _POGL_H_
#error "You must include pogl.h before poglext.h"
#endif

/*!
	\brief Load the supplied TGA fileName into a 2D texture and returns it

	\param context
	\param fileName
	\return
*/
extern POGLAPI IPOGLTexture2D* POGLXLoadTGAImageFromFile(IPOGLRenderContext* context, const POGL_CHAR* fileName);

/*!
	\brief Load the supplied BMP fileName into a 2D texture and returns it

	\param context
	\param fileName
	\return
*/
extern POGLAPI IPOGLTexture2D* POGLXLoadBMPImageFromFile(IPOGLRenderContext* context, const POGL_CHAR* fileName);

/*!
	\brief Load the supplied BMP data into a 2D texture and returns it

	\param context
	\param bytes
	\param size
	\return
*/
extern POGLAPI IPOGLTexture2D* POGLXLoadBMPImageFromMemory(IPOGLRenderContext* context, const POGL_CHAR* bytes, POGL_UINT32 size);

/*!
	\brief Create a IPOGLVertexBuffer instance containing the vertices needed to draw a sphere

	\param context
	\param R
			The radius of the sphere
	\param Q
			The quality of the sphere
	\param H
	\param K
	\param Z
	\param usage
	\return
*/
extern POGLAPI IPOGLVertexBuffer* POGLXCreateSphere(IPOGLRenderContext* context, POGL_FLOAT R, POGL_UINT32 Q, POGL_UINT32 H, POGL_FLOAT K, POGL_UINT32 Z, POGLBufferUsage::Enum usage);

#endif
