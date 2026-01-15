#pragma once
#include "board_utilities/chess_data.hpp"
#include "piece.hpp"

struct Rook : virtual Piece {
    Rook() {piece_type = Pieces::Rook;}
    uint64_t generate_movement_moves(int square_index, GameState &board) const override {
        RankFile rankfile_index = index_to_rankfile(square_index);
        
        return
            generate_h_quintessence(square_index, ShapeMask::VERTICAL.mask << rankfile_index.rank, board.bitboards.all_pieces) |
            generate_h_quintessence(square_index, ShapeMask::HORIZONTAL.mask << 8 * rankfile_index.file, board.bitboards.all_pieces);
    }
};
