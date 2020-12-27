#include "Utils.h"

#include <iostream>

Board Utils::get_start_position()
{
	Board b = get_blank_board();

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
	b.castle_rights = CastleRightsAll;
	for (int i = 0; i < 120; i++)
		b.sq[i] = Piece::OffBoard;

	for (int i = 0; i < 64; i++)
		b.sq[mailbox64[i]] = Piece::None;

	return b;
}

static std::vector<std::string> split_args(const std::string &text, char sep)
{
	std::vector<std::string> tokens;
	std::size_t start = 0, end = 0;
	while ((end = text.find(sep, start)) != std::string::npos)
	{
		tokens.push_back(text.substr(start, end - start));
		start = end + 1;
	}
	tokens.push_back(text.substr(start));
	return tokens;
}

Board Utils::get_board_from_fen(const std::string& fen)
{
	Board b = get_blank_board();

	auto sections = split_args(fen, ' ');

	if (sections.size() < 3)
	{
		printf("Invalid fen: not enough sections\n");
		exit(0);
	}

	auto load_piece_section = [&b](const std::string& piece_section)
	{
		int fen_rank = 0;
		int file = 0;
		int fen_addr = 0;

		auto putPiece = [&fen_rank, &file, &b](uint8_t piece)
		{
			b.sq[mailbox64[(7 - fen_rank) * 8 + file]] = piece;
			file += 1;
		};

		while (true)
		{
			if (fen_addr > piece_section.size())
			{
				printf("Invalid fen: read past end\n");
				exit(0);
			}
			else if (fen_addr == piece_section.size())
			{
				if (file == 8 && fen_rank == 7)
				{
					return;
				}
				else
				{
					printf("Invalid fen: incomplete piece data section\n");
					exit(0);
				}
			}
			char c = piece_section[fen_addr];
			fen_addr += 1;
			if (c >= '1' && c <= '8')
			{
				int num_blanks = c - '0';
				file += num_blanks;
			}
			else if (c == '/')
			{
				if (file == 8)
				{
					file = 0;
					fen_rank += 1;
				}
				else
				{
					printf("Invalid fen: unexpected /\n");
					exit(0);
				}
			}
			else if (c == 'P') putPiece(Piece::WhitePawn);
			else if (c == 'R') putPiece(Piece::WhiteRook);
			else if (c == 'N') putPiece(Piece::WhiteKnight);
			else if (c == 'B') putPiece(Piece::WhiteBishop);
			else if (c == 'Q') putPiece(Piece::WhiteQueen);
			else if (c == 'K') putPiece(Piece::WhiteKing);
			else if (c == 'p') putPiece(Piece::BlackPawn);
			else if (c == 'r') putPiece(Piece::BlackRook);
			else if (c == 'n') putPiece(Piece::BlackKnight);
			else if (c == 'b') putPiece(Piece::BlackBishop);
			else if (c == 'q') putPiece(Piece::BlackQueen);
			else if (c == 'k') putPiece(Piece::BlackKing);
			else
			{
				printf("Invalid fen: unexpected char %c\n", c);
				exit(0);
			}
		}
	};

	auto load_castle_section = [&b](const std::string& castle_section)
	{
		b.castle_rights = CastleRightsNone;
		
		if (castle_section.find('K') != std::string::npos)
			b.castle_rights |= CastleRightsWhiteKingside;
		if (castle_section.find('Q') != std::string::npos)
			b.castle_rights |= CastleRightsWhiteQueenside;
		if (castle_section.find('k') != std::string::npos)
			b.castle_rights |= CastleRightsBlackKingside;
		if (castle_section.find('q') != std::string::npos)
			b.castle_rights |= CastleRightsBlackQueenside;
	};

	load_piece_section(sections[0]);
	load_castle_section(sections[2]);

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

const char* Utils::get_move_type_as_string(MoveType mt)
{
	if (mt == MoveType::Move)
		return "";
	else if (mt == MoveType::Capture)
		return "capture";
	else if (mt == MoveType::CaptureEnPassant)
		return "capture (ep)";
	else if (mt == MoveType::Castle)
		return "castle";
	else
		return "?";
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
			if (node.move_type == MoveType::Castle)
				accum.castles += 1;
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