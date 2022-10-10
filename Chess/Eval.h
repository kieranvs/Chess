#pragma once

#include "Board.h"

struct EvalScore
{
	double score = 0.0; // Positive for white winning, negative for black winning
	bool is_mate_white_wins = false;
	bool is_mate_black_wins = false;
	int mate_in = 0;

	// Is this better than other for white?
	bool operator>(const EvalScore& other) const
	{
		/*
		+-------------------------+----------------+----------------+---------------+
		|                         |                    this                         |
		|                         | Mate for white | Mate for black | Not mate      |
		+-------------------------+----------------+----------------+---------------+
		| other    Mate for white | Fewest moves   | false          | false         |
		|          Mate for black | true           | Most moves     | true          |
		|          Not Mate       | true           | false          | Highest score |
		+-------------------------+----------------+----------------+---------------+
		*/

		if (is_mate_white_wins && other.is_mate_white_wins)
			return mate_in < other.mate_in;
		else if (!is_mate_white_wins && other.is_mate_white_wins)
			return false;
		else if (is_mate_white_wins && !other.is_mate_white_wins)
			return true;
		else if (is_mate_black_wins && other.is_mate_black_wins)
			return mate_in > other.mate_in;
		else if (!is_mate_black_wins && other.is_mate_black_wins)
			return true;
		else if (is_mate_black_wins && !other.is_mate_black_wins)
			return false;
		else
			return score > other.score;
	}

	bool operator>=(const EvalScore& other) const
	{
		return (*this == other) || (*this > other);
	}

	bool operator<(const EvalScore& other) const
	{
		return !(*this == other) && !(*this > other);
	}

	bool operator<=(const EvalScore& other) const
	{
		return !(*this > other);
	}

	bool operator==(const EvalScore& other) const
	{
		if (is_mate_white_wins == other.is_mate_white_wins &&
			is_mate_black_wins == other.is_mate_black_wins)
		{
			if (is_mate_white_wins || is_mate_black_wins) return mate_in == other.mate_in;
			else return score == other.score;
		}
		else
			return false;
	}

	bool operator!=(const EvalScore& other) const
	{
		return !(*this == other);
	}
};

// Evaluate the position and return a score. Doesn't consider checkmate, stalemate, mobility etc because it doesn't do movegen - expanded into an EvalScore by search.
double evaluate_position(const Board& board);