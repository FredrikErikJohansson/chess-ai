#include "board.h"
#include "movegen.h"
#include "search.h"

#include <random>

int main() {
    Board chessBoard;
    chessBoard.initialize();
    //chessBoard.clear();
    // chessBoard.print();
    // chessBoard.pieces[BLACK][KING] = (chessBoard.pieces[BLACK][KING] >> 7);
    // chessBoard.print();
    //chessBoard.print(Bitboards::KING_START << 9);
    // chessBoard.print(Bitboards::COLUMN_CLEAR[0]);
    Movegen moveGen = Movegen(&chessBoard);
    Search search = Search(&chessBoard, &moveGen);

    for(int turn = 0; turn < 1000; turn++) {

        
        moveGen.calculate_all_moves(&chessBoard);

        //chessBoard.print(moveGen.get_all_moves(WHITE, &chessBoard));

        std::cout << "White move count: " << chessBoard.moves[WHITE].size() << std::endl;
        std::cout << "Black move count: " << chessBoard.moves[BLACK].size() << std::endl;
        std::cout << "White pieces: " << chessBoard.num_of_pieces[WHITE] << std::endl;
        std::cout << "Black Black: " << chessBoard.num_of_pieces[BLACK] << std::endl;
        std::random_device dev;
        std::mt19937 rng(dev());
        
        bool tmp_color = false;
        if(turn % 2 == 0) tmp_color = true;
        std::uniform_int_distribution<std::mt19937::result_type> dist(0, chessBoard.moves[tmp_color].size() - 1); // distribution in range [1, 6]
        int rand_num = dist(rng);

        std::vector<Bitboard> white_king_bb_pos = moveGen.seperate_bitboards(chessBoard.pieces[WHITE][KING]);
        std::vector<Bitboard> black_king_bb_pos = moveGen.seperate_bitboards(chessBoard.pieces[BLACK][KING]);

        if(white_king_bb_pos.size() < 1) {
            std::cout << "Black won!" << std::endl;
            break;
        }
        if(black_king_bb_pos.size() < 1) {
            std::cout << "White won!" << std::endl;
            break;
        }

        if(chessBoard.moves[tmp_color].size() < 1) {
            std::cout << !tmp_color << " won!" << std::endl;
            break;
        }
        int depth = 6; // Even = defense, Uneven = aggressive
        int alpha = INT32_MIN;
        int beta = INT32_MAX;
        int score = 0;
        //int tmp_score = 0;
        //int counter = 0;
        int move_idx = 0;
        int iterations = 0;
        auto root_moves = chessBoard.moves[tmp_color].size();
        //uint i;
        // Sometime segfault (castle???)
        //if(tmp_color) {
            //#pragma omp parallel for schedule(dynamic, 1)// private(tmp_board, tmp_gen, tmp_search) 
            //for(i = 0; i < root_moves; i++) {
                //std::cout << i << std::endl;
                // Board tmp_board(chessBoard);
                // Movegen tmp_gen = Movegen(&tmp_board);
                // Search tmp_search = Search(&tmp_board, &tmp_gen);
                auto move = search.alpha_beta_first(alpha, beta, depth, tmp_color, iterations, &chessBoard);

                //chessBoard.last_move[tmp_color] = chessBoard.moves[tmp_color][move_idx];
                std::cout << "asd";
                //else move_idx = -search.alpha_beta_first(alpha, beta, depth-1, tmp_color, iterations);
                // tmp_gen.calculate_all_moves();
                // tmp_gen.make_move(chessBoard.moves[tmp_color][i], &tmp_board);
                // tmp_gen.calculate_all_moves();

                // int alpha = INT32_MIN;
                // int beta = INT32_MAX;
                // //#pragma omp single
                // int tmp_iterations = 0;
                
                // int tmp_score = -tmp_search.alpha_beta(alpha, beta, depth-1, BLACK, tmp_iterations);
                // //int tmp_score = -search.alpha_beta_min(alpha, beta, depth-1, iterations);
                // tmp_gen.unmake_move(&tmp_board);
                // std::cout << tmp_score << std::endl;
                // tmp_gen.calculate_all_moves();

                // //#pragma omp critical
                // if(score < tmp_score) {
                //     std::cout << i << std::endl;
                //     score = tmp_score;
                //     //#pragma omp atomic write
                //     move_idx = i;
                // }
                // //#pragma omp atomic write
                // iterations = iterations + tmp_iterations;
            //}
            std::cout << "SCORE FOR WHITE: " << score << std::endl;
        //}
        //std::cout << move_idx << std::endl;

        std::cout << "Iterations: " << iterations << std::endl;
        std::cout << "STACK SIZE: " << chessBoard.history.size() << std::endl;
        std::cout << "First move: " << move_idx << std::endl;
        //chessBoard.print(chessBoard.moves[tmp_color][move_idx].from);
        //if(tmp_color) moveGen.make_move(chessBoard.moves[tmp_color][move_idx], &chessBoard); // First pawn do first move
        moveGen.make_move(move, &chessBoard); // First pawn do first move
        //std::cout << "color " << tmp_color << " can castle: " << chessBoard.can_castle[tmp_color]<< std::endl;
        
        if(tmp_color) std::cout << "WHITE:" << std::endl;
        else std::cout << "BLACK:" << std::endl;
        chessBoard.print_visualization(chessBoard);

        std::cout << "Press any key to continue: " << std::endl;
        // chessBoard.moves[WHITE].clear();
        // chessBoard.moves[BLACK].clear();
        std::cin.get();

    }
    while(chessBoard.history.size() > 0) {
        Movegen moveGen = Movegen(&chessBoard);
        moveGen.unmake_move(&chessBoard);
        chessBoard.print_visualization(chessBoard);
        std::cin.get();
    }
}
