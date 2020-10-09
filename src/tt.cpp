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
    //numEntries_1 = num_entries - 1;
    table = new HASHE[num_entries];

    // for (int i = 0; i < num_entries; i++){
    // 	table[i].key = 0;
    //     table[i].depth = 0;
    //     table[i].flags = 0;
    //     table[i].best = 0;    	
    // }

    // newWrite = 0;
    // overWrite = 0;
    // hit = 0;
    // cut = 0;
    std::cout << "Hash Table size: " << num_entries * sizeof(HASHE)/0x100000 << " MB" << std::endl;
}

int TT::probe_hash(int depth, int alpha, int beta, bool color) {

    HASHE* phashe = &table[Zobrist::get_key(board) % num_entries];
    if (phashe->key == Zobrist::get_key(board)) {
        if (phashe->depth >= depth) {
            if (phashe->flags == hashfEXACT)
                return phashe->value;
            // if ((phashe->flags == hashfALPHA) && (phashe->value <= alpha))
            //     return alpha;
            // if ((phashe->flags == hashfBETA) && (phashe->value >= beta))
            //     return beta;
        }

        //RememberBestMove();

    }

    return valUNKNOWN;
}

void TT::record_hash(int depth, int val, int hashf, bool color) {

    HASHE* phashe = &table[Zobrist::get_key(board) % num_entries];

    phashe->key = Zobrist::get_key(board);
    phashe->best = 0;
    phashe->value = val;
    phashe->flags = hashf;
    phashe->depth = depth;
}

 
