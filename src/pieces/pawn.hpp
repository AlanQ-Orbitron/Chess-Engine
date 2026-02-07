#pragma once
#include "piece.hpp"

struct Pawn : virtual Piece {
    Pawn(Pieces type, std::vector<Pieces> Promations) : Piece(type, Promations) {}

    uint64_t generate_enpassant_moves() const {
        RankFile rankfile_index = index_to_rankfile(square_index);
        uint64_t movement_bitboard = shape_group.bitboard[int(MoveType::EnPassantCapture)];
        
        if (is_white) {
            if (rankfile_index.file < 7) {
                movement_bitboard |= (rankfile_index.file == 1) ? movement_bitboard << 8 : 0ULL;
            }
        } else {
            if (rankfile_index.file > 0) {
            movement_bitboard |= (rankfile_index.file == 6) ? movement_bitboard >> 8 : 0ULL;
            }
        }
    }

    uint64_t generate_movement_moves() const {
        RankFile rankfile_index = index_to_rankfile(square_index);
        uint64_t movement_bitboard = 0ULL;
        
        if (is_white) {
            if (rankfile_index.file < 7) {
                movement_bitboard = (1ULL << (square_index + 8)) & ~Board.bitboards.total_pieces;
            }
        } else {
            if (rankfile_index.file > 0) {
            movement_bitboard = (1ULL << (square_index - 8)) & ~Board.bitboards.total_pieces;
            }
        }

        return movement_bitboard;
    }
    
    uint64_t generate_attack_moves() const {
        RankFile rankfile_index = index_to_rankfile(square_index);
        Position position_offset = (is_white) ? Position{1, 0} : Position{1, 2};

        if ((is_white && rankfile_index.file < 7) || (!is_white && rankfile_index.file > 0)) {
            return generate_shape_translation(square_index, ShapeMask::PAWN_SHAPE, position_offset);
        }
        return 0ULL;
    }

    ShapeGroup generate_shape_group() const override {
        uint64_t bitboard[int(MoveType::Total)]{};
        bitboard[int(MoveType::Movement)] = generate_movement_moves();
        bitboard[int(MoveType::Attack)] = generate_attack_moves();
        bitboard[int(MoveType::EnPassantCapture)] = generate_enpassant_moves();

        return ShapeGroup {*bitboard};
    }
};
