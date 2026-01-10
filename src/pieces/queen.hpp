#pragma once
#include "bishop.hpp"
#include "rook.hpp"

struct Queen : virtual Bishop, virtual Rook {
    Queen() {piece_type = Pieces::Queen;}
    void generate_movement_moves(int square_index, GameState &board) const override {
        Bishop::generate_movement_moves(square_index, board);
        uint64_t bishop_movement_bitboard = shape_group.movement_bitboard;
        Rook::generate_movement_moves(square_index, board);
        shape_group.movement_bitboard |= bishop_movement_bitboard;
    }
};
