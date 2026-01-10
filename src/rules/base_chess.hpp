#pragma once
#include "rule.hpp"

struct GameState;

struct BaseChess : Rule {
    void pre_proccessing(int square_index, const bool &is_white, const GameState &board, ShapeGroup &mask_group) const override {
        return;
    }
    virtual uint64_t post_proccessing(int square_index, const GameState &board, ShapeGroup &mask_group) const = 0;
};
