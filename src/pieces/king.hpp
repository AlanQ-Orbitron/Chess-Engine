#pragma once
#include "piece.hpp"

struct King : virtual Piece {
    King(Pieces type) : Piece(type) {}

    uint64_t generate_movement_moves() const {
        return generate_shape_translation(square_index, ShapeMask::KING_SHAPE, {1, 1});
    }

    ShapeGroup generate_shape_group() const override {
        uint64_t bitboard[int(MoveType::Total)]{};
        bitboard[int(MoveType::Movement)] = generate_movement_moves();
        bitboard[int(MoveType::Attack)] = bitboard[int(MoveType::Movement)];

        return ShapeGroup {*bitboard};
    }
};
