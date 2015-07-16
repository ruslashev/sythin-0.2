#include "fontloader.hh"
#include "constants.hh"

#include "../bzip2-1.0.6/bzlib.h"
#include <memory>

void printBzipError(int bzerror);

bool loadFontToLibs(sf::Font &sfFont, ImFont *&imFont,
		std::unique_ptr<char> *uncompBuffer, unsigned int *uncompSize)
{
	if (!sfFont.loadFromMemory(uncompBuffer->get(), *uncompSize))
		return false;

	char *uncompBufferCopy = new char [*uncompSize];
	for (unsigned int i = 0; i < *uncompSize; i++)
		uncompBufferCopy[i] = uncompBuffer->get()[i];

	imFont = ImGui::GetIO().Fonts->AddFontFromMemoryTTF(uncompBufferCopy,
			*uncompSize,
			Constants.gui.fontSize);

	return true;
}

// Warning: while trying to understand the pointers,
// you may pull your hair out.
// To understand easier, note the following usage:
//  * everything passed to the function that is going to be modified
//    is passed as a reference, even if it is a pointer.
//    - these are: sfFont, imFont and uncompBuffer
//  * if it is not going to be modified, but to avoid copying, pointers
//    are used
//    - this is: compData
bool FontLoader::loadEmbeddedFont(sf::Font &sfFont, ImFont *&imFont,
		std::unique_ptr<char> &uncompBuffer,
		const char *compData, unsigned int compSize)
{
	char *compressedBuffer = new char [compSize];
	int compressedBufferCounter = 0;
	for (size_t i = 0; i < compSize*2; i += 2) {
		const char byte[3] = { compData[i], compData[i+1], '\0' };
		compressedBuffer[compressedBufferCounter++] =
			strtol(byte, nullptr, 16);
	}

	unsigned int uncompSize = Constants.fontSizeGuess;
	uncompBuffer = std::unique_ptr<char>(new char [uncompSize]);
	int bzerror = BZ2_bzBuffToBuffDecompress(
			uncompBuffer.get(), &uncompSize,
			compressedBuffer, compSize,
			0, 0);
	if (bzerror != BZ_OK) {
		printBzipError(bzerror);
		return false;
	}

	delete [] compressedBuffer;

	return loadFontToLibs(sfFont, imFont, &uncompBuffer, &uncompSize);
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

