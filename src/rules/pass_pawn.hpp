#pragma once
#include "../pieces/piece.hpp"
#include "board_utilities/chess_data.hpp"

struct PassPawn : virtual Rule {
	void pre_proccessing(const Piece &piece) const override {
        if (piece.piece_type != Pieces::Pawn) return;
        RankFile rankfile_index = index_to_rankfile(piece.square_index);
        uint64_t movement_bitboard = piece.shape_group.bitboard[int(MoveType::Movement)];
        
        if (piece.is_white) {
            if (rankfile_index.file < 7) {
                movement_bitboard = (rankfile_index.file == 1) ? movement_bitboard << 8 : 0ULL;
            }
        } else {
            if (rankfile_index.file > 0) {
                movement_bitboard = (rankfile_index.file == 6) ? movement_bitboard >> 8 : 0ULL;
            }
        }
        piece.shape_group.bitboard[int(MoveType::PassPawn)] = movement_bitboard;
    }
};
