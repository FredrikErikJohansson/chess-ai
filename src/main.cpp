#include "board.h"
#include "movegen.h"

#include <random>

int main() {
    Board chessBoard;
    chessBoard.initialize();

    for(int turn = 0; turn < 1000; turn++) {
        Movegen moveGen = Movegen(&chessBoard);
        std::vector<Bitboard> white_pawn_bb_pos = moveGen.seperate_bitboards(chessBoard.pieces[WHITE][PAWN]);
        std::vector<Bitboard> white_rook_bb_pos = moveGen.seperate_bitboards(chessBoard.pieces[WHITE][ROOK]);
        std::vector<Bitboard> white_knight_bb_pos = moveGen.seperate_bitboards(chessBoard.pieces[WHITE][KNIGHT]);
        std::vector<Bitboard> white_bishop_bb_pos = moveGen.seperate_bitboards(chessBoard.pieces[WHITE][BISHOP]);
        std::vector<Bitboard> white_queen_bb_pos = moveGen.seperate_bitboards(chessBoard.pieces[WHITE][QUEEN]);
        std::vector<Bitboard> white_king_bb_pos = moveGen.seperate_bitboards(chessBoard.pieces[WHITE][KING]);
        size_t white_count = white_pawn_bb_pos.size() + white_rook_bb_pos.size() + white_knight_bb_pos.size() + 
                            white_bishop_bb_pos.size() + white_queen_bb_pos.size() + white_king_bb_pos.size();

        std::vector<Bitboard> black_pawn_bb_pos = moveGen.seperate_bitboards(chessBoard.pieces[BLACK][PAWN]);
        std::vector<Bitboard> black_rook_bb_pos = moveGen.seperate_bitboards(chessBoard.pieces[BLACK][ROOK]);
        std::vector<Bitboard> black_knight_bb_pos = moveGen.seperate_bitboards(chessBoard.pieces[BLACK][KNIGHT]);
        std::vector<Bitboard> black_bishop_bb_pos = moveGen.seperate_bitboards(chessBoard.pieces[BLACK][BISHOP]);
        std::vector<Bitboard> black_queen_bb_pos = moveGen.seperate_bitboards(chessBoard.pieces[BLACK][QUEEN]);
        std::vector<Bitboard> black_king_bb_pos = moveGen.seperate_bitboards(chessBoard.pieces[BLACK][KING]);
        size_t black_count = black_pawn_bb_pos.size() + black_rook_bb_pos.size() + black_knight_bb_pos.size() + 
                            black_bishop_bb_pos.size() + black_queen_bb_pos.size() + black_king_bb_pos.size();

        std::cout << "White_count: " << white_count << ", Black_count: " << black_count << std::endl;
       
        // White moves
        for (auto p : white_pawn_bb_pos) {
            moveGen.get_moves_for(p, WHITE, PAWN, &chessBoard);
        }
        for (auto p : white_rook_bb_pos) {
            moveGen.get_moves_for(p, WHITE, ROOK, &chessBoard);
        }
        for (auto p : white_knight_bb_pos) {
            moveGen.get_moves_for(p, WHITE, KNIGHT, &chessBoard);
        }
        for (auto p : white_bishop_bb_pos) {
            moveGen.get_moves_for(p, WHITE, BISHOP, &chessBoard);
        }
        for (auto p : white_queen_bb_pos) {
            moveGen.get_moves_for(p, WHITE, QUEEN, &chessBoard);
        }
        for (auto p : white_king_bb_pos) {
            moveGen.get_moves_for(p, WHITE, KING, &chessBoard);
        }

        // Black moves
        for (auto p : black_pawn_bb_pos) {
            moveGen.get_moves_for(p, BLACK, PAWN, &chessBoard);
        }
        for (auto p : black_rook_bb_pos) {
            moveGen.get_moves_for(p, BLACK, ROOK, &chessBoard);
        }
        for (auto p : black_knight_bb_pos) {
            moveGen.get_moves_for(p, BLACK, KNIGHT, &chessBoard);
        }
        for (auto p : black_bishop_bb_pos) {
            moveGen.get_moves_for(p, BLACK, BISHOP, &chessBoard);
        }
        for (auto p : black_queen_bb_pos) {
            moveGen.get_moves_for(p, BLACK, QUEEN, &chessBoard);
        }
        for (auto p : black_king_bb_pos) {
            moveGen.get_moves_for(p, BLACK, KING, &chessBoard);
        }

        std::cout << "White move count: " << chessBoard.moves[WHITE].size() << std::endl;
        std::cout << "Black move count: " << chessBoard.moves[BLACK].size() << std::endl;
        std::random_device dev;
        std::mt19937 rng(dev());
        
        bool tmp_color = false;
        if(turn % 2 == 0) tmp_color = true;
        std::uniform_int_distribution<std::mt19937::result_type> dist(0, chessBoard.moves[tmp_color].size() - 1); // distribution in range [1, 6]
        int rand_num = dist(rng);

        if(chessBoard.moves[tmp_color].size() < 1) {
            std::cout << tmp_color << " won!" << std::endl;
            break;
        }

        if(black_king_bb_pos.size() < 1) {
            std::cout << tmp_color << "White won!" << std::endl;
            break;
        }
        if(white_king_bb_pos.size() < 1) {
            std::cout << tmp_color << "Black won!" << std::endl;
            break;
        }

        moveGen.make_move(chessBoard.moves[tmp_color][rand_num], &chessBoard); // First pawn do first move
        std::cout << "Moved: " << chessBoard.moves[tmp_color][rand_num].type << ", color " << tmp_color << " can castle: " << chessBoard.can_castle[tmp_color]<< std::endl;
        
        if(tmp_color) std::cout << "WHITE:" << std::endl;
        else std::cout << "BLACK:" << std::endl;
        chessBoard.print_visualization(chessBoard);

        std::cout << "Press any key to continue: " << std::endl;
        chessBoard.moves[WHITE].clear();
        chessBoard.moves[BLACK].clear();
        std::cin.get();

    }
    while(chessBoard.history.size() > 0) {
        Movegen moveGen = Movegen(&chessBoard);
        moveGen.unmake_move(&chessBoard);
        chessBoard.print_visualization(chessBoard);
        std::cin.get();
    }
}
