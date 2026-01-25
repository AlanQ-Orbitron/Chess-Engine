#pragma once
#include "../pieces/piece.hpp"
#include "board_utilities/chess_data.hpp"

struct Pinning : virtual Rule {
	void pre_proccessing(const Piece &piece, const bool &is_white, ShapeGroup &mask_group) const override {
		for (uint64_t ray : piece.rays) {
			if (Board.bitboards.pieces[int(Board.states.royal)] & Board.bitboards.color[!is_white] & ray) {
				Board.bitboards.pins[is_white] |= ray & ~Board.bitboards.pieces[int(Board.states.royal)];
			}
		}

		if (Board.bitboards.pins[!is_white] & (1ULL << piece.square_index)) {
			piece.shape_group.bitboard[int(MoveType::Movement)] &= Board.bitboards.pins[!is_white];
			piece.shape_group.bitboard[int(MoveType::Attack)] &= Board.bitboards.pins[!is_white];
		}
	}
};
