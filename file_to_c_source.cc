#include <fstream>
#include <string>
#include <algorithm>

bool openFile();
void writeBuffer();

std::string filename;
unsigned char *buffer;
size_t fileSize;

int main(int argc, char **argv)
{
	if (argc < 2) {
		printf("Usage: %s <file>\n", argv[0]);
		return 1;
	}

	filename = argv[1];

	if (!openFile())
		return 1;

	writeBuffer();

	delete [] buffer;
	return 0;
}

bool openFile() {
	FILE *f = fopen(filename.c_str(), "rb");
	if (!f) {
		puts("failed to open file for reading");
		return false;
	}

	fseek(f, 0, SEEK_END);
	fileSize = ftell(f);
	rewind(f);

	buffer = new unsigned char[fileSize+4];
	size_t read = fread(buffer, 1, fileSize, f);
	fclose(f);
	if (read != fileSize) {
		puts("failed to read file");
		delete [] buffer;
		return false;
	}
	return true;
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

