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
    std::vector<Bitboard> black_pawn_bb_pos;
    //std::vector<Bitboard> black_pawn_bb_pos_moves;
    //std::vector<Bitboard> black_pawn_bb_pos_moves;
    //std::vector<Bitboard> index2;
    //std::vector<Bitboard> index3;
    black_pawn_bb_pos = moveGen.seperate_bitboards(chessBoard.pieces[WHITE][KNIGHT]); //Seperated pawns

    std::vector<Move> moves;
    //index = moveGen.seperate_bitboards(chessBoard.get_pieces(BLACK, PAWN));
    moves = moveGen.get_moves_for(black_pawn_bb_pos[0], WHITE, KNIGHT, &chessBoard);
    
    // //for(auto ind : index) {
    // for(auto bpbp : black_pawn_bb_pos) {
    //     black_pawn_bb_pos_moves.push_back(moveGen.get_pawn_moves(bpbp, BLACK, &chessBoard)); // First pawn's moves
    // }
    // for(auto bpbpm : black_pawn_bb_pos_moves) {
    //     black_pawn_bb_pos_moves = moveGen.seperate_bitboards(bpbpm); // First pawns seperated moves
    // }


    moveGen.make_move(moves[0], &chessBoard); // First pawn do first move
    chessBoard.print();
    moveGen.unmake_move(&chessBoard);
    chessBoard.print();
    //}
}
