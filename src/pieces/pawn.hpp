#pragma once
#include <cstdint>
#include "piece.hpp"

struct Pawn : Piece {
    uint64_t generate_moves(int square, int color, const GameState &state) const override {

    }
};
