#pragma once
#include <cstdint>
#include "board_utilities/chess_data.hpp"
#include "piece.hpp"

struct Bishop : virtual Piece {
    Bishop() {piece_type = Pieces::Bishop;}
    void generate_movement_moves(int square_index, GameState &board) const override {
        RankFile rankfile_index = index_to_rankfile(square_index);
        int difference_L = rankfile_index.rank - rankfile_index.file;
        int difference_R = (7 - rankfile_index.rank) - rankfile_index.file;
        
        uint64_t D1 = (difference_L > 0) ? ShapeMask::DIAGONAL_L.mask >> 8 * difference_L: ShapeMask::DIAGONAL_L.mask << 8 * abs(difference_L);
        uint64_t D2 = (difference_R > 0) ? ShapeMask::DIAGONAL_R.mask >> 8 * difference_R: ShapeMask::DIAGONAL_R.mask << 8 * abs(difference_R);

        
        shape_group.movement_bitboard = (D1 | D2) & (
            generate_h_quintessence(square_index, D1, board.bitboards.all_pieces) |
            generate_h_quintessence(square_index, D2, board.bitboards.all_pieces));
    }
	const ShapeMask::Mask *shapes[2] = { &ShapeMask::DIAGONAL_L, &ShapeMask::DIAGONAL_R };
	const int direction[4] = { int(Directions::BottomLeft), int(Directions::TopLeft), int(Directions::BottomRight), int(Directions::TopRight) };
	const Dimensions offsets[4] = { { 0, 0 }, { 7, 7 }, { 7, 0 }, { 0, 7 } };
};
