#include "gtest/gtest.h"

#include <Chess/MoveGen.h>
#include <Chess/Utils.h>

TEST(MoveGen, Perft_Init)
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

TEST(MoveGen, Perft_FEN2)
{
	auto board = Utils::get_board_from_fen("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ");
	std::vector<PerftResults> perft_results;
	perft_results.emplace_back(); // dummy node for perft(0)
	for (int i = 1; i <= 4; i++)
		perft_results.push_back(Utils::perft(board, Player::White, i));

	EXPECT_EQ(perft_results[1].nodes, 48);
	EXPECT_EQ(perft_results[2].nodes, 2039);
	EXPECT_EQ(perft_results[3].nodes, 97862);
	EXPECT_EQ(perft_results[4].nodes, 4085603);
	//EXPECT_EQ(perft_results[5].nodes, );

	EXPECT_EQ(perft_results[1].captures, 8);
	EXPECT_EQ(perft_results[2].captures, 351);
	EXPECT_EQ(perft_results[3].captures, 17102);
	EXPECT_EQ(perft_results[4].captures, 757163);
	//EXPECT_EQ(perft_results[5].captures, );

	EXPECT_EQ(perft_results[1].en_passants, 0);
	EXPECT_EQ(perft_results[2].en_passants, 1);
	EXPECT_EQ(perft_results[3].en_passants, 45);
	EXPECT_EQ(perft_results[4].en_passants, 1929);
	//EXPECT_EQ(perft_results[5].en_passants, );
}

TEST(MoveGen, Perft_FEN3)
{
	auto board = Utils::get_board_from_fen("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - ");
	std::vector<PerftResults> perft_results;
	perft_results.emplace_back(); // dummy node for perft(0)
	for (int i = 1; i <= 5; i++)
		perft_results.push_back(Utils::perft(board, Player::White, i));

	EXPECT_EQ(perft_results[1].nodes, 14);
	EXPECT_EQ(perft_results[2].nodes, 191);
	EXPECT_EQ(perft_results[3].nodes, 2812);
	EXPECT_EQ(perft_results[4].nodes, 43238);
	EXPECT_EQ(perft_results[5].nodes, 674624);

	EXPECT_EQ(perft_results[1].captures, 1);
	EXPECT_EQ(perft_results[2].captures, 14);
	EXPECT_EQ(perft_results[3].captures, 209);
	EXPECT_EQ(perft_results[4].captures, 3348);
	EXPECT_EQ(perft_results[5].captures, 52051);

	EXPECT_EQ(perft_results[1].en_passants, 0);
	EXPECT_EQ(perft_results[2].en_passants, 0);
	EXPECT_EQ(perft_results[3].en_passants, 2);
	EXPECT_EQ(perft_results[4].en_passants, 123);
	EXPECT_EQ(perft_results[5].en_passants, 1165);
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