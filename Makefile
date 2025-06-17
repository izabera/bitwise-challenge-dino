CXX = clang++
CXXFLAGS = -ggdb3 -Wall -Wextra
CPPFLAGS += -MP -MMD -Iengine/include
LINK.o := $(CXX) $(LDFLAGS)

engine = engine/scene.o engine/screen.o

test: test.o $(engine)

dino: dino.o $(engine)

-include *.d

.PHONY: clean

clean:
	find -name '*.[do]' -exec rm {} +
	rm -f test dino
