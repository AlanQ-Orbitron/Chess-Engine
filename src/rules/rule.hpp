#pragma once
#include "../board_utilities/includer.hpp"

struct GameState;

struct Rule {
    virtual uint64_t apply(int square_index, int color, const GameState &board) const = 0;

    virtual ~Rule() = default;
};
