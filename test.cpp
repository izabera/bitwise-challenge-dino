#include <string>
#include "dino.hpp"
#include "lib.hpp"
#include "scene.hpp"
#include "screen.hpp"
#include "sprite.hpp"
#include "test.hpp"

using namespace std::string_literals;

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

TEST("screen/draw sprite") {
    sprite square;
    square.H = 4;
    square.W = 4;
    square.data.resize(square.H*square.W, true);

    screen screen;
    screen.W = 8;
    screen.H = 4;
    screen.fb.resize(screen.W*screen.H);

    auto header = "\x1b[H"s;
    auto nl = "\r\n"s;
    auto emptychar = " "s, fullchar = "█"s;
    auto emptyline = ""s, fullline = ""s, halfline = ""s;
    for (auto i = 0; i < screen.W; i++) {
        emptyline += emptychar;
        fullline += fullchar;
        halfline += i < screen.W / 2 ? fullchar : emptychar;
    }

    // drawing on an empty screen can't collide
    if (drawsprite(screen, square, 0, 0) == true)
        return FAIL;

    //puts("empty screen didn't collide");
    auto drawn = screen.draw();
    auto expected = header + halfline + nl + halfline + nl;

    //#include <cctype>
    //auto dump = [](const auto& s) {
    //    for (auto c : s)
    //        printf("%02x ", (u8)c);
    //    puts("");
    //    for (auto c : s) {
    //        char str[3] {};
    //        str[0] = c;
    //        printf("%2s ", std::isgraph(c) ? str :
    //                       c == ' ' ? "SP" :
    //                       c == '\r' ? "\\r" :
    //                       c == '\n' ? "\\n" : ".");
    //    }
    //    puts("");
    //};
    //puts("drawn:");
    //dump(drawn);
    //puts("expected:");
    //dump(expected);

    if (drawn != expected)
        return FAIL;

    // drawing over the previous sprite must collide
    if (drawsprite(screen, square, 0, 0) == false)
        return FAIL;

    // and it doesn't change the content of the screen
    drawn = screen.draw();
    if (drawn != expected)
        return FAIL;

    // drawing on the other side must not collide
    if (drawsprite(screen, square, 4, 0) == true)
        return FAIL;
    drawn = screen.draw();
    expected = header + fullline + nl + fullline + nl;
    if (drawn != expected)
        return FAIL;

    // drawing out of bounds can't collide
    if (drawsprite(screen, square, 10, 10) == true)
        return FAIL;

    return OK;
};

TEST("interactive/screen and keyboard detection") {
    ttyscreen s;
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
    ttyscreen screen;
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
    ttyscreen screen;
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
    ttyscreen screen;
    input i;

    loadsprites();
    const auto& dino = *sprites["dinorun1"];

    state state;
    while (1) {
        screen.clear();
        i.get();
        if (i.quit) break;

        scene s;
        if ((state.jumpframe == 0 && i.w) || state.jumpframe)
            s.dino.height = jump(++state.jumpframe);

        if (s.dino.height == 0)
            state.jumpframe = 0;

        auto h = screen.H*3/4 - (s.dino.height * screen.H/2 / 2500) - dino.H;
        drawsprite(screen, dino, screen.W/10-dino.W, h);
        screen.draw();

        std::string msg = "height=" + std::to_string(s.dino.height);
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

TEST("dino/jumps last .85s") {
    int t = 0;
    while (jump(++t)) ;
    return t == 51;
};

TEST("dino/jumps follow gravity") {
    constexpr static auto gravity = -5;
    constexpr static auto jumpspeed = 125;

    int g_height = 0, speed = jumpspeed;

    int t = 0;
    do {
        int j_height = jump(++t);

        g_height += speed;
        if (g_height != j_height)
            return FAIL;

        if (g_height <= 0)
            break;
        else
            speed += gravity;
    } while (1);

    return OK;
};

int main(int argc, char **argv) {
    return run_tests(argc, argv) ? 0 : 1;
}
