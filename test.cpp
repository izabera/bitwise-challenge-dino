#include <string>
#include "screen.hpp"
#include "test.hpp"

TEST("load sprite") {
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
        std::string msg = "press a wasd key within the next " + std::to_string(limit-i.tick) + " frames";
        s.debugmsg(msg.data());
        if (i.w || i.a || i.s || i.d)
            return OK;
        if (i.quit)
            break;
    }
    return FAIL;
};

int main(int argc, char **argv) {
    return run_tests(argc, argv) ? 0 : 1;
}
