#include <vector>
#include "types.hpp"
#include "screen.hpp"

struct gamestate {
    i16 height;
    i8 speed;
    u8 seed;
} __attribute__((packed));
static_assert(sizeof(gamestate) <= sizeof(u64));

struct scene {
    std::vector<int> birds;
    std::vector<int> cactuses;
    int dinoheight;
    int dinoframe;
    int dinostate;
    void render(screen& s) {
    }
};


// takes keyboard input, game state and a (cleared) frame
// returns the new game state
u64 makeframe(const input& i, u64 state, screen& output) {
    u16 seed = state & 0xffff;
    i8 speed = (state >> 16) & 0xff;
    i16 height = (state >> 24) & 0xffff;

    if (height == 0 && i.w)
        speed = 126;

    height += speed;

    if (height <= 0)
        height = speed = 0;
    else
        speed -= 10;

    return seed | u64(u8(speed)) << 16 | u64(u16(height)) << 24;
}

int main() {
    uint64_t state = 0;

    screen scr;
    input in;

    while (1) {
        in.get();

        scr.clear();
        state = makeframe(in, state, scr);
        scr.drawframe();
    }
}
