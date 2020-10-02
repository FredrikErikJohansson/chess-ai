#include "search.h"

int Search::evaluate(bool color) {
    int score = 0;
    for(int i = 0; i < 6; ++i) {
        Bitboard bb = board->pieces[color][i];
        int counter = 0;
        for (int row = 7; row >= 0; --row) {
            for (int col = 0; col <= 7; ++col) {
                if (bb & (1ULL << ((row * 8) + col))) {
                    score += piece_score[i];
                    int test = score;
                    switch (i)
                    {
                    case 0:
                        score += pawn_square_values[(color) ? counter : 63 - counter];
                        break;
                    case 1:
                        score += rook_square_values[(color) ? counter : 63 - counter];
                        break;
                    case 2:
                        score += knight_square_values[(color) ? counter : 63 - counter];
                        break;
                    case 3:
                        score += bishop_square_values[(color) ? counter : 63 - counter];
                        break;
                    case 4:
                        score += queen_square_values[(color) ? counter : 63 - counter];
                        break;
                    case 5:
                        score += king_early_square_values[(color) ? counter : 63 - counter];
                        break;
                    
                    default:
                        break;
                    }
                }
            }
            counter++;
        }
    }
    if(!color) return -score;
    return score;
}

int Search::alpha_beta_max(int alpha, int beta, int depth_left, int& iterations) {
    if (depth_left == 0) return evaluate(WHITE);
    int score = INT32_MIN;
    movegen->calculate_all_moves(WHITE);
    movegen->calculate_all_moves(BLACK);
    for (auto move : board->moves[WHITE]) {
        movegen->make_move(move, board);
        movegen->calculate_all_moves(WHITE);
        movegen->calculate_all_moves(BLACK);
        ++iterations;
        score = alpha_beta_min(alpha, beta, depth_left - 1, iterations);
        movegen->unmake_move(board);
        movegen->calculate_all_moves(WHITE);
        movegen->calculate_all_moves(BLACK);
        if(score >= beta) return beta;   // fail hard beta-cutoff
        if(score > alpha) alpha = score; // alpha acts like max in MiniMax
    }
    return alpha;
}

int Search::alpha_beta_min(int alpha, int beta, int depth_left, int& iterations) {
    if (depth_left == 0) return evaluate(BLACK);
    int score = INT32_MAX;
    movegen->calculate_all_moves(BLACK);
    movegen->calculate_all_moves(WHITE);
    for (auto move : board->moves[BLACK]) {
        movegen->make_move(move, board);
        movegen->calculate_all_moves(BLACK);
        movegen->calculate_all_moves(WHITE);
        ++iterations;
        score = alpha_beta_max( alpha, beta, depth_left - 1, iterations);
        movegen->unmake_move(board);
        movegen->calculate_all_moves(BLACK);
        movegen->calculate_all_moves(WHITE);
        if(score <= alpha) return alpha; // fail hard alpha-cutoff
        if(score < beta) beta = score; // beta acts like min in MiniMax
    }
    
   return beta;
}

// int Search::alpha_beta(Board* board, int depth, int alpha, int beta, bool color) {
//     movegen->calculate_all_moves(color);
//     movegen->calculate_all_moves(!color);
//     if (depth == 0 || board->moves[color].size() < 1) return evaluate(color);
//     if (color) {
//         int score = INT32_MIN;
//         for (auto move : board->moves[WHITE]) {
//             movegen->make_move(move, board);
//             movegen->calculate_all_moves(color);
//             movegen->calculate_all_moves(!color);
//             score = std::max(score, alpha_beta(board, depth-1, alpha, beta, BLACK));
//             movegen->unmake_move(board);
//             movegen->calculate_all_moves(color);
//             movegen->calculate_all_moves(!color);
//             alpha = std::max(alpha, score);
//             if(alpha >= beta) break;
//         }
//         return score;
//     }
//    else {
//        int score = INT32_MAX;
//        for (auto move : board->moves[BLACK]) {
//             movegen->make_move(move, board);
//             movegen->calculate_all_moves(color);
//             movegen->calculate_all_moves(!color);
//             score = std::min(score, alpha_beta(board, depth-1, alpha, beta, WHITE));
//             movegen->unmake_move(board);
//             movegen->calculate_all_moves(color);
//             movegen->calculate_all_moves(!color);
//             alpha = std::min(beta, score);
//             if(beta <= alpha) break;
//         }
//         return score;
//    }
// }