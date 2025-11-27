#include "chess_board.hpp"
#include <cstdint>
#include <string>
#include <godot_cpp/variant/utility_functions.hpp>
#include "godot_cpp/variant/array.hpp"
#include "godot_cpp/variant/string.hpp"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;
using namespace std;

void ChessBoard::reset_board()
{
    Board.white_pieces = 0ULL;
    Board.black_pieces = 0ULL;
    Board.pawns = 0ULL;
    Board.rooks = 0ULL;
    Board.knights = 0ULL;
    Board.bishops = 0ULL;
    Board.queens = 0ULL;
    Board.kings = 0ULL;
    memset(Board.attack_boards, 0, sizeof(Board.attack_boards));
}

Array ChessBoard::get_valid_moves()
{
    auto generate_attack_list = [this](uint64_t bitboard, int piece_type) {
        Array singular_pieces_attack_list;
        int square_index;
        int square_attack_index;
        while (bitboard) {
            square_index = pop_least_significant(&bitboard);
            uint64_t attack_bitboard = Board.attack_boards[square_index][piece_type];
            while (attack_bitboard) {
                square_attack_index = pop_least_significant(&attack_bitboard);
                singular_pieces_attack_list.append(String(to_UCI[square_index]) + String(to_UCI[square_attack_index]));
            }
        }
        return singular_pieces_attack_list;
    };
    Array chessmove_list;
    generate_moves();

    chessmove_list.append_array(generate_attack_list(Board.rooks & Board.white_pieces, WRook));
    chessmove_list.append_array(generate_attack_list(Board.knights & Board.white_pieces, WKnight));
    chessmove_list.append_array(generate_attack_list(Board.bishops & Board.white_pieces, WBishop));
    chessmove_list.append_array(generate_attack_list(Board.queens & Board.white_pieces, WQueen));
    chessmove_list.append_array(generate_attack_list(Board.kings & Board.white_pieces, WKing));
    chessmove_list.append_array(generate_attack_list(Board.rooks & Board.black_pieces, BRook));
    chessmove_list.append_array(generate_attack_list(Board.knights & Board.black_pieces, BKnight));
    chessmove_list.append_array(generate_attack_list(Board.bishops & Board.black_pieces, BBishop));
    chessmove_list.append_array(generate_attack_list(Board.queens & Board.black_pieces, BQueen));
    chessmove_list.append_array(generate_attack_list(Board.kings & Board.black_pieces, BKing));
    return chessmove_list;
}

ChessBoard::GameState ChessBoard::fen_to_bit(String board)
{
    
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
    String position = String(states[0]).replace("/", "").to_lower();

    converted_state.white_pieces = convert_to_binary(process_position(position, U'B', U'R'));
    converted_state.black_pieces = convert_to_binary(process_position(position, U'b', U'r'));
    converted_state.pawns = convert_to_binary(process_position(position, U'p', U'p'));
    converted_state.rooks = convert_to_binary(process_position(position, U'r', U'r'));
    converted_state.knights = convert_to_binary(process_position(position, U'n', U'n'));
    converted_state.bishops = convert_to_binary(process_position(position, U'b', U'b'));
    converted_state.queens = convert_to_binary(process_position(position, U'q', U'q'));
    converted_state.kings = convert_to_binary(process_position(position, U'k', U'k'));

    return converted_state;
}

// String ChessBoard::bit_to_fen(ChessBoard::GameState board)
// {
    
// }

void ChessBoard::generate_board(String board)
{
    ChessBoard::GameState fen_board = fen_to_bit(board);
    Board.white_pieces = fen_board.white_pieces;
    Board.black_pieces = fen_board.black_pieces;
    Board.pawns = fen_board.pawns;
    Board.rooks = fen_board.rooks;
    Board.knights = fen_board.knights;
    Board.bishops = fen_board.bishops;
    Board.queens = fen_board.queens;
    Board.kings = fen_board.kings;
}


void ChessBoard::generate_moves()
{
    auto generate_attack_mask = [this](uint64_t bitboard, int piece_type, auto generate) {
        int square_index;
        while (bitboard) {
            square_index = pop_least_significant(&bitboard);
            Board.attack_boards[square_index][piece_type] = generate(square_index);
        }
    };
    
    generate_attack_mask(Board.rooks & Board.white_pieces, WRook, [this](int square_index) {return generate_rook_movement(square_index);});
    generate_attack_mask(Board.knights & Board.white_pieces, WKnight, [this](int square_index) {return generate_knight_movement(square_index);});
    generate_attack_mask(Board.bishops & Board.white_pieces, WBishop, [this](int square_index) {return generate_bishop_movement(square_index);});
    generate_attack_mask(Board.queens & Board.white_pieces, WQueen, [this](int square_index) {return generate_queen_movement(square_index);});
    generate_attack_mask(Board.kings & Board.white_pieces, WKing, [this](int square_index) {return generate_king_movement(square_index);});
    generate_attack_mask(Board.rooks & Board.black_pieces, BRook, [this](int square_index) {return generate_rook_movement(square_index);});
    generate_attack_mask(Board.knights & Board.black_pieces, BKnight, [this](int square_index) {return generate_knight_movement(square_index);});
    generate_attack_mask(Board.bishops & Board.black_pieces, BBishop, [this](int square_index) {return generate_bishop_movement(square_index);});
    generate_attack_mask(Board.queens & Board.black_pieces, BQueen, [this](int square_index) {return generate_queen_movement(square_index);});
    generate_attack_mask(Board.kings & Board.black_pieces, BKing, [this](int square_index) {return generate_king_movement(square_index);});
       
}

// uint64_t ChessBoard::generate_pawn_movement(int index)
// {
    
// }

uint64_t ChessBoard::generate_rook_movement(int square_index)
{
    pair rankfile_index = index_to_rankfile(square_index);

    return (VERTICAL << rankfile_index.first) | (HORIZONTAL << 8 * rankfile_index.second);
}

uint64_t ChessBoard::generate_knight_movement(int index)
{
    uint64_t knights_pattern = 0x0000001221001221;
    return 0ULL;
}

uint64_t ChessBoard::generate_bishop_movement(int square_index)
{
    pair rankfile_index = index_to_rankfile(square_index);
    int difference_L = (7 - rankfile_index.first) - rankfile_index.second;
    int difference_R = rankfile_index.first - rankfile_index.second;
    uint64_t D1 = (difference_L < 0) ? DIAGANOL_L << 8 * difference_L : DIAGANOL_L >> 8 * difference_L;
    uint64_t D2 = (difference_L < 0) ? DIAGANOL_R << 8 * difference_L : DIAGANOL_R >> 8 * difference_L;
    return D1 | D2;
}

uint64_t ChessBoard::generate_queen_movement(int index)
{
    return generate_bishop_movement(index) | generate_rook_movement(index);
}

uint64_t ChessBoard::generate_king_movement(int square_index)
{
    pair rankfile_index = index_to_rankfile(square_index);
    uint64_t square_3x3 = 0x0000000000070707;
    return square_3x3 << (8 * (square_index - 1) + square_index);
}


void ChessBoard::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("reset_board"), &ChessBoard::reset_board);
    ClassDB::bind_method(D_METHOD("generate_board", "board"), &ChessBoard::generate_board);
    ClassDB::bind_method(D_METHOD("generate_moves"), &ChessBoard::generate_moves);
    ClassDB::bind_method(D_METHOD("get_valid_moves"), &ChessBoard::get_valid_moves);


    ClassDB::bind_method(D_METHOD("set_board", "board"), &ChessBoard::set_board);
    ClassDB::bind_method(D_METHOD("get_board"), &ChessBoard::get_board);

    ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "board"), "set_board", "get_board");

}
