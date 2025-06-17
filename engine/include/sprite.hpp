#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

// we only support netbpm P1
struct sprite {
    int W{}, H{};
    std::vector<bool> data;
    sprite();
    sprite(const std::string& filename);
};

extern std::unordered_map<std::string, std::unique_ptr<sprite>> sprites;

struct screen;
bool drawsprite(screen& scr, const sprite& spr, int W, int H);
