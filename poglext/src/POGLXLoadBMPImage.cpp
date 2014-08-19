#include "config.h"
#include <memory>
#include <algorithm>

////////////////////////
/*!
	\brief Custom delete struct used with std::shared_ptr to delete arrays

	Example:
	{@code
	std::shared_ptr<byte> bytes(new byte[10], TDeleteArray<byte>());
	}
	*/
template<typename T>
struct TDeleteArray
{
	void operator()(T const * p) {
		delete[] p;
	}
};

IPOGLTexture2D* POGLXLoadBMPImageFromFile(IPOGLDeviceContext* context, const POGL_CHAR* fileName)
{
	assert_not_null(context);
	assert_not_null(fileName);

	// Open file
	FILE* file = open_file(fileName, POGL_TOCHAR("rb"));
	if (file == nullptr)
		THROW_EXCEPTION(POGLResourceException, "File not found: %s", fileName);

	fseek(file, 0L, SEEK_END);
	POGL_UINT32 sz = ftell(file);
	fseek(file, 0L, SEEK_SET);

	std::shared_ptr<POGL_CHAR> bytes(new POGL_CHAR[sz], TDeleteArray<POGL_CHAR>());
	fread(bytes.get(), 1, sz, file);
	fclose(file);
	return POGLXLoadBMPImageFromMemory(context, bytes.get(), sz);
}

IPOGLTexture2D* POGLXLoadBMPImageFromMemory(IPOGLDeviceContext* context, const POGL_CHAR* bytes, POGL_UINT32 size)
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
