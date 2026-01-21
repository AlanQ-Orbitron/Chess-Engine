#pragma once
#include "../pieces/piece.hpp"

struct Royalty : virtual Rule {
	void pre_proccessing(const Piece &piece, const bool &is_white, GameState &board, ShapeGroup &mask_group) const override {
        if (piece.piece_type == board.states.royal) {
            piece.shape_group.movement_bitboard &= ~board.bitboards.total_moves_bitboard[!is_white][int(MoveType::Movement)];
        }
	}
};
