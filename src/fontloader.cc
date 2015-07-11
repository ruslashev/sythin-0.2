#include "fontloader.hh"
#include "constants.hh"

#include "../bzip2-1.0.6/bzlib.h"
#include <memory>

void printBzipError(int bzerror);

bool loadEmbeddedFont(sf::Font *font, std::unique_ptr<char> *uncompBuffer,
		const char *compData, unsigned int size)
{
	std::unique_ptr<char> compressedBuffer(new char [size]);
	int compressedBufferCounter = 0;
	for (size_t i = 0; i < size*2; i += 2) {
		const char byte[3] = { compData[i], compData[i+1], '\0' };
		compressedBuffer.get()[compressedBufferCounter++] =
			strtol(byte, NULL, 16);
	}

	unsigned int uncompSize = Constants.fontSizeGuess;
	*uncompBuffer = std::unique_ptr<char>(new char [uncompSize]);
	int bzerror = BZ2_bzBuffToBuffDecompress(
			uncompBuffer->get(), &uncompSize,
			compressedBuffer.get(), size,
			0, 0);
	if (bzerror != BZ_OK) {
		printBzipError(bzerror);
		return false;
	}

	if (!font->loadFromMemory(uncompBuffer->get(), uncompSize))
		return false;

	return true;
}

void printBzipError(int bzerror)
{
	puts("Error at decompressing font data:");
	switch (bzerror) {
		case BZ_CONFIG_ERROR:
			puts("The library has been mis-compiled");
			break;
		case BZ_PARAM_ERROR:
			puts("Parameter error at BZ2_bzBuffToBuffDecompress");
			break;
		case BZ_MEM_ERROR:
			puts("Insufficient memory available") ;
			break;
		case BZ_OUTBUFF_FULL:
			puts("The size of the compressed data exceeds *destLen");
			break;
		case BZ_DATA_ERROR:
			puts("A data integrity error was detected in the compressed data");
			break;
		case BZ_DATA_ERROR_MAGIC:
			puts("The compressed data doesn't begin with the right bytes");
			break;
		case BZ_UNEXPECTED_EOF:
			puts("The compressed data ends unexpectedly");
			break;
		default:
			puts("Unhandled error code");
	}
}

