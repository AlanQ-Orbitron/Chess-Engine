#include "chess_board.hpp"
#include <algorithm>
#include <climits>
#include <cstdint>
#include <cstring>
#include <string>
#include <godot_cpp/variant/utility_functions.hpp>
#include "godot_cpp/variant/array.hpp"
#include "godot_cpp/variant/string.hpp"
#include <stdlib.h>
#include <godot_cpp/core/class_db.hpp>
#include <utility>

using namespace godot;
using namespace std;

inline uint64_t reverse_bit(uint64_t x) { /*Not mine*/
    x = ((x >> 1)  & 0x5555555555555555ULL) | ((x & 0x5555555555555555ULL) << 1);
    x = ((x >> 2)  & 0x3333333333333333ULL) | ((x & 0x3333333333333333ULL) << 2);
    x = ((x >> 4)  & 0x0F0F0F0F0F0F0F0FULL) | ((x & 0x0F0F0F0F0F0F0F0FULL) << 4);
    x = ((x >> 8)  & 0x00FF00FF00FF00FFULL) | ((x & 0x00FF00FF00FF00FFULL) << 8);
    x = ((x >> 16) & 0x0000FFFF0000FFFFULL) | ((x & 0x0000FFFF0000FFFFULL) << 16);
    x = (x >> 32) | (x << 32);
    return x;
}

inline uint64_t flipDiag(uint64_t x) { /*Stollen from https://www.chessprogramming.org/Flipping_Mirroring_and_Rotating then translated to c++*/
   uint64_t t;
   const uint64_t k1 = 0x5500550055005500;
   const uint64_t k2 = 0x3333000033330000;
   const uint64_t k4 = 0x0f0f0f0f00000000;
   t  = k4 & (x ^ (x << 28));
   x ^=       t ^ (t >> 28) ;
   t  = k2 & (x ^ (x << 14));
   x ^=       t ^ (t >> 14) ;
   t  = k1 & (x ^ (x <<  7));
   x ^=       t ^ (t >>  7) ;
   return x;
}

inline uint64_t mirrorHorizontal (uint64_t x) { /*Stollen from https://www.chessprogramming.org/Flipping_Mirroring_and_Rotating then translated to c++*/
   const uint64_t k1 = 0x5555555555555555;
   const uint64_t k2 = 0x3333333333333333;
   const uint64_t k4 = 0x0f0f0f0f0f0f0f0f;
   x = ((x >> 1) & k1) +  2*(x & k1);
   x = ((x >> 2) & k2) +  4*(x & k2);
   x = ((x >> 4) & k4) + 16*(x & k4);
   return x;
}

inline uint64_t flipVertical(uint64_t x) { /*Stollen from https://www.chessprogramming.org/Flipping_Mirroring_and_Rotating then translated to c++*/
   return _byteswap_uint64(x);
}

inline uint64_t cw90degrees(uint64_t bitboard) {
    return mirrorHorizontal(flipDiag(bitboard));
}

inline uint64_t ccw90degrees(uint64_t bitboard) {
    return flipVertical(flipDiag(bitboard));
}

const char* ChessBoard::to_UCI[64] = {
    "h1", "g1", "f1", "e1", "d1", "c1", "b1", "a1",
    "h2", "g2", "f2", "e2", "d2", "c2", "b2", "a2",
    "h3", "g3", "f3", "e3", "d3", "c3", "b3", "a3",
    "h4", "g4", "f4", "e4", "d4", "c4", "b4", "a4",
    "h5", "g5", "f5", "e5", "d5", "c5", "b5", "a5",
    "h6", "g6", "f6", "e6", "d6", "c6", "b6", "a6",
    "h7", "g7", "f7", "e7", "d7", "c7", "b7", "a7",
    "h8", "g8", "f8", "e8", "d8", "c8", "b8", "a8"
};

const std::unordered_map<string, int> ChessBoard::to_index = {
    {"h1", 0}, {"g1", 1}, {"f1", 2}, {"e1", 3}, {"d1", 4}, {"c1", 5}, {"b1", 6}, {"a1", 7},
    {"h2", 8}, {"g2", 9}, {"f2", 10}, {"e2", 11}, {"d2", 12}, {"c2", 13}, {"b2", 14}, {"a2", 15},
    {"h3", 16}, {"g3", 17}, {"f3", 18}, {"e3", 19}, {"d3", 20}, {"c3", 21}, {"b3", 22}, {"a3", 23},
    {"h4", 24}, {"g4", 25}, {"f4", 26}, {"e4", 27}, {"d4", 28}, {"c4", 29}, {"b4", 30}, {"a4", 31},
    {"h5", 32}, {"g5", 33}, {"f5", 34}, {"e5", 35}, {"d5", 36}, {"c5", 37}, {"b5", 38}, {"a5", 39},
    {"h6", 40}, {"g6", 41}, {"f6", 42}, {"e6", 43}, {"d6", 44}, {"c6", 45}, {"b6", 46}, {"a6", 47},
    {"h7", 48}, {"g7", 49}, {"f7", 50}, {"e7", 51}, {"d7", 52}, {"c7", 53}, {"b7", 54}, {"a7", 55},
    {"h8", 56}, {"g8", 57}, {"f8", 58}, {"e8", 59}, {"d8", 60}, {"c8", 61}, {"b8", 62}, {"a8", 63}
};

const std::unordered_map<string, pair<int, int>> ChessBoard::to_pieces = {
    {"P", {ChessBoard::White, ChessBoard::Pawn}}, {"p", {ChessBoard::Black, ChessBoard::Pawn}},
    {"R", {ChessBoard::White, ChessBoard::Rook}}, {"r", {ChessBoard::Black, ChessBoard::Rook}},
    {"N", {ChessBoard::White, ChessBoard::Knight}}, {"n", {ChessBoard::Black, ChessBoard::Knight}},
    {"B", {ChessBoard::White, ChessBoard::Bishop}}, {"b", {ChessBoard::Black, ChessBoard::Bishop}},
    {"Q", {ChessBoard::White, ChessBoard::Queen}}, {"q", {ChessBoard::Black, ChessBoard::Queen}},
    {"K", {ChessBoard::White, ChessBoard::King}}, {"k", {ChessBoard::Black, ChessBoard::King}}
};

ChessBoard::MoveGenerator ChessBoard::move_generator[6] = {
    &ChessBoard::generate_pawn_movement,
    &ChessBoard::generate_rook_movement,
    &ChessBoard::generate_knight_movement,
    &ChessBoard::generate_bishop_movement,
    &ChessBoard::generate_queen_movement,
    &ChessBoard::generate_king_movement,
};

void ChessBoard::reset_board() {
    memset(Board.color, 0, sizeof(Board.color));
    memset(Board.pieces, 0, sizeof(Board.pieces));
    memset(Board.attack_boards, 0, sizeof(Board.attack_boards));
    memset(Board.total_attack, 0, sizeof(Board.total_attack));
    Board.pin = 0ULL;
    Board.check = 0LL;
}

Array ChessBoard::get_valid_moves() {
    auto generate_attack_list = [this](int color, int piece) {
        Array singular_pieces_attack_list;
        uint64_t bitboard = Board.color[color] & Board.pieces[piece];
        int square_index;
        int square_attack_index;
        while (bitboard) {
            square_index = pop_least_significant(&bitboard);
            uint64_t attack_bitboard = Board.attack_boards[color][piece][square_index];
            while (attack_bitboard) {
                square_attack_index = pop_least_significant(&attack_bitboard);
                singular_pieces_attack_list.append(String(to_UCI[square_index]) + String(to_UCI[square_attack_index]));
            }
        }
        return singular_pieces_attack_list;
    };
    
    Array chessmove_list;
    for (int color = 0; color < 2; color++) {
        for (int piece = 0; piece < 6; piece++) {
        chessmove_list.append_array(generate_attack_list(color, piece));
        }
    }
    return chessmove_list;
}

void ChessBoard::set_settings(godot::Dictionary settings) {

}

ChessBoard::GameState ChessBoard::fen_to_bit(String board) {
    
    auto process_position = [](String state, char32_t start, char32_t end) {
        String result;
        for (int i = 0; i < state.length(); i++) {
            char32_t c = state[i];
            result += (c >= start && c <= end) ? "1" : (c >= U'0' && c <= U'9') ? String("0").repeat(c - U'0') : "0";
        }
        return result;
    };

    auto convert_to_binary = [](String string_binary) {
        return stoull(string_binary.utf8().get_data(), nullptr, 2);
    };

    // Only works with standard FEN chess notation
    ChessBoard::GameState converted_state;
    Array states = board.split(" ");
    String position = String(states[0]).replace("/", "");
    converted_state.white_to_move = (states[1] == String("w"));

    converted_state.color[White] = convert_to_binary(process_position(position, U'B', U'R'));
    converted_state.color[Black] = convert_to_binary(process_position(position, U'b', U'r'));
    position = position.to_lower();

    converted_state.pieces[Pawn] = convert_to_binary(process_position(position, U'p', U'p'));
    converted_state.pieces[Rook] = convert_to_binary(process_position(position, U'r', U'r'));
    converted_state.pieces[Knight] = convert_to_binary(process_position(position, U'n', U'n'));
    converted_state.pieces[Bishop] = convert_to_binary(process_position(position, U'b', U'b'));
    converted_state.pieces[Queen] = convert_to_binary(process_position(position, U'q', U'q'));
    converted_state.pieces[King] = convert_to_binary(process_position(position, U'k', U'k'));

    return converted_state;
}

// String ChessBoard::bit_to_fen(ChessBoard::GameState board)
// {
    
// }

void ChessBoard::generate_board(String board) {
    reset_board();
    ChessBoard::GameState fen_board = fen_to_bit(board);

    /*
    Convert board to array of index
    
    
    */ 


    Board = fen_board;

    generate_moves();
}

bool ChessBoard::move_to(String str_move) {
    string move = string(str_move.utf8().get_data());
    int from = to_index.at(move.substr(1, 2));
    int to = to_index.at(move.substr(3, 2));
    pair type = to_pieces.at(move.substr(0, 1));
    if (get_valid_moves().has(str_move.substr(1, 2) + str_move.substr(3, 2))) {
        Board.white_to_move = !Board.white_to_move;
        if (move.length() == 6) {
            pair promotion_type = to_pieces.at(move.substr(move.length() - 1, 1));
        }
        uint64_t square = 1ULL << from;
        Board.color[type.first] ^= square & Board.color[type.first];
        Board.pieces[type.second] ^= square & Board.pieces[type.second];
        generate_moves();
        return true;
    }

    return false;
}

void ChessBoard::generate_moves() {
    auto generate_attack_mask = [this](int color, int pieces, MoveGenerator generate) {
        int square_index;
        uint64_t generated_move;
        uint64_t full = Board.color[color] & Board.pieces[pieces];
        while (full) {
            square_index = pop_least_significant(&full);
            generated_move = (this->*generate)(square_index, bool(color)) & (~Board.color[color]);
            Board.attack_boards[color][pieces][square_index] = generated_move;
            Board.total_attack[color] |= generated_move;
        }
    };

    for (int piece = 0; piece < 6; piece++) {
        generate_attack_mask(!Board.white_to_move, piece, move_generator[piece]);
    }

    for (int piece = 0; piece < 6; piece++) {
        generate_attack_mask(Board.white_to_move, piece, move_generator[piece]);
    }
}

uint64_t ChessBoard::generate_h_quintessence(int square_index, uint64_t mask, uint64_t pieces) { /*Stollen from https://www.chessprogramming.org/Hyperbola_Quintessence then translated to c++*/ 
   uint64_t forward, reverse;
   forward = (pieces & mask) - (1ULL << square_index);
   reverse = reverse_bit(forward);
   forward -= 1ULL << square_index;
   reverse -= reverse_bit(1ULL << square_index);
   forward ^= reverse_bit(reverse);
   forward &= mask;
   return forward;
}

uint64_t ChessBoard::generate_shape_translation(int square_index, Mask mask) {
    pair rankfile_index = index_to_rankfile(square_index);
    int radius = static_cast<int> (ceil(mask.width / 2.0));
    int horizontal_index = (rankfile_index.first - radius + 1);
    int vertical_index = ((rankfile_index.second - radius + 1) * 8);
    int left_index = (rankfile_index.first + radius) * 8;
    int right_index = ((7 - rankfile_index.first) + radius) * 8;
    int top_index = (rankfile_index.second + radius) * 8;
    int bottom_index = ((7 - rankfile_index.second) + radius) * 8;
    uint64_t masked_shape = ccw90degrees(
        ((left_index < 64) ? ULLONG_MAX << left_index : 0ULL) | 
         ((right_index < 64) ? ULLONG_MAX >> right_index : 0ULL)) | 
        ((top_index < 64) ? ULLONG_MAX << top_index : 0ULL) | 
        ((bottom_index < 64) ? ULLONG_MAX >> bottom_index : 0ULL);
    UtilityFunctions::print(horizontal_index);
    mask.mask = (horizontal_index >= 0) ? mask.mask << horizontal_index : mask.mask >> abs(horizontal_index);
    mask.mask = (vertical_index >= 0) ? mask.mask << vertical_index : mask.mask >> abs(vertical_index);
    return mask.mask & ~(masked_shape);
}

uint64_t ChessBoard::generate_pawn_movement(int square_index, bool is_white) { /* Condenced the pawn functions*/
    uint64_t all_pieces = Board.color[White] | Board.color[Black];
    pair rankfile_index = index_to_rankfile(square_index);

    auto generate_wpawn_movement = [this, square_index, all_pieces, rankfile_index]() {
        uint64_t pawn_attack = ((1ULL << ( square_index + 9)) | (1ULL << (square_index + 7))) & all_pieces;
        uint64_t pawn_movement = (
            1ULL << max(square_index, square_index + 8) | 
            ((rankfile_index.second == 1) ?  1ULL << (square_index + 16) : 0ULL)
        ) & (~all_pieces);
        return (pawn_attack | pawn_movement) & ((rankfile_index.second == 7) ? 0ULL : ULLONG_MAX << (rankfile_index.second + 1) * 8); 
    };

    auto generate_bpawn_movement = [this, square_index, all_pieces, rankfile_index]() {
        uint64_t pawn_attack = ((1ULL << ( square_index - 9)) | (1ULL << (square_index - 7))) & all_pieces;
        uint64_t pawn_movement = (
            1ULL << min(square_index, square_index - 8) | 
            ((rankfile_index.second == 6) ?  1ULL << (square_index - 16) : 0ULL)
        ) & (~all_pieces);
        return (pawn_attack | pawn_movement) & ((rankfile_index.second == 0) ? 0ULL : ULLONG_MAX >> (8 - rankfile_index.second) * 8); 
    };

    
    return (is_white) ? generate_wpawn_movement() : generate_bpawn_movement();

}
uint64_t ChessBoard::generate_rook_movement(int square_index, bool) {
    uint64_t all_pieces = Board.color[White] | Board.color[Black];
    pair rankfile_index = index_to_rankfile(square_index);
    return 
        generate_h_quintessence(square_index, VERTICAL << rankfile_index.first, all_pieces) |
        generate_h_quintessence(square_index, HORIZONTAL << 8 * rankfile_index.second, all_pieces);
}

uint64_t ChessBoard::generate_knight_movement(int square_index, bool) {
    pair rankfile_index = index_to_rankfile(square_index);
    uint64_t knights_pattern = 0x0000000A1100110A;
    uint64_t mask_vertical = 
        (0x0707070707070707 << max(0, rankfile_index.first - 2)) |
        (0xE0E0E0E0E0E0E0E0 >> max(0, 5 - rankfile_index.first));
    rankfile_index.first -= 2; rankfile_index.second -= 2;
    int rankfile = rankfile_index.first + (rankfile_index.second * 8);
    return ((rankfile > 0) ? knights_pattern << rankfile : knights_pattern >>  abs(rankfile)) & mask_vertical;
}

uint64_t ChessBoard::generate_bishop_movement(int square_index, bool) {
    pair rankfile_index = index_to_rankfile(square_index);
    int difference_L = rankfile_index.first - rankfile_index.second;
    int difference_R = (7 - rankfile_index.first) - rankfile_index.second;
    uint64_t D1 = (difference_L > 0) ? ShapeMask::DIAGONAL_L.mask >> 8 * difference_L: ShapeMask::DIAGONAL_L.mask << 8 * abs(difference_L);
    uint64_t D2 = (difference_R > 0) ? ShapeMask::DIAGONAL_R.mask >> 8 * difference_R: ShapeMask::DIAGONAL_R.mask << 8 * abs(difference_R);
    return (D1 | D2) & (
        generate_h_quintessence(square_index, D1, (Board.color[White] | Board.color[Black])) |
        generate_h_quintessence(square_index, D2, (Board.color[White] | Board.color[Black]))
    );
}

uint64_t ChessBoard::generate_queen_movement(int square_index, bool) {
    return generate_bishop_movement(square_index) | generate_rook_movement(square_index);
}

uint64_t ChessBoard::generate_king_movement(int square_index, bool is_white) {
    pair rankfile_index = index_to_rankfile(square_index);
    return generate_shape_translation(square_index, ShapeMask::KING_SHAPE);
}


void ChessBoard::_bind_methods() {
    ClassDB::bind_method(D_METHOD("reset_board"), &ChessBoard::reset_board);
    ClassDB::bind_method(D_METHOD("generate_board", "board"), &ChessBoard::generate_board);
    ClassDB::bind_method(D_METHOD("generate_moves"), &ChessBoard::generate_moves);
    ClassDB::bind_method(D_METHOD("get_valid_moves"), &ChessBoard::get_valid_moves);
    ClassDB::bind_method(D_METHOD("move_to", "str_move"), &ChessBoard::move_to);
    ClassDB::bind_method(D_METHOD("set_settings", "settings"), &ChessBoard::set_settings);


    ClassDB::bind_method(D_METHOD("set_board", "board"), &ChessBoard::set_board);
    ClassDB::bind_method(D_METHOD("get_board"), &ChessBoard::get_board);

    ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "board"), "set_board", "get_board");

}
