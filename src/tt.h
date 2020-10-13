#pragma once
#include "bitboards.h"
#include "movegen.h"
#include "board.h"
#include "zobrist.h"

#define valUNKNOWN -1
#define hashfEXACT 0

typedef struct tagHASHE {
    Bitboard key = 0;
    int depth = 0;
    int value = -1;
    bool color = 0;
    
} HASHE;

class TT {
public:
    TT(Board* board) { this->board = board; }
    TT() {};

    void init_hash(int size = 512);
    int probe_hash(int depth, int alpha, int beta, bool color);
    void record_hash(int depth, int val, bool color);
    uint32_t num_entries;
    
private:
    Board* board;
    HASHE* table;
};