#pragma once
#include <cstdint>
#include <stdlib.h>
#include <unordered_map>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;
using namespace std;

enum Color {
    Black, White
};

enum Pieces {
    Pawn, Rook, Knight, Bishop, Queen, King, EnPassant
};

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

static const std::unordered_map<string, int> to_index = {
    {"h1", 0}, {"g1", 1}, {"f1", 2}, {"e1", 3}, {"d1", 4}, {"c1", 5}, {"b1", 6}, {"a1", 7},
    {"h2", 8}, {"g2", 9}, {"f2", 10}, {"e2", 11}, {"d2", 12}, {"c2", 13}, {"b2", 14}, {"a2", 15},
    {"h3", 16}, {"g3", 17}, {"f3", 18}, {"e3", 19}, {"d3", 20}, {"c3", 21}, {"b3", 22}, {"a3", 23},
    {"h4", 24}, {"g4", 25}, {"f4", 26}, {"e4", 27}, {"d4", 28}, {"c4", 29}, {"b4", 30}, {"a4", 31},
    {"h5", 32}, {"g5", 33}, {"f5", 34}, {"e5", 35}, {"d5", 36}, {"c5", 37}, {"b5", 38}, {"a5", 39},
    {"h6", 40}, {"g6", 41}, {"f6", 42}, {"e6", 43}, {"d6", 44}, {"c6", 45}, {"b6", 46}, {"a6", 47},
    {"h7", 48}, {"g7", 49}, {"f7", 50}, {"e7", 51}, {"d7", 52}, {"c7", 53}, {"b7", 54}, {"a7", 55},
    {"h8", 56}, {"g8", 57}, {"f8", 58}, {"e8", 59}, {"d8", 60}, {"c8", 61}, {"b8", 62}, {"a8", 63}
};

static const std::unordered_map<string, pair<int, int>> to_pieces = {
    {"P", {White, Pawn}}, {"p", {Black, Pawn}},
    {"R", {White, Rook}}, {"r", {Black, Rook}},
    {"N", {White, Knight}}, {"n", {Black, Knight}},
    {"B", {White, Bishop}}, {"b", {Black, Bishop}},
    {"Q", {White, Queen}}, {"q", {Black, Queen}},
    {"K", {White, King}}, {"k", {Black, King}}
};


inline constexpr uint64_t reverse_bit(uint64_t x) { /*Not mine*/
    x = ((x >> 1)  & 0x5555555555555555ULL) | ((x & 0x5555555555555555ULL) << 1);
    x = ((x >> 2)  & 0x3333333333333333ULL) | ((x & 0x3333333333333333ULL) << 2);
    x = ((x >> 4)  & 0x0F0F0F0F0F0F0F0FULL) | ((x & 0x0F0F0F0F0F0F0F0FULL) << 4);
    x = ((x >> 8)  & 0x00FF00FF00FF00FFULL) | ((x & 0x00FF00FF00FF00FFULL) << 8);
    x = ((x >> 16) & 0x0000FFFF0000FFFFULL) | ((x & 0x0000FFFF0000FFFFULL) << 16);
    x = (x >> 32) | (x << 32);
    return x;
}
inline constexpr uint64_t flipDiag(uint64_t x) { /*Stollen from https://www.chessprogramming.org/Flipping_Mirroring_and_Rotating then translated to c++*/
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
inline constexpr uint64_t mirrorHorizontal (uint64_t x) { /*Stollen from https://www.chessprogramming.org/Flipping_Mirroring_and_Rotating then translated to c++*/
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
    return(flipDiag(bitboard));
}
inline uint64_t ccw90degrees(uint64_t bitboard) {
    return flipVertical(flipDiag(bitboard));
}
