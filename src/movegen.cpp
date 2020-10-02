#include "movegen.h"
#include "magics.h"
#include "bitboards.h"
#include "board.h"


// TODO: May need more testing
// Move has to be copy
void Movegen::make_move(Move move, Board* board, bool from_check) {
    Bitboard from_to_bb = move.from ^ move.to;
    board->pieces[move.color][move.type] ^= from_to_bb;
    // Since there was a move avaiable the player is not in check
    board->is_checked[move.color] = false; // TODO: Probably not correct

    // Check for capture
    Bitboard capture_pos = 0;
    if(move.color == WHITE) capture_pos = move.to & board->get_all_black_pieces();
    else if(move.color == BLACK) capture_pos = move.to & board->get_all_white_pieces();
    
    
    if(capture_pos != 0 && !from_check) {
        for(uint capture_type = 0; capture_type < 6; ++capture_type) {
            if(move.color == WHITE) {
                if((capture_pos & board->pieces[BLACK][capture_type]) != 0) {
                    board->pieces[BLACK][capture_type] ^= move.to;
                    move.capture = true;
                    move.capture_type = capture_type;
                }

            }
            if(move.color == BLACK) {
                if((capture_pos & board->pieces[WHITE][capture_type]) != 0) {
                    board->pieces[WHITE][capture_type] ^= move.to;
                    move.capture = true;
                    move.capture_type = capture_type;
                }
            }
        }
    }

    // TODO: Also consider knight promotion
    // Check for promotion
    if(move.type == PAWN) {
        if((move.color && (move.to & Bitboards::ROW_MASK[7])) || (!move.color && (move.to & Bitboards::ROW_MASK[0]))) {   
            board->pieces[move.color][PAWN] ^= move.to; // Remove pawn
            board->pieces[move.color][QUEEN] ^= move.to; // Add queen
            move.promotion = true;
            move.type = QUEEN;
        }
    }

    // Check for castle
    Bitboard rook_start = Bitboards::ROOK_START & ((move.color) ? Bitboards::ALL_WHITE_START : Bitboards::ALL_BLACK_START);
    if(move.castle && (rook_start & board->pieces[move.color][ROOK]) != 0) {
        if((move.to & ((rook_start << 1) & Bitboards::COLUMN_MASK[1])) != 0) { // Left
            board->pieces[move.color][ROOK] ^= (move.to << 1);
            board->pieces[move.color][ROOK] ^= (move.to >> 1);
            board->can_castle[move.color] = false;
        }
        if((move.to & ((rook_start >> 1) & Bitboards::COLUMN_MASK[6])) != 0) { // Right
            board->pieces[move.color][ROOK] ^= (move.to >> 1);
            board->pieces[move.color][ROOK] ^= (move.to << 1);
            board->can_castle[move.color] = false;
        }
    }
    // TODO: Not correct -> this may lead to multiple castles
    if(move.type == KING) board->can_castle[move.color] = false;
    // if(move.type == ROOK && (rook_start & board->pieces[move.color][ROOK]) == 0) board->can_castle[move.color] = false;

    // Check for check
    if(move.check) {
        board->is_checked[!move.color] = true;
    }

    board->history.push(move);
}

void Movegen::unmake_move(Board* board) {
    Move move = board->history.top();
    Bitboard from_to_bb = move.from ^ move.to;
    board->pieces[move.color][move.type] ^= from_to_bb;

    if(move.capture) board->pieces[!move.color][move.capture_type] ^= move.to; // Undo capture

    // Undo promotion
    if(move.promotion) {
        if((move.color && (move.to & Bitboards::ROW_MASK[7])) || (!move.color && (move.to & Bitboards::ROW_MASK[0]))) {   
            board->pieces[move.color][PAWN] ^= move.from; // Add pawn
            board->pieces[move.color][QUEEN] ^= move.from; // Remove queen
        } 
    }
   
    // Undo castle
    if(move.type == KING && move.castle) {
        if((move.to & Bitboards::COLUMN_MASK[1]) != 0) { // Left
            board->pieces[move.color][ROOK] ^= (move.to << 1);
            board->pieces[move.color][ROOK] ^= (move.to >> 1);
            board->can_castle[move.color] = true; 
        }
        if((move.to & Bitboards::COLUMN_MASK[6]) != 0) { // Right
            board->pieces[move.color][ROOK] ^= (move.to >> 1);
            board->pieces[move.color][ROOK] ^= (move.to << 1);
            board->can_castle[move.color] = true; 
        }
    }

     // Undo check
    if(move.check) {
        board->is_checked[!move.color] = false;
    }

    board->history.pop();
}

std::vector<Bitboard> Movegen::seperate_bitboards(Bitboard const& bb) {
    std::vector<Bitboard> res;
    uint_fast64_t idx = 0b1;
    for(uint_fast16_t i = 0; i < 64; ++i) {
        if((bb & (idx << i)) != 0) res.push_back((bb & (idx << i)));
    }
    return res;
}

void Movegen::get_moves_for(Bitboard from, bool color, uint type, Board* board) {
    Move move;
    move.from = from;
    move.color = color;
    move.type = type;
    std::vector<Bitboard> to;

    switch (type)
    {
    case PAWN:
        to = seperate_bitboards(get_pawn_moves(from, color, board));
        break;
    case ROOK:
        to = seperate_bitboards(get_rook_moves(from, color, board));
        break;
    case KNIGHT:
        to = seperate_bitboards(get_knight_moves(from, color, board));
        break;
    case BISHOP:
        to = seperate_bitboards(get_bishop_moves(from, color, board));
        break;
    case QUEEN:
        to = seperate_bitboards(get_queen_moves(from, color, board));
        break;
    case KING:
        to = seperate_bitboards(get_king_moves(color, board));
        break;
    default:
        std::cout << "Error: Unknown type " << type << std::endl;
        break;
    }

    // Check for castle
    for(auto t : to) {
        move.to = t;
        if(type == KING && ((t & (Bitboards::COLUMN_MASK[1] | Bitboards::COLUMN_MASK[6])) != 0)) {
            if(from == (Bitboards::KING_START & ((color) ? Bitboards::ALL_WHITE_START : Bitboards::ALL_BLACK_START))) {
                move.castle = true;
            }
        }

        // Check for self check and opponent check
        if(this->check(move)) {
            board->moves[move.color].push_back(move);
        }
    }

    std::sort( board->moves[move.color].begin( ), board->moves[move.color].end(), []( const Move& lhs, const Move& rhs )
    {
    return lhs.capture > rhs.capture;
    });
}


Bitboard Movegen::get_bishop_moves(Bitboard bb, bool color, Board* board) {
    uint_fast16_t pos = log2(bb & -bb);
    Bitboard index = ((board->get_all_pieces() & BOccupancy[pos]) * BMagic[pos]) >> (64-BBits[pos]);
    return bishopMoves[pos][index] & ~(color ? board->get_all_white_pieces() : board->get_all_black_pieces());
}

Bitboard Movegen::get_rook_moves(Bitboard bb, bool color, Board* board) {
    uint_fast16_t pos = log2(bb & -bb);
    Bitboard index = ((board->get_all_pieces() & ROccupancy[pos]) * RMagic[pos]) >> (64-RBits[pos]);
    return rookMoves[pos][index] & ~(color ? board->get_all_white_pieces() : board->get_all_black_pieces());
}

Bitboard Movegen::get_knight_moves(Bitboard start, bool color, Board* board) {
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

    // TODO: Test if correct
    Bitboard left_castle = 0;
    Bitboard right_castle = 0;
    Bitboard left_columns = (Bitboards::COLUMN_MASK[1] | Bitboards::COLUMN_MASK[2] | Bitboards::COLUMN_MASK[3]);
    Bitboard right_columns = (Bitboards::COLUMN_MASK[5] | Bitboards::COLUMN_MASK[6]);
    if (board->can_castle[color] && color) {
        if(((Bitboards::ROW_MASK[0] & left_columns) & board->get_all_white_pieces()) == 0) {
            left_castle = (((board->pieces[color][KING] & (Bitboards::KING_START & Bitboards::ALL_WHITE_START)) >> 3) & 
                (board->pieces[color][ROOK] & (Bitboards::ROOK_START & Bitboards::ALL_WHITE_START)) << 1);
        }
        if(((Bitboards::ROW_MASK[0] & right_columns) & board->get_all_white_pieces()) == 0) {
            right_castle = (((board->pieces[color][KING] & (Bitboards::KING_START & Bitboards::ALL_WHITE_START)) << 2) & 
                (board->pieces[color][ROOK] & (Bitboards::ROOK_START & Bitboards::ALL_WHITE_START)) >> 1);
        }
    } else if(board->can_castle[color] && !color){
        if(((Bitboards::ROW_MASK[7] & right_columns) & board->get_all_black_pieces()) == 0) {
            right_castle = (((board->pieces[color][KING] & (Bitboards::KING_START & Bitboards::ALL_BLACK_START)) >> 3) & 
                (board->pieces[color][ROOK] & (Bitboards::ROOK_START & Bitboards::ALL_BLACK_START)) << 1);
        }
        if(((Bitboards::ROW_MASK[7] & left_columns) & board->get_all_black_pieces()) == 0) {
            left_castle = (((board->pieces[color][KING] & (Bitboards::KING_START & Bitboards::ALL_BLACK_START)) << 2) & 
                (board->pieces[color][ROOK] & (Bitboards::ROOK_START & Bitboards::ALL_BLACK_START)) >> 1);
        }
    }

    left_attack = ((Bitboards::COLUMN_CLEAR[0] & board->pieces[color][KING]) >> 1);
    right_attack = ((Bitboards::COLUMN_CLEAR[7] & board->pieces[color][KING]) << 1);
    forward_attack = (board->pieces[color][KING] << 8);
    backward_attack = (board->pieces[color][KING] >> 8);
    left_forward_diagonal_attack = ((Bitboards::COLUMN_CLEAR[0] & board->pieces[color][KING]) << 7);
    right_forward_diagonal_attack = ((Bitboards::COLUMN_CLEAR[7] & board->pieces[color][KING]) << 9);
    left_backward_diagonal_attack = ((Bitboards::COLUMN_CLEAR[0] & board->pieces[color][KING]) >> 9);
    right_backward_diagonal_attack = ((Bitboards::COLUMN_CLEAR[7] & board->pieces[color][KING]) >> 7);
    all_moves_possible = (left_attack | right_attack | forward_attack | 
                        backward_attack | left_forward_diagonal_attack | 
                        right_forward_diagonal_attack | left_backward_diagonal_attack | 
                        right_backward_diagonal_attack | right_castle | left_castle);
    return all_moves_possible & ~(color ? board->get_all_white_pieces() : board->get_all_black_pieces());
}

Bitboard Movegen::get_pawn_moves(Bitboard start, bool color, Board* board) {
    Bitboard left_attack;
    Bitboard right_attack;
    Bitboard pawn_one_forward;
    Bitboard pawn_two_forward;
    Bitboard all_moves_possible;

    if (color) {
        left_attack = ((Bitboards::COLUMN_CLEAR[0] & start) << 7) & (board->get_all_black_pieces() | board->enpessant[color]);
        right_attack = ((Bitboards::COLUMN_CLEAR[7] & start) << 9) & (board->get_all_black_pieces() | board->enpessant[color]);
        pawn_one_forward = ((start << 8) & ~(board->get_all_pieces()));
        pawn_two_forward = (((((start & Bitboards::ROW_MASK[1]) << 8) & ~board->get_all_pieces()) << 8) & ~board->get_all_pieces());
    }
    else {
        left_attack = ((Bitboards::COLUMN_CLEAR[0] & start) >> 9) & (board->get_all_white_pieces() | board->enpessant[color]);
        right_attack = ((Bitboards::COLUMN_CLEAR[7] & start) >> 7) & (board->get_all_white_pieces() | board->enpessant[color]);
        pawn_one_forward = ((start >> 8) & (~board->get_all_pieces()));
        pawn_two_forward = (((((start & Bitboards::ROW_MASK[6]) >> 8) & ~board->get_all_pieces()) >> 8) & ~board->get_all_pieces());
    }
    all_moves_possible = (left_attack | right_attack | pawn_one_forward | pawn_two_forward);
    return all_moves_possible;
}


Bitboard Movegen::get_queen_moves(Bitboard bb, bool color, Board* board) {
    return get_rook_moves(bb, color, board) | get_bishop_moves(bb, color, board);
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
    for(auto move : board->moves[color]) {
        total_moves |= move.to;
    }
    
 	return total_moves;
 }

 void Movegen::calculate_all_moves() {
    board->moves[WHITE].clear();
    board->moves[BLACK].clear();
    std::vector<Bitboard> white_pawn_bb_pos = this->seperate_bitboards(board->pieces[WHITE][PAWN]);
    std::vector<Bitboard> white_rook_bb_pos = this->seperate_bitboards(board->pieces[WHITE][ROOK]);
    std::vector<Bitboard> white_knight_bb_pos = this->seperate_bitboards(board->pieces[WHITE][KNIGHT]);
    std::vector<Bitboard> white_bishop_bb_pos = this->seperate_bitboards(board->pieces[WHITE][BISHOP]);
    std::vector<Bitboard> white_queen_bb_pos = this->seperate_bitboards(board->pieces[WHITE][QUEEN]);
    std::vector<Bitboard> white_king_bb_pos = this->seperate_bitboards(board->pieces[WHITE][KING]);

    std::vector<Bitboard> black_pawn_bb_pos = this->seperate_bitboards(board->pieces[BLACK][PAWN]);
    std::vector<Bitboard> black_rook_bb_pos = this->seperate_bitboards(board->pieces[BLACK][ROOK]);
    std::vector<Bitboard> black_knight_bb_pos = this->seperate_bitboards(board->pieces[BLACK][KNIGHT]);
    std::vector<Bitboard> black_bishop_bb_pos = this->seperate_bitboards(board->pieces[BLACK][BISHOP]);
    std::vector<Bitboard> black_queen_bb_pos = this->seperate_bitboards(board->pieces[BLACK][QUEEN]);
    std::vector<Bitboard> black_king_bb_pos = this->seperate_bitboards(board->pieces[BLACK][KING]);
    // White moves
    for (auto p : white_pawn_bb_pos) {
        this->get_moves_for(p, WHITE, PAWN, board);
    }
    for (auto p : white_rook_bb_pos) {
        this->get_moves_for(p, WHITE, ROOK, board);
    }
    for (auto p : white_knight_bb_pos) {
        this->get_moves_for(p, WHITE, KNIGHT, board);
    }
    for (auto p : white_bishop_bb_pos) {
        this->get_moves_for(p, WHITE, BISHOP, board);
    }
    for (auto p : white_queen_bb_pos) {
        this->get_moves_for(p, WHITE, QUEEN, board);
    }
    for (auto p : white_king_bb_pos) {
        this->get_moves_for(p, WHITE, KING, board);
    }

    // Black moves
    for (auto p : black_pawn_bb_pos) {
        this->get_moves_for(p, BLACK, PAWN, board);
    }
    for (auto p : black_rook_bb_pos) {
        this->get_moves_for(p, BLACK, ROOK, board);
    }
    for (auto p : black_knight_bb_pos) {
        this->get_moves_for(p, BLACK, KNIGHT, board);
    }
    for (auto p : black_bishop_bb_pos) {
        this->get_moves_for(p, BLACK, BISHOP, board);
    }
    for (auto p : black_queen_bb_pos) {
        this->get_moves_for(p, BLACK, QUEEN, board);
    }
    for (auto p : black_king_bb_pos) {
        this->get_moves_for(p, BLACK, KING, board);
    }
 }

 Bitboard Movegen::attacks_to_king(Bitboard king, bool color) {
    Bitboard xray_rooks = this->get_rook_moves(king, color, board );
    Bitboard xray_bishops = this->get_bishop_moves(king, color, board);
    Bitboard xray_knights = this->get_knight_moves(king, color, board);
    Bitboard xray_queen = this->get_queen_moves(king, color, board);
    Bitboard xray_king = this->get_king_moves(color, board);
    Bitboard xray_pawn = this->get_pawn_moves(king, color, board);

    return ((board->pieces[!color][ROOK] & xray_rooks)
        | (board->pieces[!color][BISHOP] & xray_bishops)
        | (board->pieces[!color][KNIGHT] & xray_knights)
        | (board->pieces[!color][QUEEN] & xray_queen)
        | (board->pieces[!color][KING] & xray_king)
        | (board->pieces[!color][PAWN] & xray_pawn));
}

 bool Movegen::check(Move& move) {
    bool prev_can_castle = board->can_castle[move.color];
    make_move(move, board, true);

    if(move.type == KING) {
        board->can_castle[move.color] = prev_can_castle;
        // Check opponent pawns
        if(move.color) {
            if(((move.to << 9) & board->pieces[BLACK][PAWN]) != 0 ||
                ((move.to << 7) & board->pieces[BLACK][PAWN]) != 0) {
                unmake_move(board);
                return false;
            }
        } else {
            if(((move.to >> 9) & board->pieces[WHITE][PAWN]) != 0 ||
                ((move.to >> 7) & board->pieces[WHITE][PAWN]) != 0) {
                unmake_move(board);
                return false;
            }
        }
    }

    // Self check
    if(attacks_to_king(board->pieces[move.color][KING], move.color) != 0) {
        unmake_move(board);
        return false;
    }

    // Opponent check
    if(attacks_to_king(board->pieces[!move.color][KING], !move.color) != 0) {
        unmake_move(board);
        move.check = true;
        return true;
    } 

    // In check after move
    if(board->is_checked[move.color]) {
        unmake_move(board);
        return false;
    }
    unmake_move(board);
    return true;
 }
