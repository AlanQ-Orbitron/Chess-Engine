#pragma once
#include "board_utilities/chess_data.hpp"
#include "piece.hpp"

struct King : virtual Piece {
    King(Pieces type) : Piece(type) {}

    uint64_t generate_movement_moves() const override {
        // return generate_shape_translation(square_index, ShapeMask::KING_SHAPE, {1, 1});
        return Board.bitboards.pass_pawns[is_white];
    }

    virtual void moves_out() const override {
        write_moves(MoveType::Movement);
        write_moves(MoveType::Attack);
        write_moves(MoveType::Castle);
    }

};
