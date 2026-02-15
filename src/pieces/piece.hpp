#pragma once
#include "../rules/rule.hpp"
#include "board_utilities/chess_data.hpp"

struct Piece {
    const Pieces piece_type;
    const std::vector<Pieces> promotions;
    mutable ShapeGroup shape_group{};
    mutable bool is_white{};
    mutable int square_index{};
    mutable uint64_t rays[8]{};

    virtual uint64_t generate_movement_moves() const = 0;
    virtual uint64_t generate_attack_moves() const {return shape_group.bitboard[int(MoveType::Movement)];}
    virtual void moves_out() const {
        write_moves(MoveType::Movement);
        write_moves(MoveType::Attack);
    }
    virtual void write_moves(const MoveType &move_type) const {
        uint64_t full = shape_group.bitboard[int(move_type)];
        while (full) {
            uint8_t to_square_index = pop_least_significant(&full);
            Board.move_list[is_white].add(
                Move{
                    static_cast<uint8_t>(square_index),
                    to_square_index,
                    piece_type,
                    Board.states.piece_at_index[to_square_index].piece,
                    promotions,
                    move_type,
                }
            );
        }
    }

    explicit Piece(Pieces type, std::vector<Pieces> promotion_list) : piece_type(type), promotions(promotion_list) {}
    explicit Piece(Pieces type) : piece_type(type), promotions({}) {}

    virtual void generate_moves(bool is_white) const {
        uint64_t full = Board.bitboards.color[is_white] & Board.bitboards.pieces[int(piece_type)];
        this->is_white = is_white;

        while (full) {
            shape_group = {};
            square_index = pop_least_significant(&full);
            shape_group.bitboard[int(MoveType::Movement)] = generate_movement_moves();
            shape_group.bitboard[int(MoveType::Attack)] = generate_attack_moves();
            /* Check Checker - TODO */

            for (const auto &Rule : Board.ruleSet.modified_rules) {Rule->pre_proccessing(*this);}
            
            Board.bitboards.total_moves_bitboard[is_white][int(MoveType::Movement)] |= shape_group.bitboard[int(MoveType::Movement)];
            Board.bitboards.total_moves_bitboard[is_white][int(MoveType::Attack)] |= shape_group.bitboard[int(MoveType::Attack)];

            shape_group.bitboard[int(MoveType::Movement)] &= ~Board.bitboards.total_pieces;
            shape_group.bitboard[int(MoveType::Attack)] &= Board.bitboards.color[!is_white];
            shape_group.bitboard[int(MoveType::PromotionMovement)] &= ~Board.bitboards.total_pieces;
            shape_group.bitboard[int(MoveType::PromotionAttack)] &= Board.bitboards.color[!is_white];
            shape_group.bitboard[int(MoveType::Castle)] &= ~Board.bitboards.total_pieces;

            for (const auto &Rule : Board.ruleSet.modified_rules) {Rule->post_proccessing(*this);}

            moves_out();
        }

    }

    
    virtual ~Piece() = default;
};
