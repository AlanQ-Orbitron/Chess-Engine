#pragma once

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
    protected:
        enum Color {
            White, Black
        };
        enum Pieces {
            Pawn, Rook, Knight, Bishop, Queen, King, EnPassant
        };
        static const char* to_UCI[64];
        struct GameState {
        // Bitboards
            uint64_t color[2];
            uint64_t pieces[6];
            uint64_t attack_boards[2][6][64];

            // States
            uint8_t castling;
            bool white_to_move;
            uint8_t halfmove_clock;
            uint16_t fullmove_number;

        };
        GameState Board;
        static const uint64_t HORIZONTAL = 0x00000000000000FF;
        static const uint64_t VERTICAL = 0x0101010101010101;
        static const uint64_t DIAGONAL_L = 0x8040201008040201;
        static const uint64_t DIAGONAL_R = 0x0102040810204080;
    public:
        void reset_board();
        void generate_board(godot::String board);
        godot::Array get_valid_moves();
    protected:
        static void _bind_methods();
        void generate_moves();
        static GameState fen_to_bit(godot::String board);
        static godot::String bit_to_fen(GameState board);
        static pair<int, int> index_to_rankfile(int square_index) {return {square_index % 8, square_index / 8};}; // Rank, File
        static uint64_t mirrorHorizontal (uint64_t x);
        uint64_t pop_least_significant(uint64_t* bitboard) {
            unsigned long popped_index;
            _BitScanForward64(&popped_index, *bitboard);
            *bitboard &= *bitboard -1;
            return static_cast<int>(popped_index);
        }
    private:
        void set_board(godot::Dictionary board) {
            Board.color[White] = board["white_pieces"];
            Board.color[Black] = board["black_pieces"];
            Board.pieces[Pawn] = board["pawns"];
            Board.pieces[Rook] = board["rooks"];
            Board.pieces[Knight] = board["knights"];
            Board.pieces[Bishop] = board["bishops"];
            Board.pieces[Queen] = board["queens"];
            Board.pieces[King] = board["kings"];
        };
        godot::Dictionary get_board() const {
            godot::Dictionary board;
            board["white_pieces"] = Board.color[White];
            board["black_pieces"] = Board.color[Black];
            board["pawns"] = Board.pieces[Pawn];
            board["rooks"] = Board.pieces[Rook];
            board["knights"] = Board.pieces[Knight];
            board["bishops"] = Board.pieces[Bishop];
            board["queens"] = Board.pieces[Queen];
            board["kings"] = Board.pieces[King];
            return board;
        };
        using MoveGenerator = uint64_t (ChessBoard::*)(int, bool);

        static MoveGenerator move_generator[12];
        uint64_t generate_h_quintessence(int square_index, uint64_t mask);
        uint64_t generate_pawn_movement(int square_index, bool is_white);
        uint64_t generate_wpawn_movement(int square_index);
        uint64_t generate_bpawn_movement(int square_index);
        uint64_t generate_rook_movement(int square_index, bool = true);
        uint64_t generate_knight_movement(int square_index, bool = true);
        uint64_t generate_bishop_movement(int square_index, bool = true);
        uint64_t generate_queen_movement(int square_index, bool = true);
        uint64_t generate_king_movement(int square_index, bool = true);
};
