#pragma once
#include <string>
#include <vector>
#include "types.hpp"

#ifndef FPS
#define FPS 60
#endif

struct screen {
    int W{}, H{};
    std::vector<bool> fb;

    void clear();
    std::string draw() const;
};

struct ttyscreen : screen {
    ttyscreen();
    ~ttyscreen();

    void draw() const;
    void debug(const char *msg);
};

struct input {
    bool w{},a{},s{},d{};
    bool quit{};
    u32 tick{};

    void get();
};
