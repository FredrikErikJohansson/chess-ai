#include "board.h"
#include "bitboards.h"

void Board::initialize() {
    pieces[WHITE][PAWN] = Bitboards::PAWN_START & Bitboards::ALL_WHITE_START;
    pieces[WHITE][ROOK] = Bitboards::ROOK_START & Bitboards::ALL_WHITE_START;
    pieces[WHITE][KNIGHT] = Bitboards::KNIGHT_START & Bitboards::ALL_WHITE_START;
    pieces[WHITE][BISHOP] = Bitboards::BISHOP_START & Bitboards::ALL_WHITE_START;
    pieces[WHITE][QUEEN] = Bitboards::QUEEN_START & Bitboards::ALL_WHITE_START;
    pieces[WHITE][KING] = Bitboards::KING_START & Bitboards::ALL_WHITE_START;

    pieces[BLACK][PAWN] = Bitboards::PAWN_START & Bitboards::ALL_BLACK_START;
    pieces[BLACK][ROOK] = Bitboards::ROOK_START & Bitboards::ALL_BLACK_START;
    pieces[BLACK][KNIGHT] = Bitboards::KNIGHT_START & Bitboards::ALL_BLACK_START;
    pieces[BLACK][BISHOP] = Bitboards::BISHOP_START & Bitboards::ALL_BLACK_START;
    pieces[BLACK][QUEEN] = Bitboards::QUEEN_START & Bitboards::ALL_BLACK_START;
    pieces[BLACK][KING] = Bitboards::KING_START & Bitboards::ALL_BLACK_START;

    // TODO: Add castle and enpessant
    // castle[WHITE] = 
}

void Board::clear() {
    pieces[WHITE].fill(0);
    pieces[BLACK].fill(0);
    //pieces[BLACK][KING] = Bitboards::KING_START & Bitboards::ALL_BLACK_START;
    //pieces[BLACK][ROOK] = Bitboards::ROOK_START & Bitboards::ALL_BLACK_START;
}

Bitboard Board::get_all_white_pieces() const {
    return (pieces[WHITE][PAWN] | pieces[WHITE][ROOK] | pieces[WHITE][KNIGHT] | pieces[WHITE][BISHOP] | pieces[WHITE][QUEEN] | pieces[WHITE][KING]);
}

Bitboard Board::get_all_black_pieces() const {
    return (pieces[BLACK][PAWN] | pieces[BLACK][ROOK] | pieces[BLACK][KNIGHT] | pieces[BLACK][BISHOP] | pieces[BLACK][QUEEN] | pieces[BLACK][KING]);
}

Bitboard Board::get_all_pieces() const {
    return (this->get_all_white_pieces() | this->get_all_black_pieces());
}

void Board::print() const {
    this->print(this->get_all_pieces());
}

void Board::print(Bitboard const& b) const {
    for (int row = 7; row >= 0; --row) {
        for (int col = 0; col <= 7; ++col) {
            if (b & (1ULL << ((row * 8) + col))) {
                std::cout << "1 ";
            }
            else {
                std::cout << "0 ";
            }
        }
        std::cout << std::endl;
    }
    std::cout  << std::endl;
}

uint_fast16_t Board::find_board(bool color, Bitboard pos) {
    uint_fast16_t i;
    for (i = 5; i >= 0; --i) {
        if (pieces[color][i] & pos) {
            break;
        }
    }
    return i;
}
