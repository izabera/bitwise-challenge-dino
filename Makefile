CXX = clang++
CXXFLAGS = -ggdb3 -Wall -Wextra
CPPFLAGS += -MP -MMD
LINK.o := $(CXX) $(LDFLAGS)

test: test.o screen.o

dino: dino.o screen.o

-include *.d

.PHONY: clean

clean:
	rm -f *.[do] test dino
