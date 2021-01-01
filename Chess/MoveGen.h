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

void move_gen(const Board& board, std::vector<MoveGenResult>& output);