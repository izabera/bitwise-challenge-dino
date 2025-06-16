#include <string>
#include "lib.hpp"
#include "screen.hpp"
#include "test.hpp"

TEST("sprite/load") {
    sprite dino("./dinorun1.pbm");

    if (dino.W != 20 || dino.H != 22)
        return FAIL;

    auto sum = 0;
    for (auto c : dino.data)
        sum += c;

    return sum == 186;
};

TEST("interactive/screen and keyboard detection") {
    screen s;
    input i;
    u32 limit = 1000;
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
