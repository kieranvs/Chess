#pragma once

#include "Board.h"

#include <string>

struct PerftResults
{
	int nodes = 0;
	int captures = 0;
	int en_passants = 0;
};

namespace Utils
{
	Board get_start_position();
	Board get_blank_board();
	Board get_board_from_fen(const std::string& fen);
	void print_board(const Board& board);
	const char* get_square_name(int sq);

	const char* get_player_name(Player p);
	inline Player opposite_player(Player p)
	{
		return p == Player::White ? Player::Black : Player::White;
	}

	PerftResults perft(const Board& board, Player p, int depth);
}