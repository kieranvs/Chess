#pragma once

#include "Board.h"
#include "Eval.h"
#include "MoveGen.h"

struct SearchResult
{
	Board next;
	EvalScore score;
	MoveGenResult move;
};

inline EvalScore worst_score_for_white()
{
	EvalScore score{};
	score.is_mate_black_wins = true;
	score.mate_in = 1;
	return score;
}

inline EvalScore worst_score_for_black()
{
	EvalScore score{};
	score.is_mate_white_wins = true;
	score.mate_in = 1;
	return score;
}

SearchResult search(const Board& root_position, int search_depth, bool use_alpha_beta = true, EvalScore alpha_score = worst_score_for_white(), EvalScore beta_score = worst_score_for_black());