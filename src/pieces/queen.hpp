#pragma once
#include "bishop.hpp"
#include "rook.hpp"

struct Queen : virtual Bishop, virtual Rook {
	Queen() { piece_type = Pieces::Queen; }

	uint64_t generate_movement_moves(int square_index, GameState &board) const override {
		return Bishop::generate_movement_moves(square_index, board) | Rook::generate_movement_moves(square_index, board);
	}
};
