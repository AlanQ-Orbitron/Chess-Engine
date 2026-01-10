#pragma once
#include "board_utilities/chess_data.hpp"
#include "piece.hpp"

struct Duck : Piece {
    Duck() {piece_type = Pieces::Duck;}
    void generate_movement_moves(int square_index, GameState &board) const override {
        shape_group.movement_bitboard = ~board.bitboards.all_pieces;
    }
    void generate_attack_moves(int square_index, GameState &board) const override {
        shape_group.attack_bitboard = 0ULL;
    }

    void generate_moves(bool is_white, GameState &board) const override {
        uint64_t full = board.bitboards.color[int(Color::Blocker)] & board.bitboards.pieces[int(piece_type)];
        
        this->is_white = false;
        while (full) {
            int square_index = pop_least_significant(&full);
            generate_movement_moves(square_index, board);
            generate_attack_moves(square_index, board);
            
            /* Check Checker - TODO */

            for (const auto &Rule : board.ruleSet.modified_rules) {Rule->pre_proccessing(square_index, is_white, board, shape_group);}

            shape_group.movement_bitboard &= ~board.bitboards.all_pieces;

            for (const auto &Rule : board.ruleSet.modified_rules) {Rule->post_proccessing(square_index, is_white, board, shape_group);}

            /*Pin Checker - TODO*/

            board.bitboards.total_moves_bitboard[is_white][int(MoveType::Movement)] = shape_group.movement_bitboard;
        }
    }
};
