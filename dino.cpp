#include <ctime>
#include "types.hpp"
#include "screen.hpp"
#include "lib.hpp"
#include "scene.hpp"

struct state {
    u8 seed;
    i8 speed;
    i16 height;
} __attribute__((packed));
static_assert(sizeof(state) <= sizeof(u64));

// jump comes back to the ground in .8s at 60fps
constexpr static auto gravity = -5;
constexpr static auto jumpspeed = 125;


std::pair<state, scene> makescene(const input& i, state state) {
    i8 speed = state.speed;
    i16 height = state.height;

    rng rng{state.seed};
    rng.advance(i.tick+12345); // ignore the first few values

    if (height == 0 && i.w)
        speed = jumpspeed;

    height += speed;

    if (height <= 0)
        height = speed = 0;
    else
        speed += gravity;

    scene s;
    s.dino.height = height;

    // auto hspeed = s.W / (FPS * 2); // birds/cactuses traverse the screen in 2s

    for (auto w = 0; w < s.W; w++) {
        auto r = rng.gen();
        if (i.tick + w < FPS * 3)
            continue; // first 3 seconds are peaceful

        if (r % 100 == 0)
            s.birds.push_back(w);
        if (r % 100 == 1)
            s.cactuses.push_back(w);
    }
    return {state, s};
}

int main() {
    state state{};
    state.seed = time(0);

    loadsprites();

    ttyscreen screen;
    input in;
    scene::H = screen.H;
    scene::W = screen.W;

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
