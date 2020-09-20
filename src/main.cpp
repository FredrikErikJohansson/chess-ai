#include "board.h"
#include "movegen.h"

int main() {
    Board chessBoard;
    std::cout << 12*sizeof(Bitboard) << std::endl;;
    chessBoard.initialize();
    chessBoard.print();
    chessBoard.clear();
    chessBoard.print();

    Movegen moveGen = Movegen(&chessBoard);
    auto test = moveGen.get_king_moves(BLACK, &chessBoard);
    // TODO: Geneterate moves from the bitboard instead of pos
    chessBoard.print(test);
}
