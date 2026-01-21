#pragma once
#include "board_utilities/chess_data.hpp"
#include "piece.hpp"

struct King : virtual Piece {
    King() {piece_type = Pieces::King;}
    uint64_t generate_movement_moves(int square_index, GameState &board) const override {
        return generate_shape_translation(square_index, ShapeMask::KING_SHAPE, {1, 1});
    }
};
