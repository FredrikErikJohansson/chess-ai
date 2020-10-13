#pragma once

#include "board.h"
#include "bitboards.h"

#include <random>

class Zobrist{
public:
    static Bitboard get_key(const Board* board);
    static void init_keys();
    static Bitboard random64();
		
private:
    static std::random_device rd;
    static std::mt19937_64 gen;
    static std::uniform_int_distribution<Bitboard> dis;
    static Bitboard zobrist[64][2][6];		
};