#include "search.h"

int Search::evaluate(bool color, int depth_left) {
    int tmp_score = 0;

    for(int i = 0; i < 6; ++i) {
        Bitboard bb = board->pieces[color][i];
        Bitboard bb_opp = board->pieces[!color][i];
        int counter = 0;
        for (int row = 7; row >= 0; --row) {
            for (int col = 0; col <= 7; ++col) {
                if (bb_opp & (1ULL << counter)) tmp_score -= piece_score[i];
                if (bb & (1ULL << counter)) {
                    tmp_score += piece_score[i]; // Material score

                    // Positional score
                    switch (i)
                    {
                    case 0:
                        tmp_score += pawn_square_values[(color) ? counter : 63 - counter];
                        tmp_score -= pawn_square_values[(!color) ? counter : 63 - counter];
                        break;
                    case 1:
                        tmp_score += rook_square_values[(color) ? counter : 63 - counter];
                        tmp_score -= rook_square_values[(!color) ? counter : 63 - counter];
                        break;
                    case 2:
                        tmp_score += knight_square_values[(color) ? counter : 63 - counter];
                        tmp_score -= knight_square_values[(!color) ? counter : 63 - counter];
                        break;
                    case 3:
                        tmp_score += bishop_square_values[(color) ? counter : 63 - counter];
                        tmp_score -= bishop_square_values[(!color) ? counter : 63 - counter];
                        break;
                    case 4:
                        tmp_score += queen_square_values[(color) ? counter : 63 - counter];
                        tmp_score -= queen_square_values[(!color) ? counter : 63 - counter];
                        break;
                    case 5:
                        tmp_score += king_early_square_values[(color) ? counter : 63 - counter];
                        tmp_score -= king_early_square_values[(!color) ? counter : 63 - counter];
                        break;
                    
                    default:
                        break;
                    }
                }
            counter++;
            }
        }
    }

    return tmp_score;//((color) ? tmp_score : -tmp_score);
}

Move Search::alpha_beta_first( int alpha, int beta, int depth_left, bool color, int& tmp_iterations) {
    movegen->calculate_all_moves();

    std::sort( board->moves[color].begin(), board->moves[color].end(), [this]( const Move& lhs, const Move& rhs )
    {   
        //if(lhs.type == rhs.type) {
            return ((piece_score[lhs.type] - piece_score[find_type(lhs)]) < (piece_score[rhs.type] - piece_score[find_type(rhs)]));
        //}
        //return (piece_score[lhs.type] < piece_score[rhs.type]);
    });
    int max_score = INT32_MIN;
    int best_score = max_score;
    int counter = 0;
    //int sign = (color) ? -1 : 1;
    std::vector<int> best_moves;
    int best_move = 0;

    for(size_t i = 0; i < board->moves[color].size(); ++i) {
        movegen->make_move(board->moves[color][i]);
        movegen->calculate_all_moves();
        ++tmp_iterations;
        best_score = -alpha_beta( -beta, -alpha, depth_left - 1, !color, tmp_iterations);

        movegen->unmake_move();
        movegen->calculate_all_moves();

        std::cout << best_score << std::endl;

        // Move repetition check
        // if(board->moves[color][counter].from != board->last_move[color].to &&
        // board->moves[color][counter].to != board->last_move[color].from) {
            // else if(best_score == max_score) { // Equally good move
            //     best_moves.push_back(counter);
            // }
        
        // alpha = std::max(alpha, best_score);
        // if(alpha >= beta) break;

        if(best_score > max_score) { // New best move
            max_score = best_score;
            //best_moves.clear();
            //best_moves.push_back(counter);
            best_move = counter;
        } 
        // if(best_score >= beta) return beta;
        // if(best_score > beta) {
        //     break;
        // }
        
        counter++;
    }

    // Return random move if there are more than one
    // if(best_moves.size() == 0) return board->moves[color][0]; 
    // board->last_move[color] = board->moves[color][Zobrist::random64() % best_moves.size()];
    return board->moves[color][best_move];
}

int Search::alpha_beta( int alpha, int beta, int depth_left, bool color, int& tmp_iterations) {
    movegen->calculate_all_moves();


    std::sort( board->moves[color].begin(), board->moves[color].end(), [this]( const Move& lhs, const Move& rhs )
    {   
        //if(lhs.type == rhs.type) {
            return ((piece_score[lhs.type] - piece_score[find_type(lhs)]) < (piece_score[rhs.type] - piece_score[find_type(rhs)]));
        //}
        //return (piece_score[lhs.type] < piece_score[rhs.type]);
    });

    int score = tt->probe_hash(depth_left, alpha, beta, color);
    if (score != valUNKNOWN) {
        return score;
    }

    int best_score = INT32_MIN;
    score = INT32_MIN;

    if(board->moves[color].size() < 1) {
        if(movegen->attacks_to_king(board->pieces[color][KING], color) != 0) {
            score = -((INT32_MAX/2) + (depth_left));
            tt->record_hash(depth_left, score, hashfEXACT, color);
            return score; // Self checkmate
        }
    }

    if(board->moves[!color].size() < 1) {
        if(movegen->attacks_to_king(board->pieces[!color][KING], !color) != 0) {
            score = ((INT32_MAX/2) + (depth_left));
            tt->record_hash(depth_left, score, hashfEXACT, color);
            return score; // Opp checkmate
        }
    }

    // Terminal node or maximum depth
    if(depth_left == 0 || board->moves[color].size() < 1) {
        score = quiesce(alpha, beta, color, depth_left+4, tmp_iterations);
        tt->record_hash(depth_left, score, hashfEXACT, color);
        return score;
    }


    for(size_t i = 0; i < board->moves[color].size(); ++i) {
        movegen->make_move(board->moves[color][i]);
        movegen->calculate_all_moves();
        ++tmp_iterations;
        score = -alpha_beta( -beta, -alpha, depth_left - 1, !color, tmp_iterations);
        movegen->unmake_move();
        movegen->calculate_all_moves();
        if( score >= beta) {
            return score;
        }
        if(score > best_score) {
            best_score = score;
            if(score > alpha) {
                alpha = score;
            }
        }
        // alpha = std::max(alpha, score);
        // if(alpha >= beta) break;
        // //if(score > alpha) alpha = score;
    }

    return best_score;
}

int Search::find_type(Move move ) {
    for (int i = 0; i < 6; ++i) {
        if((move.to & board->pieces[!move.color][i]) != 0) return i;
    }
    return move.type;
}

int Search::quiesce(int alpha, int beta, bool color, int depth_left, int& tmp_iterations) {
    // int stand_pat = evaluate(color, 0);//((color) ? evaluate(color, depth_left) : -evaluate(color, depth_left));
    // if(stand_pat >= beta) return beta;
    // if(alpha < stand_pat) alpha = stand_pat;

    
    movegen->calculate_all_moves();

    int score = tt->probe_hash(depth_left, alpha, beta, color);
    if (score != valUNKNOWN) {
        return score;
    }

    int best_score = INT32_MIN;
    score = INT32_MIN;

    // Terminal node or maximum depth
    if(board->moves[color].size() < 1) {
        if(movegen->attacks_to_king(board->pieces[color][KING], color) != 0) {
            score = -((INT32_MAX/2) + (depth_left));
            tt->record_hash(depth_left, score, hashfEXACT, color);
            return score; // Self checkmate
        }
    }

    if(board->moves[!color].size() < 1) {
        if(movegen->attacks_to_king(board->pieces[!color][KING], !color) != 0) {
            score = ((INT32_MAX/2) + (depth_left));
            tt->record_hash(depth_left, score, hashfEXACT, color);
            return score; // Opp checkmate
        }
    }

    // if(board->moves[color].size() < 1) {
    //     if(movegen->attacks_to_king(board->pieces[color][KING], color) != 0) {
    //         score = -((INT32_MAX/2) + (depth_left));
    //         tt->record_hash(depth_left, score, hashfEXACT, color);
    //         return score; // Self checkmate
    //     }
    // }

    // if(board->moves[!color].size() < 1) {
    //     if(movegen->attacks_to_king(board->pieces[!color][KING], !color) != 0) {
    //         score = ((INT32_MAX/2) + (depth_left));
    //         tt->record_hash(depth_left, score, hashfEXACT, color);
    //         return score; // Opp checkmate
    //     }
    // }

    if(depth_left == 0) {
        score = evaluate(color, 0);
        tt->record_hash(depth_left, score, hashfEXACT, color);
        return score;
    }


    Bitboard attacking_pos = movegen->under_attack(!color);
    std::sort( board->moves[color].begin(), board->moves[color].end(), [this]( const Move& lhs, const Move& rhs )
    {   
        //if(lhs.type == rhs.type) {
            return ((piece_score[lhs.type] - piece_score[find_type(lhs)]) < (piece_score[rhs.type] - piece_score[find_type(rhs)]));
        //}
        //return (piece_score[lhs.type] < piece_score[rhs.type]);
    });

    // for(size_t i = 0; i < board->moves[color].size(); ++i) {
    //     if((board->moves[color][i].to & attacking_pos) == 0) continue;
    //     std::cout << piece_score[board->moves[color][i].type] << " and " << piece_score[find_type(board->moves[color][i])] << std::endl;
    // }
    // std::cout << std::endl;

    for(size_t i = 0; i < board->moves[color].size(); ++i) {

        if((board->moves[color][i].to & attacking_pos) == 0) continue;

        movegen->make_move(board->moves[color][i]);
        movegen->calculate_all_moves();
        ++tmp_iterations;
        score = -quiesce(-beta, -alpha, !color, depth_left - 1, tmp_iterations);
        movegen->unmake_move();
        movegen->calculate_all_moves();

        if(score >= beta) {
            return score;
        }
        if(score > best_score) {
            best_score = score;
            if(score > alpha) {
                alpha = score;
            }
        }

        // alpha = std::max(alpha, score);

        // if(alpha >= beta) break;
        // if(score >= beta) return beta;
        // if(score > alpha) {
        //     alpha = score;
        // }
    }

    if(best_score == INT32_MIN) {
        best_score = evaluate(color, 0);
        tt->record_hash(depth_left, score, hashfEXACT, color);
        return best_score;
    }
    return best_score;
}
