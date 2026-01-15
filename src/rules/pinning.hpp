// #pragma once
// #include "../pieces/piece.hpp"

// struct Pinning : virtual Rule {
// 	void pre_proccessing(const Piece &piece, const bool &is_white, GameState &board, ShapeGroup &mask_group) const override {
// 		for (uint64_t ray : piece.rays) {
// 			if (board.bitboards.pieces[int(board.states.royal)] & ray) {
// 				board.bitboards.pins[is_white] |= ray & ~board.bitboards.pieces[int(board.states.royal)];
// 			}
// 		}

// 		if (board.bitboards.pins[!is_white] & 1ULL << piece.square_index) {
// 			piece.shape_group.movement_bitboard &= board.bitboards.pins[!is_white];
// 			piece.shape_group.attack_bitboard &= board.bitboards.pins[!is_white];
// 		}
// 	}
// };
