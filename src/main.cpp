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

        std::vector<Move> moves;
        for (auto p : white_pawn_bb_pos) {
            std::vector<Move> tmp;
            tmp = moveGen.get_moves_for(p, WHITE, PAWN, &chessBoard);
            moves.insert( moves.end(), tmp.begin(), tmp.end() );
        }
        for (auto p : white_rook_bb_pos) {
            std::vector<Move> tmp;
            tmp = moveGen.get_moves_for(p, WHITE, ROOK, &chessBoard);
            moves.insert( moves.end(), tmp.begin(), tmp.end() );
        }
        for (auto p : white_knight_bb_pos) {
            std::vector<Move> tmp;
            tmp = moveGen.get_moves_for(p, WHITE, KNIGHT, &chessBoard);
            moves.insert( moves.end(), tmp.begin(), tmp.end() );
        }
        for (auto p : white_bishop_bb_pos) {
            std::vector<Move> tmp;
            tmp = moveGen.get_moves_for(p, WHITE, BISHOP, &chessBoard);
            moves.insert( moves.end(), tmp.begin(), tmp.end() );
        }
        for (auto p : white_queen_bb_pos) {
            std::vector<Move> tmp;
            tmp = moveGen.get_moves_for(p, WHITE, QUEEN, &chessBoard);
            moves.insert( moves.end(), tmp.begin(), tmp.end() );
        }

        for (auto p : white_king_bb_pos) {
            std::vector<Move> tmp;
            tmp = moveGen.get_moves_for(p, WHITE, KING, &chessBoard);
            moves.insert( moves.end(), tmp.begin(), tmp.end() );
        }

        std::cout << "TEST: " << std::endl;
        chessBoard.print(moveGen.get_all_moves(BLACK, &chessBoard));

        std::cout << "Length of moves: " << moves.size() << std::endl;
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> dist(0, moves.size() - 1); // distribution in range [1, 6]
        int rand_num = dist(rng);

        moveGen.make_move(moves[rand_num], &chessBoard); // First pawn do first move
        std::cout << "Moved: " << moves[rand_num].type << ", castle: " << moves[rand_num].castle << std::endl;
        std::cout << "White_count: " << white_count << ", Black_count: " << black_count << std::endl;
        std::cout << "From:" << std::endl;
        chessBoard.print(moves[rand_num].from);
        std::cout << "To:" << std::endl;
        chessBoard.print(moves[rand_num].to);
        chessBoard.print();

        std::cout << "Press any key to continue: " << std::endl;
        moves.clear();
        white_pawn_bb_pos.clear();
        white_rook_bb_pos.clear();
        white_knight_bb_pos.clear();
        white_bishop_bb_pos.clear();
        white_queen_bb_pos.clear();
        white_king_bb_pos.clear();
        black_pawn_bb_pos.clear();
        black_rook_bb_pos.clear();
        black_knight_bb_pos.clear();
        black_bishop_bb_pos.clear();
        black_queen_bb_pos.clear();
        black_king_bb_pos.clear();

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
