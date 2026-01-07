#include "chess_board.hpp"
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
    if (str_move.length() < 5) {
        UtilityFunctions::printerr("Incorrect String Length");
        return false;
    }
    string move = string(str_move.utf8().get_data());
    int from = to_index.at(move.substr(1, 2));
    int to = to_index.at(move.substr(3, 2));
    pair type = to_pieces.at(move.substr(0, 1));

    if (get_valid_moves().has(str_move.substr(1, 2) + str_move.substr(3, 2))) {
        Board.white_to_move = !Board.white_to_move;
        if (move.length() == 6) {
            pair promotion_type = to_pieces.at(move.substr(move.length() - 1, 1));
        }
        uint64_t from_square = 1ULL << from;
        uint64_t to_square = 1ULL << to;

        for (int piece = 0; piece < 6; piece++) {
            Board.color[White] &= ~from_square;
            Board.color[Black] &= ~from_square;
            Board.pieces[piece] &= ~from_square;
            
            Board.color[White] &= ~to_square;
            Board.color[Black] &= ~to_square;
            Board.pieces[piece] &= ~to_square;
        }
        Board.color[type.first] |= to_square;
        Board.pieces[type.second] |= to_square;

        generate_moves();
        return true;
    }
    return false;
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
    mask.mask = (horizontal_index >= 0) ? mask.mask << horizontal_index : mask.mask >> abs(horizontal_index);
    mask.mask = (vertical_index >= 0) ? mask.mask << vertical_index : mask.mask >> abs(vertical_index);
    return mask.mask & ~(masked_shape);
}

uint64_t ChessBoard::generate_pawn_movement(int square_index, bool is_white) { /* Condenced the pawn functions*/
    uint64_t all_pieces = Board.color[White] | Board.color[Black];
    pair rankfile_index = index_to_rankfile(square_index);
    Mask attack_mask;
    uint64_t attack = 0ULL;
    uint64_t movement = 0ULL;
    if (is_white) {
        attack_mask = ShapeMask::WPAWN_SHAPE;
        if (rankfile_index.second < 7) {
            movement = (1ULL << (square_index + 8)) & ~ all_pieces;
            movement |= (rankfile_index.second == 1) ? movement << 8 : 0ULL;
        }
    } else {
        attack_mask = ShapeMask::BPAWN_SHAPE;
        if (rankfile_index.second > 0) {
        movement = (1ULL << (square_index - 8)) & ~ all_pieces;
        movement |= (rankfile_index.second == 6) ? movement >> 8 : 0ULL;
        }
    }
    if ((is_white && rankfile_index.second < 7) || (!is_white && rankfile_index.second > 0)) {
        attack = generate_shape_translation(square_index, attack_mask) & Board.color[!is_white];
    }
    
    return attack | movement;

}

uint64_t ChessBoard::generate_rook_movement(int square_index, bool) {
    uint64_t all_pieces = Board.color[White] | Board.color[Black];
    pair rankfile_index = index_to_rankfile(square_index);
    return 
        generate_h_quintessence(square_index, ShapeMask::VERTICAL.mask << rankfile_index.first, all_pieces) |
        generate_h_quintessence(square_index, ShapeMask::HORIZONTAL.mask << 8 * rankfile_index.second, all_pieces);
}

uint64_t ChessBoard::generate_knight_movement(int square_index, bool) {
    pair rankfile_index = index_to_rankfile(square_index);
    return generate_shape_translation(square_index, ShapeMask::KNIGHT_SHAPE);
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
    return generate_shape_translation(square_index, ShapeMask::KING_SHAPE) & ~Board.total_attack[!is_white];
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
