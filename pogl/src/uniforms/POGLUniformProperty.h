#pragma once
#include "config.h"
#include <gl/pogl.h>

struct POGLUniformProperty
{
	/* The component ID */
	GLint componentID;

	/* The uniform type */
	GLenum uniformType;

	/* The name of the component */
	POGL_STRING name;

	/* Texture min filter */
	POGLMinFilter::Enum minFilter;

	/* Texture mag filter */
	POGLMagFilter::Enum magFilter;

	/* Texture wrap S,T,R coordinates */
	POGLTextureWrap::Enum wrap[3];

	/* Compare function */
	POGLCompareFunc::Enum compareFunc;

	/* Compare mode */
	POGLCompareMode::Enum compareMode;

	POGLUniformProperty() {
		componentID = 0;
		uniformType = 0;
		minFilter = POGLMinFilter::DEFAULT;
		magFilter = POGLMagFilter::DEFAULT;
		wrap[0] = wrap[1] = wrap[2] = POGLTextureWrap::DEFAULT;
		compareFunc = POGLCompareFunc::DEFAULT;
		compareMode = POGLCompareMode::DEFAULT;
	}

	~POGLUniformProperty() {
	}
};
