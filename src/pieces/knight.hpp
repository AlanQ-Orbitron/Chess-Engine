#pragma once
#include "board_utilities/chess_data.hpp"
#include "piece.hpp"

struct Knight : virtual Piece {
    Knight() {piece_type = Pieces::Knight;}
    void generate_movement_moves(int square_index, GameState &board) const override {
    }
		return generate_shape_translation(square_index, ShapeMask::KNIGHT_SHAPE, { 2, 2 });
};
