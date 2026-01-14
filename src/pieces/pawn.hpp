#pragma once
#include "board_utilities/chess_data.hpp"
#include "piece.hpp"

struct Pawn : virtual Piece {
	Pawn() { piece_type = Pieces::Pawn; }
	uint64_t generate_movement_moves(int square_index, GameState &board) const override {
		Dimensions rankfile		   = index_to_rankfile(square_index);
		uint64_t movement_bitboard = 0ULL;

		if (is_white) {
			if (rankfile.y < 7) {
				movement_bitboard = (1ULL << (square_index + 8)) & ~board.bitboards.all_pieces;
				movement_bitboard |= (rankfile.y == 1) ? movement_bitboard << 8 : 0ULL;
			}
		} else {
			if (rankfile.y > 0) {
				movement_bitboard = (1ULL << (square_index - 8)) & ~board.bitboards.all_pieces;
				movement_bitboard |= (rankfile.y == 6) ? movement_bitboard >> 8 : 0ULL;
			}
		}
		return movement_bitboard;
	}

	uint64_t generate_attack_moves(int square_index, GameState &board) const override {
		Dimensions rankfile			= index_to_rankfile(square_index);
		ShapeMask::Mask attack_mask = (is_white) ? ShapeMask::WPAWN_SHAPE : ShapeMask::BPAWN_SHAPE;

		if ((is_white && rankfile.y < 7) || (!is_white && rankfile.y > 0)) {
			return generate_shape_translation(square_index, attack_mask, { 1, 1 });
		}
		return 0ULL;
	}
};
