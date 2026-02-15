#pragma once
#include "../pieces/piece.hpp"

struct Promotion : virtual Rule {
	void pre_proccessing(const Piece &piece) const override {
        if (piece.promotions.empty()) return;
        uint64_t mask = (piece.is_white) ? flipVertical(ShapeMask::HORIZONTAL.mask) : ShapeMask::HORIZONTAL.mask;
        piece.shape_group.bitboard[int(MoveType::PromotionAttack)] |= piece.generate_attack_moves() & mask;
        piece.shape_group.bitboard[int(MoveType::PromotionMovement)] |= piece.generate_movement_moves() & mask;
    }
};
