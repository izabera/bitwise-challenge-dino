#pragma once
#include "scene.hpp"
#include "screen.hpp"
#include "types.hpp"
#include <utility>

struct state {
    u8 seed{};
    u8 jumpframe{};
} __attribute__((packed));

static_assert(sizeof(state) <= sizeof(u64));

u64 jump(u64 t);

std::pair<state, scene> makescene(const input& i, state state);
