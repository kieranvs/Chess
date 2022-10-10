#include "Search.h"
#include "Eval.h"
#include "Utils.h"

#include <vector>
#include <optional>

SearchResult search(const Board& board, int search_depth, bool use_alpha_beta, EvalScore alpha_score, EvalScore beta_score)
{
	Player playerToMove = board.playerToMove();
	Player otherPlayer = board.oppositePlayer();
	std::vector<MoveGenResult> results;
	move_gen(board, results);

	// PlayerToMove has no moves so it's either a checkmate or a stalemate
	if (results.empty())
	{
		if (is_in_check(board, playerToMove))
		{
			// Checkmate
			SearchResult sr{};
			sr.score.is_mate_white_wins = playerToMove == Player::Black;
			sr.score.is_mate_black_wins = playerToMove == Player::White;
			sr.score.mate_in = 0;
			return sr;
		}
		else
		{
			// Stalemate
			SearchResult sr{};
			sr.score.score = 0.0;
			return sr;
		}
	}

	std::optional<EvalScore> best_score;
	MoveGenResult* best_result = &results[0];

	auto better_score = [playerToMove, &best_score](const EvalScore& score)
	{
		if (!best_score.has_value()) return true;

		if (playerToMove == Player::White)
			return score > *best_score;
		else
			return score < *best_score;
	};

	if (search_depth == 1)
	{
		for (int i = 0; i < results.size(); i++)
		{
			EvalScore score{};
			score.score = evaluate_position(results[i].board);

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
			EvalScore score = search(results[i].board, search_depth - 1, use_alpha_beta, alpha_score, beta_score).score;

			if (score.is_mate_white_wins && playerToMove == Player::White)
				score.mate_in += 1;

			if (score.is_mate_black_wins && playerToMove == Player::Black)
				score.mate_in += 1;

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

