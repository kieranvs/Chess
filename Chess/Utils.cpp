#include "Utils.h"

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