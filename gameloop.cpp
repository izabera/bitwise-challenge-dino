#include <ctime>
#include "screen.hpp"
#include "scene.hpp"
#include "dino.hpp"

int main() {
    state state{};
    state.seed = time(0);

    loadsprites();

    ttyscreen screen;
    input in{};
    in.W = screen.W;
    in.H = screen.H;

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
