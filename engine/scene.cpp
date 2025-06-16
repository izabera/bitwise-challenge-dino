#include <filesystem>
#include <fstream>
#include <string>
#include <unordered_map>
#include "scene.hpp"
#include "sprite.hpp"

using namespace std::string_literals;

std::unordered_map<std::string, std::unique_ptr<sprite>> sprites;

sprite::sprite() {}

sprite::sprite(const std::string& filename) {
    std::ifstream file(filename);
    if (file.bad())
        return;

    char header[3];
    file >> header;
    if (header[0] != 'P' || header[1] != '1')
        return;

    file >> W >> H;
    data.resize(W*H);
    char c;
    for (int i = 0; i < W * H; i++) {
        file >> c;
        data[i] = c == '1';
    }
}

sprite* createsprite(const char *name) {
    auto filename = "assets/"s + name + ".pbm";
    const auto& ref = sprites[name] = std::make_unique<sprite>(filename);
    return ref.get();
}

void loadsprites() {
    for (auto const& dir_entry : std::filesystem::directory_iterator{"assets"}) {
        auto name = dir_entry.path();
        sprites.emplace(name.stem(), std::make_unique<sprite>(name));
    }
}


//#include "screen.hpp"
//void scene::render(screen& s) {
//    for (auto b : birds) {
//    }
//}
