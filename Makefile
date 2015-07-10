CXX = g++
SRCDIR = src
BZIP_SRCS = bzip2-1.0.6/bzlib.c bzip2-1.0.6/crctable.c bzip2-1.0.6/huffman.c bzip2-1.0.6/randtable.c bzip2-1.0.6/compress.c bzip2-1.0.6/decompress.c bzip2-1.0.6/blocksort.c
BZIP_OBJS = $(patsubst bzip2-1.0.6/%.c, .objs/%.o, $(BZIP_SRCS))
OBJS = $(patsubst $(SRCDIR)/%.cc, .objs/%.o, $(shell find $(SRCDIR) -type f -name '*.cc' ))
CXXFLAGS = -Wall -Wextra -Werror -g -std=c++0x
LDFLAGS = -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system
EXECNAME = sythin2
CC = gcc
CCFLAGS = -w -fpermissive

all: objdir $(EXECNAME)
	./$(EXECNAME)

.objs/%.o: $(SRCDIR)/%.cc
	@echo "Compiling $<"
	@$(CXX) -c -o $@ $< $(CXXFLAGS)

.objs/%.o: bzip2-1.0.6/%.c
	@echo "Compiling $<"
	@$(CC) -c -o $@ $< $(CCFLAGS)

$(EXECNAME): $(OBJS) $(BZIP_OBJS)
	@echo "Linking to $@"
	@$(CXX) -o $@ $^ $(LDFLAGS)

valgrind: objdir $(EXECNAME)
	valgrind --leak-check=full ./$(EXECNAME)

callgrind: objdir $(EXECNAME)
	@valgrind --tool=callgrind ./$(EXECNAME)
	@kcachegrind callgrind.out.$!

objdir:
	@mkdir -p .objs

clean:
	rm -f $(EXECNAME)
	rm -rf $(OBJS)

ftcc:
	g++ file_to_c_source.cc -std=c++0x -o ftcc
	./ftcc *.ttf > src/font.hh
	rm ftcc

