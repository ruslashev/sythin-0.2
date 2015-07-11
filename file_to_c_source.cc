// file_to_c_source - tool to embed binary files as C source code

#include <fstream>
#include <string>
#include <algorithm>
#include <cstdint>
#include "bzip2-1.0.6/bzlib.h"

bool openFile();
void writeBuffer(char *buffer, size_t size);

std::string filename;
char *buffer;
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

	buffer = new char[fileSize];
	size_t read = fread(buffer, 1, fileSize, f);
	fclose(f);
	if (read != fileSize) {
		puts("failed to read file");
		delete [] buffer;
		return 1;
	}

	char *compressed = new char [fileSize];
	unsigned int compSize = fileSize;
	int bzerror = BZ2_bzBuffToBuffCompress(
			compressed, &compSize,
			buffer, fileSize,
			9,
			0,
			250);
	if (bzerror != BZ_OK)
		return bzerror;

	writeBuffer(compressed, compSize);

	delete [] buffer;
	delete [] compressed;

	return 0;
}

void writeBuffer(char *buffer, size_t size)
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
	p("static const struct {");
	p("    unsigned int size;");
	p("    char data[%d*2+1];", size);
	p("} %s = {", struct_name.c_str());
	p("%d,", size);

	int counter = 0;
	fprintf(out, "\"");
	for (int i = 0; i < size; i++) {
		unsigned char d = buffer[i];
		fprintf(out, "%02x", d);
		counter++;
		if (counter >= 70) {
			p("\"");
			counter = 0;
			fprintf(out, "\"");
		}
	}

	p("\"\n};");
	p("");
	p("#endif");
	p("");
}

