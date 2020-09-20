#pragma once
#include "bitboards.h"
#include "board.h"

class Movegen {
public:
    Movegen(Board* board) { this->board = board; }
    Movegen(){};

    Bitboard get_bishop_moves(uint_fast16_t position, bool color, Board*);
    Bitboard get_knight_moves(uint_fast16_t position, bool color, Board*);
    Bitboard get_pawn_moves(uint_fast16_t position, bool color, Board*);
    Bitboard get_queen_moves(uint_fast16_t position, bool color, Board*);
    Bitboard get_rook_moves(uint_fast16_t position, bool color, Board*);
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


