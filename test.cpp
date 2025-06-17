#include <string>
#include "lib.hpp"
#include "scene.hpp"
#include "screen.hpp"
#include "sprite.hpp"
#include "test.hpp"

TEST("sprite/load single") {
    sprite dino("assets/dinorun1.pbm");

    if (dino.W != 20 || dino.H != 22)
        return FAIL;

    auto sum = 0;
    for (auto c : dino.data)
        sum += c;

    return sum == 186;
};

TEST("sprite/load all") {
    loadsprites();
    if (sprites.size() != 4)
        return FAIL;

    if (sprites["dinorun1"]->W != 20)
        return FAIL;

    return OK;
};

TEST("interactive/screen and keyboard detection") {
    screen s;
    input i;
    u32 limit = FPS * 5;
    while (i.tick < limit) {
        i.get();
        std::string msg = "press a wasd key within the next " \
                           + std::to_string(limit-i.tick) \
                           + " frames";
        s.debug(msg.data());
        if (i.w || i.a || i.s || i.d)
            return OK;
        if (i.quit)
            break;
    }
    return FAIL;
};

TEST("interactive/display image") {
    screen screen;
    input i;
    u32 limit = FPS * 5;;

    loadsprites();
    scene scene;
    scene.cactuses.push_back(4);
    scene.render(screen);

    drawsprite(screen, *sprites["bird1"], 10, 10);
    screen.draw();
    while (i.tick < limit) {
        i.get();
        std::string msg = "press a wasd key within the next " \
                           + std::to_string(limit-i.tick) \
                           + " frames";
        screen.debug(msg.data());
        if (i.w || i.a || i.s || i.d)
            return OK;
        if (i.quit)
            break;
    }

    return OK;
};

TEST("interactive/move bird") {
    screen screen;
    input i;

    loadsprites();
    const auto& bird = *sprites["bird1"];
    int w = 0, h = 0;
    while (1) {
        screen.clear();

        i.get();
        if (i.quit) break;
        if (i.w) h--;
        if (i.s) h++;
        if (i.a) w--;
        if (i.d) w++;

        drawsprite(screen, bird, w, h);
        screen.draw();
        std::string msg = "current coords: w=" + std::to_string(w) +
                          " h=" + std::to_string(h);
        screen.debug(msg.data());
    }
    return OK;
};

TEST("interactive/jump") {
    screen screen;
    input i;

    loadsprites();
    const auto& dino = *sprites["dinorun1"];
    int height = 0, speed = 0;
    while (1) {
        screen.clear();

        i.get();
        if (i.quit) break;

        auto gravity = -5;
        auto jumpspeed = 125;

        if (height == 0 && i.w)
            speed = jumpspeed;

        height += speed;

        if (height <= 0)
            height = speed = 0;
        else
            speed += gravity;

        auto h = screen.H - (height * screen.H/2 / 2500 + screen.H/2);
        drawsprite(screen, dino, 0, h);
        screen.draw();

        std::string msg = "height=" + std::to_string(height) +
                          " speed=" + std::to_string(speed);
        screen.debug(msg.data());
    }
    return OK;
};

TEST("lib/rng advance") {
    u64 seed = 0xf0cacc1af0cacc1a;

    rng rng;
    rng.state = seed;

    auto limit = 1000;
    u32 r0;
    for (auto i = 0; i < limit; i++)
        r0 = rng.gen();

    rng.state = seed;
    rng.advance(limit-1);
    auto r1 = rng.gen();
    if (r0 != r1)
        return FAIL;

    rng.state = seed;
    rng.advance(-1ull);
    if (rng.gen() != seed >> 32)
        return FAIL;

    return OK;
};


int main(int argc, char **argv) {
    return run_tests(argc, argv) ? 0 : 1;
}
