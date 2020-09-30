#pragma once
#include <vector>
#include <cmath>

#include "bitboards.h"
#include "board.h"

class Movegen {
public:
    Movegen(Board* board) { this->board = board; }
    Movegen() {};

    // Maybe remove board from argument (everywhere)
    void make_move(Move&, Board*);
    void unmake_move(Board*);

    // Get board index from bitboard
    std::vector<Bitboard> seperate_bitboards(Bitboard const&);

    //Move get_pawn_moves(Bitboard bb, bool color, Board*);
    std::vector<Move> get_moves_for(Bitboard bb, bool color, uint type, Board*);

    Bitboard get_bishop_moves(Bitboard bb, bool color, Board*);
    Bitboard get_knight_moves(Bitboard bb, bool color, Board*);
    Bitboard get_pawn_moves(Bitboard bb, bool color, Board*);
    Bitboard get_queen_moves(Bitboard bb, bool color, Board*);
    Bitboard get_rook_moves(Bitboard bb, bool color, Board*);
    Bitboard get_king_moves(bool color, Board*);
    Bitboard get_all_moves(bool color, Board*);

    bool check(Move&);
    // bool checkmate(bool color, Board*);

    // bool isValidMove(uint_fast16_t userMove);
    // void uncheckedMove(bool, short, uint_fast16_t, uint_fast16_t, Board*);

private:
    Board* board;
    // bool checkmate1(bool color, Bitboard);
	// bool checkmate2(bool color, Board*);
};

