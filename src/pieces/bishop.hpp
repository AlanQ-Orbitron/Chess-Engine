#pragma once
#include "piece.hpp"

struct Bishop : virtual Piece {
        RankFile rankfile_index = index_to_rankfile(square_index);
	Bishop() { piece_type = Pieces::Bishop; }

	const ShapeMask::Mask *shapes[2] = { &ShapeMask::DIAGONAL_L, &ShapeMask::DIAGONAL_R };
	const int direction[4] = { int(Directions::BottomLeft), int(Directions::TopLeft), int(Directions::BottomRight), int(Directions::TopRight) };
	const Dimensions offsets[4] = { { 0, 0 }, { 7, 7 }, { 7, 0 }, { 0, 7 } };

	uint64_t generate_movement_moves(int square_index, GameState &board) const override {
		uint64_t merged_movements = 0ULL;
		for (int i = 0; i < 4; i++) {
			uint64_t raw_ray = generate_shape_translation(square_index, *shapes[i / 2], offsets[i]);
			uint64_t movement = generate_h_quintessence(square_index, raw_ray, board.bitboards.all_pieces);
			merged_movements |= movement;
			rays[direction[i]] = generate_h_quintessence(square_index, raw_ray, board.bitboards.all_pieces & ~movement) | 1ULL << square_index;
		}
		return merged_movements;
	}
};
