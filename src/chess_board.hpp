#pragma once

// IWYU pragma: begin_exports
#include "rules/includer.hpp"
#include "board_utilities/includer.hpp"
#include "pieces/includer.hpp"
// IWYU pragma: end_exports

#include "godot_cpp/classes/node.hpp"
#include "godot_cpp/variant/array.hpp"
#include "godot_cpp/variant/dictionary.hpp"
#include "godot_cpp/variant/string.hpp"
#include "godot_cpp/variant/vector2.hpp"

using namespace std;

class ChessBoard: public godot::Node{
    GDCLASS(ChessBoard, Node)
    public:
        void generate_board(godot::String board);
        bool move_to(godot::String move);
        godot::Array get_valid_moves();
        void set_settings(godot::Dictionary settings);
        void reset_settings();
    protected:
        static void _bind_methods();
        void generate_moves();
        void castling_checker(const Move &move_state);
    private:

        
        void set_board(godot::Dictionary board) {
            Board.bitboards.color[int(Color::White)] = board["white_pieces"];
            Board.bitboards.color[int(Color::Black)] = board["black_pieces"];
            Board.bitboards.pieces[int(Pieces::Pawn)] = board["pawns"];
            Board.bitboards.pieces[int(Pieces::Rook)] = board["rooks"];
            Board.bitboards.pieces[int(Pieces::Knight)] = board["knights"];
            Board.bitboards.pieces[int(Pieces::Bishop)] = board["bishops"];
            Board.bitboards.pieces[int(Pieces::Queen)] = board["queens"];
            Board.bitboards.pieces[int(Pieces::King)] = board["kings"];
        };
        godot::Dictionary get_board() const {
            godot::Dictionary board;
            board["white_pieces"] = Board.bitboards.color[int(Color::White)];
            board["black_pieces"] = Board.bitboards.color[int(Color::Black)];
            board["pawns"] = Board.bitboards.pieces[int(Pieces::Pawn)];
            board["rooks"] = Board.bitboards.pieces[int(Pieces::Rook)];
            board["knights"] = Board.bitboards.pieces[int(Pieces::Knight)];
            board["bishops"] = Board.bitboards.pieces[int(Pieces::Bishop)];
            board["queens"] = Board.bitboards.pieces[int(Pieces::Queen)];
            board["kings"] = Board.bitboards.pieces[int(Pieces::King)];
            return board;
        };
        void promotion_logic(int square_index);
};
