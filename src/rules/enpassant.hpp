#pragma once
#include "../pieces/piece.hpp"
#include "board_utilities/chess_data.hpp"

struct EnPassant : virtual Rule {
	void pre_proccessing(const Piece &piece) const override {
        if (piece.piece_type == Pieces::Pawn) piece.shape_group.bitboard[int(MoveType::EnPassant)] = piece.generate_attack_moves() & Board.bitboards.pass_pawns[!piece.is_white];
    }
};
