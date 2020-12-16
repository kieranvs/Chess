#include "gtest/gtest.h"

#include <Chess/MoveGen.h>
#include <Chess/Utils.h>

TEST(Board, BoardFunctions)
{
	Board init_position = Utils::get_start_position();

	EXPECT_FALSE(init_position.isPawn(SquareName::A1));
	EXPECT_TRUE(init_position.isPawn(SquareName::B1));
}

std::vector<Board> perft_inc(const std::vector<Board>& input, Player p)
{
	std::vector<Board> results;
	for (const auto& b : input)
		move_gen(b, p, results);
	return results;
}

TEST(MoveGen, Perft)
{
	auto perft0 = std::vector<Board>{ Utils::get_start_position() };
	auto perft1 = perft_inc(perft0, Player::White);
	auto perft2 = perft_inc(perft1, Player::Black);
	auto perft3 = perft_inc(perft2, Player::White);
	auto perft4 = perft_inc(perft3, Player::Black);
	auto perft5 = perft_inc(perft4, Player::White);
	EXPECT_EQ(perft1.size(), 20);
	EXPECT_EQ(perft2.size(), 400);
	EXPECT_EQ(perft3.size(), 8902);
	EXPECT_EQ(perft4.size(), 197281);
	EXPECT_EQ(perft5.size(), 4865609);
	//EXPECT_EQ(perft6.size(), 119060324);
}

TEST(MoveGen, Moves)
{
	{
		Board b = Utils::get_blank_board();
		b.sq[SquareName::C3] = Piece::WhitePawn;

		std::vector<Board> results;
		move_gen(b, Player::White, results);

		EXPECT_EQ(results.size(), 1);
	}
	{
		Board b = Utils::get_blank_board();
		b.sq[SquareName::C3] = Piece::WhitePawn;
		b.sq[SquareName::D4] = Piece::BlackPawn;

		std::vector<Board> results;
		move_gen(b, Player::White, results);

		EXPECT_EQ(results.size(), 2);
	}
	{
		Board b = Utils::get_blank_board();
		b.sq[SquareName::C3] = Piece::WhiteKnight;

		std::vector<Board> results;
		move_gen(b, Player::White, results);

		EXPECT_EQ(results.size(), 8);
	}
	{
		Board b = Utils::get_blank_board();
		b.sq[SquareName::C3] = Piece::WhiteRook;

		std::vector<Board> results;
		move_gen(b, Player::White, results);

		EXPECT_EQ(results.size(), 14);
	}
	{
		Board b = Utils::get_blank_board();
		b.sq[SquareName::C3] = Piece::WhiteRook;
		b.sq[SquareName::D3] = Piece::BlackPawn;

		std::vector<Board> results;
		move_gen(b, Player::White, results);

		EXPECT_EQ(results.size(), 10);
	}
		{
		Board b = Utils::get_blank_board();
		b.sq[SquareName::C3] = Piece::WhiteRook;
		b.sq[SquareName::D3] = Piece::WhitePawn;

		std::vector<Board> results;
		move_gen(b, Player::White, results);

		EXPECT_EQ(results.size(), 10);
	}
}