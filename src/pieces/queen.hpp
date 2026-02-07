#pragma once
#include "bishop.hpp"
#include "rook.hpp"

struct Queen : virtual Piece, virtual Bishop, virtual Rook {
    Queen(Pieces type) : Piece(type), Rook(type), Bishop(type) {}
    uint64_t generate_movement_moves() const override{
        return
            Bishop::generate_movement_moves() |
            Rook::generate_movement_moves();
    }
};
