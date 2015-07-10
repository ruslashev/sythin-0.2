/* file_to_c_source - tool to embed binary files as C source code
 */

#include <fstream>
#include <string>
#include <algorithm>
#include "bzip2-1.0.6/bzlib.h"

bool openFile();
void writeBuffer();

std::string filename;
unsigned char *buffer;
size_t fileSize;

int main(int argc, char **argv)
{
	if (argc < 2) {
		printf("Usage: %s <file>\n", argv[0]);
		printf("Prints resulting code to stdout\n");
		return 1;
	}

	filename = argv[1];

	FILE *f = fopen(filename.c_str(), "rb");
	if (!f) {
		puts("Failed to open file for reading");
		return 1;
	}

	fseek(f, 0, SEEK_END);
	fileSize = ftell(f);
	rewind(f);

	buffer = new unsigned char[fileSize];
	size_t read = fread(buffer, 1, fileSize, f);
	if (read != fileSize) {
		puts("failed to read file");
		delete [] buffer;
		fclose(f);
		return 1;
	}

	BZFILE *bzf;
	int bzerror;
	bzf = BZ2_bzReadOpen(&bzerror, f, 0, 0, NULL, 0);
	if (bzerror != BZ_OK) {
		fprintf(stderr, "E: BZ2_bzReadOpen: %d\n", bzerror);
		delete [] buffer;
		fclose(f);
		return 1;
	}

	char buf[2 * 1024 * 1024];
	while (bzerror == BZ_OK) {
		int nread = BZ2_bzRead(&bzerror, bzf, buf, sizeof(buf));
		if (bzerror == BZ_OK || bzerror == BZ_STREAM_END) {
			size_t nwritten = fwrite(buf, 1, nread, stdout);
			if (nwritten != (size_t) nread) {
				fprintf(stderr, "E: short write\n");
				return 1;
				delete [] buffer;
				fclose(f);
			}
		}
	}

	if (bzerror != BZ_STREAM_END) {
		fprintf(stderr, "E: bzip error after read: %d\n", bzerror);
		return -1;
	}

	BZ2_bzReadClose(&bzerror, bzf);

	writeBuffer();

	delete [] buffer;
	return 0;
}

void writeBuffer()
{
	std::string struct_name = "_";
	for (auto &c : filename)
		if (c == '.' || c == '_')
			struct_name += '_';
		else if
			((c >= '0' && c <= '9') ||
			 (c >= 'A' && c <= 'Z') ||
			 (c >= 'a' && c <= 'z'))
				struct_name += c;
	// struct_name += "_data";

	std::string header_guard = struct_name;
	for (auto &c : header_guard)
		c = toupper(c);
	header_guard += "_HH";

	FILE *out = stdout;
#define p(...) fprintf(out, __VA_ARGS__); fprintf(out, "\n")
	p("// Binary file embedded as C source using file_to_c_source.cc");
	p("#ifndef %s", header_guard.c_str());
	p("#define %s", header_guard.c_str());
	p("");
	p("#include <cstdint>");
	p("");
	p("const struct {");
	p("    unsigned int size;");
	p("    uint32_t data[%d/4];", fileSize);
	p("} %s = {", struct_name.c_str());
	p("%d,", fileSize);

	int counter = 0;
	for (int i = 0; i < fileSize; i += 4) {
		uint32_t d = *(uint32_t*)(buffer + i);
		fprintf(out, "0x%08x,", d);
		counter++;
		if (counter > 10) {
			p("");
			counter = 0;
		}
	}

	p("};");
	p("");
	p("#endif");
}

