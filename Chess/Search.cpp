#include "Search.h"
#include "Eval.h"
#include "Utils.h"

#include <vector>
#include <optional>

SearchResult search(const Board& board, Player playerToMove, int search_depth, bool use_alpha_beta, double alpha_score, double beta_score)
{
	Player otherPlayer = Utils::opposite_player(playerToMove);
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

	std::optional<double> best_score;
	MoveGenResult* best_result = &results[0];

	auto better_score = [playerToMove, &best_score](double score)
	{
		if (!best_score.has_value() ||
		   (playerToMove == Player::White && score > *best_score) ||
		   (playerToMove == Player::Black && score < *best_score))
		{
			return true;
		}
		return false;
	};

	if (search_depth == 1)
	{
		for (int i = 0; i < results.size(); i++)
		{
			double score = evaluate_position(results[i].board);

			if (better_score(score))
			{
				best_score = score;
				best_result = &results[i];
			}
		}
	}
	else
	{
		for (int i = 0; i < results.size(); i++)
		{
			double score = search(results[i].board, otherPlayer, search_depth - 1, use_alpha_beta, alpha_score, beta_score).score;

			if (better_score(score))
			{
				best_score = score;
				best_result = &results[i];
			}

			if (use_alpha_beta)
			{
				if (playerToMove == Player::White)
				{
					if (score > alpha_score)
						alpha_score = score;

					if (alpha_score >= beta_score)
						break;
				}
				else
				{
					if (score < beta_score)
						beta_score = score;

					if (beta_score <= alpha_score)
						break;
				}
			}
		}
	}

	SearchResult sr;
	sr.next = best_result->board;
	sr.score = *best_score;
	sr.move = *best_result;

	return sr;
}

