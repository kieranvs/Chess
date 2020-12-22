#pragma once

#include "Board.h"

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
	void print_board(const Board& board);
	const char* get_player_name(Player p);

	PerftResults perft(const Board& board, Player p, int depth);
}