#include "board.h"
#include "movegen.h"
#include "search.h"
#include "tt.h"
#include "player.h"

#include <chrono>

int main() {
    Board chessBoard;
    Player player;
    chessBoard.initialize();
    chessBoard.puzzle_two(0);
    chessBoard.print_visualization(chessBoard);
    Movegen moveGen = Movegen(&chessBoard);
    TT table = TT(&chessBoard);
    table.init_hash(256);
    Zobrist::init_keys();
    Search search = Search(&chessBoard, &moveGen, &table);
    int turn = 0;

    // Game loop
    while(1) { 
        moveGen.calculate_all_moves();
        std::cout << "W move count: " << chessBoard.moves[WHITE].size() << std::endl;
        std::cout << "B move count: " << chessBoard.moves[BLACK].size() << std::endl;
        
        bool is_player = false;
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

        if(!is_white_turn || !is_player) {
            // Set search depth (ply)
            search.set_max_depth(4);
            search.set_q_max_depth(4);
            int alpha = INT32_MIN;
            int beta = INT32_MAX;
            int iterations = 0;
            int cutoffs = 0;
            int in_tt = 0;

            // Search and measure time
            auto begin = std::chrono::high_resolution_clock::now();
            auto move = search.alpha_beta_first(alpha, beta, search.get_max_depth(), is_white_turn, iterations, cutoffs, in_tt);
            auto end = std::chrono::high_resolution_clock::now();

            std::cout << "Iterations: " << iterations << std::endl;
            std::cout << "Cutoffs: " << cutoffs << std::endl;
            // std::cout << "Found in tt: " << in_tt << std::endl;
            std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count() << "ms" << std::endl;
            std::cout << "State stack size: " << chessBoard.history.size() << std::endl;

            // Make found move
            moveGen.make_move(move);
        } else {
            std::string from_to;
            Move player_move;
            player_move.from = INT64_MAX;
            chessBoard.print_visualization(chessBoard);
            while(player_move.from == INT64_MAX) {
                std::cout << "Make a move: ";
                std::cin >> from_to;
                player_move = player.find_move(&chessBoard, is_white_turn, from_to);
                if(player_move.from == INT64_MAX) {
                    std::cout << "Invalid movement" << std::endl;
                }
            }

            moveGen.make_move(player_move);
        }

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
