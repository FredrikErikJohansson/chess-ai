#pragma once
#include <cstdint>
#include <iostream>
#include <array>
#include <stack>

#include "bitboards.h"

#define WHITE 1
#define BLACK 0
#define PAWN 0
#define ROOK 1
#define KNIGHT 2
#define BISHOP 3
#define QUEEN 4
#define KING 5

struct Move {
    Bitboard from;
    Bitboard to;
    bool color;
    uint type;
};

class Board {
public:
	void initialize();
	void clear();
	
	Bitboard get_all_white_pieces() const;
    Bitboard get_all_black_pieces() const;
    Bitboard get_all_pieces() const;

	// TODO: make a setter instead of changing to get value
	//Bitboard& get_pieces(bool color, uint type) { return pieces[color][type]; }
    //std::array<Bitboard, 6> get_white() { return pieces[0]; }
    //std::array<Bitboard, 6> get_black() { return pieces[1]; }

	//Bitboard get_enpessant(bool color) { return enpessant[color]; }
	//bool get_castle(bool color) { return can_castle[color]; }

	//void set_castle(bool color, bool can) { can_castle[color] = can; }

	void print() const;
	void print(Bitboard const&) const;
	// short getRowFromChar(char letter);
	// bool queryPiece(char column,short row, Bitboard board);

	uint_fast16_t find_board(bool color, Bitboard pos);

	std::stack<Move> history;
	std::array<std::array<Bitboard, 6>, 2> pieces = {};
	std::array<Bitboard, 2> enpessant = {};
	bool can_castle[2] = {true, true};	
};