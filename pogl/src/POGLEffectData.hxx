#pragma once
#include "uniforms/POGLUniformProperty.hxx"
#include <memory>

struct POGLEffectData
{
	/* The OpenGL program ID */
	GLuint programID;
	
	/* Containing all the uniform properties for this effect */
	std::hash_map<POGL_STRING, std::shared_ptr<POGLUniformProperty>> uniformProperties;
	
	/* Should depth test be enabled for this effect */
	bool depthTest;

	/* Function used to manage depth validation */
	POGLDepthFunc::Enum depthFunc;

	/* Should we write to the depth buffer or not */
	bool depthMask;

	/* Should we write to the color buffer parts or not */
	POGL_UINT8 colorMask;

	/* Should we write to the stencil buffer or not */
	bool stencilTest;
};
