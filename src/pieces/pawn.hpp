#pragma once
#include "piece.hpp"

struct Pawn : virtual Piece {
    Pawn(Pieces type, std::vector<Pieces> Promations) : Piece(type, Promations) {}

    uint64_t generate_movement_moves() const override {
        RankFile rankfile_index = index_to_rankfile(square_index);
        uint64_t movement_bitboard = 0ULL;
        
        if (is_white) {
            if (rankfile_index.file < 7) {
                movement_bitboard = (1ULL << (square_index + 8)) & ~Board.bitboards.total_pieces;
                movement_bitboard |= (rankfile_index.file == 1) ? movement_bitboard << 8 : 0ULL;
            }
        } else {
            if (rankfile_index.file > 0) {
            movement_bitboard = (1ULL << (square_index - 8)) & ~Board.bitboards.total_pieces;
            movement_bitboard |= (rankfile_index.file == 6) ? movement_bitboard >> 8 : 0ULL;
            }
        }

        return movement_bitboard;
    }
    
    uint64_t generate_attack_moves() const override {
        RankFile rankfile_index = index_to_rankfile(square_index);
        ShapeMask::Mask attack_mask = (is_white) ? ShapeMask::WPAWN_SHAPE : ShapeMask::BPAWN_SHAPE;

        if ((is_white && rankfile_index.file < 7) || (!is_white && rankfile_index.file > 0)) {
            return generate_shape_translation(square_index, attack_mask, {1, 1});
        }
        return 0ULL;
    }
};
