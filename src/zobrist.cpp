#include "zobrist.h"

std::random_device Zobrist::rd;
std::mt19937_64 Zobrist::gen;
std::uniform_int_distribution<Bitboard> Zobrist::dis;
Bitboard Zobrist::zobrist[64][2][6];

Bitboard Zobrist::get_key(Board const* board) {
	Bitboard key = 0;

	//pieces
    uint_fast64_t pos = 0b1;
    for (uint_fast16_t i = 0; i < 64; ++i) {
        Bitboard idx = (board->get_all_pieces() & (pos << i));
        for(uint_fast16_t j = 0; j < 6; ++j) { 
            if((idx & board->pieces[WHITE][j]) != 0) {
                key ^= zobrist[i][WHITE][j];
            } 
            else if((idx & board->pieces[BLACK][j]) != 0) {
                key ^= zobrist[i][BLACK][j];
            }
        }   
    }  

	return key;
}

Bitboard Zobrist::random64(){
	return dis(gen);
}

void Zobrist::init_keys(){
	//piece keys
	for (int sq = 0; sq < 64; ++sq) {
        for(int color = 0; color < 2; ++color) {
            for (int p = 0; p < 6; ++p){
                zobrist[sq][color][p] = random64();
            }
        }
	}
}