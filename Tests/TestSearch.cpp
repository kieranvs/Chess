#include "gtest/gtest.h"

#include <Chess/MoveGen.h>
#include <Chess/Search.h>
#include <Chess/Utils.h>

TEST(Search, SearchBasic)
{
	Board b = Utils::get_start_position();
	Player p = Player::White;
	
	auto sr = search(b, p, 5, false);
	EXPECT_EQ(sr.next.sq[SquareName::D4], Piece::WhitePawn);

	auto sr_ab = search(b, p, 5, true);
	EXPECT_EQ(sr.score, sr_ab.score);
}

TEST(Search, AlphaBeta)
{
	Board b = Utils::get_start_position();

	std::vector<MoveGenResult> results;
	move_gen(b, Player::White, results);

	for (const auto& mgr : results)
	{
		auto sr = search(mgr.board, Player::Black, 4, false);
		auto sr_ab = search(mgr.board, Player::Black, 4, true);

		EXPECT_EQ(sr.score, sr_ab.score);
	}
}