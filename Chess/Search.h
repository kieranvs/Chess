#include "Board.h"
#include "MoveGen.h"

struct SearchResult
{
	Board next;
	double score;
	MoveGenResult move;
};

SearchResult search(const Board& root_position, Player playerToMove, int search_depth, bool use_alpha_beta = true, double alpha_score = -1000000.0, double beta_score = 1000000.0);