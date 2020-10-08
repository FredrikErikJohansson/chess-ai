#pragma once
#include <vector>
#include <cmath>
#include <algorithm>
#include <cassert>

#include "bitboards.h"
#include "board.h"

class Movegen {
public:
    Movegen(Board* board) { this->board = board; }
    Movegen() {};

    // Maybe remove board from argument (everywhere)
    void make_move(Move, bool from_check = false);
    void unmake_move();

    // Get board index from bitboard
    std::vector<Bitboard> seperate_bitboards(Bitboard const&);

    void get_moves_for(Bitboard bb, bool color, unsigned int type);

    Bitboard get_bishop_moves(Bitboard bb, bool color);
    Bitboard get_knight_moves(Bitboard bb, bool color);
    Bitboard get_pawn_moves(Bitboard bb, bool color);
    Bitboard get_queen_moves(Bitboard bb, bool color);
    Bitboard get_rook_moves(Bitboard bb, bool color);
    Bitboard get_king_moves(bool color);
    Bitboard get_all_moves(bool color);

    Bitboard under_attack(bool color);

    void calculate_all_moves();
    Bitboard attacks_to_king(Bitboard bb, bool color);

    bool check(Move&);

private:
    Board* board;
};

