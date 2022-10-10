#pragma once

#include "Board.h"

#include <vector>

enum class MoveType
{
	Move,
	Castle,
	CaptureEnPassant,
	Capture,
	Promotion,
	CapturePromotion
};

struct MoveGenResult
{
	Board board;
	MoveType move_type;
	int sq_from;
	int sq_to;
};

bool is_in_check(const Board& board, Player player);
void move_gen(const Board& board, std::vector<MoveGenResult>& output);