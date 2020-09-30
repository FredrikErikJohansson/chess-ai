#include "board.h"
#include "movegen.h"

#include <random>

int main() {
    Board chessBoard;
    std::cout << 12*sizeof(Bitboard) << std::endl;;
    chessBoard.initialize();
    //chessBoard.clear();
    chessBoard.print();

    //auto asd = Bitboards::ROW_MASK[0];
    //chessBoard.print(asd);
    //chessBoard.clear();
    //chessBoard.print();

    for(int turn = 0; turn < 40; turn++) {
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


        // std::cout << "TEST: " << std::endl;
        // chessBoard.print(moveGen.get_all_moves(WHITE, &chessBoard));

        std::cout << "White move count: " << chessBoard.moves[WHITE].size() << std::endl;
        std::cout << "Black move count: " << chessBoard.moves[BLACK].size() << std::endl;
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> dist(0, chessBoard.moves[WHITE].size() - 1); // distribution in range [1, 6]
        int rand_num = dist(rng);

        moveGen.make_move(chessBoard.moves[WHITE][rand_num], &chessBoard); // First pawn do first move
        std::cout << "Moved: " << chessBoard.moves[WHITE][rand_num].type << ", castle: " << chessBoard.moves[WHITE][rand_num].castle << std::endl;
        std::cout << "White_count: " << white_count << ", Black_count: " << black_count << std::endl;
        std::cout << "From:" << std::endl;
        chessBoard.print(chessBoard.moves[WHITE][rand_num].from);
        std::cout << "To:" << std::endl;
        chessBoard.print(chessBoard.moves[WHITE][rand_num].to);
        chessBoard.print();

        std::cout << "Press any key to continue: " << std::endl;
        chessBoard.moves[WHITE].clear();
        chessBoard.moves[BLACK].clear();
        std::cin.get();

    }
    while(chessBoard.history.size() > 0) {
        Movegen moveGen = Movegen(&chessBoard);
        std::cout << "Undid move number: " <<  chessBoard.history.size() << std::endl;
        std::cout << "Moved " << chessBoard.history.top().type << std::endl;
        std::cout << "From: " << std::endl;
        chessBoard.print(chessBoard.history.top().to);
        std::cout << "To: " << std::endl;
        chessBoard.print(chessBoard.history.top().from);
        moveGen.unmake_move(&chessBoard);
        chessBoard.print();
        std::cin.get();
    }
    // moveGen.unmake_move(&chessBoard);
    // chessBoard.print();
    //}
}
