#pragma once
#include "../pieces/piece.hpp"
#include "board_utilities/chess_data.hpp"

struct Castling : virtual Rule {
	void pre_proccessing(const Piece &piece) const override {
        uint8_t blocked;
        if (piece.piece_type != Pieces::King || (Board.states.castling[int(piece.is_white)] & (1ULL << 4))) return;
        if ((Board.bitboards.total_pieces | Board.bitboards.total_moves_bitboard[!piece.is_white][int(MoveType::Attack)]) & (1ULL << (piece.square_index - 1))) blocked = 0x07;
        if ((Board.bitboards.total_pieces | Board.bitboards.total_moves_bitboard[!piece.is_white][int(MoveType::Attack)]) & ((1ULL << (piece.square_index + 1)) | (1ULL << (piece.square_index + 3)))) blocked = 0xE0;
        piece.shape_group.bitboard[int(MoveType::Castle)] = (piece.is_white) ? 
            generate_shape_translation(piece.square_index, ShapeMask::CASTLING, {2, 0}) & ~(Board.states.castling[int(piece.is_white)] | blocked) :
            flipVertical(flipVertical(generate_shape_translation(piece.square_index, ShapeMask::CASTLING, {2, 0})) & ~(Board.states.castling[int(piece.is_white)] | blocked));
    }
};
