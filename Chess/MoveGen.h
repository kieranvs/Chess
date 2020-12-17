#pragma once

#include "Board.h"

#include <vector>

enum class MoveType
{
	Move,
	Capture,
	CaptureEnPassant
};

struct MoveGenResult
{
	Board board;
	MoveType move_type;
};

void move_gen(const Board& board, Player playerToMove, std::vector<MoveGenResult>& output);