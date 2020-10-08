#include "search.h"

int Search::evaluate(bool color, int depth_left) {
    int tmp_score = 0;

    if(board->moves[color].size() < 1) {
        if(movegen->attacks_to_king(board->pieces[color][KING], color) != 0) {
            return (INT32_MAX/max_depth)*(depth_left + 1); // Checkmate
        }
        return 0; // Draw
    }
    
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

int Search::alpha_beta( int alpha, int beta, int depth_left, bool color, int& tmp_iterations) {
    movegen->calculate_all_moves();

    // Terminal node or maximum depth
    if(depth_left == 0 || board->moves[color].size() < 1) {
        return quiesce(alpha, beta, color, depth_left, tmp_iterations);
    }

    for (auto move : board->moves[color]) {
        movegen->make_move(move);
        movegen->calculate_all_moves();
        ++tmp_iterations;
        score = -alpha_beta( -alpha, -beta, depth_left - 1, !color, tmp_iterations);
        movegen->unmake_move();
        movegen->calculate_all_moves();
        if(score >= beta) {
            return score;
        }
        if(score > alpha) alpha = score;
    }
    
   return alpha;
}

Move Search::alpha_beta_first( int alpha, int beta, int depth_left, bool color, int& tmp_iterations) {
    movegen->calculate_all_moves();
    int best_score = 0;
    int max_score = INT32_MIN;
    int counter = 0;
    int sign = (color) ? -1 : 1;
    std::vector<int> best_moves;

    for (auto move : board->moves[color]) {
        movegen->make_move(move);
        movegen->calculate_all_moves();
        ++tmp_iterations;
        best_score = sign * alpha_beta( -alpha, -beta, depth_left - 1, !color, tmp_iterations);
        movegen->unmake_move();
        movegen->calculate_all_moves();

        // Move repetition check
        if(board->moves[color][counter].from != board->last_move[color].to &&
        board->moves[color][counter].to != board->last_move[color].from) {
            if(best_score > max_score) { // New best move
                max_score = best_score;
                best_moves.clear();
                best_moves.push_back(counter);
            } else if(best_score == max_score) { // Equally good move
                best_moves.push_back(counter);
            }
        }
        
        counter++;
    }

    // Return random move if there are more than one
    if(best_moves.size() == 0) return board->moves[color][0]; 
    board->last_move[color] = board->moves[color][best_moves[rand() % best_moves.size()]];
    return board->last_move[color];
}

int Search::quiesce(int alpha, int beta, bool color, int depth_left, int& tmp_iterations) {
    int stand_pat = ((color) ? evaluate(color, depth_left) : -evaluate(color, depth_left));
    if(stand_pat >= beta) return stand_pat;
    if(alpha < stand_pat) alpha = stand_pat;

    movegen->calculate_all_moves();

    // Remove quiet moves
    Bitboard attacking_pos = movegen->under_attack(!color);
    board->moves[color].erase(
    std::remove_if(board->moves[color].begin(), board->moves[color].end(),
        [attacking_pos](const Move& move) { 
            return (!((attacking_pos) & move.to)); 
        }), board->moves[color].end());

    for(auto move : board->moves[color])  {
        movegen->make_move(move);
        movegen->calculate_all_moves();
        ++tmp_iterations;
        score = -quiesce(-beta, -alpha, !color, depth_left, tmp_iterations);
        movegen->unmake_move();
        movegen->calculate_all_moves();
        if(score >= beta) return score;
        if(score > alpha) alpha = score;
    }
    return alpha;
}
