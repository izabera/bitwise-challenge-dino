#include <ctime>
#include "types.hpp"
#include "screen.hpp"
#include "scene.hpp"

struct gamestate {
    u8 seed;
    i8 speed;
    i16 height;
} __attribute__((packed));
static_assert(sizeof(gamestate) <= sizeof(u64));

std::pair<gamestate, scene> makescene(const input& i, gamestate state) {
    u8 seed = state.seed;
    i8 speed = state.speed;
    i16 height = state.height;

    if (height == 0 && i.w)
        speed = 126;

    height += speed;

    if (height <= 0)
        height = speed = 0;
    else
        speed -= 9;

    scene s;

    return {state, s};
}

int main() {
    gamestate state{};
    state.seed = time(0);

    screen screen;
    input in;

    while (1) {
        in.get();
        if (in.quit)
            break;

        auto [st, scene] = makescene(in, state);
        state = st;

        screen.clear();
        scene.render(screen);
        screen.draw();
    }
}
