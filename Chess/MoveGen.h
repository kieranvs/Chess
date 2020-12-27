#pragma once

#include "Board.h"

#include <vector>

enum class MoveType
{
	Move,
	Capture,
	CaptureEnPassant,
	CapturePromotion,
	Castle,
	Promotion
};

struct MoveGenResult
{
	Board board;
	MoveType move_type;
	int sq_from;
	int sq_to;
};

void move_gen(const Board& board, Player playerToMove, std::vector<MoveGenResult>& output);