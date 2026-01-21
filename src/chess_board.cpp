#include "chess_board.hpp"
#include <cstdint>
#include <memory>
#include <string>
#include <godot_cpp/variant/utility_functions.hpp>
#include "godot_cpp/variant/array.hpp"
#include "godot_cpp/variant/string.hpp"
#include "rules/royalty.hpp"
#include <godot_cpp/core/class_db.hpp>

using namespace std;

void ChessBoard::generate_moves() {
    Board.bitboards.reset_board();
    Board.bitboards.all_pieces = Board.bitboards.color[int(Color::White)] | Board.bitboards.color[int(Color::Black)];
    
    for (const auto &PieceType : Board.ruleSet.enabled_piece_type) {
        PieceType->generate_moves(!Board.states.white_to_move, Board);
    }

    for (const auto &PieceType : Board.ruleSet.enabled_piece_type) {
        PieceType->generate_moves(Board.states.white_to_move, Board);
    }
}


godot::Array ChessBoard::get_valid_moves() {
    auto generate_attack_list = [this](int color, int piece) {
        godot::Array singular_pieces_attack_list;
        uint64_t bitboard = Board.bitboards.color[color] & Board.bitboards.pieces[piece];
        int square_index;
        int square_attack_index;
        while (bitboard) {
            square_index = pop_least_significant(&bitboard);
            uint64_t attack_bitboard = Board.bitboards.moves_bitboard[color][piece][square_index];
            while (attack_bitboard) {
                square_attack_index = pop_least_significant(&attack_bitboard);
                singular_pieces_attack_list.append(godot::String(to_UCI[square_index]) + godot::String(to_UCI[square_attack_index]));
            }
        }
        return singular_pieces_attack_list;
    };
    
    godot::Array chessmove_list;
    for (int color = 0; color < 2; color++) {
        for (int piece = 0; piece < 6; piece++) {
        chessmove_list.append_array(generate_attack_list(Board.states.white_to_move, piece));
        }
    }
    return chessmove_list;
}

void ChessBoard::set_settings(godot::Dictionary settings) {

}

void ChessBoard::reset_settings() {
    Board.ruleSet.modified_rules.push_back(std::make_unique<Pinning>());
    Board.ruleSet.modified_rules.push_back(std::make_unique<Royalty>());

    Board.ruleSet.enabled_piece_type.push_back(std::make_unique<King>());
    Board.ruleSet.enabled_piece_type.push_back(std::make_unique<Pawn>());
    Board.ruleSet.enabled_piece_type.push_back(std::make_unique<Rook>());
    Board.ruleSet.enabled_piece_type.push_back(std::make_unique<Knight>());
    Board.ruleSet.enabled_piece_type.push_back(std::make_unique<Bishop>());
    Board.ruleSet.enabled_piece_type.push_back(std::make_unique<Queen>());
    Board.ruleSet.enabled_piece_type.push_back(std::make_unique<Duck>());
}

void ChessBoard::generate_board(godot::String board) {
    Board.bitboards.reset_board();
    Board.bitboards.reset_piece();
    reset_settings();
    fen_to_bit(board, Board);

    /*Convert board to array of index - TODO*/ 

    generate_moves();
}

bool ChessBoard::move_to(godot::String str_move) {
    if (str_move.length() < 5) {
        godot::UtilityFunctions::printerr("Incorrect String Length");
        return false;
    }
    string move = string(str_move.utf8().get_data());
    int from = to_index.at(move.substr(1, 2));
    int to = to_index.at(move.substr(3, 2));
    PieceType type = to_pieces.at(move.substr(0, 1));

    if (get_valid_moves().has(str_move.substr(1, 2) + str_move.substr(3, 2))) {
        Board.states.white_to_move = !Board.states.white_to_move;
        if (move.length() == 6) {
            PieceType promotion_type = to_pieces.at(move.substr(move.length() - 1, 1));
        }
        uint64_t from_square = 1ULL << from;
        uint64_t to_square = 1ULL << to;

        for (int piece = 0; piece < int(Pieces::Total); piece++) {
            Board.bitboards.color[int(Color::White)] &= ~from_square;
            Board.bitboards.color[int(Color::Black)] &= ~from_square;
            Board.bitboards.pieces[piece] &= ~from_square;
            
            Board.bitboards.color[int(Color::White)] &= ~to_square;
            Board.bitboards.color[int(Color::Black)] &= ~to_square;
            Board.bitboards.pieces[piece] &= ~to_square;
        }
        Board.bitboards.color[int(type.color)] |= to_square;
        Board.bitboards.pieces[int(type.piece)] |= to_square;

        generate_moves();
        return true;
    }
    return false;
}

void ChessBoard::_bind_methods() {
    godot::ClassDB::bind_method(godot::D_METHOD("generate_board", "board"), &ChessBoard::generate_board);
    godot::ClassDB::bind_method(godot::D_METHOD("generate_moves"), &ChessBoard::generate_moves);
    godot::ClassDB::bind_method(godot::D_METHOD("get_valid_moves"), &ChessBoard::get_valid_moves);
    godot::ClassDB::bind_method(godot::D_METHOD("move_to", "str_move"), &ChessBoard::move_to);
    godot::ClassDB::bind_method(godot::D_METHOD("set_settings", "settings"), &ChessBoard::set_settings);


    godot::ClassDB::bind_method(godot::D_METHOD("set_board", "board"), &ChessBoard::set_board);
    godot::ClassDB::bind_method(godot::D_METHOD("get_board"), &ChessBoard::get_board);

    ADD_PROPERTY(godot::PropertyInfo(godot::Variant::DICTIONARY, "board"), "set_board", "get_board");

}
