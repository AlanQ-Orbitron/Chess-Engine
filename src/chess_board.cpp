#include "chess_board.hpp"
#include <memory>
#include <godot_cpp/variant/utility_functions.hpp>
#include "board_utilities/board_utility.hpp"
#include "board_utilities/chess_data.hpp"
#include "godot_cpp/variant/array.hpp"
#include "godot_cpp/variant/string.hpp"
#include "pieces/queen.hpp"
#include "rules/castling.hpp"
#include "rules/royalty.hpp"
#include <godot_cpp/core/class_db.hpp>
#include <vector>
#include <algorithm>


using namespace std;

void ChessBoard::generate_moves() {
    Board.reset_board();
    Board.bitboards.update_total_pieces();
    
    for (const auto &PieceType : Board.ruleSet.enabled_piece_type) {
        PieceType->generate_moves(!Board.states.white_to_move);
    }

    for (const auto &PieceType : Board.ruleSet.enabled_piece_type) {
        PieceType->generate_moves(Board.states.white_to_move);
    }
}


godot::Array ChessBoard::get_valid_moves() {
    auto generate_attack_list = [this](int color, int piece) {
        godot::Array singular_pieces_attack_list;
        for (const std::optional<Move> (&moves)[64] : Board.move_list[Board.states.white_to_move].moves) {
            for (const std::optional<Move> move : moves) {
                if (move.has_value()) {
                    if (move->promotion.empty()) {
                        singular_pieces_attack_list.push_back(godot::String(to_UCI[move->from]) + godot::String(to_UCI[move->to]));
                    } else {
                        for (Pieces promation : move->promotion) {
                            
                        }
                    }
                }
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
    Board.ruleSet.modified_rules.push_back(std::make_unique<Castling>());

    Board.ruleSet.enabled_piece_type.push_back(std::make_unique<King>   (Pieces::King));
    Board.ruleSet.enabled_piece_type.push_back(std::make_unique<Pawn>   (Pieces::Pawn, std::vector<Pieces> {Pieces::Bishop, Pieces::Rook, Pieces::Queen}));
    Board.ruleSet.enabled_piece_type.push_back(std::make_unique<Rook>   (Pieces::Rook));
    Board.ruleSet.enabled_piece_type.push_back(std::make_unique<Knight> (Pieces::Knight));
    Board.ruleSet.enabled_piece_type.push_back(std::make_unique<Bishop> (Pieces::Bishop));
    Board.ruleSet.enabled_piece_type.push_back(std::make_unique<Queen>  (Pieces::Queen));
    Board.ruleSet.enabled_piece_type.push_back(std::make_unique<Duck>   (Pieces::Duck));
}

void ChessBoard::generate_board(godot::String board) {
    Board.reset_board();
    Board.reset_start();
    reset_settings();
    fen_to_bit(board, Board);

    /*Convert board to array of index - TODO*/ 

    generate_moves();
}

void ChessBoard::castling_checker(const Move &move_state) {
    if (move_state.piece == Pieces::King) {
        godot::UtilityFunctions::print("ran");
        Board.states.castling[Board.states.white_to_move] |= 1ULL << 4;
    }
    if (move_state.piece == Pieces::Rook) {
        RankFile rankfile = index_to_rankfile(move_state.from);
        if (rankfile.rank == 0) Board.states.castling[Board.states.white_to_move] |= 0x07;
        if (rankfile.rank == 7) Board.states.castling[Board.states.white_to_move] |= 0xE0;
    }
}

bool ChessBoard::move_to(godot::String str_move) {
    if (str_move.length() < 5) {
        godot::UtilityFunctions::printerr("Incorrect String Length");
        return false;
    }
    string move = string(str_move.utf8().get_data());
    int from = to_index.at(move.substr(1, 2));
    int to = to_index.at(move.substr(3, 2));

    if (get_valid_moves().has(str_move.substr(1, 2) + str_move.substr(3, 2))) {
        
        uint64_t from_square = 1ULL << from;
        uint64_t to_square = 1ULL << to;
        PieceType from_type = Board.states.piece_at_index[from];
        PieceType to_type = Board.states.piece_at_index[to];
            
        std::optional<Move> move_state = Board.move_list[Board.states.white_to_move].moves[from][to];
        if (move_state.has_value()) {
            castling_checker(move_state.value());
            std::vector<Pieces> promotions = move_state.value().promotion;
            if (std::find(promotions.begin(), promotions.end(), Pieces::None) != promotions.end()) {}
        }
            
    
        Board.bitboards.color[int(to_type.color)] &= ~to_square;
        Board.bitboards.pieces[int(to_type.piece)] &= ~to_square;

        Board.bitboards.color[int(from_type.color)] &= ~from_square;
        Board.bitboards.pieces[int(from_type.piece)] &= ~from_square;
        Board.bitboards.color[int(from_type.color)] |= to_square;
        Board.bitboards.pieces[int(from_type.piece)] |= to_square;

        Board.states.piece_at_index[to] = Board.states.piece_at_index[from];
        Board.states.piece_at_index[from] = {Color::None, Pieces::None};
        
        Board.states.white_to_move = !Board.states.white_to_move;
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
