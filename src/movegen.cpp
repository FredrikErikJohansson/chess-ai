#include "movegen.h"
#include "magics.h"
#include "bitboards.h"
#include "board.h"

Bitboard Movegen::get_bishop_moves(uint_fast16_t pos, bool color, Board* board) {
    Bitboard index = ((board->get_all_pieces() & BOccupancy[pos]) * BMagic[pos]) >> (64-BBits[pos]);
    return bishopMoves[pos][index] & ~(color ? board->get_all_white_pieces() : board->get_all_black_pieces());
}

Bitboard Movegen::get_rook_moves(uint_fast16_t pos, bool color, Board* board) {
    Bitboard index = ((board->get_all_pieces() & ROccupancy[pos]) * RMagic[pos]) >> (64-RBits[pos]);
    return rookMoves[pos][index] & ~(color ? board->get_all_white_pieces() : board->get_all_black_pieces());
}

Bitboard Movegen::get_knight_moves(uint_fast16_t pos, bool color, Board* board) {
    Bitboard start = static_cast<Bitboard>(1) << pos;
    Bitboard clear_A = Bitboards::COLUMN_CLEAR[0];
    Bitboard clear_AB = Bitboards::COLUMN_CLEAR[0] & Bitboards::COLUMN_CLEAR[1];
    Bitboard clear_H = Bitboards::COLUMN_CLEAR[7];
    Bitboard clear_GH = Bitboards::COLUMN_CLEAR[6] & Bitboards::COLUMN_CLEAR[7];
    Bitboard own_pieces;
    if (color) own_pieces = board->get_all_white_pieces();
    else own_pieces = board->get_all_black_pieces();

    return ((((start & clear_A) << 15) | ((start & clear_AB) << 6) | 
            ((start & clear_AB) >> 10) | ((start & clear_A) >> 17) | 
            ((start & clear_H) >> 15) | ((start & clear_GH) >> 6) | 
            ((start & clear_GH) << 10) | (start & clear_H) << 17) & ~own_pieces);
}

Bitboard Movegen::get_king_moves(bool color, Board* board) {
    Bitboard left_attack;
    Bitboard right_attack;
    Bitboard forward_attack;
    Bitboard backward_attack;
    Bitboard left_forward_diagonal_attack;
    Bitboard right_forward_diagonal_attack;
    Bitboard left_backward_diagonal_attack;
    Bitboard right_backward_diagonal_attack;
    Bitboard all_moves_possible;

    // TODO: Castle
    // Add flag if king or rook has moved
    Bitboard left_castle;
    Bitboard right_castle;
    if (color) {
        left_castle = (((board->get_pieces(color, KING) & (Bitboards::KING_START & Bitboards::ALL_WHITE_START)) >> 3) & 
            (board->get_pieces(color, ROOK) & (Bitboards::ROOK_START & Bitboards::ALL_WHITE_START)) << 1);
        right_castle = (((board->get_pieces(color, KING) & (Bitboards::KING_START & Bitboards::ALL_WHITE_START)) << 2) & 
            (board->get_pieces(color, ROOK) & (Bitboards::ROOK_START & Bitboards::ALL_WHITE_START)) >> 1);
    } else {
        right_castle = (((board->get_pieces(color, KING) & (Bitboards::KING_START & Bitboards::ALL_BLACK_START)) >> 3) & 
            (board->get_pieces(color, ROOK) & (Bitboards::ROOK_START & Bitboards::ALL_BLACK_START)) << 1);
        left_castle = (((board->get_pieces(color, KING) & (Bitboards::KING_START & Bitboards::ALL_BLACK_START)) << 2) & 
            (board->get_pieces(color, ROOK) & (Bitboards::ROOK_START & Bitboards::ALL_BLACK_START)) >> 1);
    }

    left_attack = ((Bitboards::COLUMN_CLEAR[0] & (board->get_pieces(color, KING)) >> 1));
    right_attack = ((Bitboards::COLUMN_CLEAR[7] & (board->get_pieces(color, KING)) << 1));
    forward_attack = (board->get_pieces(color, KING) << 8);
    backward_attack = (board->get_pieces(color, KING) >> 8);
    left_forward_diagonal_attack = ((Bitboards::COLUMN_CLEAR[0] & (board->get_pieces(color, KING)) << 7));
    right_forward_diagonal_attack = ((Bitboards::COLUMN_CLEAR[7] & (board->get_pieces(color, KING)) << 9));
    left_backward_diagonal_attack = ((Bitboards::COLUMN_CLEAR[0] & (board->get_pieces(color, KING)) >> 9));
    right_backward_diagonal_attack = ((Bitboards::COLUMN_CLEAR[7] & (board->get_pieces(color, KING)) >> 7));
    all_moves_possible = (left_attack | right_attack | forward_attack | 
                        backward_attack | left_forward_diagonal_attack | 
                        right_forward_diagonal_attack | left_backward_diagonal_attack | 
                        right_backward_diagonal_attack | right_castle | left_castle);
    return all_moves_possible & ~(color ? board->get_all_white_pieces() : board->get_all_black_pieces());
}

Bitboard Movegen::get_pawn_moves(uint_fast16_t pos, bool color, Board* board) {
    Bitboard start = static_cast<Bitboard>(1) << pos;
    Bitboard left_attack;
    Bitboard right_attack;
    Bitboard pawn_one_forward;
    Bitboard pawn_two_forward;
    Bitboard all_moves_possible;

    if (color) {
        left_attack = ((Bitboards::COLUMN_CLEAR[0] & start) << 7) & (board->get_all_black_pieces() | board->get_enpessant(color));
        right_attack = ((Bitboards::COLUMN_CLEAR[7] & start) << 9) & (board->get_all_black_pieces() | board->get_enpessant(color));
        pawn_one_forward = ((start << 8) & ~(board->get_all_pieces()));
        pawn_two_forward = (((((start & Bitboards::ROW_MASK[1]) << 8) & ~board->get_all_pieces()) << 8) & ~board->get_all_pieces());
    }
    else {
        left_attack = ((Bitboards::COLUMN_CLEAR[0] & start) >> 9) & (board->get_all_white_pieces() | board->get_enpessant(color));
        right_attack = ((Bitboards::COLUMN_CLEAR[7] & start) >> 7) & (board->get_all_white_pieces() | board->get_enpessant(color));
        pawn_one_forward = ((start >> 8) & (~board->get_all_pieces()));
        pawn_two_forward = (((((start & Bitboards::ROW_MASK[6]) >> 8) & ~board->get_all_pieces()) >> 8) & ~board->get_all_pieces());
    }
    all_moves_possible = (left_attack | right_attack | pawn_one_forward | pawn_two_forward);
    return all_moves_possible;
}


Bitboard Movegen::get_queen_moves(uint_fast16_t pos, bool color, Board* board) {
    return get_rook_moves(pos, color, board) | get_bishop_moves(pos, color, board);
}


// bool Movegen::is_valid_move(uint_fast16_t userMove) {

//     Bitboard valid_moves;
//     uint_fast16_t bStart = (userMove & 0b0000111111000000)>>6;
//     uint_fast16_t bEnd = (userMove & 0b111111);
//     uint_fast16_t otherStuff = (userMove & 0b0111000000000000) >>12;
//     bool color = (userMove & 0b1000000000000000)>>15;
//     short piece = chessBoard[0].findBoard(color, (Bitboard)1<<bStart);

//     if (piece>=0) std::cout << "Found " << PIECE_NAMES[piece] << "\n";
//     else {std::cout << "No piece found" << std::endl;}

//     switch (piece) {

//         case 0:
//             valid_moves = getPawnMoves(bStart, color, chessBoard);
//             break;

//         case 1:
//             valid_moves = getRookMoves(bStart, color, chessBoard);
//             break;

//         case 2:

//             valid_moves = getKnightMoves(bStart, color, chessBoard);
//             break;

//         case 3:

//             valid_moves = getBishopMoves(bStart, color, chessBoard);
//             break;

//         case 4:

//             valid_moves = getQueenMoves(bStart, color, chessBoard);
//             break;

//         case 5:

//             valid_moves = getKingMoves(bStart, color, chessBoard);
//             break;

//         default:

//             std::cout<<"No piece found at that spot!"<<std::endl;
//             valid_moves = (Bitboard)0x0000;

//     }
// #if DEBUG
//     std::cout <<"Your choice\n";
//     printBitboard((Bitboard)1<<bEnd);
//     std::cout <<"Valid moves:\n";
//     printBitboard(valid_moves);
//     std::cout<<"Your choice and valid moves:\n";
//     printBitboard((Bitboard)1<<bEnd & valid_moves);
// #endif
//     if ((Bitboard)1<<bEnd & valid_moves) {
//         ChessBoard* testBoard = new ChessBoard(chessBoard[0]);
//         uncheckedMove(color, piece, bStart, bEnd, testBoard);
//         //Bitboard newKing = chessBoard[0].pieces[color][5];
//         // if(!check(color, testBoard)) {
//         //     //chessBoard = testBoard;
//         //     uncheckedMove(color, piece, bStart, bEnd, chessBoard);
//         //     return true;
//         // } else {
//         //     std::cout << "That move would put you into check\n";
//         //     return false;
//         // }
//     }
//     else {
// #if DEBUG
//         std::cout << "The valid moves are: \n";
//         printBitboard(valid_moves);
//         std::cout << "\n";
// #endif
//         return false;
//     }
// }

// void Movegen::uncheckedMove(bool player, short piece, uint_fast16_t start, uint_fast16_t end, ChessBoard* cb) {

//     cb[0].pieces[player][piece] = cb[0].pieces[player][piece] - ((Bitboard)1<<start) + ((Bitboard)1<<end);
//     cb[0].AllPieces = cb[0].AllPieces - ((Bitboard)1<<start) + ((Bitboard)1<<end);
//     short captureType = cb[0].findBoard(!player, (Bitboard)1<<end);
//     if (captureType > -1) {
//         cb[0].pieces[!player][captureType] -= ((Bitboard) 1 << end);
//         std::cout << "You captured a " << PIECE_NAMES[captureType];
//     }
//     if (player) {   // black
//         cb[0].AllWhitePieces -= cb[0].AllWhitePieces & ((Bitboard)1 << end);    // capture white
//         cb[0].AllBlackPieces = (cb[0].AllBlackPieces - ((Bitboard)1 << start)) + ((Bitboard)1 << end);
//     } else {        // white
//         cb[0].AllBlackPieces -= cb[0].AllBlackPieces & ((Bitboard)1 << end);    // capture black
//         cb[0].AllWhitePieces = (cb[0].AllWhitePieces - ((Bitboard)1 << start)) + ((Bitboard)1 << end);
//     }
// }


 Bitboard Movegen::get_all_moves(bool color, Board* board) {
 	Bitboard total_moves = 0;
 	int piece;
 	uint_fast16_t pos;
 	for (pos = 0; pos < 64; pos++) {
 		piece = board->find_board(color, static_cast<Bitboard>(1) << pos);
 		if (piece >= 0) {
 			switch (piece) {
 			case 0:
 				total_moves |= this->get_pawn_moves(pos, color, board);
 				break;
 			case 1:
 				total_moves |= this->get_rook_moves(pos, color, board);
 				break;
 			case 2:
 				total_moves |= this->get_knight_moves(pos, color, board);
 				break;
 			case 3:
 				total_moves |= this->get_bishop_moves(pos, color, board);
 				break;
 			case 4:
 				total_moves |= this->get_queen_moves(pos, color, board);
  				break;
  			case 5:
  				total_moves |= this->get_king_moves(color, board);
  			}
  		}
  	}
 	return total_moves;
 }
