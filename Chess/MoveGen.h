#pragma once

#include "Board.h"

#include <vector>

void move_gen(const Board& board, Player playerToMove, std::vector<Board>& output);