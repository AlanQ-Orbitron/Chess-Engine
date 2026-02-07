#pragma once
#include "bishop.hpp"
#include "rook.hpp"

struct Queen : virtual Piece, virtual Bishop, virtual Rook {
    Queen(Pieces type) : Piece(type), Rook(type), Bishop(type) {}
    uint64_t generate_movement_moves() const {
        return
            Bishop::generate_movement_moves() |
            Rook::generate_movement_moves();
    }

    ShapeGroup generate_shape_group() const override {
        uint64_t bitboard[int(MoveType::Total)]{};
        bitboard[int(MoveType::Movement)] = generate_movement_moves();
        bitboard[int(MoveType::Attack)] = bitboard[int(MoveType::Movement)];

        return ShapeGroup {*bitboard};
    }
};
