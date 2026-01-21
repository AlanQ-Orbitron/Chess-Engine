#pragma once
#include "../board_utilities/includer.hpp"

struct GameState;

struct Rule {
    virtual void pre_proccessing(const Piece &piece, const bool &is_white, GameState &board, ShapeGroup &mask_group) const {};
    virtual void post_proccessing(const Piece &piece, const bool &is_white, GameState &board, ShapeGroup &mask_group) const {};

    virtual ~Rule() = default;
};
