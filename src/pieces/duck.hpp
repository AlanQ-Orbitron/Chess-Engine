#pragma once
#include "board_utilities/chess_data.hpp"
#include "piece.hpp"

struct Duck : Piece {
    Duck(Pieces type) : Piece(type) {}

    uint64_t generate_movement_moves() const {
        return ~Board.bitboards.total_pieces;
    }

    ShapeGroup generate_shape_group() const override {
        uint64_t bitboard[int(MoveType::Total)]{};
        bitboard[int(MoveType::Movement)] = generate_movement_moves();

        return ShapeGroup {*bitboard};
    }

    void generate_moves(bool is_white) const override {
        uint64_t full = Board.bitboards.color[int(Color::Blocker)] & Board.bitboards.pieces[int(piece_type)];
        
        this->is_white = false;
        while (full) {
            square_index = pop_least_significant(&full);
            shape_group = generate_shape_group();
            
            /* Check Checker - TODO */

            for (const auto &Rule : Board.ruleSet.modified_rules) {Rule->pre_proccessing(*this);}

            shape_group.bitboard[int(MoveType::Movement)] &= ~Board.bitboards.total_pieces;

            for (const auto &Rule : Board.ruleSet.modified_rules) {Rule->post_proccessing(*this);}

            /*Pin Checker - TODO*/

            Board.bitboards.total_moves_bitboard[int(Color::Blocker)][int(MoveType::Movement)] = shape_group.bitboard[int(MoveType::Movement)];
        }
    }
};
