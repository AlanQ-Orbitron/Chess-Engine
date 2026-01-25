#pragma once
#include "board_utilities/chess_data.hpp"
#include "piece.hpp"

struct Duck : Piece {
    Duck(Pieces type) : Piece(type) {}

    uint64_t generate_movement_moves() const override {
        return ~Board.bitboards.total_pieces;
    }
    
    uint64_t generate_attack_moves() const override {
        return 0ULL;
    }

    void generate_moves(bool is_white) const override {
        uint64_t full = Board.bitboards.color[int(Color::Blocker)] & Board.bitboards.pieces[int(piece_type)];
        
        this->is_white = false;
        while (full) {
            int square_index = pop_least_significant(&full);
            generate_movement_moves();
            generate_attack_moves();
            
            /* Check Checker - TODO */

            for (const auto &Rule : Board.ruleSet.modified_rules) {Rule->pre_proccessing(*this, is_white, shape_group);}

            shape_group.bitboard[int(MoveType::Movement)] &= ~Board.bitboards.total_pieces;

            for (const auto &Rule : Board.ruleSet.modified_rules) {Rule->post_proccessing(*this, is_white, shape_group);}

            /*Pin Checker - TODO*/

            Board.bitboards.total_moves_bitboard[int(Color::Blocker)][int(MoveType::Movement)] = shape_group.bitboard[int(MoveType::Movement)];
        }
    }
};
