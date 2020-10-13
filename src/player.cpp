#include "player.h"

Move Player::find_move(Board* board, bool color, std::string from_to) {
    Move move;
    Bitboard pos = 0b1;
    if(from_to.size() != 4) { // Wrong input 
        move.from = INT64_MAX;
        return move; 
    }

    std::string from = from_to.substr(0,2);
    std::string to = from_to.substr(2,2);
    for (int row = 7; row >= 0; --row) {
        for (int col = 0; col <= 7; ++col) {
            int counter = ((row * 8) + col);
            if(move_to_ind[63 - counter] == from) move.from = (pos << counter);
            if(move_to_ind[63 - counter] == to) move.to = (pos << counter);
        }
    }

    for(unsigned int i = 0; i < board->moves[color].size(); ++i) {
        if(board->moves[color][i].from == move.from &&
            board->moves[color][i].to == move.to) return board->moves[color][i];
    }

    // No move found
    move.from = INT64_MAX;
    return move; 
}
