#include "fontloader.hh"
#include "constants.hh"

#include "../bzip2-1.0.6/bzlib.h"
#include <memory>

void printBzipError(int bzerror);

// SFML requires pointed buffer to be kept alive as long as program is
// running and will not deallocate it.
// ImGui takes ownership of the buffer and will free it by itself

// * everything passed to the function that is going to be modified
//   is passed as a reference, even if it is a pointer.
//   - these are: sfFont, imFont* and uncompBuffer
// * if it is not going to be modified, but to avoid copying, pointers
//   are used
//   - this is: compData

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

	uncompBuffer = std::unique_ptr<char>(new char [Constants.fontSizeGuess]);
	unsigned int uncompSize = Constants.fontSizeGuess;
	int bzerror = BZ2_bzBuffToBuffDecompress(
			uncompBuffer.get(), &uncompSize,
			compressedBuffer, compSize,
			0, 0);
	if (bzerror != BZ_OK) {
		printBzipError(bzerror);
		return false;
	}

	delete [] compressedBuffer;

	if (!sfFont.loadFromMemory(uncompBuffer.get(), uncompSize))
		return false;

	ImFontConfig config;
	config.FontDataOwnedByAtlas = false;
	config.OversampleH = 8;
	config.OversampleV = 8;
	imFont = ImGui::GetIO().Fonts->AddFontFromMemoryTTF(uncompBuffer.get(),
			uncompSize, Constants.gui.fontSize, &config);

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

