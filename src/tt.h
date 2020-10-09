#pragma once
#include "bitboards.h"
#include "movegen.h"
#include "board.h"
#include "zobrist.h"

#define valUNKNOWN -1
#define hashfEXACT 0
#define hashfALPHA 1
#define hashfBETA 2

typedef struct tagHASHE {
    Bitboard key = 0;
    int depth = 0;
    int flags = 0;
    int value = -1;
    int best = 0;
    
} HASHE;

class TT {
public:
    TT(Board* board) { this->board = board; }
    TT() {};

    // tt
    void init_hash(int size = 256);
    int probe_hash(int depth, int alpha, int beta, bool color);
    void record_hash(int depth, int val, int hashf, bool color);
    uint32_t num_entries;
    
private:
    Board* board;
    HASHE* table;
};