#pragma once
#include <cstdint>
#include <vector>

struct Rule;

struct GameState {
    struct Bitboards {
        uint64_t color[2];
        uint64_t pieces[7];
        uint64_t attack_boards[2][6][64];
        uint64_t total_attack[2];
        uint64_t pins[2];
    } bitboards;

    struct States {
        uint8_t castling;
        bool white_to_move = false;
        bool in_check = false;
        uint8_t EnPassant[2];
        uint8_t halfmove_clock;
        uint16_t fullmove_number;
    } states;
    struct RuleSet {
        std::vector<Rule*> modified_rules;
        // bool capture_the_king;
        // bool alicorn_promotion;
        // bool giant_pawns;
        // bool lancer;
        // bool is_bounce;
        // bool is_reflect;
        // bool amazon;
        // bool triple_move_pawns;
    } ruleSet;
};

struct RankFile {
    int rank;
    int file;
};

struct ShapeMask {
    struct Mask {uint64_t mask; int width; int height;};
    static constexpr Mask HORIZONTAL {0x00000000000000FF, 8, 1 };
    static constexpr Mask VERTICAL {0x0101010101010101, 1, 1 };
    static constexpr Mask DIAGONAL_L {0x8040201008040201, 8, 8 };
    static constexpr Mask DIAGONAL_R {0x0102040810204080, 8, 8 };
    static constexpr Mask KNIGHT_SHAPE {0x0000000A1100110A, 5, 5};
    static constexpr Mask KING_SHAPE {0x0000000000070707, 3, 3};
    static constexpr Mask BPAWN_SHAPE {0x0000000000000005, 3, 3};
    static constexpr Mask WPAWN_SHAPE {0x0000000000050000, 3, 3};
};


// struct PieceMove {
//     uint64_t (ChessBoard::*generate_base)(int square_index, bool is_white);
//     uint64_t (*modifier)(uint64_t base_moves, const GameState& state, bool is_white) = nullptr;
// };
