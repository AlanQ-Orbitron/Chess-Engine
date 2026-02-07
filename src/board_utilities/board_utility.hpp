#pragma once
#include <climits>
#include <cstdint>
#include <stdlib.h>
#include <unordered_map>
#include <string>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include "chess_data.hpp"

static const char* to_UCI[64] = {
    "h1", "g1", "f1", "e1", "d1", "c1", "b1", "a1",
    "h2", "g2", "f2", "e2", "d2", "c2", "b2", "a2",
    "h3", "g3", "f3", "e3", "d3", "c3", "b3", "a3",
    "h4", "g4", "f4", "e4", "d4", "c4", "b4", "a4",
    "h5", "g5", "f5", "e5", "d5", "c5", "b5", "a5",
    "h6", "g6", "f6", "e6", "d6", "c6", "b6", "a6",
    "h7", "g7", "f7", "e7", "d7", "c7", "b7", "a7",
    "h8", "g8", "f8", "e8", "d8", "c8", "b8", "a8"
};

static const std::unordered_map<std::string, int> to_index = {
    {"h1", 0}, {"g1", 1}, {"f1", 2}, {"e1", 3}, {"d1", 4}, {"c1", 5}, {"b1", 6}, {"a1", 7},
    {"h2", 8}, {"g2", 9}, {"f2", 10}, {"e2", 11}, {"d2", 12}, {"c2", 13}, {"b2", 14}, {"a2", 15},
    {"h3", 16}, {"g3", 17}, {"f3", 18}, {"e3", 19}, {"d3", 20}, {"c3", 21}, {"b3", 22}, {"a3", 23},
    {"h4", 24}, {"g4", 25}, {"f4", 26}, {"e4", 27}, {"d4", 28}, {"c4", 29}, {"b4", 30}, {"a4", 31},
    {"h5", 32}, {"g5", 33}, {"f5", 34}, {"e5", 35}, {"d5", 36}, {"c5", 37}, {"b5", 38}, {"a5", 39},
    {"h6", 40}, {"g6", 41}, {"f6", 42}, {"e6", 43}, {"d6", 44}, {"c6", 45}, {"b6", 46}, {"a6", 47},
    {"h7", 48}, {"g7", 49}, {"f7", 50}, {"e7", 51}, {"d7", 52}, {"c7", 53}, {"b7", 54}, {"a7", 55},
    {"h8", 56}, {"g8", 57}, {"f8", 58}, {"e8", 59}, {"d8", 60}, {"c8", 61}, {"b8", 62}, {"a8", 63}
};

static const std::unordered_map<std::string, PieceType> to_pieces = {
    {"P", {Color::White, Pieces::Pawn}},   {"p", {Color::Black, Pieces::Pawn}},
    {"R", {Color::White, Pieces::Rook}},   {"r", {Color::Black, Pieces::Rook}},
    {"N", {Color::White, Pieces::Knight}}, {"n", {Color::Black, Pieces::Knight}},
    {"B", {Color::White, Pieces::Bishop}}, {"b", {Color::Black, Pieces::Bishop}},
    {"Q", {Color::White, Pieces::Queen}},  {"q", {Color::Black, Pieces::Queen}},
    {"K", {Color::White, Pieces::King}},   {"k", {Color::Black, Pieces::King}}
};

// Helper Methods

static constexpr RankFile index_to_rankfile(int square_index) {return {square_index % 8, square_index / 8};}; // Rank, File

#if defined(_MSC_VER)
    #include <intrin.h>

    inline uint64_t pop_least_significant(uint64_t* bitboard) {
        unsigned long popped_index;

    #if defined(_M_X64)
        _BitScanForward64(&popped_index, *bitboard);
        
    #else
        if (_BitScanForward(&popped_index, static_cast<unsigned long>(*bitboard))) {}
        else {
            _BitScanForward(&popped_index, static_cast<unsigned long>(*bitboard >> 32));
            popped_index += 32;
        }

    #endif
        *bitboard &= *bitboard - 1;
        return popped_index;
    }

#else
    inline uint64_t pop_least_significant(uint64_t* bitboard) {
        uint64_t popped_index = __builtin_ctzll(*bitboard);
        *bitboard &= *bitboard - 1;
        return popped_index;
    }
#endif




inline std::string expandFEN(const std::string& fen) {
    std::string result;

    for (char c : fen) {
        if (c == ' ') break; // stop at the end of the board part
        if (c == '/') continue; // skip row separators
        if (isdigit(c)) {
            int empty = c - '0';
            result.append(empty, '-'); // fill empty squares with '.'
        } else {
            result += c; // copy piece
        }
    }

    return result;
}

inline void fen_to_bit(godot::String string_board, GameState &board) {
    
    auto process_position = [&board](std::string state) {
        state = expandFEN(state);
        for (int i = 0; i < state.length(); i++) {
            std::string single_string = std::string(1, state[i]);
            if (single_string == "-") {
                board.states.piece_at_index[i] = {Color::None, Pieces::None};
                continue;
            }
            PieceType piece_type = to_pieces.at(single_string);

            board.bitboards.color[int(piece_type.color)] |= 1ULL << i;
            board.bitboards.pieces[int(piece_type.piece)] |= 1ULL << i;
            board.states.piece_at_index[i] = piece_type;
            // result += (c >= start && c <= end) ? "1" : (c >= U'0' && c <= U'9') ? godot::String("0").repeat(c - U'0') : "0";
        }
    };

    auto convert_to_binary = [](godot::String string_binary) {
        return std::stoull(string_binary.utf8().get_data(), nullptr, 2);
    };

    // Only works with standard FEN chess notation
    godot::Array states = string_board.split(" ");
    godot::String position = godot::String(states[0]).replace("/", "").reverse();

    process_position(position.utf8().get_data());
    board.states.white_to_move = true;


    // board.bitboards.color[int(Color::White)] = convert_to_binary(process_position(position, U'B', U'R'));
    // board.bitboards.color[int(Color::Black)] = convert_to_binary(process_position(position, U'b', U'r'));
    // position = position.to_lower();

    // board.bitboards.pieces[int(Pieces::Pawn)]   = convert_to_binary(process_position(position, U'p', U'p'));
    // board.bitboards.pieces[int(Pieces::Rook)]   = convert_to_binary(process_position(position, U'r', U'r'));
    // board.bitboards.pieces[int(Pieces::Knight)] = convert_to_binary(process_position(position, U'n', U'n'));
    // board.bitboards.pieces[int(Pieces::Bishop)] = convert_to_binary(process_position(position, U'b', U'b'));
    // board.bitboards.pieces[int(Pieces::Queen)]  = convert_to_binary(process_position(position, U'q', U'q'));
    // board.bitboards.pieces[int(Pieces::King)]   = convert_to_binary(process_position(position, U'k', U'k'));
}

// Transformation

inline constexpr uint64_t reverse_bit(uint64_t x) { 
/*Not mine*/
    x = ((x >> 1)  & 0x5555555555555555ULL) | ((x & 0x5555555555555555ULL) << 1);
    x = ((x >> 2)  & 0x3333333333333333ULL) | ((x & 0x3333333333333333ULL) << 2);
    x = ((x >> 4)  & 0x0F0F0F0F0F0F0F0FULL) | ((x & 0x0F0F0F0F0F0F0F0FULL) << 4);
    x = ((x >> 8)  & 0x00FF00FF00FF00FFULL) | ((x & 0x00FF00FF00FF00FFULL) << 8);
    x = ((x >> 16) & 0x0000FFFF0000FFFFULL) | ((x & 0x0000FFFF0000FFFFULL) << 16);
    x = (x >> 32) | (x << 32);
    return x;
}
inline constexpr uint64_t flipDiag(uint64_t x) { 
/*Stollen from https://www.chessprogramming.org/Flipping_Mirroring_and_Rotating then translated to c++*/
   uint64_t t = 0ULL;
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
inline constexpr uint64_t mirrorHorizontal (uint64_t x) { 
/*Stollen from https://www.chessprogramming.org/Flipping_Mirroring_and_Rotating then translated to c++*/
   const uint64_t k1 = 0x5555555555555555;
   const uint64_t k2 = 0x3333333333333333;
   const uint64_t k4 = 0x0f0f0f0f0f0f0f0f;
   x = ((x >> 1) & k1) +  2*(x & k1);
   x = ((x >> 2) & k2) +  4*(x & k2);
   x = ((x >> 4) & k4) + 16*(x & k4);
   return x;
}

inline uint64_t flipVertical(uint64_t x) {
/*Stollen from https://www.chessprogramming.org/Flipping_Mirroring_and_Rotating then translated to c++*/
#if defined(_MSC_VER)
#include <intrin.h>
    return _byteswap_uint64(x);
#else
    return __builtin_bswap64(x);

#endif
}

inline uint64_t cw90degrees(uint64_t bitboard) {return(flipDiag(bitboard));}
inline uint64_t ccw90degrees(uint64_t bitboard) {return flipVertical(flipDiag(bitboard));}

// Bitboard functions

inline uint64_t generate_h_quintessence(int square_index, uint64_t mask, uint64_t pieces) { 
/*Stollen from https://www.chessprogramming.org/Hyperbola_Quintessence then translated to c++*/ 
   uint64_t forward, reverse;
   forward = (pieces & mask) - (1ULL << square_index);
   reverse = reverse_bit(forward);
   forward -= 1ULL << square_index;
   reverse -= reverse_bit(1ULL << square_index);
   forward ^= reverse_bit(reverse);
   forward &= mask;
   return forward;
}

inline uint64_t generate_shape_translation(int square_index, ShapeMask::Mask mask, Position position) {
    RankFile rankfile_index = index_to_rankfile(square_index);

    int horizontal_index = (rankfile_index.rank - position.x);
    int vertical_index = ((rankfile_index.file - position.y) << 3);
    int left_index = (rankfile_index.rank - (position.x - mask.size.width)) << 3;
    int right_index = ((7 - rankfile_index.rank) + position.x + 1) << 3;
    int top_index = (rankfile_index.file - (position.y - mask.size.height)) << 3;
    int bottom_index = ((7 - rankfile_index.file) + position.y + 1) << 3;

    uint64_t masked_shape = ccw90degrees(
        ( (left_index < 64) ? ULLONG_MAX << left_index : 0ULL) | 
        ( (right_index < 64) ? ULLONG_MAX >> right_index : 0ULL) ) |
        ( (top_index < 64) ? ULLONG_MAX << top_index : 0ULL) |
        ( (bottom_index < 64) ? ULLONG_MAX >> bottom_index : 0ULL);

    mask.mask = (horizontal_index >= 0) ? mask.mask << horizontal_index : mask.mask >> abs(horizontal_index);
    mask.mask = (vertical_index >= 0) ? mask.mask << vertical_index : mask.mask >> abs(vertical_index);
    return mask.mask & ~(masked_shape);
}
