#include "tt.h"



void TT::init_hash(int size){
	num_entries = (size * 0x100000)/sizeof(HASHE);

	//if not power of two already
	if (num_entries & (num_entries - 1)) {

        num_entries--;
        for (int i = 1; i < 32; i = i*2)
            num_entries |= num_entries >> i;
        num_entries++;
        num_entries>>= 1;
    }

    table = new HASHE[num_entries];
    //std::cout << "Hash Table size: " << num_entries * sizeof(HASHE)/0x100000 << " MB" << std::endl;
}

int TT::probe_hash(int depth, bool color) {

    HASHE* phashe = &table[Zobrist::get_key(board) % num_entries];
    if (phashe->key == Zobrist::get_key(board)) {
        if (phashe->depth >= depth && phashe->color == color) {
            return phashe->value;
        }

        //RememberBestMove();
    }

    return valUNKNOWN;
}

void TT::record_hash(int depth, int val, bool color) {

    HASHE* phashe = &table[Zobrist::get_key(board) % num_entries];

    phashe->key = Zobrist::get_key(board);
    phashe->value = (color) ? val : -val;
    phashe->depth = depth;
    phashe->color = color;
}

 
