#pragma once
#include "piece.hpp"

struct King : virtual Piece {
    King() {piece_type = Pieces::King;}
    void generate_movement_moves(int square_index, GameState &board) const override {
        shape_group.movement_bitboard = generate_shape_translation(square_index, ShapeMask::KING_SHAPE);
    }
};
