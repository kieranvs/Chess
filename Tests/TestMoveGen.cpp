#include "gtest/gtest.h"

#include <Chess/MoveGen.h>
#include <Chess/Utils.h>

TEST(Board, BoardFunctions)
{
	Board init_position = Utils::get_start_position();

	EXPECT_FALSE(init_position.isPawn(SquareName::A1));
	EXPECT_TRUE(init_position.isPawn(SquareName::B1));
}

TEST(MoveGen, Perft)
{
	std::vector<PerftResults> perft_results;
	perft_results.emplace_back(); // dummy node for perft(0)
	for (int i = 1; i <= 5; i++)
		perft_results.push_back(Utils::perft(Utils::get_start_position(), Player::White, i));

	EXPECT_EQ(perft_results[1].nodes, 20);
	EXPECT_EQ(perft_results[2].nodes, 400);
	EXPECT_EQ(perft_results[3].nodes, 8902);
	EXPECT_EQ(perft_results[4].nodes, 197281);
	EXPECT_EQ(perft_results[5].nodes, 4865609);
	//EXPECT_EQ(perft_results[6].nodes, 119060324);

	EXPECT_EQ(perft_results[1].captures, 0);
	EXPECT_EQ(perft_results[2].captures, 0);
	EXPECT_EQ(perft_results[3].captures, 34);
	EXPECT_EQ(perft_results[4].captures, 1576);
	EXPECT_EQ(perft_results[5].captures, 82719);
	//EXPECT_EQ(perft_results[6].captures, 2812008);

	EXPECT_EQ(perft_results[1].en_passants, 0);
	EXPECT_EQ(perft_results[2].en_passants, 0);
	EXPECT_EQ(perft_results[3].en_passants, 0);
	EXPECT_EQ(perft_results[4].en_passants, 0);
	EXPECT_EQ(perft_results[5].en_passants, 258);
	//EXPECT_EQ(perft_results[6].en_passants, 5248);
}

TEST(MoveGen, Moves)
{
	{
		Board b = Utils::get_blank_board();
		b.sq[SquareName::C3] = Piece::WhitePawn;

		std::vector<MoveGenResult> results;
		move_gen(b, Player::White, results);

		EXPECT_EQ(results.size(), 1);
	}
	{
		Board b = Utils::get_blank_board();
		b.sq[SquareName::C3] = Piece::WhitePawn;
		b.sq[SquareName::D4] = Piece::BlackPawn;

		std::vector<MoveGenResult> results;
		move_gen(b, Player::White, results);

		EXPECT_EQ(results.size(), 2);
	}
	{
		Board b = Utils::get_blank_board();
		b.sq[SquareName::C3] = Piece::WhiteKnight;

		std::vector<MoveGenResult> results;
		move_gen(b, Player::White, results);

		EXPECT_EQ(results.size(), 8);
	}
	{
		Board b = Utils::get_blank_board();
		b.sq[SquareName::C3] = Piece::WhiteRook;

		std::vector<MoveGenResult> results;
		move_gen(b, Player::White, results);

		EXPECT_EQ(results.size(), 14);
	}
	{
		Board b = Utils::get_blank_board();
		b.sq[SquareName::C3] = Piece::WhiteRook;
		b.sq[SquareName::D3] = Piece::BlackPawn;

		std::vector<MoveGenResult> results;
		move_gen(b, Player::White, results);

		EXPECT_EQ(results.size(), 10);
	}
		{
		Board b = Utils::get_blank_board();
		b.sq[SquareName::C3] = Piece::WhiteRook;
		b.sq[SquareName::D3] = Piece::WhitePawn;

		std::vector<MoveGenResult> results;
		move_gen(b, Player::White, results);

		EXPECT_EQ(results.size(), 10);
	}
}