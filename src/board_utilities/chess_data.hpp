#pragma once
#include <cstring>
#include <memory>
#include <cstdint>
#include <optional>
#include <vector>

enum class Color : uint8_t {Black, White, Blocker, None, Total};
enum class Pieces : uint8_t{Pawn, Rook, Knight, Bishop, Queen, King, PassPawn, Duck, None, Total};
enum class MoveType : uint8_t {Attack, Movement, Castle, PassPawn, EnPassant, None, Total};
enum class Directions : uint8_t {Top, Bottom, Left, Right, BottomLeft, BottomRight, TopLeft, TopRight};

struct Rule;
struct Piece;

struct PieceType  {Color color; Pieces piece;};
struct ShapeGroup {uint64_t bitboard[int(MoveType::Total)]{};};
struct RankFile   {int rank, file;};
struct Size       {int width, height;};
struct Position   {int x, y;};

struct ShapeMask  {
    struct Mask   {uint64_t mask; Size size;};
    static constexpr Mask HORIZONTAL   {0x00000000000000FF, 8, 1};
    static constexpr Mask VERTICAL     {0x0101010101010101, 1, 8};
    static constexpr Mask DIAGONAL_L   {0x8040201008040201, 8, 8};
    static constexpr Mask DIAGONAL_R   {0x0102040810204080, 8, 8};
    static constexpr Mask KNIGHT_SHAPE {0x0000000A1100110A, 5, 5};
    static constexpr Mask KING_SHAPE   {0x0000000000070707, 3, 3};
    static constexpr Mask PAWN_SHAPE   {0x0000000000000500, 3, 3};
    static constexpr Mask CASTLING     {0x0000000000000011, 5, 1};
};

struct Move {
    uint8_t from{};
    uint8_t to{};
    Pieces piece = Pieces::None;
    Pieces captured = Pieces::None;
    std::vector<Pieces> promotion{};
    MoveType type = MoveType::None;
};

struct MoveList {
    std::optional<Move> moves[64][64] {};

    void add(const Move& m) {moves[m.from][m.to] = m;}
};

struct GameState {
    struct Bitboards {
        uint64_t color[int(Color::Total)];
        uint64_t pieces[int(Pieces::Total)];
        uint64_t total_pieces;
        // uint64_t moves_bitboard[int(Color::Total)][int(Pieces::Total)][64];
        uint64_t total_moves_bitboard[int(Color::Total)][int(MoveType::Total)] {};
        uint64_t pins[int(Color::Total)]{};
        uint64_t pass_pawns[int(Color::Total)]{};

        inline void update_total_pieces() {
            total_pieces = color[int(Color::White)] | color[int(Color::Black)];
        }
    } bitboards;

    struct States {
        uint8_t board_size = 64;
        bool white_to_move = true;
        bool in_check = false;
        PieceType piece_at_index[64];
        Pieces royal = Pieces::King;

        uint8_t castling[int(Color::Total)]{};
        uint8_t EnPassant[int(Color::Total)];

        uint8_t halfmove_clock;
        uint16_t fullmove_number;
    } states;

    struct RuleSet {
        std::vector<std::unique_ptr<Rule>> modified_rules;
        std::vector<std::unique_ptr<Piece>> enabled_piece_type;

        RuleSet(const RuleSet&) = delete;
        RuleSet& operator=(const RuleSet&) = delete;
        RuleSet() = default;
    } ruleSet;

    

    mutable MoveList move_list[int(Color::Total)]{};

    inline void reset_board() {
        memset(bitboards.pins, 0, sizeof(bitboards.pins));
        memset(bitboards.total_moves_bitboard, 0, sizeof(bitboards.total_moves_bitboard));
        for (MoveList &move : move_list) {
            move = {};
        }
    };
    inline void reset_start(){
        memset(bitboards.pass_pawns, 0, sizeof(bitboards.pass_pawns));
        memset(bitboards.color, 0, sizeof(bitboards.color));
        memset(bitboards.pieces, 0, sizeof(bitboards.pieces));
        memset(states.castling, 0, sizeof(states.castling));
    };
} static Board;



