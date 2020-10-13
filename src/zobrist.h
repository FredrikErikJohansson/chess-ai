#pragma once

#include "board.h"
#include "bitboards.h"

#include <random>

class Zobrist{
public:
    static Bitboard get_key(const Board* board);
    // static Bitboard xor_from_to(Bitboard key, int piece, int from, int to);
    // static Bitboard xor_square(Bitboard key, int piece, int sq);
    // static U64 xorEP(U64 key, int sq);
    // static U64 xorCastle(U64 key, int castle);
    // static U64 xorSide(U64 key);
    static void init_keys();
    static Bitboard random64();
		
private:
    static std::random_device rd;
    static std::mt19937_64 gen;
    static std::uniform_int_distribution<Bitboard> dis;
    static Bitboard zobrist[64][2][6];		
};