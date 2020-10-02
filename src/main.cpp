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

        
        moveGen.calculate_all_moves(); 

        //chessBoard.print(moveGen.get_all_moves(WHITE, &chessBoard));

        std::cout << "White move count: " << chessBoard.moves[WHITE].size() << std::endl;
        std::cout << "Black move count: " << chessBoard.moves[BLACK].size() << std::endl;
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
            break;;
        }
        int depth = 5;
        int score = INT32_MIN;
        int tmp_score = 0;
        int counter = 0;
        int move_idx = 0;
        int iterations = 0;
        if(tmp_color) {
            for(auto move : chessBoard.moves[tmp_color]) {
                //std::cout << chessBoard.moves[tmp_color].size() << std::endl;
                moveGen.make_move(move, &chessBoard);
                moveGen.calculate_all_moves();
                tmp_score = search.alpha_beta_max(INT32_MIN, INT32_MAX, depth-1, iterations);
                moveGen.unmake_move(&chessBoard);
                moveGen.calculate_all_moves();
                if(score < tmp_score) {
                    score = tmp_score;
                    move_idx = counter;
                }
                counter++;
            }
            //std::cout << "SCORE FOR WHITE: " << score << std::endl;
        }
        //std::cout << move_idx << std::endl;

        std::cout << "Iterations: " << iterations << std::endl;
        std::cout << "STACK SIZE: " << chessBoard.history.size() << std::endl;

        if(tmp_color) moveGen.make_move(chessBoard.moves[tmp_color][move_idx], &chessBoard); // First pawn do first move
        else moveGen.make_move(chessBoard.moves[tmp_color][rand_num], &chessBoard); // First pawn do first move
        std::cout << "color " << tmp_color << " can castle: " << chessBoard.can_castle[tmp_color]<< std::endl;
        
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
