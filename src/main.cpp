#include "board.h"
#include "movegen.h"

int main() {
    Board chessBoard;
    std::cout << 12*sizeof(Bitboard) << std::endl;;
    chessBoard.initialize();
    chessBoard.print();
    //chessBoard.clear();
    //chessBoard.print();

    Movegen moveGen = Movegen(&chessBoard);
    std::vector<Bitboard> index;
    //std::vector<Bitboard> index2;
    //std::vector<Bitboard> index3;
    index = moveGen.seperate_bitboards(chessBoard.get_pieces(BLACK, PAWN));

    //index = moveGen.seperate_bitboards(chessBoard.get_pieces(BLACK, PAWN));

    std::vector<Bitboard> move;
    //for(auto ind : index) {
    auto test = moveGen.get_pawn_moves(index[0], BLACK, &chessBoard);
    move = moveGen.seperate_bitboards(test);
    moveGen.make_move(index[0], move[0], BLACK, PAWN, &chessBoard);
    chessBoard.print();
    //}
}
