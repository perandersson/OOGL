#pragma once
#ifndef _POGLEXT_H_
#define _POGLEXT_H_

#ifndef _POGL_H_
#error "You must include pogl.h before poglext.h"
#endif

#include "pogl.h"

/*!
	\brief Load the supplied fileName into a 2D texture and returns it

	\param context
	\param fileName
	\return
*/
IPOGLTexture2D* POGLLoadTextureFromFile(IPOGLDeviceContext* context, const POGL_CHAR* fileName);


#endif
