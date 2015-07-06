#include <fstream>
#include <string>
#include <algorithm>

int main(int argc, char **argv)
{
	if (argc < 2) {
		printf("Usage: %s <file>\n", argv[0]);
		return 1;
	}

	std::string filename = argv[1];
	FILE *f = fopen(filename.c_str(), "rb");
	if (!f) {
		puts("failed to open file for reading");
		return 1;
	}

	fseek(f, 0, SEEK_END);
	long fileSize = ftell(f);
	rewind(f);

	char *buffer = new char[fileSize];
	size_t read = fread(buffer, 1, fileSize, f);
	fclose(f);
	if (read != fileSize) {
		puts("failed to read file");
		delete [] buffer;
		return 1;
	}

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
	p("const struct {");
	p("    unsigned int size;");
	p("    unsigned char data[%d];", fileSize);
	p("} %s = {", struct_name.c_str());
	p("%d,", fileSize);

	std::string linebuf = "\"";
	for (int i = 0; i < fileSize; i++) {
		const char b = buffer[i];
		linebuf += '\\';
		linebuf += '0' + ((b >> 6) & 7);
		linebuf += '0' + ((b >> 3) & 7);
		linebuf += '0' + (b & 7);
		if (linebuf.length() >= 80) {
			p("%s\"", linebuf.c_str());
			linebuf = "\"";
		}
	}
	p("};");
	p("");
	p("#endif");

	delete [] buffer;
	return 0;
}

