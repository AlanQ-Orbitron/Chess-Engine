#pragma once
#include "../board_utilities/includer.hpp"

struct GameState;

struct Rule {
    virtual void pre_proccessing(const Piece &piece, GameState &board, ShapeGroup &mask_group) const = 0;
    virtual void post_proccessing(const Piece &piece, GameState &board, ShapeGroup &mask_group) const = 0;

    virtual ~Rule() = default;
};
