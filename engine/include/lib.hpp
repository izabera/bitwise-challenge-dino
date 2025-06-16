#pragma once
#include "types.hpp"

struct rng {
    constexpr static u64 multiplier = 6364136223846793005; // knuth's mmix parameters
    constexpr static u64 increment = 1442695040888963407;

    u64 state;

    // Brown, F., "Random number generation with arbitrary strides",
    // Transactions of the American Nuclear Society 71 (Nov. 1994), 202-203.
    void advance(u64 steps) {
        u64 a = multiplier;
        u64 c = increment;

        u64 a_next = 1;
        u64 c_next = 0;

        while (steps > 0) {
            if (steps & 1) {
                a_next = a_next * a;
                c_next = c_next * a + c;
            }
            c = (a + 1) * c;
            a *= a;
            steps >>= 1;
        }
        state = a_next * state + c_next;
    }

    u32 gen() {
        state = state * multiplier + increment;
        return state >> 32;
    }
};


