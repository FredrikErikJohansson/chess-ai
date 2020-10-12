#include "search.h"

int Search::evaluate(bool color, int depth_left) {
    int tmp_score = 0;

    // if((movegen->get_king_moves(!color) == 0)) {
    //     if(movegen->attacks_to_king(board->pieces[!color][KING], !color) != 0)
    //         return (INT32_MAX/2) + depth_left; // Checkmate

    //     if(board->moves[!color].size() < 1)
    //         return INT32_MIN; // Stalemate
    // }

    for(int i = 0; i < 6; ++i) {
        Bitboard bb = board->pieces[color][i];
        int counter = 0;
        for (int row = 7; row >= 0; --row) {
            for (int col = 0; col <= 7; ++col) {
                if (bb & (1ULL << ((row * 8) + col))) {
                    tmp_score += piece_score[i]; // Material score

                    // Positional score
                    switch (i)
                    {
                    case 0:
                        tmp_score += pawn_square_values[(color) ? counter : 63 - counter];
                        break;
                    case 1:
                        tmp_score += rook_square_values[(color) ? counter : 63 - counter];
                        break;
                    case 2:
                        tmp_score += knight_square_values[(color) ? counter : 63 - counter];
                        break;
                    case 3:
                        tmp_score += bishop_square_values[(color) ? counter : 63 - counter];
                        break;
                    case 4:
                        tmp_score += queen_square_values[(color) ? counter : 63 - counter];
                        break;
                    case 5:
                        tmp_score += king_early_square_values[(color) ? counter : 63 - counter];
                        break;
                    
                    default:
                        break;
                    }
                }
            }
            counter++;
        }
    }
    return tmp_score;
}

Move Search::alpha_beta_first( int alpha, int beta, int depth_left, bool color, int& tmp_iterations) {
    movegen->calculate_all_moves();
    int best_score = 0;
    int max_score = INT32_MIN;
    int counter = 0;
    //int sign = (color) ? -1 : -1;
    std::vector<int> best_moves;

    for (size_t i = 0; i < board->moves[color].size(); ++i) {
        movegen->make_move(board->moves[color][i]);
        movegen->calculate_all_moves();
        ++tmp_iterations;
        best_score = alpha_beta( -alpha, -beta, depth_left - 1, color, tmp_iterations);
        std::cout << best_score << std::endl;
        movegen->unmake_move();
        movegen->calculate_all_moves();

        // Move repetition check
        // if(board->moves[color][counter].from != board->last_move[color].to &&
        // board->moves[color][counter].to != board->last_move[color].from) {
            if(best_score > max_score) { // New best move
                max_score = best_score;
                best_moves.clear();
                best_moves.push_back(counter);
            } else if(best_score == max_score) { // Equally good move
                best_moves.push_back(counter);
            }
       // }
        
        counter++;
    }

    // Return random move if there are more than one
    if(best_moves.size() == 0) return board->moves[color][0]; 
    board->last_move[color] = board->moves[color][Zobrist::random64() % best_moves.size()];
    return board->last_move[color];
}

int Search::alpha_beta( int alpha, int beta, int depth_left, bool color, int& tmp_iterations) {
    movegen->calculate_all_moves();
    //score = INT32_MIN;

    if((movegen->get_king_moves(!color) == 0)) {
        if(movegen->attacks_to_king(board->pieces[!color][KING], !color) != 0)
            return ((INT32_MAX/2) + depth_left); // Checkmate

        if(board->moves[!color].size() < 1)
            return INT32_MIN; // Stalemate
    }

    // if ((score = tt->probe_hash(depth_left, alpha, beta, color)) != valUNKNOWN) {
    //     return score;
    // }

    // Terminal node or maximum depth
    if(depth_left == 0) {
        //score = quiesce(alpha, beta, color, depth_left, tmp_iterations);
        score = ((color) ? evaluate(color, depth_left) : -evaluate(color, depth_left));
        //tt->record_hash(depth_left, score, hashfEXACT, color);
        return score;
    }

    for (size_t i = 0; i < board->moves[color].size(); ++i) {
        movegen->make_move(board->moves[color][i]);
        movegen->calculate_all_moves();
        ++tmp_iterations;
        score = -alpha_beta( -alpha, -beta, depth_left - 1, !color, tmp_iterations);
        movegen->unmake_move();
        movegen->calculate_all_moves();
        //alpha = std::max(alpha, score);
        // if(alpha >= beta) {
        //     tt->record_hash(depth_left, score, hashfBETA, color);
        //     //return beta; // fail-soft beta-cutoff
        //     break;
        // }
        // if(score > alpha) {
        //     alpha = score;
        // }
    }
    //tt->record_hash(depth_left, score, hashfALPHA, color);
    return score;
}

int Search::quiesce(int alpha, int beta, bool color, int depth_left, int& tmp_iterations) {
    movegen->calculate_all_moves();
    int stand_pat = evaluate(color, depth_left);//((color) ? evaluate(color, depth_left) : -evaluate(color, depth_left));
    if(stand_pat >= beta) return beta;
    if(alpha < stand_pat) alpha = stand_pat;


    // Remove quiet moves
    Bitboard attacking_pos = movegen->under_attack(!color);
    // board->moves[color].erase(
    // std::remove_if(board->moves[color].begin(), board->moves[color].end(),
    //     [attacking_pos](const Move& move) { 
    //         return (!((attacking_pos) & move.to)); 
    //     }), board->moves[color].end());

    for (size_t i = 0; i < board->moves[color].size(); ++i) {

        if((attacking_pos & board->moves[color][i].to) == 0) continue;

        movegen->make_move(board->moves[color][i]);
        movegen->calculate_all_moves();
        ++tmp_iterations;
        score = -quiesce(-beta, -alpha, !color, depth_left, tmp_iterations);
        movegen->unmake_move();
        movegen->calculate_all_moves();
        if(score >= beta) return beta;
        if(score > alpha) alpha = score;
    }
    return alpha;
}
