#include "gtest/gtest.h"

#include <Chess/Search.h>
#include <Chess/Utils.h>

TEST(Search, SearchBasic)
{
	Board b = Utils::get_start_position();
	Player p = Player::White;
	
	auto sr = search(b, p, 5);
	EXPECT_EQ(sr.next.sq[SquareName::D4], Piece::WhitePawn);	
}