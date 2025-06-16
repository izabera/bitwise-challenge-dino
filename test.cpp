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

int main(int argc, char **argv) {
    return run_tests(argc, argv) ? 0 : 1;
}
