CXX = g++
SRCDIR = src
OBJS = $(patsubst $(SRCDIR)/%.cpp, .objs/%.o, $(shell find $(SRCDIR) -type f -name '*.cpp' ))
CXXFLAGS = -Wall -Wextra -Werror -g -std=c++0x
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system
EXECNAME = sythin2

all: objdir $(EXECNAME)
	./$(EXECNAME)

.objs/%.o: $(SRCDIR)/%.cpp
	@$(CXX) -c -o $@ $< $(CXXFLAGS)
	@echo "Compiling $<"

$(EXECNAME): $(OBJS)
	@$(CXX) -o $@ $^ $(LDFLAGS)
	@echo "Linking to $@"

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

