#pragma once

// IWYU pragma: begin_exports
#include "board_utilities/includer.hpp"
// IWYU pragma: end_exports

#include "godot_cpp/classes/node.hpp"
#include "godot_cpp/variant/array.hpp"
#include "godot_cpp/variant/dictionary.hpp"
#include "godot_cpp/variant/string.hpp"
#include "godot_cpp/variant/vector2.hpp"
#include <cstdint>
#include <utility>


using namespace std;

class ChessBoard: public godot::Node{
    GDCLASS(ChessBoard, Node)
    public:
        void reset_board();
        void generate_board(godot::String board);
        bool move_to(godot::String move);
        godot::Array get_valid_moves();
        void set_settings(godot::Dictionary settings);
    protected:
        GameState Board;
        static void _bind_methods();
        void generate_moves();
        static GameState fen_to_bit(godot::String board);
        static godot::String bit_to_fen(GameState board);
        static constexpr pair<int, int> index_to_rankfile(int square_index) {return {square_index % 8, square_index / 8};}; // Rank, File
        uint64_t pop_least_significant(uint64_t* bitboard) {
            unsigned long popped_index;
            _BitScanForward64(&popped_index, *bitboard);
            *bitboard &= *bitboard -1;
            return static_cast<int>(popped_index);
        }


    private:
        void set_board(godot::Dictionary board) {
            Board.bitboards.color[White] = board["white_pieces"];
            Board.bitboards.color[Black] = board["black_pieces"];
            Board.bitboards.pieces[Pawn] = board["pawns"];
            Board.bitboards.pieces[Rook] = board["rooks"];
            Board.bitboards.pieces[Knight] = board["knights"];
            Board.bitboards.pieces[Bishop] = board["bishops"];
            Board.bitboards.pieces[Queen] = board["queens"];
            Board.bitboards.pieces[King] = board["kings"];
        };
        godot::Dictionary get_board() const {
            godot::Dictionary board;
            board["white_pieces"] = Board.bitboards.color[White];
            board["black_pieces"] = Board.bitboards.color[Black];
            board["pawns"] = Board.bitboards.pieces[Pawn];
            board["rooks"] = Board.bitboards.pieces[Rook];
            board["knights"] = Board.bitboards.pieces[Knight];
            board["bishops"] = Board.bitboards.pieces[Bishop];
            board["queens"] = Board.bitboards.pieces[Queen];
            board["kings"] = Board.bitboards.pieces[King];
            return board;
        };
        using MoveGenerator = uint64_t (ChessBoard::*)(int, bool);

        static MoveGenerator move_generator[6];
        void pawn_promotion_logic(int square_index);
        uint64_t generate_h_quintessence(int square_index, uint64_t mask, uint64_t pieces);
        uint64_t generate_shape_translation(int square_index, ShapeMask::Mask mask);
        uint64_t generate_pawn_movement(int square_index, bool is_white);
        uint64_t generate_rook_movement(int square_index, bool = true);
        uint64_t generate_knight_movement(int square_index, bool = true);
        uint64_t generate_bishop_movement(int square_index, bool = true);
        uint64_t generate_queen_movement(int square_index, bool = true);
        uint64_t generate_king_movement(int square_index, bool is_white);
        uint64_t generate_wking_movement(int square_index);
        uint64_t generate_bking_movement(int square_index);
};
