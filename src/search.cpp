#include "search.h"

int Search::evaluate(bool color, int depth_left) {
    int tmp_score = 0;
    
    if(board->moves[color].size() < 1) {
        if(movegen->attacks_to_king(board->pieces[color][KING], color) != 0) {
            return -((INT32_MAX/2) + depth_left);
        }
    }

    for(int i = 0; i < 6; ++i) {
        Bitboard bb_opp = board->pieces[!color][i];
        Bitboard bb = board->pieces[color][i];

        for (uint_fast16_t j = 0; j < 64; ++j) {
            if ((bb & (1ULL << j)) != 0) {
                tmp_score += piece_score[i]; // Material score

                tmp_score += board->moves[color].size() * 10;

                // Positional score
                switch (i)
                {
                case 0:
                    tmp_score += pawn_square_values[(!color) ? j : 63 - j];
                    break;
                case 1:
                    tmp_score += knight_square_values[(!color) ? j : 63 - j];
                    break;
                case 2:
                    tmp_score += bishop_square_values[(!color) ? j : 63 - j];
                    break;
                case 3:
                    tmp_score += rook_square_values[(!color) ? j : 63 - j];
                    break;
                case 4:
                    tmp_score += queen_square_values[(!color) ? j : 63 - j];
                    break;
                case 5:
                    tmp_score += king_early_square_values[(!color) ? j : 63 - j];
                    break;
                
                default:
                    break;
                }
            }

            if((bb_opp & (1ULL << j)) != 0) {
                tmp_score -= piece_score[i]; // Material score

                tmp_score -= board->moves[!color].size() * 10;

                switch (i)
                {
                case 0:
                    tmp_score -= pawn_square_values[(color) ? j : 63 - j];
                    break;
                case 1:
                    tmp_score -= knight_square_values[(color) ? j : 63 - j];
                    break;
                case 2:
                    tmp_score -= bishop_square_values[(color) ? j : 63 - j];
                    break;
                case 3:
                    tmp_score -= rook_square_values[(color) ? j : 63 - j];
                    break;
                case 4:
                    tmp_score -= queen_square_values[(color) ? j : 63 - j];
                    break;
                case 5:
                    tmp_score -= king_early_square_values[(color) ? j : 63 - j];
                    break;
                
                default:
                    break;
                }
            }
        }
    }

    return tmp_score;
}

Move Search::alpha_beta_first( int alpha, int beta, int depth_left, bool color, int& tmp_iterations, int& cutoffs, int& in_tt) {
    movegen->calculate_all_moves();

    std::sort( board->moves[color].begin(), board->moves[color].end(), [this]( const Move& lhs, const Move& rhs )
    {   
        return ((piece_score[lhs.type] - piece_score[find_attacked_type(lhs)]) < (piece_score[rhs.type] - piece_score[find_attacked_type(rhs)]));
    });

    int max_score = INT32_MIN;
    int best_score = max_score;
    int counter = 0;
    std::vector<int> best_moves;
    int best_move = 0;

    for(size_t i = 0; i < board->moves[color].size(); ++i) {
        movegen->make_move(board->moves[color][i]);
        movegen->calculate_all_moves();
        ++tmp_iterations;
        best_score = -alpha_beta( -beta, -alpha, depth_left - 1, !color, tmp_iterations, cutoffs, in_tt);

        std::cout << best_score<< std::endl;

        movegen->unmake_move();
        movegen->calculate_all_moves();

        if(best_score > max_score) { // New best move
            max_score = best_score;
            best_move = counter;
        } 
        
        counter++;
    }
    return board->moves[color][best_move];
}

int Search::alpha_beta( int alpha, int beta, int depth_left, bool color, int& tmp_iterations, int& cutoffs, int& in_tt) {
    movegen->calculate_all_moves();


    std::sort( board->moves[color].begin(), board->moves[color].end(), [this]( const Move& lhs, const Move& rhs )
    {   
        return ((piece_score[lhs.type] - piece_score[find_attacked_type(lhs)]) < (piece_score[rhs.type] - piece_score[find_attacked_type(rhs)]));
    });

    int best_score = INT32_MIN;
    score = INT32_MIN;

    // Terminal node
    if(board->moves[color].size() < 1) {
        return evaluate(color, depth_left);
    }

    // Maximum depth
    if(depth_left == 0) {
        return quiesce(alpha, beta, color, q_max_depth, tmp_iterations, cutoffs, in_tt);
    }

    for(size_t i = 0; i < board->moves[color].size(); ++i) {
        movegen->make_move(board->moves[color][i]);
        movegen->calculate_all_moves();
        ++tmp_iterations;
        score = -alpha_beta( -beta, -alpha, depth_left - 1, !color, tmp_iterations, cutoffs, in_tt);
        movegen->unmake_move();
        movegen->calculate_all_moves();
        if( score >= beta) {
            ++cutoffs;
            return score;
        }
        if(score > best_score) {
            best_score = score;
            if(score > alpha) {
                alpha = score;
            }
        }
    }

    return best_score;
}

int Search::find_attacked_type(Move const& move ) {
    for (int i = 0; i < 6; ++i) {
        if((move.to & board->pieces[!move.color][i]) != 0) return i;
    }
    return move.type;
}

int Search::quiesce(int alpha, int beta, bool color, int depth_left, int& tmp_iterations, int& cutoffs, int& in_tt) {

    movegen->calculate_all_moves();

    int best_score = INT32_MIN;
    score = INT32_MIN;

    if(depth_left == 0 || board->moves[color].size() < 1) {
        return evaluate(color, -(q_max_depth - depth_left));
    }


    Bitboard attacking_pos = movegen->under_attack(!color);
    std::sort( board->moves[color].begin(), board->moves[color].end(), [this]( const Move& lhs, const Move& rhs )
    {   
        return ((piece_score[lhs.type] - piece_score[find_attacked_type(lhs)]) < (piece_score[rhs.type] - piece_score[find_attacked_type(rhs)]));
    });

    for(size_t i = 0; i < board->moves[color].size(); ++i) {

        if((board->moves[color][i].to & attacking_pos) == 0) continue;

        movegen->make_move(board->moves[color][i]);
        movegen->calculate_all_moves();
        ++tmp_iterations;
        score = -quiesce(-beta, -alpha, !color, depth_left - 1, tmp_iterations, cutoffs, in_tt);
        movegen->unmake_move();
        movegen->calculate_all_moves();

        if(score >= beta) {
            ++cutoffs;
            return score;
        }
        if(score > best_score) {
            best_score = score;
            if(score > alpha) {
                alpha = score;
            }
        }
    }

    if(best_score == INT32_MIN) {
        return evaluate(color, -(q_max_depth - depth_left));
    }
    return best_score;
}
