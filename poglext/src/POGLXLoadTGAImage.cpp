#include "config.h"
#include <memory>
#include <algorithm>

#define POGL_SWAP(a, b) a ^= b ^= a ^=b

void POGLSwap24(unsigned char* left, unsigned char* right)
{
	while (left < right) {
		unsigned char* l_r = left++;
		unsigned char* l_g = left++;
		unsigned char* l_b = left++;

		unsigned char* r_b = right--;
		unsigned char* r_g = right--;
		unsigned char* r_r = right--;

		POGL_SWAP(*l_r, *r_r);
		POGL_SWAP(*l_g, *r_g);
		POGL_SWAP(*l_b, *r_b);
	}
}

void POGLSwap32(unsigned char* left, unsigned char* right)
{
	while (left < right) {
		unsigned char* l_r = left++;
		unsigned char* l_g = left++;
		unsigned char* l_b = left++;
		unsigned char* l_a = left++;

		unsigned char* r_a = right--;
		unsigned char* r_b = right--;
		unsigned char* r_g = right--;
		unsigned char* r_r = right--;

		POGL_SWAP(*l_r, *r_r);
		POGL_SWAP(*l_g, *r_g);
		POGL_SWAP(*l_b, *r_b);
		POGL_SWAP(*l_a, *r_a);
	}
}

IPOGLTexture2D* POGLXLoadCompressedTGAImageFromMemory(IPOGLRenderContext* context, POGL_BYTE* bytes)
{
	const POGL_SIZE imageSize(bytes[1] * 256 + bytes[0], bytes[3] * 256 + bytes[2]);
	const POGL_UINT32 bpp = (POGL_UINT32)bytes[4];
	const POGLTextureFormat::Enum textureFormat = bpp == 24 ? POGLTextureFormat::BGR : POGLTextureFormat::BGRA;
	const POGL_BYTE* imageData = bytes + 6;

	if (imageSize.width <= 0 || imageSize.height <= 0 || (bpp != 24 && bpp != 32)) {
		THROW_EXCEPTION(POGLResourceException, "Invalid TGA texture information");
	}

	const POGL_UINT32 bytesPerPixel = bpp / 8;
	const POGL_UINT32 imageDataSize = imageSize.width * imageSize.height * bytesPerPixel;
	std::shared_ptr<unsigned char> _imageDataBytes(new unsigned char[imageDataSize], TPOGLDeleteArray<unsigned char>());
	unsigned char* imageDataBytes = _imageDataBytes.get();
	unsigned char colorBuffer[4] = { 0, 0, 0, 0 };

	const POGL_UINT32 pixelCount = imageSize.width * imageSize.height;
	POGL_UINT32 currentPixel = 0;
	POGL_UINT32 currentByte = 0;
	const POGL_BYTE* ptr = imageData;
	do {
		unsigned char chunkHeader = *ptr++;
		if (chunkHeader < 128) {
			chunkHeader++;
			for (unsigned char counter = 0; counter < chunkHeader; ++counter) {
				colorBuffer[0] = *ptr++;
				colorBuffer[1] = *ptr++;
				colorBuffer[2] = *ptr++;
				if (bpp == 32)
					colorBuffer[3] = *ptr++;

				imageDataBytes[currentByte++] = colorBuffer[0];
				imageDataBytes[currentByte++] = colorBuffer[1];
				imageDataBytes[currentByte++] = colorBuffer[2];
				if (bpp == 32)
					imageDataBytes[currentByte++] = colorBuffer[3];
				currentPixel++;
				if (currentPixel > pixelCount)
					THROW_EXCEPTION(POGLResourceException, "Too many pixels read");
			}
		}
		else {
			chunkHeader -= 127;
			colorBuffer[0] = *ptr++;
			colorBuffer[1] = *ptr++;
			colorBuffer[2] = *ptr++;
			if (bpp == 32)
				colorBuffer[3] = *ptr++;

			for (unsigned char counter = 0; counter < chunkHeader; ++counter) {
				imageDataBytes[currentByte++] = colorBuffer[0];
				imageDataBytes[currentByte++] = colorBuffer[1];
				imageDataBytes[currentByte++] = colorBuffer[2];
				if (bpp == 32)
					imageDataBytes[currentByte++] = colorBuffer[3];
				currentPixel++;
				if (currentPixel > pixelCount)
					THROW_EXCEPTION(POGLResourceException, "Too many pixels read");
			}
		}
	} while (currentPixel < pixelCount);

	const POGL_UINT32 orientation = (POGL_UINT32)bytes[5] & 0x30;
	if (orientation == 0x20) {
		THROW_NOT_IMPLEMENTED_EXCEPTION();
	}
	else if (orientation == 0) {
		for (POGL_INT32 h = 0; h < imageSize.height; ++h) {
			unsigned char* left = &imageDataBytes[imageSize.width * h * bytesPerPixel];
			unsigned char* right = &imageDataBytes[imageSize.width * h * bytesPerPixel + (imageSize.height * bytesPerPixel - 1)];
			if (bpp == 24) {
				POGLSwap24(left, right);
			}
			else if (bpp == 32) {
				POGLSwap32(left, right);
			}
		}
	}

	return context->CreateTexture2D(imageSize, textureFormat, imageDataBytes);
}

IPOGLTexture2D* POGLXLoadUncompressedTGAImageFromMemory(IPOGLRenderContext* context, POGL_BYTE* bytes)
{
	const POGL_SIZE imageSize(bytes[1] * 256 + bytes[0], bytes[3] * 256 + bytes[2]);
	const POGL_UINT32 bpp = (POGL_UINT32)bytes[4];
	const POGL_UINT32 bytesPerPixel = bpp / 8;
	const POGLTextureFormat::Enum textureFormat = bpp == 24 ? POGLTextureFormat::BGR : POGLTextureFormat::BGRA;
	POGL_BYTE* imageData = bytes + 6;

	if (imageSize.width <= 0 || imageSize.height <= 0 || (bpp != 24 && bpp != 32)) {
		THROW_EXCEPTION(POGLResourceException, "Invalid TGA texture information");
	}

	const POGL_UINT32 orientation = (POGL_UINT32)bytes[5] & 0x30;
	if (orientation == 0x20) {
		THROW_NOT_IMPLEMENTED_EXCEPTION();
	}
	else if (orientation == 0) {
		for (POGL_INT32 h = 0; h < imageSize.height; ++h) {
			unsigned char* left = &imageData[imageSize.width * h * bytesPerPixel];
			unsigned char* right = &imageData[imageSize.width * h * bytesPerPixel + (imageSize.height * bytesPerPixel - 1)];
			if (bpp == 24) {
				POGLSwap24(left, right);
			}
			else if (bpp == 32) {
				POGLSwap32(left, right);
			}
		}
	}

	return context->CreateTexture2D(imageSize, textureFormat, imageData);
}

IPOGLTexture2D* POGLXLoadInternalTGAImageFromMemory(IPOGLRenderContext* context, POGL_BYTE* bytes, POGL_UINT32 size)
{
	if (context == nullptr)
		THROW_EXCEPTION(POGLResourceException, "You must supply a valid render context");

	if (bytes == nullptr || size <= 18)
		THROW_EXCEPTION(POGLResourceException, "You must supply valid image data");

	// Read header
	POGL_BYTE* header = bytes;

	// If the file is not compressed
	static char UNCOMPRESSED[12] = { 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	// If the file is compressed
	static char ISCOMPRESSED[12] = { 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	// Move the pointer beyond the header
	POGL_BYTE* data = bytes + 12;

	// Load compressed- or uncompressed TGA file
	if (std::memcmp(UNCOMPRESSED, header, sizeof(UNCOMPRESSED)) == 0) {
		return POGLXLoadUncompressedTGAImageFromMemory(context, data);
	}
	else if (std::memcmp(ISCOMPRESSED, header, sizeof(ISCOMPRESSED)) == 0) {
		return POGLXLoadCompressedTGAImageFromMemory(context, data);
	}

	THROW_EXCEPTION(POGLResourceException, "Unsupported TGA image");
}

/*!
	\see http://nehe.gamedev.net/tutorial/loading_compressed_and_uncompressed_tga's/22001/
*/
IPOGLTexture2D* POGLXLoadTGAImageFromFile(IPOGLRenderContext* context, const POGL_CHAR* fileName)
{
	if (context == nullptr)
		THROW_EXCEPTION(POGLResourceException, "You must supply a valid render context");

	if (fileName == nullptr)
		THROW_EXCEPTION(POGLResourceException, "You must supply a valid filename");

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
	return POGLXLoadTGAImageFromMemory(context, bytes.get(), sz);
}

IPOGLTexture2D* POGLXLoadTGAImageFromMemory(IPOGLRenderContext* context, const POGL_BYTE* bytes, POGL_UINT32 size)
{
	if (context == nullptr)
		THROW_EXCEPTION(POGLResourceException, "You must supply a valid render context");

	if (bytes == nullptr || size <= 18)
		THROW_EXCEPTION(POGLResourceException, "You must supply valid image data");

	std::shared_ptr<POGL_BYTE> copiedBytes(new POGL_BYTE[size], TPOGLDeleteArray<POGL_BYTE>());
	memcpy(copiedBytes.get(), bytes, size);
	return POGLXLoadInternalTGAImageFromMemory(context, copiedBytes.get(), size);
}
