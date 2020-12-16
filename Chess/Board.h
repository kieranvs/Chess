#pragma once

#include <stdint.h>

constexpr uint8_t PieceTypePawn = 0x1;
constexpr uint8_t PieceTypeRook = 0x2;
constexpr uint8_t PieceTypeKnight = 0x3;
constexpr uint8_t PieceTypeBishop = 0x4;
constexpr uint8_t PieceTypeQueen = 0x5;
constexpr uint8_t PieceTypeKing = 0x6;

constexpr uint8_t PieceTypeMask = 0x7;

constexpr uint8_t PieceTypeWhite = 0x8;
constexpr uint8_t PieceTypeBlack = 0x10;

constexpr uint8_t PieceTypeInvalid = 0x80;

enum class Player : uint8_t
{
	White = PieceTypeWhite,
	Black = PieceTypeBlack
};

struct Piece
{
	static constexpr uint8_t WhitePawn = PieceTypeWhite | PieceTypePawn;
	static constexpr uint8_t WhiteRook = PieceTypeWhite | PieceTypeRook;
	static constexpr uint8_t WhiteKnight = PieceTypeWhite | PieceTypeKnight;
	static constexpr uint8_t WhiteBishop = PieceTypeWhite | PieceTypeBishop;
	static constexpr uint8_t WhiteQueen = PieceTypeWhite | PieceTypeQueen;
	static constexpr uint8_t WhiteKing = PieceTypeWhite | PieceTypeKing;
	static constexpr uint8_t BlackPawn = PieceTypeBlack | PieceTypePawn;
	static constexpr uint8_t BlackRook = PieceTypeBlack | PieceTypeRook;
	static constexpr uint8_t BlackKnight = PieceTypeBlack | PieceTypeKnight;
	static constexpr uint8_t BlackBishop = PieceTypeBlack | PieceTypeBishop;
	static constexpr uint8_t BlackQueen = PieceTypeBlack | PieceTypeQueen;
	static constexpr uint8_t BlackKing = PieceTypeBlack | PieceTypeKing;
	static constexpr uint8_t None = 0x00;
	static constexpr uint8_t OffBoard = PieceTypeInvalid;
};

constexpr int mailbox120[120] =
{
     -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
     -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
     -1,  0,  1,  2,  3,  4,  5,  6,  7, -1,
     -1,  8,  9, 10, 11, 12, 13, 14, 15, -1,
     -1, 16, 17, 18, 19, 20, 21, 22, 23, -1,
     -1, 24, 25, 26, 27, 28, 29, 30, 31, -1,
     -1, 32, 33, 34, 35, 36, 37, 38, 39, -1,
     -1, 40, 41, 42, 43, 44, 45, 46, 47, -1,
     -1, 48, 49, 50, 51, 52, 53, 54, 55, -1,
     -1, 56, 57, 58, 59, 60, 61, 62, 63, -1,
     -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
     -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
};

constexpr int mailbox64[64] =
{
    21, 22, 23, 24, 25, 26, 27, 28,
    31, 32, 33, 34, 35, 36, 37, 38,
    41, 42, 43, 44, 45, 46, 47, 48,
    51, 52, 53, 54, 55, 56, 57, 58,
    61, 62, 63, 64, 65, 66, 67, 68,
    71, 72, 73, 74, 75, 76, 77, 78,
    81, 82, 83, 84, 85, 86, 87, 88,
    91, 92, 93, 94, 95, 96, 97, 98
};

struct SquareName
{
	static constexpr int A1 = mailbox64[0];
	static constexpr int B1 = mailbox64[8];
	static constexpr int C1 = mailbox64[16];
	static constexpr int C2 = mailbox64[17];
	static constexpr int C3 = mailbox64[18];
	static constexpr int C4 = mailbox64[19];
	static constexpr int D3 = mailbox64[26];
	static constexpr int D4 = mailbox64[27];
};

struct Board
{
	// va -  -  Bl Wh [Piece]
	// .  .  .  .  .  .  .  .
	uint8_t sq[120]; //12 rows x 10 cols
	int en_passant_target;

	bool isValid(int i) const { return sq[i] != Piece::OffBoard; }
	bool isFree(int i) const { return sq[i] == Piece::None; }
	bool isPlayer(int i, Player p) const { return (sq[i] & (uint8_t)p) != 0; }
	bool isPawn(int i) const { return (sq[i] & PieceTypeMask) == PieceTypePawn; }
	int rankOf(int i) const { return (i / 10) - 2 + 1; }
};
