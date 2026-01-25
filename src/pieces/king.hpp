#pragma once
#include "piece.hpp"

struct King : virtual Piece {
    King(Pieces type) : Piece(type) {}

    uint64_t generate_movement_moves() const override {
        return generate_shape_translation(square_index, ShapeMask::KING_SHAPE, {1, 1});
    }
};
