#include "board.h"
#include "movegen.h"
#include "search.h"
#include "tt.h"

int main() {
    Board chessBoard;
    chessBoard.initialize();
    Movegen moveGen = Movegen(&chessBoard);
    TT table = TT(&chessBoard);
    table.init_hash(512);
    Zobrist::init_keys();
    Search search = Search(&chessBoard, &moveGen, &table);
    int turn = 0;

    // Game loop
    while(1) { 
        moveGen.calculate_all_moves();
        std::cout << "White move count: " << chessBoard.moves[WHITE].size() << std::endl;
        std::cout << "Black move count: " << chessBoard.moves[BLACK].size() << std::endl;
        std::cout << "White piece count: " << chessBoard.num_of_pieces[WHITE] << std::endl;
        std::cout << "Black piece count: " << chessBoard.num_of_pieces[BLACK] << std::endl;
        
        bool is_white_turn = false;
        if(turn % 2 == 0) is_white_turn = true;
 
        std::vector<Bitboard> white_king_bb_pos = moveGen.seperate_bitboards(chessBoard.pieces[WHITE][KING]);
        std::vector<Bitboard> black_king_bb_pos = moveGen.seperate_bitboards(chessBoard.pieces[BLACK][KING]);

        if(white_king_bb_pos.size() < 1) {
            std::cout << "\nBlack won!" << std::endl;
            std::cout << "Press ENTER to continue: " << std::endl;
            std::cin.get();
            break;
        } 

        if(black_king_bb_pos.size() < 1) {
            std::cout << "\nWhite won!" << std::endl;
            std::cout << "Press ENTER to continue: " << std::endl;
            std::cin.get();
            break;
        } 

        if(chessBoard.moves[is_white_turn].size() < 1) {
            std::cout << "\n" << ((!is_white_turn) ? "White " : "Black ") << " won!" << std::endl;
            std::cout << "Press ENTER to continue: " << std::endl;
            std::cin.get();
            break;
        } 

        // Set search depth (ply)
        search.set_max_depth(8);
        int alpha = INT32_MIN;
        int beta = INT32_MAX;
        int iterations = 0;

        // Search
        auto move = search.alpha_beta_first(alpha, beta, search.get_max_depth(), is_white_turn, iterations);

        std::cout << "Iterations: " << iterations << std::endl;
        std::cout << "State stack size: " << chessBoard.history.size() << std::endl;

        // Make found move
        moveGen.make_move(move);

        std::cout << std::endl;
        if(is_white_turn) std::cout << "White's move:" << std::endl;
        else std::cout << "Black's move:" << std::endl;
        chessBoard.print_visualization(chessBoard);
        
        turn++;
        std::cout << std::endl;
        std::cout << "Press ENTER to continue: " << std::endl;
        std::cin.get();
    }

    return 1;
}
