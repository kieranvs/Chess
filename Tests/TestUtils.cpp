#include "gtest/gtest.h"

#include <Chess/MoveGen.h>
#include <Chess/Utils.h>

TEST(Board, BoardFunctions)
{
	Board init_position = Utils::get_start_position();

	EXPECT_FALSE(init_position.isPawn(SquareName::A1));
	EXPECT_FALSE(init_position.isPawn(SquareName::B1));
	EXPECT_TRUE(init_position.isPawn(SquareName::A2));
	EXPECT_TRUE(init_position.isPawn(SquareName::B2));
}

TEST(Utils, FEN)
{
	Board b = Utils::get_board_from_fen("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 b - - ");
	EXPECT_EQ(b.sq[SquareName::C7], Piece::BlackPawn);
	EXPECT_EQ(b.sq[SquareName::D6], Piece::BlackPawn);
	EXPECT_EQ(b.sq[SquareName::F4], Piece::BlackPawn);
	EXPECT_EQ(b.sq[SquareName::H5], Piece::BlackRook);
	EXPECT_EQ(b.sq[SquareName::H4], Piece::BlackKing);
	EXPECT_EQ(b.sq[SquareName::B5], Piece::WhitePawn);
	EXPECT_EQ(b.sq[SquareName::E2], Piece::WhitePawn);
	EXPECT_EQ(b.sq[SquareName::G2], Piece::WhitePawn);
	EXPECT_EQ(b.sq[SquareName::A5], Piece::WhiteKing);
	EXPECT_EQ(b.sq[SquareName::B4], Piece::WhiteRook);
	EXPECT_EQ(b.playerToMove(), Player::Black);
}