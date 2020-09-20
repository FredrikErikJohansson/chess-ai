#include "board.h"
#include "movegen.h"

int main() {
    Board chessBoard;
    chessBoard.initialize();

    Movegen moveGen = Movegen(&chessBoard);
    auto test = moveGen.get_all_moves(BLACK, &chessBoard);
    chessBoard.print(test);
}
