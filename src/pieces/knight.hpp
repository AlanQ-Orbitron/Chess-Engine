#pragma once
#include "piece.hpp"

struct Knight : virtual Piece {
    Knight(Pieces type) : Piece(type) {}
    uint64_t generate_movement_moves() const {
        return generate_shape_translation(square_index, ShapeMask::KNIGHT_SHAPE, {2, 2});
    }

    ShapeGroup generate_shape_group() const override {
        uint64_t bitboard[int(MoveType::Total)]{};
        bitboard[int(MoveType::Movement)] = generate_movement_moves();
        bitboard[int(MoveType::Attack)] = bitboard[int(MoveType::Movement)];

        return ShapeGroup {*bitboard};
    }
};
