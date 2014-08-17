#include "MemCheck.h"
#include "POGLEnum.h"

//
//GLenum OGLEnum::Convert(Access::Enum e)
//{
//	static const GLenum enums[Access::SIZE] = {
//		GL_READ_ONLY,
//		GL_WRITE_ONLY,
//		GL_READ_WRITE,
//	};
//
//	return enums[(uint32)e];
//}

GLenum POGLEnum::Convert(POGLCompareFunc::Enum e)
{
	static const GLenum enums[POGLCompareFunc::COUNT] = {
		GL_NEVER,
		GL_LESS,
		GL_EQUAL,
		GL_LEQUAL,
		GL_GREATER,
		GL_NOTEQUAL,
		GL_GEQUAL,
		GL_ALWAYS
	};

	return enums[(POGL_UINT32)e];
}

GLenum POGLEnum::Convert(POGLCompareMode::Enum e)
{
	static const GLenum enums[POGLCompareMode::COUNT] = {
		GL_COMPARE_R_TO_TEXTURE,
		GL_NONE
	};

	return enums[(POGL_UINT32)e];
}

GLenum POGLEnum::Convert(POGLMagFilter::Enum e)
{
	static const GLenum enums[POGLMagFilter::COUNT] = {
		GL_NEAREST,
		GL_LINEAR
	};

	return enums[(POGL_UINT32)e];
}

GLenum POGLEnum::Convert(POGLMinFilter::Enum e)
{
	static const GLenum enums[POGLMinFilter::COUNT] = {
		GL_NEAREST,
		GL_LINEAR,
		GL_NEAREST_MIPMAP_NEAREST,
		GL_LINEAR_MIPMAP_NEAREST,
		GL_NEAREST_MIPMAP_LINEAR,
		GL_LINEAR_MIPMAP_LINEAR
	};

	return enums[(POGL_UINT32)e];
}

GLenum POGLEnum::Convert(POGLTextureWrap::Enum e)
{
	static const GLenum enums[POGLTextureWrap::COUNT] = {
		GL_CLAMP_TO_EDGE,
		GL_CLAMP_TO_BORDER,
		GL_MIRRORED_REPEAT,
		GL_REPEAT
	};

	return enums[(POGL_UINT32)e];
}

GLenum POGLEnum::Convert(POGLBufferUsage::Enum e)
{
	static GLenum values[POGLBufferUsage::COUNT] = {
		GL_STATIC_DRAW,
		GL_DYNAMIC_DRAW
	};

	return values[(POGL_UINT32)e];
}

GLenum POGLEnum::Convert(POGLVertexType::Enum e)
{
	static const GLenum enums[POGLVertexType::COUNT] = {
		GL_BYTE,
		GL_UNSIGNED_BYTE,
		GL_SHORT,
		GL_UNSIGNED_SHORT,
		GL_INT,
		GL_UNSIGNED_INT,
		GL_FLOAT,
		GL_DOUBLE
	};

	return enums[(POGL_UINT32)e];
}

GLenum POGLEnum::Convert(POGLShaderType::Enum e)
{
	static const GLenum enums[POGLShaderType::COUNT] = {
		GL_GEOMETRY_SHADER,
		GL_VERTEX_SHADER,
		GL_FRAGMENT_SHADER
	};

	return enums[(POGL_UINT32)e];
}

GLenum POGLEnum::Convert(POGLDepthFunc::Enum depthFunc)
{
	static const GLenum enums[POGLDepthFunc::COUNT] = {
		GL_NEVER,
		GL_LESS,
		GL_EQUAL,
		GL_LEQUAL,
		GL_GREATER,
		GL_NOTEQUAL,
		GL_GEQUAL,
		GL_ALWAYS
	};

	return enums[(POGL_UINT32)depthFunc];
}

GLenum POGLEnum::Convert(POGLSrcFactor::Enum sfactor)
{
	static const GLenum enums[POGLSrcFactor::COUNT] = {
		GL_ZERO,
		GL_ONE,
		GL_SRC_COLOR,
		GL_ONE_MINUS_SRC_COLOR,
		GL_SRC_ALPHA,
		GL_ONE_MINUS_SRC_ALPHA,
		GL_DST_ALPHA,
		GL_ONE_MINUS_DST_ALPHA,
		GL_DST_COLOR,
		GL_ONE_MINUS_DST_COLOR,
		GL_CONSTANT_COLOR,
		GL_ONE_MINUS_CONSTANT_COLOR,
		GL_CONSTANT_ALPHA,
		GL_ONE_MINUS_CONSTANT_ALPHA
	};

	return enums[(POGL_UINT32)sfactor];
}

GLenum POGLEnum::Convert(POGLDstFactor::Enum dfactor)
{
	static const GLenum enums[POGLDstFactor::COUNT] = {
		GL_ZERO,
		GL_ONE,
		GL_SRC_COLOR,
		GL_ONE_MINUS_SRC_COLOR,
		GL_SRC_ALPHA,
		GL_ONE_MINUS_SRC_ALPHA,
		GL_DST_ALPHA,
		GL_ONE_MINUS_DST_ALPHA,
		GL_DST_COLOR,
		GL_ONE_MINUS_DST_COLOR,
		GL_CONSTANT_COLOR,
		GL_ONE_MINUS_CONSTANT_COLOR,
		GL_CONSTANT_ALPHA,
		GL_ONE_MINUS_CONSTANT_ALPHA
	};

	return enums[(POGL_UINT32)dfactor];
}


GLenum POGLEnum::Convert(POGLFrontFace::Enum frontFace)
{
	static const GLenum enums[POGLFrontFace::COUNT] = {
		GL_CW,
		GL_CCW
	};

	return enums[(POGL_UINT32)frontFace];
}

GLenum POGLEnum::Convert(POGLCullFace::Enum cullFace)
{
	static const GLenum enums[POGLCullFace::COUNT] = {
		0,
		GL_FRONT,
		GL_BACK,
		GL_FRONT_AND_BACK,
	};

	return enums[(POGL_UINT32)cullFace];
}

//GLenum OGLEnum::Convert(PolygonMode::Enum mode)
//{
//	static const GLenum enums[PolygonMode::SIZE] = {
//		GL_POINT,
//		GL_LINE,
//		GL_FILL
//	};
//
//	return enums[(uint32)mode];
//}

GLenum POGLEnum::Convert(POGLPrimitiveType::Enum e)
{
	static const GLenum enums[POGLPrimitiveType::COUNT] = {
		GL_POINTS,
		GL_TRIANGLES,
		GL_TRIANGLE_STRIP,
		GL_TRIANGLE_FAN,
		GL_LINES,
		GL_LINE_LOOP,
	};
	return enums[(POGL_UINT32)e];
}

//GLenum OGLEnum::Convert(TextureCubeSide::Enum e) {
//	static const GLenum textureTargets[TextureCubeSide::SIZE] = {
//		GL_TEXTURE_CUBE_MAP_POSITIVE_X,
//		GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
//		GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
//		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
//		GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
//		GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
//	};
//
//	return textureTargets[(uint32)e];
//}

GLenum POGLEnum::ConvertToTextureFormatEnum(POGLTextureFormat::Enum format)
{
	GLint _format = GL_RGBA;
	switch (format)
	{
	case POGLTextureFormat::RGB:
		_format = GL_RGB;
		break;
	case POGLTextureFormat::RGB8:
		_format = GL_RGB;
		break;
	case POGLTextureFormat::RGB12:
		_format = GL_RGB;
		break;
	case POGLTextureFormat::RGB16:
		_format = GL_RGB;
		break;
	case POGLTextureFormat::RGBA:
		break;
	case POGLTextureFormat::RGBA8:
		_format = GL_RGBA;
		break;
	case POGLTextureFormat::RGBA12:
		_format = GL_RGBA;
		break;
	case POGLTextureFormat::RGBA16:
		_format = GL_RGBA;
		break;
	case POGLTextureFormat::RGB10_A2:
		_format = GL_RGBA;
		break;
	case POGLTextureFormat::RGBA16F:
		_format = GL_RGBA;
		break;
	case POGLTextureFormat::RGBA32F:
		_format = GL_RGBA;
		break;
	case POGLTextureFormat::DEPTH24:
		_format = GL_DEPTH_COMPONENT;
		break;
	case POGLTextureFormat::DEPTH32F:
		_format = GL_DEPTH_COMPONENT;
		break;
	case POGLTextureFormat::DEPTH24_STENCIL8:
		_format = GL_DEPTH_STENCIL;
		break;
	case POGLTextureFormat::DEPTH32F_STENCIL8:
		_format = GL_DEPTH_STENCIL;
		break;
	case POGLTextureFormat::R:
		_format = GL_RED;
		break;
	case POGLTextureFormat::R16F:
		_format = GL_RED;
		break;
	case POGLTextureFormat::R32F:
		_format = GL_RED;
		break;
	case POGLTextureFormat::BGR:
		_format = GL_BGR;
		break;
	case POGLTextureFormat::BGRA:
		_format = GL_BGRA;
		break;
	}
	return _format;
}

GLenum POGLEnum::ConvertToInternalTextureFormatEnum(POGLTextureFormat::Enum format)
{
	GLint _internalFormat = GL_RGBA;
	switch (format)
	{
	case POGLTextureFormat::RGB:
		_internalFormat = GL_RGB;
		break;
	case POGLTextureFormat::RGB8:
		_internalFormat = GL_RGB8;
		break;
	case POGLTextureFormat::RGB12:
		_internalFormat = GL_RGB12;
		break;
	case POGLTextureFormat::RGB16:
		_internalFormat = GL_RGB16;
		break;
	case POGLTextureFormat::RGBA:
		break;
	case POGLTextureFormat::RGBA8:
		_internalFormat = GL_RGBA8;
		break;
	case POGLTextureFormat::RGBA12:
		_internalFormat = GL_RGBA12;
		break;
	case POGLTextureFormat::RGBA16:
		_internalFormat = GL_RGBA16;
		break;
	case POGLTextureFormat::RGB10_A2:
		_internalFormat = GL_RGB10_A2;
		break;
	case POGLTextureFormat::RGBA16F:
		_internalFormat = GL_RGBA16F;
		break;
	case POGLTextureFormat::RGBA32F:
		_internalFormat = GL_RGBA32F;
		break;
	case POGLTextureFormat::DEPTH24:
		_internalFormat = GL_DEPTH_COMPONENT24;
		break;
	case POGLTextureFormat::DEPTH32F:
		_internalFormat = GL_DEPTH_COMPONENT32F;
		break;
	case POGLTextureFormat::DEPTH24_STENCIL8:
		_internalFormat = GL_DEPTH24_STENCIL8;
		break;
	case POGLTextureFormat::DEPTH32F_STENCIL8:
		_internalFormat = GL_DEPTH32F_STENCIL8;
		break;
	case POGLTextureFormat::R:
		_internalFormat = GL_R8;
		break;
	case POGLTextureFormat::R16F:
		_internalFormat = GL_R16F;
		break;
	case POGLTextureFormat::R32F:
		_internalFormat = GL_R32F;
		break;
	case POGLTextureFormat::BGR:
		_internalFormat = GL_RGB;
		break;
	case POGLTextureFormat::BGRA:
		_internalFormat = GL_RGBA;
		break;
	}
	return _internalFormat;
}

POGL_UINT32 POGLEnum::VertexTypeSize(POGLVertexType::Enum e)
{
	static const POGL_UINT32 sizes[POGLVertexType::COUNT] = {
		sizeof(POGL_INT8),
		sizeof(POGL_UINT8),
		sizeof(POGL_INT16),
		sizeof(POGL_UINT16),
		sizeof(POGL_INT32),
		sizeof(POGL_UINT32),
		sizeof(POGL_FLOAT),
		sizeof(POGL_DOUBLE),
	};

	return sizes[(POGL_UINT32)e];
}

POGL_UINT32 POGLEnum::TextureFormatToSize(POGLTextureFormat::Enum e, const POGL_SIZE& size)
{
	POGL_UINT32 bitsPerPixel = 0;
	switch (e)
	{
	case POGLTextureFormat::RGB:
	case POGLTextureFormat::BGR:
		bitsPerPixel = 24;
		break;
	case POGLTextureFormat::RGBA:
	case POGLTextureFormat::BGRA:
		bitsPerPixel = 32;
		break;
	case POGLTextureFormat::R:
		bitsPerPixel = 8;
		break;
	default:
		THROW_EXCEPTION(POGLException, "Cannot calculate texture format size for: %d", e);
	}

	const POGL_UINT32 numComponents = bitsPerPixel / 8;
	return size.width * size.height * numComponents;
}
