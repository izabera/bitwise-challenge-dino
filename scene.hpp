#pragma once
#include <vector>
#include "screen.hpp"

struct scene {
    std::vector<int> birds;
    std::vector<int> cactuses;

    struct {
        int height;
        int frame;
        enum { stand, duck } state;
    } dino;

    void render(screen& s);
};

