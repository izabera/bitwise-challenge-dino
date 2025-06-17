CXX = clang++
CXXFLAGS = -ggdb3 -Wall -Wextra
CPPFLAGS += -MP -MMD -Iengine/include
LINK.o := $(CXX) $(LDFLAGS)

engine = engine/scene.o engine/screen.o

impl = dino.o

test: $(impl) test.o $(engine)

dino: gameloop.o $(impl) $(engine)

-include *.d

.PHONY: clean

clean:
	find -name '*.[do]' -exec rm {} +
	rm -f test dino
