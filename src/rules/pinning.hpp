#pragma once
#include "../pieces/piece.hpp"
#include "board_utilities/chess_data.hpp"

struct Pinning : virtual Rule {
	void pre_proccessing(const Piece &piece) const override {
		for (uint64_t ray : piece.rays) {
			if (Board.bitboards.pieces[int(Board.states.royal)] & Board.bitboards.color[!piece.is_white] & ray) {
				Board.bitboards.pins[piece.is_white] |= ray & ~Board.bitboards.pieces[int(Board.states.royal)];
			}
		}

		if (Board.bitboards.pins[!piece.is_white] & (1ULL << piece.square_index)) {
			piece.shape_group.bitboard[int(MoveType::Movement)] &= Board.bitboards.pins[!piece.is_white];
			piece.shape_group.bitboard[int(MoveType::Attack)] &= Board.bitboards.pins[!piece.is_white];
		}
	}
};
