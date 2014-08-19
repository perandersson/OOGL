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
extern IPOGLTexture2D* POGLXLoadTGAImageFromFile(IPOGLDeviceContext* context, const POGL_CHAR* fileName);

/*!
	\brief Load the supplied BMP fileName into a 2D texture and returns it

	\param context
	\param fileName
	\return
*/
extern IPOGLTexture2D* POGLXLoadBMPImageFromFile(IPOGLDeviceContext* context, const POGL_CHAR* fileName);

/*!
	\brief Load the supplied BMP data into a 2D texture and returns it

	\param context
	\param bytes
	\param size
	\return
*/
extern IPOGLTexture2D* POGLXLoadBMPImageFromMemory(IPOGLDeviceContext* context, const POGL_CHAR* bytes, POGL_UINT32 size);

#endif
