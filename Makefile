CXX = g++
SRCDIR = src
OBJS = $(patsubst $(SRCDIR)/%.cc, .objs/%.o, $(shell find $(SRCDIR) -type f -name '*.cc' ))
CXXFLAGS = -Wall -Wextra -Werror -g -std=c++0x
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system
EXECNAME = sythin2

all: objdir $(EXECNAME)
	./$(EXECNAME)

.objs/%.o: $(SRCDIR)/%.cc
	@echo "Compiling $<"
	@$(CXX) -c -o $@ $< $(CXXFLAGS)

$(EXECNAME): $(OBJS)
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

