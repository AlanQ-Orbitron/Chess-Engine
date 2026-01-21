#pragma once
#include "piece.hpp"

struct Knight : virtual Piece {
    Knight() {piece_type = Pieces::Knight;}
    uint64_t generate_movement_moves(int square_index, GameState &board) const override {
        return generate_shape_translation(square_index, ShapeMask::KNIGHT_SHAPE, {2, 2});
    }
};
