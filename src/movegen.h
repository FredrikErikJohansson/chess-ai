#pragma once
#include <vector>
#include <cmath>

#include "bitboards.h"
#include "board.h"

struct Move {
    Bitboard from;
    Bitboard to;
    uint color;
    uint type;
};

class Movegen {
public:
    Movegen(Board* board) { this->board = board; }
    Movegen() {};

    void make_move(Bitboard& from, Bitboard& to, bool color, uint type, Board*);

    // Get board index from bitboard
    std::vector<Bitboard> seperate_bitboards(Bitboard const&);

    Bitboard get_bishop_moves(Bitboard bb, bool color, Board*);
    Bitboard get_knight_moves(Bitboard bb, bool color, Board*);
    Bitboard get_pawn_moves(Bitboard bb, bool color, Board*);
    Bitboard get_queen_moves(Bitboard bb, bool color, Board*);
    Bitboard get_rook_moves(Bitboard bb, bool color, Board*);
    Bitboard get_king_moves(bool color, Board*);
    Bitboard get_all_moves(bool color, Board*);

    // bool check(bool, Board*);
    // bool checkmate(bool color, Board*);

    // bool isValidMove(uint_fast16_t userMove);
    // void uncheckedMove(bool, short, uint_fast16_t, uint_fast16_t, Board*);

private:
    Board* board;
    // bool checkmate1(bool color, Bitboard);
	// bool checkmate2(bool color, Board*);
};


