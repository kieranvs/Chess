#include "Board.h"

struct SearchResult
{
	Board next;
	double score;
};

SearchResult search(const Board& root_position, Player playerToMove, int search_depth);