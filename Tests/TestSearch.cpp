#include "gtest/gtest.h"

#include <Chess/MoveGen.h>
#include <Chess/Search.h>
#include <Chess/Utils.h>

TEST(Search, SearchBasic)
{
	Board b = Utils::get_start_position();
	
	auto sr = search(b, 5, false);
	EXPECT_EQ(sr.next.sq[SquareName::E4], Piece::WhitePawn);

	auto sr_ab = search(b, 5, true);
	EXPECT_EQ(sr.score, sr_ab.score);
}

TEST(Search, AlphaBeta)
{
	Board b = Utils::get_start_position();

	std::vector<MoveGenResult> results;
	move_gen(b, results);

	for (const auto& mgr : results)
	{
		auto sr = search(mgr.board, 4, false);
		auto sr_ab = search(mgr.board, 4, true);

		EXPECT_EQ(sr.score, sr_ab.score);
	}
}

void run_puzzle_test(const char* fen, int min_search_depth, int max_search_depth, int expected_sq_from, int expected_sq_to, int expected_mate_in)
{
	Board b = Utils::get_board_from_fen(fen);

	for (int i = min_search_depth; i <= max_search_depth; i++)
	{
		auto sr = search(b, i, true);
		EXPECT_EQ(sr.move.sq_from, expected_sq_from);
		EXPECT_EQ(sr.move.sq_to, expected_sq_to);
		EXPECT_EQ(sr.score.mate_in, expected_mate_in);
	}
}

TEST(Search, MatePuzzle1)
{
	run_puzzle_test("1Bb3BN/R2Pk2r/1Q5B/4q2R/2bN4/4Q1BK/1p6/1bq1R1rb w - -", 2, 5, SquareName::E3, SquareName::A3, 1);
}

TEST(Search, MatePuzzle2)
{
	run_puzzle_test("r2qk2r/pb4pp/1n2Pb2/2B2Q2/p1p5/2P5/2B2PPP/RN2R1K1 w - - 1 0", 4, 5, SquareName::F5, SquareName::G6, 2);
}

TEST(Search, MatePuzzle3)
{
	run_puzzle_test("6k1/pp4p1/2p5/2bp4/8/P5Pb/1P3rrP/2BRRN1K b - - 0 1", 4, 5, SquareName::G2, SquareName::G1, 2);
}
