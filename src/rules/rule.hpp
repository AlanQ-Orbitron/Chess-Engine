#pragma once
#include "../board_utilities/includer.hpp"

struct GameState;

struct Rule {
    virtual void pre_proccessing(const Piece &piece) const {};
    virtual void post_proccessing(const Piece &piece) const {};

    virtual ~Rule() = default;
};
