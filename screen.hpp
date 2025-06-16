#pragma once

#include <vector>
#include "types.hpp"

#ifndef FPS
#define FPS 60
#endif

// we only need netbpm P1
struct sprite {
    int W{}, H{};
    std::vector<bool> data;
    sprite(const char *filename);
    sprite();
};

struct entity {
    sprite s;
    int W{}, H{};
};

struct screen {
    int W{}, H{};
    std::vector<entity> objects;

    screen();
    ~screen();

    void clear();
    void drawframe();
    void debugmsg(const char *);
};

struct input {
    bool w{},a{},s{},d{};
    bool quit{};
    u32 tick{};

    void get();
};
