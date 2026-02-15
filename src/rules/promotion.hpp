#pragma once
#include "../pieces/piece.hpp"

struct Promotion : virtual Rule {
	void pre_proccessing(const Piece &piece) const override {
        if (piece.promotions.empty()) return;
        piece.shape_group.bitboard[int(MoveType::Promotion)] |= (piece.generate_attack_moves() | piece.generate_movement_moves()) & ((piece.is_white) ? flipVertical(ShapeMask::HORIZONTAL.mask) : ShapeMask::HORIZONTAL.mask);
    }
};
