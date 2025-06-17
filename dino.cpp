#include <ctime>
#include "dino.hpp"
#include "lib.hpp"
#include "scene.hpp"
#include "screen.hpp"
#include "types.hpp"

u64 jump(u64 t) {
    return 5 * t * (51 - t) / 2;
}


std::pair<state, scene> makescene(const input& i, state state) {
    scene s;
    if ((state.jumpframe == 0 && i.w) || state.jumpframe)
        s.dino.height = jump(++state.jumpframe);

    if (s.dino.height == 0)
        state.jumpframe = 0;

    // auto hspeed = s.W / (FPS * 2); // birds/cactuses traverse the screen in 2s

    rng rng{state.seed};
    rng.advance(i.tick+12345); // ignore the first few values

    for (auto w = -100; w < i.W; w++) {
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

//int main() {
//    state state{};
//    state.seed = time(0);
//
//    loadsprites();
//
//    ttyscreen screen;
//    input in;
//    scene::H = screen.H;
//    scene::W = screen.W;
//
//    while (1) {
//        in.get();
//        if (in.quit)
//            break;
//
//        auto [st, scene] = makescene(in, state);
//        state = st;
//
//        screen.clear();
//        scene.render(screen);
//        screen.draw();
//    }
//}
