#pragma once
#include <vector>
#include "screen.hpp"

struct scene {
    // horizontal position
    std::vector<int> birds;
    std::vector<int> cactuses;

    struct {
        int height;
        int anim_frame;
        enum { stand, duck } state;
    } dino{};

    void render(screen& s);
};

// loads everything from the asset directory
void loadsprites();
