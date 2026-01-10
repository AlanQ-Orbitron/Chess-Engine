#pragma once
#include "../rules/includer.hpp"
#include "board_utilities/chess_data.hpp"

struct Piece {
    mutable ShapeGroup shape_group;
    mutable bool is_white;
    Pieces piece_type;
    virtual void generate_movement_moves(int square_index, GameState &board) const = 0;
    virtual void generate_attack_moves(int square_index, GameState &board) const {
        shape_group.attack_bitboard = shape_group.movement_bitboard;
    }

    virtual void generate_moves(bool is_white, GameState &board) const {
        uint64_t full = board.bitboards.color[is_white] & board.bitboards.pieces[int(piece_type)];
        
        this->is_white = is_white;
        while (full) {
            int square_index = pop_least_significant(&full);
            generate_movement_moves(square_index, board);
            generate_attack_moves(square_index, board);
            
            /* Check Checker - TODO */

            for (const auto &Rule : board.ruleSet.modified_rules) {Rule->pre_proccessing(square_index, is_white, board, shape_group);}

            shape_group.movement_bitboard &= ~board.bitboards.all_pieces;
            shape_group.attack_bitboard &= board.bitboards.color[!is_white];

            for (const auto &Rule : board.ruleSet.modified_rules) {Rule->post_proccessing(square_index, is_white, board, shape_group);}

            /*Pin Checker - TODO*/

            board.bitboards.total_moves_bitboard[is_white][int(MoveType::Movement)] = shape_group.movement_bitboard;
            board.bitboards.total_moves_bitboard[is_white][int(MoveType::Attack)] = shape_group.attack_bitboard;

            board.bitboards.moves_bitboard[is_white][int(piece_type)][square_index] = shape_group.attack_bitboard | shape_group.movement_bitboard & (~board.bitboards.color[is_white]);
        }
    }

    
    virtual ~Piece() = default;
};
