#include "search.h"

int Search::evaluate(bool color) {
    int tmp_score = board->moves[color].size(); // Weight number of moves
    for(int i = 0; i < 6; ++i) {
        Bitboard bb = board->pieces[color][i];
        int counter = 0;
        for (int row = 7; row >= 0; --row) {
            for (int col = 0; col <= 7; ++col) {
                if (bb & (1ULL << ((row * 8) + col))) {
                    tmp_score += piece_score[i];
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

// int Search::alpha_beta_max(int& alpha, int& beta, int depth_left, int& iterations) {
//     if (depth_left == 0) return evaluate(WHITE);
//     //int score = INT32_MIN;
//     //int score;
//     movegen->calculate_all_moves();
//     for (auto move : board->moves[WHITE]) {
//         movegen->make_move(move, board);
//         movegen->calculate_all_moves();
//         ++iterations;
//         score = alpha_beta_min(alpha, beta, depth_left - 1, iterations);
//         movegen->unmake_move(board);
//         movegen->calculate_all_moves();
//         if(score >= beta) return beta;   // fail hard beta-cutoff
//         if(score > alpha) alpha = score; // alpha acts like max in MiniMax
//     }
//     return alpha;
// }

// int Search::alpha_beta_min(int& alpha, int& beta, int depth_left, int& iterations) {
//     if (depth_left == 0) return -evaluate(BLACK);
//     //int score = INT32_MAX;
//     //int score;
//     movegen->calculate_all_moves();
//     for (auto move : board->moves[BLACK]) {
//         movegen->make_move(move, board);
//         movegen->calculate_all_moves();
//         ++iterations;
//         score = alpha_beta_max( alpha, beta, depth_left - 1, iterations);
//         movegen->unmake_move(board);
//         movegen->calculate_all_moves();
//         if(score <= alpha) return alpha; // fail hard alpha-cutoff
//         if(score < beta) beta = score; // beta acts like min in MiniMax
//     }
    
//    return beta;
// }

int Search::alpha_beta( int alpha, int beta, int depth_left, bool color, int& tmp_iterations) {
   if (depth_left == 0) return quiesce(alpha, beta, color);//(color) ? quiesce(alpha, beta, color) : -quiesce(alpha, beta, color);
    movegen->calculate_all_moves();
    for (auto move : board->moves[color]) {
        movegen->make_move(move, board);
        movegen->calculate_all_moves();
        ++tmp_iterations;
        score = -alpha_beta( -alpha, -beta, depth_left - 1, !color, tmp_iterations);
        movegen->unmake_move(board);
        movegen->calculate_all_moves();
        if(score >= beta) {
            return score; // fail hard alpha-cutoff
        }
        if(score > alpha) alpha = score; // beta acts like min in MiniMax
    }
    
   return alpha;
}

int Search::alpha_beta_first( int alpha, int beta, int depth_left, bool color, int& tmp_iterations) {
    movegen->calculate_all_moves();
    int tmp_score = 0;
    int max_score = 0;
    int counter = 0;
    int sign = (color) ? -1 : 1;
    std::vector<int> best_moves;
    for (auto move : board->moves[color]) {
        movegen->make_move(move, board);
        movegen->calculate_all_moves();
        ++tmp_iterations;
        tmp_score = sign * alpha_beta( -alpha, -beta, depth_left - 1, !color, tmp_iterations);
        //std::cout << tmp_score << std::endl;
        movegen->unmake_move(board);
        movegen->calculate_all_moves();
        if(tmp_score > max_score) {
            max_score = tmp_score;
            best_moves.clear();
            best_moves.push_back(counter);
        } else if(tmp_score == max_score) {
            best_moves.push_back(counter);
        }
        counter++;
    }
    std::cout << best_moves.size() << std::endl;
    return best_moves[rand() % best_moves.size()];
}

int Search::quiesce(int alpha, int beta, bool color) {
    int stand_pat = ((color) ? evaluate(color) : -evaluate(color));
    if(stand_pat >= beta) return stand_pat;
    if(alpha < stand_pat) alpha = stand_pat;

    movegen->calculate_all_moves();
    // std::vector<Move> capture_moves;
    // std::back_insert_iterator< std::vector<Move>> back_it (capture_moves);

    // for(auto move : board->moves[color])
    //     if(move.capture) std:: cout << "Capture!!: " << std::endl;

    Bitboard attacked_pos = movegen->under_attack(!color);
    //board->print(attacked_pos);
    board->moves[color].erase(
    std::remove_if(board->moves[color].begin(), board->moves[color].end(),
        [attacked_pos](const Move& move) { 
            return !(attacked_pos & move.to); 
            }),
    board->moves[color].end());

    // board->moves[!color].erase(
    // std::remove_if(board->moves[!color].begin(), board->moves[!color].end(),
    //     [](const Move& move) { return !move.capture; }),
    // board->moves[!color].end());

    //for(int i = 0; i < 2; i++) {
    for(auto move : board->moves[color])  {
        //if(move.capture) 
        movegen->make_move(move, board);
        movegen->calculate_all_moves();
        score = -quiesce(-beta, -alpha, !color);
        movegen->unmake_move(board);
        movegen->calculate_all_moves();

        if(score >= beta) return score;
        if(score > alpha) alpha = score;
    }
    //}
    return alpha;
}



// int Search::alpha_beta(int depth, int& alpha, int& beta, bool color, int& iterations) {
//     movegen->calculate_all_moves();
//     if (depth == 0 || board->moves[color].size() < 1) return evaluate(color);
//     if (color) {
//         for (auto move : board->moves[WHITE]) {
//             movegen->make_move(move, board);
//             movegen->calculate_all_moves();
//             ++iterations;
//             alpha = std::max(alpha, alpha_beta(depth-1, alpha, beta, BLACK, iterations));
//             movegen->unmake_move(board);
//             movegen->calculate_all_moves();
//             if(alpha >= beta) break;
//         }
//         return alpha;
//     }
//    else {
//        for (auto move : board->moves[BLACK]) {
//             movegen->make_move(move, board);
//             movegen->calculate_all_moves();
//             ++iterations;
//             beta = std::min(beta, alpha_beta(depth-1, alpha, beta, WHITE, iterations));
//             movegen->unmake_move(board);
//             movegen->calculate_all_moves();
//             if(beta <= alpha) break;
//         }
//         return beta;
//    }
// }