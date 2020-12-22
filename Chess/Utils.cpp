#include "Utils.h"

#include "MoveGen.h"

#include <iostream>

Board Utils::get_start_position()
{
	Board b;
	b.en_passant_target = 0;
	for (int i = 0; i < 120; i++)
		b.sq[i] = Piece::OffBoard;

	for (int i = 16; i < 48; i++)
		b.sq[mailbox64[i]] = Piece::None;

	b.sq[mailbox64[0]] = Piece::WhiteRook;
	b.sq[mailbox64[1]] = Piece::WhiteKnight;
	b.sq[mailbox64[2]] = Piece::WhiteBishop;
	b.sq[mailbox64[3]] = Piece::WhiteQueen;
	b.sq[mailbox64[4]] = Piece::WhiteKing;
	b.sq[mailbox64[5]] = Piece::WhiteBishop;
	b.sq[mailbox64[6]] = Piece::WhiteKnight;
	b.sq[mailbox64[7]] = Piece::WhiteRook;

	for (int i = 8; i < 16; i++)
		b.sq[mailbox64[i]] = Piece::WhitePawn;

	b.sq[mailbox64[56]] = Piece::BlackRook;
	b.sq[mailbox64[57]] = Piece::BlackKnight;
	b.sq[mailbox64[58]] = Piece::BlackBishop;
	b.sq[mailbox64[59]] = Piece::BlackQueen;
	b.sq[mailbox64[60]] = Piece::BlackKing;
	b.sq[mailbox64[61]] = Piece::BlackBishop;
	b.sq[mailbox64[62]] = Piece::BlackKnight;
	b.sq[mailbox64[63]] = Piece::BlackRook;
	
	for (int i = 48; i < 56; i++)
		b.sq[mailbox64[i]] = Piece::BlackPawn;

	return b;
}

Board Utils::get_blank_board()
{
	Board b;
	b.en_passant_target = 0;
	for (int i = 0; i < 120; i++)
		b.sq[i] = Piece::OffBoard;

	for (int i = 0; i < 64; i++)
		b.sq[mailbox64[i]] = Piece::None;

	return b;
}

const char* get_piece_disp(uint8_t piece)
{
	if (piece == Piece::WhitePawn) return "P";
	if (piece == Piece::WhiteRook) return "R";
	if (piece == Piece::WhiteKnight) return "N";
	if (piece == Piece::WhiteBishop) return "B";
	if (piece == Piece::WhiteQueen) return "Q";
	if (piece == Piece::WhiteKing) return "K";
	if (piece == Piece::BlackPawn) return "p";
	if (piece == Piece::BlackRook) return "r";
	if (piece == Piece::BlackKnight) return "n";
	if (piece == Piece::BlackBishop) return "b";
	if (piece == Piece::BlackQueen) return "q";
	if (piece == Piece::BlackKing) return "k";
	if (piece == Piece::None) return ".";
	return "?";
}

void Utils::print_board(const Board& board)
{
	printf("\n----------------\n");
	for (int row = 7; row >= 0; row--)
	{
		for (int col = 0; col < 8; col++)
		{
			auto piece = board.sq[mailbox64[row*8 + col]];
			printf("%s ", get_piece_disp(piece));
		}
		printf("\n");
	}
	printf("----------------\n\n");
}

const char* sq_names[] =
{
	"a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
	"a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
	"a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
	"a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
	"a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
	"a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
	"a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
	"a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
};

const char* Utils::get_square_name(int sq)
{
	return sq_names[mailbox120[sq]];
}

const char* Utils::get_player_name(Player p)
{
	return p == Player::White ? "White" : "Black";
}

void perft_dfs(const Board& board, Player p, int depth_left, PerftResults& accum)
{
	Player otherPlayer = Utils::opposite_player(p);
	std::vector<MoveGenResult> results;
	move_gen(board, p, results);

	if (depth_left == 1)
	{
		for (const auto& node : results)
		{
			accum.nodes += 1;
			if (node.move_type == MoveType::Capture || node.move_type == MoveType::CaptureEnPassant)
				accum.captures += 1;
			if (node.move_type == MoveType::CaptureEnPassant)
				accum.en_passants += 1;
		}
	}
	else
	{
		for (const auto& node : results)
			perft_dfs(node.board, otherPlayer, depth_left - 1, accum);
	}
}

PerftResults Utils::perft(const Board& b, Player p, int depth)
{
	PerftResults accum;
	perft_dfs(b, p, depth, accum);
	return accum;
}