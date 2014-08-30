#include "config.h"
#include <memory>
#include <algorithm>

IPOGLTexture2D* POGLXLoadBMPImageFromFile(IPOGLRenderContext* context, const POGL_CHAR* fileName)
{
	assert_not_null(context);
	assert_not_null(fileName);

	// Open file
	FILE* file = open_file(fileName, POGL_TOCHAR("rb"));
	if (file == nullptr)
		THROW_EXCEPTION(POGLResourceException, "File not found: %s", fileName);

	fseek(file, 0L, SEEK_END);
	const POGL_UINT32 sz = ftell(file);
	fseek(file, 0L, SEEK_SET);

	std::shared_ptr<POGL_BYTE> bytes(new POGL_BYTE[sz], TPOGLDeleteArray<POGL_BYTE>());
	fread(bytes.get(), 1, sz, file);
	fclose(file);
	return POGLXLoadBMPImageFromMemory(context, bytes.get(), sz);
}

IPOGLTexture2D* POGLXLoadBMPImageFromMemory(IPOGLRenderContext* context, const POGL_BYTE* bytes, POGL_UINT32 size)
{
	assert_not_null(context);
	assert_not_null(bytes);
	assert_with_message(size > 0, "You cannot load a non-existing image");

	if (size < 54)
		THROW_EXCEPTION(POGLResourceException, "Invalid BMP file");

	if (bytes[0] != 'B' || bytes[1] != 'M')
		THROW_EXCEPTION(POGLResourceException, "Invalid BMP file");

	// Get num bits per pixel
	const POGL_UINT16 bitsPerPixel = bytes[28];

	// Verify 24 or 32 bit image type
	if (bitsPerPixel != 24 && bitsPerPixel != 32)
	{
		THROW_EXCEPTION(POGLResourceException, "Invalid File Format for file. 24 or 32 bit Image Required.");
	}

	// Data offset
	const POGL_UINT32 offset = bytes[10] + (bytes[11] << 8);

	// Read image size from header
	const POGL_SIZE imageSize(*(POGL_INT32*)&(bytes[0x12]), *(POGL_INT32*)&(bytes[0x16]));

	// Create a texture2D resource
	return context->CreateTexture2D(imageSize, POGLTextureFormat::BGR, &bytes[offset]);
}
