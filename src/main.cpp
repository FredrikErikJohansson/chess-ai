#include "board.h"
#include "movegen.h"

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


    for(int turn = 0; turn < 1000; turn++) {

        Movegen moveGen = Movegen(&chessBoard);
        moveGen.calculate_all_moves(WHITE); 
        moveGen.calculate_all_moves(BLACK);  

        //chessBoard.print(moveGen.get_all_moves(WHITE, &chessBoard));

        std::cout << "White move count: " << chessBoard.moves[WHITE].size() << std::endl;
        std::cout << "Black move count: " << chessBoard.moves[BLACK].size() << std::endl;
        std::random_device dev;
        std::mt19937 rng(dev());
        
        bool tmp_color = false;
        if(turn % 2 == 0) tmp_color = true;
        std::uniform_int_distribution<std::mt19937::result_type> dist(0, chessBoard.moves[tmp_color].size() - 1); // distribution in range [1, 6]
        int rand_num = dist(rng);

        if(chessBoard.moves[tmp_color].size() < 1) {
            std::cout << !tmp_color << " won!" << std::endl;
            break;
        }

        moveGen.make_move(chessBoard.moves[tmp_color][rand_num], &chessBoard); // First pawn do first move
        std::cout << "Moved: " << chessBoard.moves[tmp_color][rand_num].type << ", color " << tmp_color << " can castle: " << chessBoard.can_castle[tmp_color]<< std::endl;
        
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
