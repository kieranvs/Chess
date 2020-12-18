#include "Search.h"
#include "Eval.h"
#include "MoveGen.h"
#include "Utils.h"

#include <vector>

SearchResult search(const Board& board, Player playerToMove, int search_depth)
{
	Player otherPlayer = playerToMove == Player::White ? Player::Black : Player::White;
	std::vector<MoveGenResult> results;
	move_gen(board, playerToMove, results);

	// PlayerToMove has no moves so return the worst possible score
	if (results.empty())
	{
		if (playerToMove == Player::White)
		{
			SearchResult sr;
			sr.score = -1000000.0;
			return sr;
		}
		else
		{
			SearchResult sr;
			sr.score = 1000000.0;
			return sr;
		}
	}

	double best_score;
	if (search_depth == 1)
		best_score = evaluate_position(results[0].board);
	else
		best_score = search(results[0].board, otherPlayer, search_depth - 1).score;
	MoveGenResult* best_result = &results[0];

	for (int i = 1; i < results.size(); i++)
	{
		double score;
		if (search_depth == 1)
		{
			score = evaluate_position(results[i].board);
		}
		else
		{
			score = search(results[i].board, otherPlayer, search_depth - 1).score;
		}

		if ((playerToMove == Player::White && score > best_score) || (playerToMove == Player::Black && score < best_score))
		{
			best_score = score;
			best_result = &results[i];
		}
	}

	SearchResult sr;
	sr.next = best_result->board;
	sr.score = best_score;

	return sr;
}

