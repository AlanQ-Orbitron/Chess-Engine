#pragma once
#include <cstdint>
#include "../board_utilities/includer.hpp"
#include "../rules/includer.hpp"

struct Piece {
    virtual uint64_t generate_normal_moves(int square_index, int color, const GameState &board) const = 0;

    uint64_t generate_moves(int square_index, int color, const GameState &board) const {
        uint64_t moves = generate_normal_moves(square_index, color, board);
        for (const auto &Rule : board.ruleSet.modified_rules) {
            moves |= Rule->apply(square_index, color, board);
        }
        return moves;
    }
    
    virtual ~Piece() = default;
};
