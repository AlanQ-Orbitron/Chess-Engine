#pragma once
#include "piece.hpp"

struct Rook : virtual Piece {
	Rook() { piece_type = Pieces::Rook; }

	const ShapeMask::Mask *shapes[2] = { &ShapeMask::VERTICAL, &ShapeMask::HORIZONTAL };
	const int direction[4] = { int(Directions::Top), int(Directions::Bottom), int(Directions::Left), int(Directions::Right) };
	const Dimensions offsets[4] = { { 0, 0 }, { 0, 7 }, { 0, 0 }, { 7, 0 } };

	uint64_t generate_movement_moves(int square_index, GameState &board) const override {
		uint64_t merged_movements = 0ULL;
		for (int i = 0; i < 4; i++) {
			uint64_t raw_ray = generate_shape_translation(square_index, *shapes[i >> 1], offsets[i]);
			uint64_t movement = generate_h_quintessence(square_index, raw_ray, board.bitboards.all_pieces);
			merged_movements |= movement;
			rays[direction[i]] = generate_h_quintessence(square_index, raw_ray, board.bitboards.all_pieces & ~movement) | 1ULL << square_index;
		}
		return merged_movements;
	}
};
