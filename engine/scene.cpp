#include <filesystem>
#include <fstream>
#include <string>
#include <unordered_map>
#include "scene.hpp"
#include "screen.hpp"
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

bool drawsprite(screen& screen, const sprite& sprite, int W, int H) {
    bool collision = false;
    for (auto h = 0; h < sprite.H; h++) {
        if (H + h < 0 || H + h >= screen.H)
            continue;
        for (auto w = 0; w < sprite.W; w++) {
            if (W + w < 0 || W + w >= screen.W)
                continue;
            if (!sprite.data[h * sprite.W + w])
                continue;
            auto pos = (H + h) * screen.W + W + w;
            if (screen.fb[pos])
                collision = true;
            screen.fb[pos] = true;
        }
    }
    return collision;
}

void scene::render(screen& scr) {
    auto* cactus = sprites["smallcactus1"].get();
    auto* player = sprites[dino.anim_frame == 0 ? "dinorun1" : "dinorun2"].get();
    if (!cactus)
        return;

    for (auto c : cactuses) {
        drawsprite(scr, *cactus, c, scr.H*3/4 - cactus->H);
    }

    auto h = scr.H*3/4 - (dino.height * scr.H/2 / 2500) - player->H;
    drawsprite(scr, *player, scr.W/10-player->W, h);
}
