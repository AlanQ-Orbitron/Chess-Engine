#pragma once
#include "../pieces/piece.hpp"

struct Royalty : virtual Rule {
	void pre_proccessing(const Piece &piece, const bool &is_white, ShapeGroup &mask_group) const override {
        if (piece.piece_type == Board.states.royal) {
            piece.shape_group.bitboard[int(MoveType::Movement)] &= ~Board.bitboards.total_moves_bitboard[!is_white][int(MoveType::Movement)];
        }
	}
};
