#include "gtest/gtest.h"

#include <Chess/MoveGen.h>
#include <Chess/Utils.h>

TEST(MoveGen, Perft_Init)
{
	std::vector<PerftResults> perft_results;
	perft_results.emplace_back(); // dummy node for perft(0)
	for (int i = 1; i <= 6; i++)
		perft_results.push_back(Utils::perft(Utils::get_start_position(), Player::White, i));

	EXPECT_EQ(perft_results[1].nodes, 20);
	EXPECT_EQ(perft_results[2].nodes, 400);
	EXPECT_EQ(perft_results[3].nodes, 8902);
	EXPECT_EQ(perft_results[4].nodes, 197281);
	EXPECT_EQ(perft_results[5].nodes, 4865609);
	EXPECT_EQ(perft_results[6].nodes, 119060324);

	EXPECT_EQ(perft_results[1].captures, 0);
	EXPECT_EQ(perft_results[2].captures, 0);
	EXPECT_EQ(perft_results[3].captures, 34);
	EXPECT_EQ(perft_results[4].captures, 1576);
	EXPECT_EQ(perft_results[5].captures, 82719);
	EXPECT_EQ(perft_results[6].captures, 2812008);

	EXPECT_EQ(perft_results[1].en_passants, 0);
	EXPECT_EQ(perft_results[2].en_passants, 0);
	EXPECT_EQ(perft_results[3].en_passants, 0);
	EXPECT_EQ(perft_results[4].en_passants, 0);
	EXPECT_EQ(perft_results[5].en_passants, 258);
	EXPECT_EQ(perft_results[6].en_passants, 5248);
}

TEST(MoveGen, Perft_FEN2)
{
	auto board = Utils::get_board_from_fen("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ");
	std::vector<PerftResults> perft_results;
	perft_results.emplace_back(); // dummy node for perft(0)
	for (int i = 1; i <= 5; i++)
		perft_results.push_back(Utils::perft(board, Player::White, i));

	EXPECT_EQ(perft_results[1].nodes, 48);
	EXPECT_EQ(perft_results[2].nodes, 2039);
	EXPECT_EQ(perft_results[3].nodes, 97862);
	EXPECT_EQ(perft_results[4].nodes, 4085603);
	EXPECT_EQ(perft_results[5].nodes, 193690690);

	EXPECT_EQ(perft_results[1].captures, 8);
	EXPECT_EQ(perft_results[2].captures, 351);
	EXPECT_EQ(perft_results[3].captures, 17102);
	EXPECT_EQ(perft_results[4].captures, 757163);
	EXPECT_EQ(perft_results[5].captures, 35043416);

	EXPECT_EQ(perft_results[1].en_passants, 0);
	EXPECT_EQ(perft_results[2].en_passants, 1);
	EXPECT_EQ(perft_results[3].en_passants, 45);
	EXPECT_EQ(perft_results[4].en_passants, 1929);
	EXPECT_EQ(perft_results[5].en_passants, 73365);
}

TEST(MoveGen, Perft_FEN3)
{
	auto board = Utils::get_board_from_fen("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - ");
	std::vector<PerftResults> perft_results;
	perft_results.emplace_back(); // dummy node for perft(0)
	for (int i = 1; i <= 7; i++)
		perft_results.push_back(Utils::perft(board, Player::White, i));

	EXPECT_EQ(perft_results[1].nodes, 14);
	EXPECT_EQ(perft_results[2].nodes, 191);
	EXPECT_EQ(perft_results[3].nodes, 2812);
	EXPECT_EQ(perft_results[4].nodes, 43238);
	EXPECT_EQ(perft_results[5].nodes, 674624);
	EXPECT_EQ(perft_results[6].nodes, 11030083);
	EXPECT_EQ(perft_results[7].nodes, 178633661);

	EXPECT_EQ(perft_results[1].captures, 1);
	EXPECT_EQ(perft_results[2].captures, 14);
	EXPECT_EQ(perft_results[3].captures, 209);
	EXPECT_EQ(perft_results[4].captures, 3348);
	EXPECT_EQ(perft_results[5].captures, 52051);
	EXPECT_EQ(perft_results[6].captures, 940350);
	EXPECT_EQ(perft_results[7].captures, 14519036);

	EXPECT_EQ(perft_results[1].en_passants, 0);
	EXPECT_EQ(perft_results[2].en_passants, 0);
	EXPECT_EQ(perft_results[3].en_passants, 2);
	EXPECT_EQ(perft_results[4].en_passants, 123);
	EXPECT_EQ(perft_results[5].en_passants, 1165);
	EXPECT_EQ(perft_results[6].en_passants, 33325);
	EXPECT_EQ(perft_results[7].en_passants, 294874);
}

TEST(MoveGen, Perft_FEN4)
{
	auto board = Utils::get_board_from_fen("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
	std::vector<PerftResults> perft_results;
	perft_results.emplace_back(); // dummy node for perft(0)
	for (int i = 1; i <= 5; i++)
		perft_results.push_back(Utils::perft(board, Player::White, i));

	EXPECT_EQ(perft_results[1].nodes, 6);
	EXPECT_EQ(perft_results[2].nodes, 264);
	EXPECT_EQ(perft_results[3].nodes, 9467);
	EXPECT_EQ(perft_results[4].nodes, 422333);
	EXPECT_EQ(perft_results[5].nodes, 15833292);

	EXPECT_EQ(perft_results[1].captures, 0);
	EXPECT_EQ(perft_results[2].captures, 87);
	EXPECT_EQ(perft_results[3].captures, 1021);
	EXPECT_EQ(perft_results[4].captures, 131393);
	EXPECT_EQ(perft_results[5].captures, 2046173);

	EXPECT_EQ(perft_results[1].en_passants, 0);
	EXPECT_EQ(perft_results[2].en_passants, 0);
	EXPECT_EQ(perft_results[3].en_passants, 4);
	EXPECT_EQ(perft_results[4].en_passants, 0);
	EXPECT_EQ(perft_results[5].en_passants, 6512);
}

TEST(MoveGen, Perft_FEN5)
{
	auto board = Utils::get_board_from_fen("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8  ");
	std::vector<PerftResults> perft_results;
	perft_results.emplace_back(); // dummy node for perft(0)
	for (int i = 1; i <= 5; i++)
		perft_results.push_back(Utils::perft(board, Player::White, i));

	EXPECT_EQ(perft_results[1].nodes, 44);
	EXPECT_EQ(perft_results[2].nodes, 1486);
	EXPECT_EQ(perft_results[3].nodes, 62379);
	EXPECT_EQ(perft_results[4].nodes, 2103487);
	EXPECT_EQ(perft_results[5].nodes, 89941194);
}

TEST(MoveGen, Perft_FEN6)
{
	auto board = Utils::get_board_from_fen("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10 ");
	std::vector<PerftResults> perft_results;
	perft_results.emplace_back(); // dummy node for perft(0)
	for (int i = 1; i <= 5; i++)
		perft_results.push_back(Utils::perft(board, Player::White, i));
	
	EXPECT_EQ(perft_results[1].nodes, 46);
	EXPECT_EQ(perft_results[2].nodes, 2079);
	EXPECT_EQ(perft_results[3].nodes, 89890);
	EXPECT_EQ(perft_results[4].nodes, 3894594);
	EXPECT_EQ(perft_results[5].nodes, 164075551);
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