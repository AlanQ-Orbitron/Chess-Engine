#pragma once
#include "piece.hpp"

struct Knight : virtual Piece {
    Knight(Pieces type) : Piece(type) {}
    uint64_t generate_movement_moves() const override {
        return generate_shape_translation(square_index, ShapeMask::KNIGHT_SHAPE, {2, 2});
    }
};
