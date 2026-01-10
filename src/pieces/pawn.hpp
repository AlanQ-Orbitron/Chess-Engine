#pragma once
#include "board_utilities/chess_data.hpp"
#include "piece.hpp"

struct Pawn : Piece {
    Pawn() {piece_type = Pieces::Pawn;}
    void generate_movement_moves(int square_index, GameState &board) const override {
        RankFile rankfile_index = index_to_rankfile(square_index);
        uint64_t movement_bitboard = 0ULL;
        
        if (is_white) {
            if (rankfile_index.file < 7) {
                movement_bitboard = (1ULL << (square_index + 8)) & ~board.bitboards.all_pieces;
                movement_bitboard |= (rankfile_index.file == 1) ? movement_bitboard << 8 : 0ULL;
            }
        } else {
            if (rankfile_index.file > 0) {
            movement_bitboard = (1ULL << (square_index - 8)) & ~board.bitboards.all_pieces;
            movement_bitboard |= (rankfile_index.file == 6) ? movement_bitboard >> 8 : 0ULL;
            }
        }

        shape_group.movement_bitboard = movement_bitboard;
    }
    void generate_attack_moves(int square_index, GameState &board) const override {
        RankFile rankfile_index = index_to_rankfile(square_index);
        ShapeMask::Mask attack_mask = (is_white) ? ShapeMask::WPAWN_SHAPE : ShapeMask::BPAWN_SHAPE;

        if ((is_white && rankfile_index.file < 7) || (!is_white && rankfile_index.file > 0)) {
            shape_group.attack_bitboard = generate_shape_translation(square_index, attack_mask);
            return;
        }
        shape_group.attack_bitboard = 0ULL;
    }
};
