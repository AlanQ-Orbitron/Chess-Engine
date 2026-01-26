#pragma once
#include "../pieces/piece.hpp"

struct Royalty : virtual Rule {
	void pre_proccessing(const Piece &piece) const override {
        if (piece.piece_type == Board.states.royal) {
            piece.shape_group.bitboard[int(MoveType::Movement)] &= ~Board.bitboards.total_moves_bitboard[!piece.is_white][int(MoveType::Attack)];
            piece.shape_group.bitboard[int(MoveType::Attack)] &= ~Board.bitboards.total_moves_bitboard[!piece.is_white][int(MoveType::Attack)];
        }
	}
};
