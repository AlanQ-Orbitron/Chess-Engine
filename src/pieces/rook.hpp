#pragma once
#include "piece.hpp"

struct Rook : virtual Piece {
	const ShapeMask::Mask *shapes[2] = { &ShapeMask::VERTICAL, &ShapeMask::HORIZONTAL };
	const int direction[4] = { int(Directions::Top), int(Directions::Bottom), int(Directions::Left), int(Directions::Right) };
	const Position offsets[4] = { { 0, 0 }, { 0, 7 }, { 0, 0 }, { 7, 0 } };

	Rook(Pieces type) : Piece(type) {}
	
    uint64_t generate_movement_moves() const override {
		uint64_t merged_movements = 0ULL;
		for (int i = 0; i < 4; i++) {
			uint64_t raw_ray = generate_shape_translation(square_index, *shapes[i >> 1], offsets[i]);
			uint64_t movement = generate_h_quintessence(square_index, raw_ray, Board.bitboards.total_pieces);
			merged_movements |= movement;
			rays[direction[i]] = generate_h_quintessence(square_index, raw_ray, Board.bitboards.total_pieces & ~movement) | 1ULL << square_index;
		}
        return merged_movements;
    }

};
