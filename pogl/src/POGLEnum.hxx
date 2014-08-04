#pragma once
#include "config.hxx"
#include <gl/pogl.hxx>

class POGLEnum
{
public:
	//static GLenum Convert(Access::Enum e);
	static GLenum Convert(POGLCompareFunc::Enum e);
	static GLenum Convert(POGLCompareMode::Enum e);
	static GLenum Convert(POGLMagFilter::Enum e);
	static GLenum Convert(POGLMinFilter::Enum e);
	static GLenum Convert(POGLTextureWrap::Enum e);
	static GLenum Convert(POGLBufferUsage::Enum e);
	static GLenum Convert(POGLVertexType::Enum e);

	static GLenum Convert(POGLDepthFunc::Enum e);
	//static GLenum Convert(SrcFactor::Enum e);
	//static GLenum Convert(DestFactor::Enum e);
	//static GLenum Convert(FrontFace::Enum e);
	//static GLenum Convert(CullFace::Enum e);
	//static GLenum Convert(PolygonMode::Enum e);
	static GLenum Convert(POGLPrimitiveType::Enum e);
	//static GLenum Convert(TextureCubeSide::Enum e);

	//static GLenum ConvertToTextureFormatEnum(TextureFormat::Enum format);
	//static GLenum ConvertToInternalTextureFormatEnum(TextureFormat::Enum format);
};
