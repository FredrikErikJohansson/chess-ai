#pragma once
#include <cstdint>
#include <iostream>
#include <array>
#include <stack>
#include <vector>

#include "bitboards.h"

#define WHITE 1
#define BLACK 0
#define PAWN 0
#define KNIGHT 1
#define BISHOP 2
#define ROOK 3
#define QUEEN 4
#define KING 5

struct Move {
    Bitboard from;
    Bitboard to;
    bool color;
	bool capture = false;
	bool castle = false;
	bool check = false;
	bool promotion = false;
    unsigned int type;
	unsigned int capture_type;
};

class Board {
public:
    Board() {
		last_move[WHITE].from = 0;
		last_move[WHITE].to = 0;
		last_move[BLACK].from = 0;
		last_move[BLACK].to = 0;
	};

	void initialize();
	void puzzle_one(int);
	void puzzle_two(int);
	void clear();
	
	Bitboard get_all_white_pieces() const;
    Bitboard get_all_black_pieces() const;
    Bitboard get_all_pieces() const;

	void print() const;
	void print(Bitboard const&) const;
	void print_visualization(Board const);

	uint_fast16_t find_board(bool color, Bitboard pos);

	// Common variables
	std::stack<Move> history;
	std::array<std::array<Bitboard, 6>, 2> pieces = {};
	std::array<Bitboard, 2> enpessant = {0, 0};
	bool can_castle[2] = {true, true};
	bool is_checked[2] = {false, false};
	std::array<std::vector<Move>, 2> moves;
	std::array<Move, 2> last_move;
	std::array<uint, 2> num_of_pieces {16, 16};
};